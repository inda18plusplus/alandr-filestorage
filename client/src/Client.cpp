//
// Created by lunar on 31/10/18.
//

#include "Client.hpp"

#include <fstream>
#include <ios>
#include <iostream>
#include <sstream>
#include <string>

#include <cryptopp/aes.h>
#include <cryptopp/base64.h>
#include <cryptopp/hkdf.h>
#include <cryptopp/sha.h>
#include <cryptopp/osrng.h>

#include <fmt/format.h>

#include <SSLWrap/Packet.hpp>

#include "Commands.hpp"
#include "File.hpp"
#include "MerkleTree.hpp"

Client::Client(fs::path configFile) : _ctx(new sw::ClientContext({}, {{"cert"}})), _server(_ctx), _configPath(configFile) {

	_configDoc.load_file(_configPath.c_str());
	auto cfgNode = _configDoc.child("config");

	_serverIP = {cfgNode.child("serverIP").text().as_string()};
	_serverPort = cfgNode.child("serverPort").text().as_int();

	_fsRoot = cfgNode.child("rootPath").text().as_string();
	fs::create_directories(_fsRoot);

	if(!fs::is_directory(_fsRoot)) {
		fmt::print(stderr, "The rootPath specified ({}) is not a path, switching to parent", fs::absolute(_fsRoot).string());
		_fsRoot = _fsRoot.parent_path();
	}

	if(auto saltNode = cfgNode.child("passSalt")) {
		std::string str = saltNode.text().as_string();
		CryptoPP::StringSource(str, true,
		    new CryptoPP::Base64Decoder(
				new CryptoPP::StringSink(_passSalt)
		    )
		);
	} else {
		std::cout << "Generating salt\n";

		_passSalt.resize(32);

		CryptoPP::OS_GenerateRandomBlock(false, (byte*)_passSalt.data(), _passSalt.size());

		saltNode = cfgNode.append_child("passSalt");

		std::string encoded;
		CryptoPP::StringSource(_passSalt, true,
			new CryptoPP::Base64Encoder(
				new CryptoPP::StringSink(encoded)
			)
		);
		saltNode.text().set(encoded.c_str());
	}

	_hashNode = cfgNode.child("topHash");
	if(_hashNode.empty()) {
		_firstTime = true;
		_hashNode = cfgNode.append_child("topHash");
		setStoredHash("");
	}

	_configDoc.save_file(_configPath.c_str());

	_handlers["ll"] = [this](const std::string str){ handleListLocal(str); };
	_handlers["ls"] = [this](const std::string str){ handleListServer(str); };
	_handlers["ul"] = [this](const std::string str){ handleUpload(str); };
	_handlers["dl"] = [this](const std::string str){ handleDownload(str); };
	_handlers["rm"] = [this](const std::string str){ handleRemove(str); };
	_handlers["q"]  = [this](const std::string str){ handleQuit(str); };

}

Client::~Client() {

	setStoredHash(_tree.topHash());

	_configDoc.save_file(_configPath.c_str());

}

void Client::run() {

	_server.connect(_serverIP, _serverPort);

	{
		std::cout << "Please enter a password: ";
		std::string pass;
		std::getline(std::cin, pass);
		CryptoPP::HKDF<CryptoPP::SHA256> hkdf;
		hkdf.DeriveKey(_encKey, sizeof(_encKey), (byte*)pass.data(), pass.size(), (byte*)_passSalt.data(), _passSalt.size(), nullptr, 0);
	}

	if(_firstTime)
		doFirstTime();

	doInit();

	while(!_exit) {

		std::cout << "Enter a command (ll, ls, ul, dl, rm, q)\n";

		std::string in;
		std::getline(std::cin, in);
		std::stringstream ss(in);

		std::string command;
		ss >> command;

		if(_handlers.find(command) != _handlers.end()) {
			_handlers[command](in);
		} else {
			fmt::print(stderr, "Unrecognized command: {}\n", command);
		}

	}

}

void Client::doFirstTime() {

	sw::Packet p;
	p << COMMAND::FIRSTTIME;
	_server.send(p);

	COMMAND ret;
	std::string topHash;
	_server.receive(p);
	p >> ret >> topHash;

	if(topHash != _tree.topHash()) {
		std::cerr << "The empty topHash doesn't match!\n\n";
	}

	setStoredHash(topHash);

}
void Client::doInit() {

	sw::Packet p;
	p << COMMAND::INIT;
	_server.send(p);

	_server.receive(p);
	int numHashes;
	p >> numHashes;

	for (int i = 0; i < numHashes; ++i) {

		std::string hash;
		p >> hash;

		_tree.addString(i, hash);

	}

	if(getStoredHash() != _tree.topHash()) {
		std::cerr << "Server hash doesn't match stored hash!\n\n";
	}

}

std::string Client::getStoredHash() {

	std::string encodedData = _hashNode.text().as_string();
	std::string ret;

	CryptoPP::StringSource(encodedData, true,
		new CryptoPP::Base64Decoder(
			new CryptoPP::StringSink(ret)
		)
	);

	return ret;

}
void Client::setStoredHash(const std::string &s) {

	std::string encodedData;

	CryptoPP::StringSource(s, true,
		new CryptoPP::Base64Encoder(
			new CryptoPP::StringSink(encodedData)
		)
	);

	_hashNode.text().set(encodedData.c_str());

}

void Client::handleListLocal(const std::string&) {

	for(auto& p : fs::directory_iterator(_fsRoot)) {
		fmt::print("{}\n", p.path().filename().string());
	}

}
void Client::handleListServer(const std::string&) {
	sw::Packet p;
	p << COMMAND::LIST;
	_server.send(p);

	p.clear();
	_server.receive(p);

	COMMAND ret;
	int numFiles;
	p >> ret >> numFiles;

	fmt::print("These files are on the server\n");
	for (int i = 0; i < numFiles; ++i) {
		std::string name;
		p >> name;
		fmt::print("[{}]\t{}\n", i, name);
	}

	std::cout << '\n';
}
void Client::handleUpload(const std::string &in) {
	int id;

	sw::Packet p;
	p << COMMAND::GETID;
	_server.send(p);

	_server.receive(p);
	COMMAND ret;
	p >> ret >> id;

	//TODO: literally any error handling
	std::stringstream ss(in);
	ss.ignore(3);
	
	std::string name;
	std::getline(ss, name);
	
	auto f = std::make_shared<File>((_fsRoot / fs::path(name)).string());
	f->setName(name);
	f->setId(id);
	f->encrypt(_encKey);

	p.clear();
	p << COMMAND::UPLOAD << *f;
	_server.send(p);

	_files[id] = std::move(f);
	_tree.addFile(id, _files[id]);

	std::string topHash;
	_server.receive(p);
	p >> ret >> topHash;

	if(topHash != _tree.topHash()) {
		std::cerr << "New tophash doesn't match!\n\n";
	}
	
}
void Client::handleDownload(const std::string &in) {
	//TODO: literally any error handling
	std::stringstream ss(in);
	ss.ignore(3);

	int id;
	ss >> id;

	sw::Packet p;
	p << COMMAND::DOWNLOAD << id;
	_server.send(p);

	_server.receive(p);
	COMMAND ret;
	File f;

	p >> ret >> f;

	f.decrypt(_encKey);
	f.dump(_fsRoot);

}
void Client::handleRemove(const std::string &in) {

	std::stringstream ss(in);
	ss.ignore(3);
	int id;
	ss >> id;

	sw::Packet p;
	p << COMMAND::DELETE << id;
	_server.send(p);

	COMMAND ret;
	std::string topHash;
	_server.receive(p);
	p >> ret;

	if(ret == COMMAND::ERROR) {
		std::string str;
		p >> str;
		std::cerr << str << "\n\n";
		return;
	}

	if(_files.find(id) != _files.end()) {
		_files.erase(_files.find(id));
	}
	_tree.removeFile(id);

	p >> topHash;

	if(topHash != _tree.topHash()) {
		std::cerr << "New tophash doesn't match!\n\n";
	}

}
void Client::handleQuit(const std::string&) {
	sw::Packet p;

	p << COMMAND::DISCONNECT;
	_server.send(p);

	_exit = true;
}

//
// Created by lunar on 31/10/18.
//

#include "Server.hpp"

#include <csignal>
#include <fstream>
#include <ios>
#include <sstream>
#include <string>

#include <fmt/format.h>

#include <pugixml.hpp>

#include "Commands.hpp"

Server* Server::_handler = nullptr;

Server::Server() : _ctx(new sw::ServerContext("cert/key.pem", "cert/cert.pem")), _listener(_ctx, 1337) {

	_handlers[COMMAND::INIT] =       [this](sw::Packet& p){ handleInit(p); };
	_handlers[COMMAND::LIST] =       [this](sw::Packet& p){ handleList(p); };
	_handlers[COMMAND::UPLOAD] =     [this](sw::Packet& p){ handleUpload(p); };
	_handlers[COMMAND::DOWNLOAD] =   [this](sw::Packet& p){ handleDownload(p); };
	_handlers[COMMAND::DELETE] =     [this](sw::Packet& p){ handleDelete(p); };
	_handlers[COMMAND::GETID] =      [this](sw::Packet& p){ handleGetId(p); };
	_handlers[COMMAND::DONE] =       [this](sw::Packet& p){ handleDone(p); };
	_handlers[COMMAND::ERROR] =      [this](sw::Packet& p){ handleError(p); };
	_handlers[COMMAND::DISCONNECT] = [this](sw::Packet& p){ handleDisconnect(p); };
	_handlers[COMMAND::FIRSTTIME]  = [this](sw::Packet& p){ handleFirstTime(p); };

}

void Server::save() {

	for(auto& p : _files) {
		fs::path path = _fsRoot / fs::path{std::to_string(p.second->id())};
		p.second->writeEncrypted(path);
	}

}
void Server::load() {
	fs::create_directories(_fsRoot);

	for(auto& p : fs::directory_iterator(_fsRoot)) {
		auto file = std::make_shared<File>();
		file->readEncrypted(p.path());

		int id = file->id();
		_files.emplace(id, std::move(file));
		_tree.addFile(id, _files[id]);

		_runningId++;
	}

	for (int i = 0; i < _runningId; ++i) {
		if(_files.find(i) == _files.end()) {
			_emptyIds.push_back(i);
		}
	}
}

void Server::start() {

	load();

	_handler = this;
	std::signal(SIGINT, &handleSIGINT);

	while(!_exit) {

		_listener.accept(_client);
		_connected = true;

		while(_connected) {
			sw::Packet p;

			_client.receive(p);

			COMMAND command;
			p >> command;

			_handlers[command](p);
		}

	}

	save();

}

void Server::handleSIGINT(int) {

	_handler->_exit = true;

}

void Server::handleInit(sw::Packet &) {

	sw::Packet p;

	int numhashes = _tree.getNumLeaves();
	p << numhashes;
	for (int i = 0; i < numhashes; ++i) {
		p << _tree.getLeaf(i).hash();
	}

	_client.send(p);

}
void Server::handleList(sw::Packet&) {

	sw::Packet ret;
	ret << COMMAND::DONE;

	ret << (int)_files.size();

	for(auto& f : _files) {
		ret << f.second->name();
	}

	_client.send(ret);

}
void Server::handleUpload(sw::Packet &p) {

	auto f = std::make_shared<File>();
	p >> *f;

	int id = f->id();
	_files.emplace(id, std::move(f));
	_tree.addFile(id, _files[id]);

	p.clear();
	p << COMMAND::DONE << _tree.topHash();
	_client.send(p);

}
void Server::handleDownload(sw::Packet &p) {

	int id;
	p >> id;

	p.clear();
	p << COMMAND::DONE << *_files[id];
	_client.send(p);

}
void Server::handleDelete(sw::Packet &p) {
	sw::Packet ret;

	int id;
	p >> id;

	if(_files.find(id) == _files.end()) {
		ret << COMMAND::ERROR << fmt::format("File [{}] does not exist!", id);
	} else {
		_files.erase(_files.find(id));

		_tree.removeFile(id);

		ret << COMMAND::DONE << _tree.topHash();
	}

	_client.send(ret);

}
void Server::handleGetId(sw::Packet&) {
	sw::Packet ret;

	ret << COMMAND::DONE;
	if(_emptyIds.empty()) {
		ret << _runningId++;
	} else {
		ret << *_emptyIds.begin();
		_emptyIds.erase(_emptyIds.begin());
	}

	_client.send(ret);
}
void Server::handleDone(sw::Packet &) {
	
}
void Server::handleError(sw::Packet &p) {
	
}
void Server::handleDisconnect(sw::Packet&) {
	_connected = false;
}
void Server::handleFirstTime(sw::Packet &) {

	_tree = MerkleTree();

	fs::remove(_fsRoot);
	fs::create_directories(_fsRoot);

	_files.clear();

	_runningId = 0;
	_emptyIds.clear();

	sw::Packet p;
	p << COMMAND::DONE << _tree.topHash();
	_client.send(p);

}
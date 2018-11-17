//
// Created by lunar on 31/10/18.
//

#ifndef FILESTORAGE_CLIENT_HPP
#define FILESTORAGE_CLIENT_HPP

#include <cstdint>
#include <filesystem>
#include <functional>
#include <map>
#include <memory>

#include <pugixml.hpp>

#include <SSLWrap/ClientContext.hpp>
#include <SSLWrap/IpAddress.hpp>
#include <SSLWrap/Socket.hpp>

#include "MerkleTree.hpp"

namespace fs = std::filesystem;
namespace sw = SSLWrap;
class Client {

	bool _firstTime = false;

	fs::path _fsRoot;

	std::string _configPath;
	pugi::xml_document _configDoc;

	unsigned char _encKey[32];
	std::string _passSalt;

	std::shared_ptr<sw::Context> _ctx;
	sw::Socket _server;

	sw::IpAddress _serverIP;
	uint16_t _serverPort;

	std::map<int, std::shared_ptr<File>> _files;

	bool _exit = false;

	std::map<std::string, std::function<void(const std::string&)>> _handlers;

	MerkleTree _tree;
	pugi::xml_node _hashNode;

public:
	explicit Client(fs::path configFile);
	~Client();

	void run();

private:
	void doFirstTime();
	void doInit();

	std::string getStoredHash();
	void setStoredHash(const std::string& s);


	void handleListLocal(const std::string& in);
	void handleListServer(const std::string& in);
	void handleUpload(const std::string& in);
	void handleDownload(const std::string& in);
	void handleRemove(const std::string& in);
	void handleQuit(const std::string& in);
};


#endif //FILESTORAGE_CLIENT_HPP

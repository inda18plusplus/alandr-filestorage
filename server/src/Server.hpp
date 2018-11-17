//
// Created by lunar on 31/10/18.
//

#ifndef FILESTORAGE_SERVER_HPP
#define FILESTORAGE_SERVER_HPP

#include <functional>
#include <map>
#include <memory>
#include <vector>
#include <filesystem>

#include <SSLWrap/Listener.hpp>
#include <SSLWrap/Packet.hpp>
#include <SSLWrap/ServerContext.hpp>
#include <SSLWrap/Socket.hpp>

#include "Commands.hpp"
#include "File.hpp"
#include "MerkleTree.hpp"

namespace sw = SSLWrap;
namespace fs = std::filesystem;
class Server {

	std::shared_ptr<sw::Context> _ctx;

	sw::Listener _listener;
	sw::Socket _client;

	std::map<int, std::shared_ptr<File>> _files;

	int _runningId = 0;
	std::vector<int> _emptyIds;

	bool _exit = false;
	bool _connected = false;

	std::map<COMMAND, std::function<void(sw::Packet&)>> _handlers;

	fs::path _fsRoot{"serverfiles"};

	MerkleTree _tree;

	static Server* _handler;
	static void handleSIGINT(int);

	void load();
	void save();
public:
	Server();

	void start();

private: //Command handlers

	void handleInit(sw::Packet&);
	void handleList(sw::Packet&);
	void handleUpload(sw::Packet& p);
	void handleDownload(sw::Packet& p);
	void handleDelete(sw::Packet& p);
	void handleGetId(sw::Packet&);
	void handleDone(sw::Packet&);
	void handleError(sw::Packet&);
	void handleDisconnect(sw::Packet&);
	void handleFirstTime(sw::Packet&);

};


#endif //FILESTORAGE_SERVER_HPP

//
// Created by lunar on 31/10/18.
//

#ifndef FILESTORAGE_COMMANDS_HPP
#define FILESTORAGE_COMMANDS_HPP

#include <SSLWrap/Packet.hpp>

enum class COMMAND {

	INIT,
	LIST,
	UPLOAD,
	DOWNLOAD,
	DELETE,
	GETID,
	DONE,
	ERROR,
	DISCONNECT,
	FIRSTTIME

};

SSLWrap::Packet& operator<<(SSLWrap::Packet& p, COMMAND c);
SSLWrap::Packet& operator>>(SSLWrap::Packet& p, COMMAND& c);

#endif //FILESTORAGE_COMMANDS_HPP

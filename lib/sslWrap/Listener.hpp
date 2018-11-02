//
// Created by lunar on 25/10/18.
//

#ifndef FILESTORAGE_SECURELISTENER_HPP
#define FILESTORAGE_SECURELISTENER_HPP

#include "Socket.hpp"

#include <cstdint>

namespace SSLWrap {

	class Listener : public Socket {

		uint16_t _port;
		bool _isSetup;

	public:
		Listener(std::shared_ptr<Context> ctx);
		Listener(std::shared_ptr<Context> ctx, uint16_t port);

		void listen(uint16_t port);

		void accept(Socket &sock);

	};

}


#endif //FILESTORAGE_SECURELISTENER_HPP

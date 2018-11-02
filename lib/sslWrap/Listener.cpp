//
// Created by lunar on 25/10/18.
//

#include "Listener.hpp"

#include <cerrno>
#include <iostream>

#include "SSLError.hpp"

namespace SSLWrap {

	Listener::Listener(std::shared_ptr<Context> ctx) : Socket(std::move(ctx), IpAddress::Any, 0), _isSetup(false) {

	}

	Listener::Listener(std::shared_ptr<Context> ctx, uint16_t port) : Socket(std::move(ctx), IpAddress::Any, port) {

		_isSetup = true;
		listen(port);

	}

	void Listener::listen(uint16_t port) {

		_port = port;

		int val = 1;
		setsockopt(_fd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val));

		if(::bind(handle(), (sockaddr*)(&address()), sizeof(address())) == -1) {
			perror("Binding failed");
			throw SSLError("Binding failed");
		}

		if(::listen(handle(), 1) == -1) {
			perror("Listening failed");
			throw SSLError("Listening failed");
		}

	}

	void Listener::accept(Socket &sock) {

		if(handle() == invalidSocket || !_isSetup) {
			throw SSLError("Listener not set up");
		}

		sock.close();

		sockaddr_in addr;
		socklen_t len = sizeof(addr);

		SockHandl newClient = ::accept(handle(), (sockaddr*)(&addr), &len);

		sock._ctx = context();
		sock._fd = newClient;
		sock._addr = addr;
		sock._addrSet = true;
		sock.setUpSSL();

		int ret;
		do {

			ret = SSL_accept(sock.ssl());

			if(ret < 0) {
				int err = SSL_get_error(sock.ssl(), ret);

				switch (err) {
					case SSL_ERROR_WANT_READ:
						break;

					case 5:
						perror("SSL Handshake");
						throw SSLError("Handshake failed");
					default:
						throw SSLError("Handshake failed");

				}
			}

		} while(ret <= 0);

	}

}
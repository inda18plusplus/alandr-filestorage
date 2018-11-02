//
// Created by lunar on 16/10/18.
//

#include "Socket.hpp"

#include <iostream>

#include <unistd.h>
#include <errno.h>
#include <cstdio>
#include <cstring>

#include "SSLError.hpp"

namespace SSLWrap {

	Socket::Socket() : Socket(nullptr) {

	}

	Socket::Socket(std::shared_ptr<Context> ctx) :
	        Socket(std::move(ctx), invalidSocket, {AF_INET, 0, {0}, {0}}) {

		_addrSet = false;
		create();

	}
	Socket::Socket(std::shared_ptr<Context> ctx, const SSLWrap::IpAddress &ipAddr, uint16_t port) :
	        Socket(std::move(ctx), invalidSocket, {AF_INET, htons(port), ipAddr.addr(), {0}}) {

		_addrSet = true;
		create();

	}
	Socket::Socket(std::shared_ptr<Context> ctx, SSLWrap::SockHandl handle, const sockaddr_in &addr) :
		_fd(handle), _ctx(std::move(ctx)), _addr(addr) {

		_addrSet = true;

	}

	Socket::~Socket() {
		close();
	}

	void Socket::create() {

		close();

		if((_fd = socket(PF_INET, SOCK_STREAM, 0)) == invalidSocket) {
			perror("Socket creation failed");
			throw SSLError("Socket creation error");
		}

	}

	void Socket::connect() {

		if (!_addrSet) {
			throw SSLError("Address not set");
		}

		if(_fd == invalidSocket)
			create();

		if(::connect(_fd, (sockaddr*)(&_addr), sizeof(_addr)) == -1) {
			perror("Connecting failed");
			throw SSLError("Connecting failed");
		}

		setUpSSL();

		if(int ret; (ret = SSL_connect(ssl())) < 0) {
			int err = SSL_get_error(ssl(), ret);
			switch (err) {
				case 5:
					perror("SSL handshake");
					throw SSLError("SSL handshake failed");
				default:
					std::cerr << err << '\n';
					throw SSLError("SSL handshake failed");
			}
		}

	}
	void Socket::connect(const SSLWrap::IpAddress &ipAddr, uint16_t port) {

		_addr.sin_addr = ipAddr.addr();
		_addr.sin_port = htons(port);

		_addrSet = true;

		return connect();

	}

	void Socket::close() {

		tearDownSSL();

		if(_fd != invalidSocket) {
			::close(_fd);
			_fd = invalidSocket;
		}
	}

	void Socket::read(void *buf, int n) {

		int ret = 0;
		if((ret = SSL_read(_ssl, buf, n)) <= 0) {
			int err = SSL_get_error(_ssl, ret);
			if(err == SSL_ERROR_SYSCALL) {
				perror("SSL_read failed");
			}
			std::cerr << "Read failed\n";
		}

	}
	void Socket::write(const void *buf, std::size_t len) {

		SSL_write(_ssl, buf, len);

	}

	void Socket::send(const SSLWrap::Packet &packet) {

		std::size_t size = packet.size();
		const void* packetPata = packet.data();

		uint32_t packetSize = htonl(size);

		auto data = new unsigned char[size + sizeof(packetSize)];

		memcpy(data, &packetSize, sizeof(packetSize));
		memcpy(data + sizeof(packetSize), packetPata, size);

		write(data, sizeof(packetSize) + size);

	}
	void Socket::receive(SSLWrap::Packet &packet) {

		packet.clear();

		uint32_t packetSize = 0;
		read((char *) &packetSize, sizeof(uint32_t));
		packetSize = ntohl(packetSize);

		std::vector<unsigned char> data;
		data.resize(packetSize);

		read(data.data(), packetSize);

		packet._data = std::move(data);

	}

	void Socket::setUpSSL() {

		tearDownSSL();

		_ssl = _ctx->newSSL();
		_bio = BIO_new_socket(_fd, BIO_NOCLOSE);
		SSL_set_bio(_ssl, _bio, _bio);

		SSL_set_mode(_ssl, SSL_MODE_AUTO_RETRY);

	}
	void Socket::tearDownSSL() {

		if(_bio)
			BIO_free(_bio);

	}

}

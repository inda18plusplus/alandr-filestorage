//
// Created by lunar on 16/10/18.
//

#ifndef FILESTORAGE_SECURESOCKET_HPP
#define FILESTORAGE_SECURESOCKET_HPP

#include <memory>
#include <string>

#include <arpa/inet.h>
#include <openssl/ssl.h>
#include <sys/socket.h>

#include "Context.hpp"
#include "SockHandl.hpp"
#include "IpAddress.hpp"
#include "Packet.hpp"

namespace SSLWrap {

	class Listener;

	class Socket {
		friend class Listener;

	public:
		static const SockHandl invalidSocket = -1;

	private:
		SockHandl _fd = invalidSocket;
		BIO *_bio = nullptr;
		SSL *_ssl = nullptr;

		std::shared_ptr<Context> _ctx = nullptr;

		sockaddr_in _addr;
		bool _addrSet;

	protected:
		Socket(std::shared_ptr<Context> ctx, SSLWrap::SockHandl handle, const sockaddr_in &addr); //Meant for the listener

		SockHandl handle() { return _fd; }

		void setPort(uint16_t port) { _addr.sin_port = htons(port); }

	public:
		Socket();

		Socket(std::shared_ptr<Context> ctx);
		Socket(std::shared_ptr<Context> ctx, const SSLWrap::IpAddress &ipAddr, uint16_t port);

		virtual ~Socket();

		Socket(const Socket&) = delete; //Lets not keep any duplicates around
		Socket(Socket&&) = default;

		Socket &operator=(Socket&) = delete;
		Socket &operator=(Socket&&) = default;

		void create();

		void connect();
		void connect(const IpAddress &ipAddr, uint16_t port);

		void close();

		void read(void *buf, int n);
		void write(const void *buf, std::size_t len);

		void receive(Packet &packet);
		void send(const Packet& packet);

		const sockaddr_in& address() const { return _addr; }

		void setUpSSL();
		void tearDownSSL();

		SSL*& ssl() { return _ssl; }
		BIO*& bio() { return _bio; }

		std::shared_ptr<Context> context() { return _ctx; }

	};

}


#endif //FILESTORAGE_SECURESOCKET_HPP

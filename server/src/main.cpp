#include <iostream>

#include <cstdio>
#include <arpa/inet.h>
#include <cerrno>

#include <openssl/ssl.h>

#include "Server.hpp"

int main() {
	SSL_library_init();
	SSL_load_error_strings();

	//auto ctx = new SSLWrap::ServerContext("cert/key.pem", "cert/cert.pem");

	Server server;
	server.start();

	return 0;

}
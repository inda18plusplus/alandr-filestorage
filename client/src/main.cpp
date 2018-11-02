#include <cstdio>
#include <iostream>

#include <openssl/ssl.h>
#include <openssl/err.h>

#include <SSLWrap/Socket.hpp>
#include <SSLWrap/ClientContext.hpp>
#include <SSLWrap/Packet.hpp>

#include "File.hpp"
#include "Client.hpp"

int main() {
	SSL_library_init();
	SSL_load_error_strings();

	Client c(fs::path{"config.xml"});
	c.run();

}
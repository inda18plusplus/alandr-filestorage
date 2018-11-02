#include <SSLWrap/init.hpp>

#include "Server.hpp"

int main() {
	SSLWrap::init();

	Server server;
	server.start();

	SSLWrap::cleanup();

	return 0;
}
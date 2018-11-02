#include <SSLWrap/init.hpp>

#include "Client.hpp"

int main() {
	SSLWrap::init();

	Client c(fs::path{"config.xml"});
	c.run();

	SSLWrap::cleanup();

	return 0;
}
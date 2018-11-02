//
// Created by lunar on 25/10/18.
//

#ifndef FILESTORAGE_SERVERSERCURECONTEXT_HPP
#define FILESTORAGE_SERVERSERCURECONTEXT_HPP

#include "Context.hpp"

#include <string>

namespace SSLWrap {

	class ServerContext : public Context {

		ServerContext();

	public:
		ServerContext(const std::string &key, const std::string &cert);

	};

}


#endif //FILESTORAGE_SERVERSERCURECONTEXT_HPP

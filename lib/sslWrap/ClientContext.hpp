//
// Created by lunar on 25/10/18.
//

#ifndef FILESTORAGE_SECURECONTEXT_HPP
#define FILESTORAGE_SECURECONTEXT_HPP

#include "Context.hpp"

#include <filesystem>
#include <vector>

#include <openssl/ssl.h>

namespace SSLWrap {

	class ClientContext : public Context {
		ClientContext();

	public:
		ClientContext(int filec, char **files, int pathc, char **paths);

		ClientContext(const std::vector<std::filesystem::path> &files, const std::vector<std::filesystem::path> &paths);

	};

}

#endif //FILESTORAGE_SECURECONTEXT_HPP

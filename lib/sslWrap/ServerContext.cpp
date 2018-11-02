//
// Created by lunar on 25/10/18.
//

#include "ServerContext.hpp"

#include <openssl/ssl.h>

namespace SSLWrap {

	ServerContext::ServerContext() : Context(SSLv23_server_method()) {

	}

	ServerContext::ServerContext(const std::string &key, const std::string &cert) : ServerContext() {

		SSL_CTX_use_PrivateKey_file(ctx(), key.c_str(), SSL_FILETYPE_PEM);
		SSL_CTX_use_certificate_file(ctx(), cert.c_str(), SSL_FILETYPE_PEM);

	}

}
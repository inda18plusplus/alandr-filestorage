//
// Created by lunar on 25/10/18.
//

#ifndef FILESTORAGE_SECURECLIENTCONTEXT_HPP
#define FILESTORAGE_SECURECLIENTCONTEXT_HPP

#include <openssl/ssl.h>

namespace SSLWrap {

	class Context {

		SSL_CTX *_ctx;

	protected:
		explicit Context(const SSL_METHOD *method);

		SSL_CTX *ctx() { return _ctx; }

	public:
		virtual ~Context();

		SSL* newSSL() { return SSL_new(_ctx); }

	};

}


#endif //FILESTORAGE_SECURECLIENTCONTEXT_HPP

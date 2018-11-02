//
// Created by lunar on 25/10/18.
//

#include "Context.hpp"

namespace SSLWrap {

	Context::Context(const SSL_METHOD *method) {

		_ctx = SSL_CTX_new(method);

	}

	Context::~Context() {

		SSL_CTX_free(_ctx);

	}

}
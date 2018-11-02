//
// Created by lunar on 25/10/18.
//

#include "ClientContext.hpp"

#include <cstring>

namespace SSLWrap {

	namespace fs = std::filesystem;

	ClientContext::ClientContext() : Context(SSLv23_client_method()) {
	}

	ClientContext::ClientContext(int filec, char **files, int pathc, char **paths) : ClientContext() {

		for (int i = 0; i < filec; ++i) {
			SSL_CTX_load_verify_locations(ctx(), files[i], nullptr);
		}
		for (int i = 0; i < pathc; ++i) {
			SSL_CTX_load_verify_locations(ctx(), nullptr, paths[i]);
		}

	}

	ClientContext::ClientContext(const std::vector<std::filesystem::path> &files, const std::vector<fs::path> &paths)
			: ClientContext() {

		for (const auto &f : files) {
			SSL_CTX_load_verify_locations(ctx(), f.string().c_str(), nullptr);
		}
		for (const auto &p : paths) {
			SSL_CTX_load_verify_locations(ctx(), nullptr, p.string().c_str());
		}

	}

}
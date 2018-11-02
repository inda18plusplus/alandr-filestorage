//
// Created by lunar on 26/10/18.
//

#include "SSLError.hpp"

namespace SSLWrap {

	SSLError::SSLError(const std::string &what) : std::runtime_error(what) {

	}

}
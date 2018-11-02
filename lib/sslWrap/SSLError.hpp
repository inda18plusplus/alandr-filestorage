//
// Created by lunar on 26/10/18.
//

#ifndef FILESTORAGE_SSLERROR_HPP
#define FILESTORAGE_SSLERROR_HPP

#include <stdexcept>

namespace SSLWrap {

	class SSLError : public std::runtime_error {

	public:
		SSLError(const std::string &what);

	};

}


#endif //FILESTORAGE_SSLERROR_HPP

//
// Created by lunar on 01/11/18.
//

#ifndef FILESTORAGE_STRINGSOURCE_HPP
#define FILESTORAGE_STRINGSOURCE_HPP

#include "LeafSource.hpp"

class StringSource : public LeafSource {

private:
	std::string _source;

public:
	explicit StringSource(const std::string &source);

	std::string hash() override;

};


#endif //FILESTORAGE_STRINGSOURCE_HPP

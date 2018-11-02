//
// Created by lunar on 01/11/18.
//

#ifndef FILESTORAGE_LEAFSOURCE_HPP
#define FILESTORAGE_LEAFSOURCE_HPP

#include <string>

class LeafSource {

protected:
	LeafSource() = default;

public:
	virtual std::string hash() = 0;

};


#endif //FILESTORAGE_LEAFSOURCE_HPP

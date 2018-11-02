//
// Created by lunar on 01/11/18.
//

#ifndef FILESTORAGE_FILESOURCE_HPP
#define FILESTORAGE_FILESOURCE_HPP

#include "LeafSource.hpp"

#include "File.hpp"

class FileSource : public LeafSource {

	std::shared_ptr<File> _file;

public:
	explicit FileSource(std::shared_ptr<File> f);

	std::string hash() override;

};


#endif //FILESTORAGE_FILESOURCE_HPP

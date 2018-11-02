//
// Created by lunar on 01/11/18.
//

#include "FileSource.hpp"

#include <cryptopp/sha.h>

FileSource::FileSource(std::shared_ptr<File> f) : LeafSource(), _file(f) {



}

std::string FileSource::hash() {

	if(_file) {

		CryptoPP::SHA256 sha;
		std::string ret(sha.DIGESTSIZE, '\0');

		sha.CalculateDigest((byte*)ret.data(), (byte*)_file->data().c_str(), _file->data().length());

		return std::move(ret);

	} else {
		return "";
	}

}

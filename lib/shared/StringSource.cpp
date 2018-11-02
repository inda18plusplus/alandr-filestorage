//
// Created by lunar on 01/11/18.
//

#include "StringSource.hpp"

#include <cryptopp/sha.h>

StringSource::StringSource(const std::string &source) : LeafSource(), _source(source) {



}

std::string StringSource::hash() {

	CryptoPP::SHA256 sha;
	std::string ret(sha.DIGESTSIZE, '\0');

	sha.CalculateDigest((byte*)ret.data(), (byte*)_source.c_str(), _source.length());

	return std::move(ret);

}

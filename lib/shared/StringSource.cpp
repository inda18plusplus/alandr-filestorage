//
// Created by lunar on 01/11/18.
//

#include "StringSource.hpp"

#include <cryptopp/sha.h>

StringSource::StringSource(const std::string &source) : LeafSource(), _source(source) {



}

std::string StringSource::hash() {

	return _source;

}

//
// Created by lunar on 01/11/18.
//

#include "Leaf.hpp"

#include <cryptopp/sha.h>

#include "MerkleTree.hpp"
#include "StringSource.hpp"

Leaf::Leaf(std::weak_ptr<Node> parent) : Node(std::move(parent)) {

	_source = std::make_unique<StringSource>("");

}

std::string Leaf::hash() const {

	return _source->hash();

}

void Leaf::registerLeaf() {

	tree()->registerLeaf(std::dynamic_pointer_cast<Leaf>(getThis().lock()));

}

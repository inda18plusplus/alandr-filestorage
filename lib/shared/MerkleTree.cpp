//
// Created by lunar on 01/11/18.
//

#include "MerkleTree.hpp"

#include <cmath>

#include <cryptopp/sha.h>

#include "FileSource.hpp"
#include "StringSource.hpp"

MerkleTree::MerkleTree() {

	_root = std::make_shared<Node>(std::shared_ptr<Node>{nullptr});

	_root->setTree(this);
	_root->setThis(_root);
	_root->createTree(maxLevels);

}
MerkleTree::~MerkleTree() {

}

void MerkleTree::registerLeaf(std::shared_ptr<Leaf> l) {

	_leaves.push_back(l);

}

const std::string& MerkleTree::topHash() {

	if(_dirty) {
		_hash = _root->hash();
		_dirty = false;
	}

	return _hash;
}
std::string MerkleTree::topHash() const {

	return std::move(_root->hash());
}

void MerkleTree::addFile(int id, std::shared_ptr<File> f) {

	auto& leaf = _leaves[id];
	leaf->setSource(std::make_unique<FileSource>(f));

}
void MerkleTree::addString(int id, const std::string &str) {

	auto& leaf = _leaves[id];
	leaf->setSource(std::make_unique<StringSource>(str));

}
void MerkleTree::removeFile(int id) {

	addString(id, "");

}


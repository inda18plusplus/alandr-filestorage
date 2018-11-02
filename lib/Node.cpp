//
// Created by lunar on 01/11/18.
//

#include "Node.hpp"

#include <cryptopp/sha.h>

#include "Leaf.hpp"

Node::Node(std::weak_ptr<Node> parent) : _parent(std::move(parent)) {

}

MerkleTree* Node::tree() {

	if(_tree)
		return _tree;

	if(_parent.expired()) {
		return nullptr;
	}

	return _parent.lock()->tree();

}

void Node::createTree(int levels) {

	if(levels == 2) {
		auto first = std::make_shared<Leaf>(_this);
		first->setThis(first);
		first->registerLeaf();
		_children.first = first;

		auto second = std::make_shared<Leaf>(_this);
		second->setThis(second);
		second->registerLeaf();
		_children.second = second;

		return;
	}

	_children.first = std::make_shared<Node>(_this);
	_children.first->setThis(_children.first);
	_children.first->createTree(levels - 1);

	_children.second = std::make_shared<Node>(_this);
	_children.second->setThis(_children.second);
	_children.second->createTree(levels - 1);

}

std::string Node::hash() const {

	CryptoPP::SHA256 sha;
	std::string ret(sha.DIGESTSIZE, '\0');
	std::string hashSum;

	hashSum = _children.first->hash() + _children.second->hash();

	sha.CalculateDigest((byte*)ret.data(), (byte*)hashSum.c_str(), hashSum.length());

	return std::move(ret);

}
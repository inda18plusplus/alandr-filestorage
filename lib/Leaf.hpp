//
// Created by lunar on 01/11/18.
//

#ifndef FILESTORAGE_LEAF_HPP
#define FILESTORAGE_LEAF_HPP

#include "Node.hpp"
#include "File.hpp"
#include "LeafSource.hpp"

class Leaf : public Node {

	std::unique_ptr<LeafSource> _source;

public:
	explicit Leaf(std::weak_ptr<Node> parent);

	void setLeftChild (std::shared_ptr<Node>) override {};
	void setRightChild(std::shared_ptr<Node>) override {};

	std::string hash() const override;

	void registerLeaf();

	void setSource(std::unique_ptr<LeafSource> src) { _source = std::move(src); }

};


#endif //FILESTORAGE_LEAF_HPP

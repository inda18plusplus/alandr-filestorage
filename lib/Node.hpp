//
// Created by lunar on 01/11/18.
//

#ifndef FILESTORAGE_NODE_HPP
#define FILESTORAGE_NODE_HPP

#include <memory>

class MerkleTree;
class Node {
	std::weak_ptr<Node> _this;
	std::weak_ptr<Node> _parent;

	MerkleTree* _tree = nullptr;

	std::pair<std::shared_ptr<Node>, std::shared_ptr<Node>> _children;

public:
	explicit Node(std::weak_ptr<Node> parent);

	virtual void setLeftChild (std::shared_ptr<Node> n) { _children.first  = std::move(n); };
	virtual void setRightChild(std::shared_ptr<Node> n) { _children.second = std::move(n); };

	void setThis(const std::weak_ptr<Node>& ptr) { _this = ptr; }

	void createTree(int levels);

	virtual std::string hash() const;

	MerkleTree* tree();
	void setTree(MerkleTree* tree) { _tree = tree; }

	std::weak_ptr<Node> parent() { return _parent; }
	void setParent(std::weak_ptr<Node> p) { _parent = p; }

protected:
	auto getThis() { return _this; }
};


#endif //FILESTORAGE_NODE_HPP

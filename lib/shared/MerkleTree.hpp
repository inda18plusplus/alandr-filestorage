//
// Created by lunar on 01/11/18.
//

#ifndef FILESTORAGE_MERKELTREE_HPP
#define FILESTORAGE_MERKELTREE_HPP

#include <array>
#include <map>
#include <memory>
#include <utility>

#include "File.hpp"
#include "Node.hpp"
#include "Leaf.hpp"

class MerkleTree {

	static const int maxLevels = 4;

	std::shared_ptr<Node> _root;

	std::vector<std::shared_ptr<Leaf>> _leaves;

	std::string _hash;
	bool _dirty = true;

public:
	MerkleTree();
	~MerkleTree();

	const std::string& topHash();
	std::string topHash() const;

	void addFile(int id, std::shared_ptr<File> f);
	void addString(int id, const std::string& str);
	void removeFile(int id);

	const Node& getLeaf(int id) const { return *_leaves[id]; }
	int getNumLeaves() const { return _leaves.size(); }

	void registerLeaf(std::shared_ptr<Leaf> l);

};


#endif //FILESTORAGE_MERKELTREE_HPP

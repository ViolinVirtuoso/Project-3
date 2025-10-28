//
// Created by samue on 10/16/2025.
//

#ifndef PROJECT_3_TREENODE_HPP
#define PROJECT_3_TREENODE_HPP

#include <string>

class TreeNode {
public:
    std::string word;
    std::size_t count;
    TreeNode* left;
    TreeNode* right;

    // constructor that takes 1 parameter - the word - only used by BinSearchTree
    TreeNode(const std::string_view newWord) : word(newWord), count(1), left(nullptr), right(nullptr) {}

    // constructor that takes 2 parameters - the word and the frequency - only used by PriorityQueue
    TreeNode(const std::string& newWord, size_t newCount) : word(newWord), count(newCount), left(nullptr), right(nullptr) {}

    // constructor that takes 2 parameters - a left and right node - for merging two nodes to make a parent, not used yet
    TreeNode(TreeNode* newLeft, TreeNode* newRight) : word(""), count(newLeft->count + newRight->count), left(newLeft), right(newRight) {
        word = (newLeft->word < newRight->word) ? newLeft->word : newRight->word;
    }

    // function that checks if a node is a leaf
    bool isLeaf() const {
        return left == nullptr && right == nullptr;
    }

    ~TreeNode() = default;
};


#endif //PROJECT_3_TREENODE_HPP
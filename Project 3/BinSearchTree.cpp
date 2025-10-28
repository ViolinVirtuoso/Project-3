//
// Created by samue on 10/16/2025.
//

#include "BinSearchTree.hpp"
#include <cstdint>

BinSearchTree::~BinSearchTree() {
    destroy(root_);
}

void BinSearchTree::insert(const std::string& word) {
    // call the insertHelper function to do the actual work
    root_ = insertHelper(root_, word);
}

void BinSearchTree::bulkInsert(const std::vector<std::string>& words) {
    // iterate over words and use the insert function to insert all of them
    for (const auto& word : words) {
        insert(word);
    }
}

bool BinSearchTree::contains(const std::string_view word) const noexcept {
    // call the contains function to do the actual work
    return findNode(root_, word) != nullptr;
}

std::optional<int> BinSearchTree::countOf(const std::string_view word) const noexcept {
    // call the findNode function and store its return value in a temporary pointer
    const TreeNode* node = findNode(root_, word);

    // if it's null, then return nullopt. otherwise, return its frequency
    if (node == nullptr)
        return std::nullopt;
    else
        return node->count;
}

void BinSearchTree::inorderCollect(std::vector<std::pair<std::string, int> > &out) const {
    // clear any previous output, then call a helper function to do the heavy lifting
    out.clear();
    inorderHelper(root_, out);
}

std::size_t BinSearchTree::size() const noexcept {
    // call helper function to do heavy lifting
    return sizeHelper(root_);
}

unsigned BinSearchTree::height() const noexcept {
    // if the root is null, then height is 0.
    // otherwise, call helper function to actually figure out the height
    if (root_ == nullptr)
        return 0;
    else
        return heightHelper(root_);
}

std::size_t BinSearchTree::minFrequency() const noexcept {
    // if the root is null, then the minimum frequency is 0.
    // otherwise, call helper function to actually figure it out
    if (root_ == nullptr)
        return 0;
    return minFreqHelper(root_);
}

std::size_t BinSearchTree::maxFrequency() const noexcept {
    // if the root is null, then the maximum frequency is 0.
    // otherwise, call helper function to actually figure it out
    if (root_ == nullptr)
        return 0;
    return maxFreqHelper(root_);
}





void BinSearchTree::destroy(TreeNode* node) noexcept {
    // if the node is null, there's nothing to destroy
    if (node == nullptr)
        return;

    // otherwise, there's stuff to destroy, so destroy it
    destroy(node->left);
    destroy(node->right);
    delete node;
}

TreeNode* BinSearchTree::insertHelper(TreeNode* node, const std::string_view word) {
    // if the node is null, then we have reached the end of the tree.
    // so, make a new node there
    if (node == nullptr)
        return new TreeNode(word);

    // otherwise, find where it should go (to the left or the right)
    // if it happens to be a duplicate, don't add a new node, just increment that node's frequency
    if (word < node->word)
        node->left = insertHelper(node->left, word);
    else if (word > node->word)
        node->right = insertHelper(node->right, word);
    else
        node->count += 1;

    return node;
}

const TreeNode* BinSearchTree::findNode(const TreeNode* node, std::string_view word) noexcept {
    // if the node is null, then there's nothing to find
    if (node == nullptr)
        return nullptr;

    // otherwise, find it
    if (word < node->word)
        return findNode(node->left, word);
    else if (word > node->word)
        return findNode(node->right, word);
    else
        return node;
}

void BinSearchTree::inorderHelper(const TreeNode* node, std::vector<std::pair<std::string,int>>& out) {
    // if the node is null, there's nothing left to do
    if (node == nullptr)
        return;

    // otherwise, there's stuff left to do, so insert the values in order
    inorderHelper(node->left, out);
    out.emplace_back(node->word, node->count);
    inorderHelper(node->right, out);
}

size_t BinSearchTree::sizeHelper(const TreeNode* node) noexcept {
    // if the node is null, size is 0
    if (node == nullptr)
        return 0;
    // otherwise, get the size recursively
    return 1 + sizeHelper(node->left) + sizeHelper(node->right);
}

unsigned BinSearchTree::heightHelper(const TreeNode* node) noexcept {
    // if the node is null, height is 0
    if (node == nullptr)
        return 0;
    // otherwise, get the height recursively
    return 1 + std::max(heightHelper(node->left), heightHelper(node->right));
}

size_t BinSearchTree::minFreqHelper(const TreeNode* node) noexcept {
    // if the node is null, return SIZE_MAX (the largest value size_t can hold).
    // because 0 might be a valid frequency, we return the largest possible value
    // to avoid confusion.
    if (node == nullptr)
        return SIZE_MAX;

    // otherwise, find the minimum frequency.
    size_t leftMin = minFreqHelper(node->left);
    size_t rightMin = minFreqHelper(node->right);
    size_t currentMin = node->count;

    size_t result = currentMin;
    if (leftMin < result)
        result = leftMin;
    if (rightMin < result)
        result = rightMin;

    return result;
}

size_t BinSearchTree::maxFreqHelper(const TreeNode* node) noexcept {
    // if the node is null, return 0.
    if (node == nullptr)
        return 0;

    // otherwise, find the maximum frequency.
    size_t leftMax = maxFreqHelper(node->left);
    size_t rightMax = maxFreqHelper(node->right);
    size_t currentMax = node->count;

    size_t result = currentMax;
    if (leftMax > result)
        result = leftMax;
    if (rightMax > result)
        result = rightMax;

    return result;
}
//
// Created by samue on 10/17/2025.
//

#include "PriorityQueue.hpp"
#include <algorithm>

PriorityQueue::PriorityQueue(std::vector<TreeNode *> nodes) : items_(std::move(nodes)) {
    std::sort(items_.begin(), items_.end(), higherPriority);
}

std::size_t PriorityQueue::size() const noexcept {
    return items_.size();
}

bool PriorityQueue::empty() const noexcept {
    return items_.empty();
}

TreeNode* PriorityQueue::findMin() const noexcept {
    // if the vector is empty, there's nothing to find.
    if (items_.empty())
        return nullptr;

    // otherwise, return the minimum value (at the back).
    return items_.back();
}

TreeNode* PriorityQueue::extractMin() noexcept {
    // if the vector is empty, there's nothing to extract.
    if (items_.empty())
        return nullptr;

    // otherwise, extract the minimum value (at the back).
    TreeNode* minNode = items_.back();
    items_.pop_back();
    return minNode;
}

void PriorityQueue::deleteMin() noexcept {
    // if the vector isn't empty, delete the minimum value.
    if (!items_.empty())
        items_.pop_back();
}

void PriorityQueue::insert(TreeNode* node) {
    // find the right place to insert...
    auto iterator = items_.begin();
    while (iterator != items_.end() && higherPriority(*iterator, node)) {
        iterator += 1;
    }

    // then insert it!
    items_.insert(iterator, node);
}

void PriorityQueue::print(std::ostream& os) const {
    // print size first
    os << "PriorityQueue (size=" << items_.size() << ")" << std::endl;

    // then, print the elements
    for (int i = 0; i < items_.size(); ++i) {
        const TreeNode* node = items_[i];
        os << "  [" << i << "] ";

        if (node->isLeaf())
            os << "\"" << node->word << "\" (freq=" << node->count << ")";
        else
            os << "(Note: Internal node) key_word=\"" << node->word << "\" (freq=" << node->count << ")";

        os << std::endl;
    }

    // (just in case, checking we reached the end of the vector)
    if (items_.empty())
        os << "  (empty)" << std::endl;
}

bool PriorityQueue::higherPriority(const TreeNode* a, const TreeNode* b) noexcept {
    // check with frequency first, then do lexicographical value for ties
    if (a->count != b->count)
        return a->count > b->count;
    else
        return a->word < b->word;
}

bool PriorityQueue::isSorted() const {
    // loop over the whole vector and check for anything out of place
    for (int i = 1; i < items_.size(); ++i) {
        if (!higherPriority(items_[i - 1], items_[i]) && items_[i - 1] != items_[i]) {
            if (higherPriority(items_[i], items_[i - 1]))
                return false;
        }
    }

    return true;
}
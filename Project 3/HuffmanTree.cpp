//
// Created by samue on 10/26/2025.
//

#include "HuffmanTree.hpp"
#include "PriorityQueue.hpp"
#include <iomanip>

HuffmanTree HuffmanTree::buildFromCounts(const std::vector<std::pair<std::string, int>> &counts) {
    HuffmanTree tree;

    // if the vector from the BST is empty, we have an empty tree
    if (counts.empty())
        return tree;

    // otherwise, we need to make a vector of leaves, and reserve enough size for it
    std::vector<TreeNode*> leaves;
    leaves.reserve(counts.size());

    // put all the elements into our vector
    for (const auto& [word, count] : counts) {
        leaves.push_back(new TreeNode(word, static_cast<size_t>(count)));
    }

    // edge case: only 1 element
    if (leaves.size() == 1) {
        tree.root_ = leaves[0];
        return tree;
    }

    // make a priority queue with our vector
    PriorityQueue pq(leaves);

    // merge the nodes to make the huffman tree
    while (pq.size() >= 2) {
        TreeNode* a = pq.extractMin();
        TreeNode* b = pq.extractMin();

        TreeNode* parent = new TreeNode(a, b);

        pq.insert(parent);
    }

    // grab the root
    tree.root_ = pq.extractMin();

    return tree;
}


HuffmanTree::~HuffmanTree() {
    destroy(root_);
}


void HuffmanTree::assignCodes(std::vector<std::pair<std::string, std::string>>& out) const {
    // make sure the vector is empty before we fill it up
    out.clear();

    // if we don't have a tree, then rest in peace
    if (root_ == nullptr) {
        return;
    }

    // otherwise, we got work to do
    std::string prefix;
    assignCodesDFS(root_, prefix, out);
}


error_type HuffmanTree::writeHeader(std::ostream& os) const {
    // check if the state of the object is fine
    if (!os.good())
        return UNABLE_TO_OPEN_FILE_FOR_WRITING;

    // if our tree is empty, we got nothing to do
    if (root_ == nullptr)
        return NO_ERROR;

    // otherwise, do the actual work
    std::string prefix;
    writeHeaderPreorder(root_, os, prefix);

    // if something happened, flag it
    if (os.fail())
        return FAILED_TO_WRITE_FILE;

    return NO_ERROR;
}


error_type HuffmanTree::encode(const std::vector<std::string>& tokens, std::ostream& os_bits, int wrap_cols) const {
    // check if the state of the object is fine
    if (!os_bits.good())
        return UNABLE_TO_OPEN_FILE_FOR_WRITING;

    // check if our tree is already empty
    if (root_ == nullptr)
        return NO_ERROR;

    // then, get the words and codes and put them into a vector
    std::vector<std::pair<std::string, std::string>> codeVector;
    assignCodes(codeVector);

    // put the words and codes into a map
    std::map<std::string, std::string> codes;
    for (const auto& [word, code] : codeVector)
        codes[word] = code;

    int col = 0;

    // finally, put the tokens into the file
    for (const auto& token : tokens) {
        // find the token
        auto iterator = codes.find(token);
        // if it didn't work, then throw an error
        if (iterator == codes.end()) {
            std::cerr << "Error: Token '" << token << "' not found in codebook\n";
            return FAILED_TO_WRITE_FILE;
        }

        // otherwise, we have found the token
        // get the code of that token
        const std::string& code = iterator->second;

        // then, put it into the file
        for (char c : code) {
            os_bits << c;
            col++;

            // check to wrap the edge
            if (col >= wrap_cols) {
                os_bits << '\n';
                col = 0;
            }
        }
    }

    // put an extra line at the end if necessary
    if (col > 0)
        os_bits << '\n';

    // one last error check
    if (os_bits.fail())
        return FAILED_TO_WRITE_FILE;

    return NO_ERROR;
}


void HuffmanTree::destroy(TreeNode* n) noexcept {
    // if there's nothing to destroy, then don't destroy
    if (n == nullptr)
        return;

    // otherwise, CONTINUE THE DESTRUCTION!
    destroy(n->left);
    destroy(n->right);
    delete n;
}


void HuffmanTree::assignCodesDFS(const TreeNode *n, std::string &prefix, std::vector<std::pair<std::string, std::string> > &out) {
    // base case
    if (n == nullptr)
        return;

    // if we've reached a leaf, put its code in the out vector
    if (n->isLeaf()) {
        std::string code = prefix.empty() ? "0" : prefix;
        out.emplace_back(n->word, code);
        return;
    }

    // otherwise, we need to find a leaf
    // explore both left and right
    prefix += '0';
    assignCodesDFS(n->left, prefix, out);
    prefix.pop_back();

    prefix += '1';
    assignCodesDFS(n->right, prefix, out);
    prefix.pop_back();
}


void HuffmanTree::writeHeaderPreorder(const TreeNode* n, std::ostream& os, std::string& prefix) {
    // base case
    if (n == nullptr)
        return;

    // if we found a leaf, put its code in the ostream
    if (n->isLeaf()) {
        std::string code = prefix.empty() ? "0" : prefix;
        os << n->word << ' ' << code << '\n';
        return;
    }

    // otherwise, find that leaf already
    prefix += '0';
    writeHeaderPreorder(n->left, os, prefix);
    prefix.pop_back();

    prefix += '1';
    writeHeaderPreorder(n->right, os, prefix);
    prefix.pop_back();
}
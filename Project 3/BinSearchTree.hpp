#ifndef BINSEARCHTREE_HPP
#define BINSEARCHTREE_HPP
#include <string>
#include <vector>
#include <optional>
#include <utility>
#include "TreeNode.hpp"

#include "utils.hpp"


class BinSearchTree {
public:
    BinSearchTree() = default;
    ~BinSearchTree(); // calls destroy(root_)

    // Insert 'word'; if present, increment its count.
    void insert(const std::string& word);

    // Convenience: loop over insert(word) for each token.
    void bulkInsert(const std::vector<std::string>& words);

    // Queries
    [[nodiscard]] bool contains(std::string_view word) const noexcept;
    [[nodiscard]] std::optional<int> countOf(std::string_view word) const noexcept;

    // In-order traversal (word-lex order) -> flat list for next stage
    void inorderCollect(std::vector<std::pair<std::string,int>>& out) const;

    // Metrics
    [[nodiscard]] std::size_t size() const noexcept;  // distinct words
    [[nodiscard]] unsigned height() const noexcept;   // empty tree = 0
    [[nodiscard]] std::size_t minFrequency() const noexcept;
    [[nodiscard]] std::size_t maxFrequency() const noexcept;

private:
    // TreeNode is defined elsewhere in the project
    TreeNode* root_ = nullptr;

    // Helpers
    static void destroy(TreeNode* node) noexcept;
    static TreeNode* insertHelper(TreeNode *node, std::string_view word);
    static const TreeNode* findNode(const TreeNode* node, std::string_view word) noexcept;
    static void inorderHelper(const TreeNode* node,
                              std::vector<std::pair<std::string,int>>& out);
    static std::size_t sizeHelper(const TreeNode* node) noexcept;
    static unsigned heightHelper(const TreeNode* node) noexcept;
    static size_t minFreqHelper(const TreeNode* node) noexcept;
    static size_t maxFreqHelper(const TreeNode* node) noexcept;
};

#endif

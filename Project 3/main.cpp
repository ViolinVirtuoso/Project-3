#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <vector>
#include <random>
#include <algorithm>
#include <iomanip>

#include "Scanner.hpp"
#include "BinSearchTree.hpp"
#include "PriorityQueue.hpp"
#include "HuffmanTree.hpp"
#include "TreeNode.hpp"
#include "utils.hpp"


int main(int argc, char *argv[]) {

    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <filename>\n";
        return 1;
    }

    const std::string dirName = "input_output";
    const std::string inputFileName = argv[1];
    const std::string inputFileBaseName = baseNameWithoutTxt(inputFileName);

    // Build paths to output files
    const std::string wordTokensFileName = dirName + "/" + inputFileBaseName + ".tokens";
    const std::string freqFileName = dirName + "/" + inputFileBaseName + ".freq";
    const std::string hdrFileName = dirName + "/" + inputFileBaseName + ".hdr";
    const std::string codeFileName = dirName + "/" + inputFileBaseName + ".code";

    // Validate input file, directory, and output files
    if (error_type status; (status = regularFileExistsAndIsAvailable(inputFileName)) != NO_ERROR)
        exitOnError(status, inputFileName);

    if (error_type status; (status = directoryExists(dirName)) != NO_ERROR)
        exitOnError(status, dirName);

    if (error_type status; (status = canOpenForWriting(wordTokensFileName)) != NO_ERROR)
        exitOnError(status, wordTokensFileName);

    if (error_type status; (status = canOpenForWriting(freqFileName)) != NO_ERROR)
        exitOnError(status, freqFileName);

    if (error_type status; (status = canOpenForWriting(hdrFileName)) != NO_ERROR)
        exitOnError(status, hdrFileName);

    if (error_type status; (status = canOpenForWriting(codeFileName)) != NO_ERROR)
        exitOnError(status, codeFileName);


    // ========== STEP 1: TOKENIZE (Scanner) ==========
    std::vector<std::string> tokens;
    Scanner scanner(inputFileName);

    if (error_type status; (status = scanner.tokenize(tokens)) != NO_ERROR)
        exitOnError(status, inputFileName);

    // Write tokens to .tokens file
    if (error_type status; (status = writeVectorToFile(wordTokensFileName, tokens)) != NO_ERROR)
        exitOnError(status, wordTokensFileName);

    // Save original token count before shuffling
    size_t totalTokens = tokens.size();

    // Calculate total letters in input words
    size_t totalLetters = 0;
    for (const auto& token : tokens) {
        totalLetters += token.length();
    }

    // ========== STEP 2: SHUFFLE TOKENS (for balanced BST) ==========
    // Use fixed seed for deterministic results
    std::mt19937 rng(0xC0FFEE);
    std::shuffle(tokens.begin(), tokens.end(), rng);


    // ========== STEP 3: BUILD BST (count frequencies) ==========
    BinSearchTree bst;
    bst.bulkInsert(tokens);

    // Get the in-order traversal (lexicographically sorted by word)
    std::vector<std::pair<std::string, int>> frequencies;
    bst.inorderCollect(frequencies);


    // ========== STEP 4: PRINT BST METRICS to stdout ==========
    std::cout << "Total tokens: " << totalTokens << '\n';
    std::cout << "Distinct words: " << bst.size() << '\n';
    std::cout << "BST height: " << bst.height() << '\n';
    std::cout << "Min frequency: " << bst.minFrequency() << '\n';
    std::cout << "Max frequency: " << bst.maxFrequency() << '\n';


    // ========== STEP 5: WRITE .freq FILE ==========
    // Create temporary nodes JUST for the priority queue (for sorting)
    std::vector<TreeNode*> tempLeavesForFreq;
    tempLeavesForFreq.reserve(frequencies.size());

    for (const auto& [word, count] : frequencies) {
        tempLeavesForFreq.push_back(new TreeNode(word, static_cast<size_t>(count)));
    }

    // Create priority queue (will sort by count desc, word asc)
    PriorityQueue pq(tempLeavesForFreq);

    // Extract all nodes and collect them
    std::vector<TreeNode*> sortedNodes;
    sortedNodes.reserve(pq.size());

    while (!pq.empty()) {
        sortedNodes.push_back(pq.extractMin());
    }

    // Reverse to get highest frequency first
    std::reverse(sortedNodes.begin(), sortedNodes.end());

    // Write to .freq file
    std::ofstream freqFile(freqFileName);
    if (!freqFile.is_open()) {
        std::cerr << "Error: Unable to open " << freqFileName << " for writing\n";

        // Clean up temp nodes
        for (TreeNode* node : tempLeavesForFreq) {
            delete node;
        }
        return 1;
    }

    // Write to file: count word (right-justified count with width 10)
    for (const TreeNode* node : sortedNodes) {
        freqFile << std::setw(10) << node->count << ' ' << node->word << '\n';
    }

    freqFile.close();

    // Clean up the temporary nodes we created for .freq file
    for (TreeNode* node : tempLeavesForFreq) {
        delete node;
    }


    // ========== STEP 6: BUILD HUFFMAN TREE ==========
    // buildFromCounts creates its OWN nodes and takes ownership of them
    HuffmanTree huffmanTree = HuffmanTree::buildFromCounts(frequencies);


    // ========== STEP 7: WRITE .hdr FILE ==========
    std::ofstream hdrFile(hdrFileName);
    if (!hdrFile.is_open()) {
        std::cerr << "Error: Unable to open " << hdrFileName << " for writing\n";
        return 1;
    }

    if (error_type status; (status = huffmanTree.writeHeader(hdrFile)) != NO_ERROR) {
        exitOnError(status, hdrFileName);
    }

    hdrFile.close();


    // ========== STEP 8: ENCODE TOKENS AND WRITE .code FILE ==========
    std::ofstream codeFile(codeFileName);
    if (!codeFile.is_open()) {
        std::cerr << "Error: Unable to open " << codeFileName << " for writing\n";
        return 1;
    }

    if (error_type status; (status = huffmanTree.encode(tokens, codeFile, 80)) != NO_ERROR) {
        exitOnError(status, codeFileName);
    }

    codeFile.close();


    // ========== STEP 9: CALCULATE ENCODED BIT COUNT ==========
    // Reopen the code file to count bits
    std::ifstream codeFileForCount(codeFileName);
    size_t totalBits = 0;
    char ch;
    while (codeFileForCount.get(ch)) {
        if (ch == '0' || ch == '1') {
            totalBits++;
        }
    }
    codeFileForCount.close();

    std::cout << "Total letters in input words: " << totalLetters << '\n';
    std::cout << "Total bits in encoded words: " << totalBits << '\n';


    // ========== STEP 10: CLEANUP ==========
    // The HuffmanTree destructor will automatically clean up all nodes it owns
    // No manual cleanup needed here!

    return 0;
}
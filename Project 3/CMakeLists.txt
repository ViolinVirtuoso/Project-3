cmake_minimum_required(VERSION 4.0)
project(Project_3)

set(CMAKE_CXX_STANDARD 20)

add_executable(Project_3 main.cpp
        Scanner.cpp
        Scanner.hpp
        utils.cpp
        utils.hpp
        BinSearchTree.cpp
        BinSearchTree.hpp
        TreeNode.hpp
        PriorityQueue.cpp
        PriorityQueue.hpp
        HuffmanTree.cpp
        HuffmanTree.hpp
)

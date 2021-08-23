#ifndef HUFFMAN_TREE
#define HUFFMAN_TREE

#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <queue>
#include <bitset>
#include <functional>
#include <iostream>

#define SOT 3
#define EOT 4

struct Node {

    char letter_;
    int freq_;
    Node * left_child_;
    Node * right_child_;

    Node(char letter, int freq) {
        letter_ = letter;
        freq_ = freq;
        left_child_ = nullptr;
        right_child_ = nullptr;
    }

    Node(char letter, int freq, Node * left_child, Node * right_child) {
        letter_ = letter;
        freq_ = freq;
        left_child_ = left_child;
        right_child_ = right_child;
    }

    bool operator<(Node other_node) const {
        return freq_ < other_node.freq_;
    }
};

class HuffmanTree {

    Node * root_;
    std::unordered_map<char, std::string> codes_;
    std::unordered_map<char, int> freqs_;

    void BuildTree();
    void DeleteTree(Node * root);
    void DFSRecurse(Node * root, std::string code);
    std::unordered_map<char, int> CountFreqs(std::string src_file);

    char BitStringToByte(std::string bit_string);
    std::string ByteToBitString(char byte);
    std::vector<char> FreqsToBytes();

public:

    HuffmanTree();
    ~HuffmanTree();

    void EncodeFile(std::string src_file);
    void CompressFile(std::string src_file, std::string dst_file);
    void DecompressFile(std::string src_file, std::string dst_file);
    std::unordered_map<char, std::string> DFSTraversal();
    void Clear();

    Node * GetRoot();
    std::unordered_map<char, std::string> GetCodes();
    std::unordered_map<char, int> GetFreqs();
};

#endif

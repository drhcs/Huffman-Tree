#include "HuffmanTree.h"

void HuffmanTree::BuildTree() {
    auto CompareFreq = [](Node * a, Node * b) {
        return a->freq_ > b->freq_;
    };
    std::priority_queue<Node *, std::vector<Node *>, std::function<bool(Node *, Node *)>> q(CompareFreq);

    for (const auto & entry : freqs_)
    {
        char letter = entry.first;
        int freq = entry.second;
        q.push(new Node(letter, freq));
    }

    while (q.size() >= 2)
    {
        Node * min_freq = q.top();
        q.pop();

        Node * sec_min_freq = q.top();
        q.pop();

        int combined_freq = min_freq->freq_ + sec_min_freq->freq_;

        q.push(new Node(NULL, combined_freq, min_freq, sec_min_freq));
    }

    root_ = q.top();
    q.pop();
}

void HuffmanTree::DeleteTree(Node * root) {

    if (root == nullptr)
    {
        return;
    }

    DeleteTree(root->left_child_);
    DeleteTree(root->right_child_);

    delete root;
}

void HuffmanTree::DFSRecurse(Node * root, std::string code) {

    if (!root->left_child_ && !root->right_child_)
    {
        codes_[root->letter_] = code;
        return;
    }

    DFSRecurse(root->left_child_, code + "0");
    DFSRecurse(root->right_child_, code + "1");

}

std::unordered_map<char, int> HuffmanTree::CountFreqs(std::string src_file) {

    std::ifstream src(src_file, std::ios::binary);

    char letter;
    while (src.get(letter))
    {
        freqs_[letter]++;
    }
    freqs_[EOT]++;

    return freqs_;
}

char HuffmanTree::BitStringToByte(std::string bit_string) {

    std::bitset<8> bits(bit_string);
    unsigned long temp = bits.to_ulong();
    char byte = static_cast<char>(temp);
    return byte;
}

std::string HuffmanTree::ByteToBitString(char byte) {

    std::bitset<8> bits(byte);
    return bits.to_string();
}

std::vector<char> HuffmanTree::FreqsToBytes() {

    std::vector<char> bytes = std::vector<char>();

    for (const auto & entry : freqs_)
    {
        char letter = entry.first;
        int freq = entry.second;

        bytes.push_back(letter);

        std::string bit_string = std::bitset<32>(freq).to_string();

        for (int i = 0; i < 32; i += 8)
        {
            std::string bit_string_chunk = bit_string.substr(i, 8);
            bytes.push_back(BitStringToByte(bit_string_chunk));
        }
    }

    bytes.push_back(SOT);

    return bytes;
}

HuffmanTree::HuffmanTree() {
    root_ = nullptr;
}

HuffmanTree::~HuffmanTree() {
    Clear();
}

void HuffmanTree::EncodeFile(std::string src_file) {
    Clear();
    CountFreqs(src_file);
    BuildTree();
    DFSTraversal();
}

void HuffmanTree::CompressFile(std::string src_file, std::string dst_file) {

    EncodeFile(src_file);

    std::ifstream src(src_file, std::ios::binary);
    std::ofstream dst(dst_file, std::ios::binary);

    std::vector<char> bytes = FreqsToBytes();

    for (const auto & byte : bytes)
    {
        dst.put(byte);
    }

    char letter;
    std::string bit_buffer;
    while (src.get(letter))
    {
        bit_buffer += codes_[letter];
    }

    bit_buffer += codes_[EOT];
    while (bit_buffer.length() % 8 != 0)
    {
        bit_buffer += "0";
    }

    for (int i = 0; i < bit_buffer.length(); i += 8)
    {
        std::string bit_string = bit_buffer.substr(i, 8);
        dst.put(BitStringToByte(bit_string));
    }
}

void HuffmanTree::DecompressFile(std::string src_file, std::string dst_file) {

    Clear();

    std::ifstream src(src_file, std::ios::binary);
    std::ofstream dst(dst_file, std::ios::binary);

    char letter;
    while (src.get(letter) && letter != SOT)
    {
        char freq_key = letter;

        std::string bit_string;
        for (int i = 0; i < 4; i++)
        {
            src.get(letter);
            bit_string += ByteToBitString(letter);
        }

        std::bitset<32> c(bit_string);
        unsigned long freq_count = c.to_ulong();

        freqs_[freq_key] = freq_count;
    }

    BuildTree();

    Node * itr = root_;
    while (src.get(letter))
    {
        std::string bit_string = ByteToBitString(letter);

        for (const auto & bit : bit_string)
        {
            bool traverse_left = bit == '0';

            if (traverse_left)
            {
                itr = itr->left_child_;
            }
            else
            {
                itr = itr->right_child_;
            }

            bool leaf_node = !itr->left_child_ && !itr->right_child_;

            if (leaf_node)
            {
                if (itr->letter_ == EOT)
                {
                    break;
                }
                else
                {
                    dst.put(itr->letter_);
                    itr = root_;
                }
            }
        }
    }
}

std::unordered_map<char, std::string> HuffmanTree::DFSTraversal() {
    DFSRecurse(root_, "");
    return codes_;
}

void HuffmanTree::Clear() {
    DeleteTree(root_);
    root_ = nullptr;
    freqs_.clear();
    codes_.clear();
}

Node * HuffmanTree::GetRoot() {
    return root_;
}

std::unordered_map<char, std::string> HuffmanTree::GetCodes() {
    return codes_;
}

std::unordered_map<char, int> HuffmanTree::GetFreqs() {
    return freqs_;
}

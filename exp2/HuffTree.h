#ifndef HUFFTREE_H
#define HUFFTREE_H

#include "BinTree.h"   // 导入二叉树头文件
#include "Bitmap.h"   // 导入位图头文件
#include <vector>
#include <algorithm>
#include <cctype>     // 引入tolower函数定义
#include <cstddef>    // 引入NULL定义

// Huffman节点类（继承BinNode，存储字符与频率）
class HuffNode : public BinNode<std::pair<char, int> > {
public:
    // 构造函数1：叶子节点（存储字符c与频率freq）
    HuffNode(char c, int freq) : BinNode<std::pair<char, int> >(std::make_pair(c, freq)) {}

    // 构造函数2：内部节点（仅存储频率，字符设为空格）
    HuffNode(int freq) : BinNode<std::pair<char, int> >(std::make_pair(' ', freq)) {}

    // 比较函数：用于优先队列（最小堆）排序，权重小的节点优先
    static bool compare(HuffNode* a, HuffNode* b) {
        return a->data.second > b->data.second;
    }
};

// Huffman树类（继承BinTree，基于字符频率构建）
class HuffTree : public BinTree<std::pair<char, int> > {
public:
    // 构造函数：输入26个字母的频率数组，构建Huffman树
    HuffTree(int freq[26]) {
        // 1. 创建优先队列（最小堆），仅添加出现过的字符（频率>0）
        std::vector<HuffNode*> heap;
        for (int i = 0; i < 26; i++) {
            if (freq[i] > 0) {
                char c = 'a' + i; // 映射索引0-25到字母a-z
                heap.push_back(new HuffNode(c, freq[i]));
            }
        }
        // 初始化最小堆
        std::make_heap(heap.begin(), heap.end(), HuffNode::compare);

        // 2. 合并节点，构建Huffman树
        while (heap.size() > 1) {
            // 提取权重最小的节点（堆顶）
            std::pop_heap(heap.begin(), heap.end(), HuffNode::compare);
            HuffNode* left = heap.back();
            heap.pop_back();

            // 提取权重第二小的节点
            std::pop_heap(heap.begin(), heap.end(), HuffNode::compare);
            HuffNode* right = heap.back();
            heap.pop_back();

            // 创建内部节点（权重为两节点之和）
            int parentFreq = left->data.second + right->data.second;
            HuffNode* parent = new HuffNode(parentFreq);
            parent->left = left;  // 左子节点为权重较小的节点
            parent->right = right; // 右子节点为权重较大的节点

            // 将内部节点加入堆，重新调整堆结构
            heap.push_back(parent);
            std::push_heap(heap.begin(), heap.end(), HuffNode::compare);
        }

        // 3. 堆中剩余节点即为Huffman树的根节点
        if (!heap.empty()) {
            this->setRoot(heap[0]);
        }
    }
};

// Huffman编码工具类（基于Bitmap存储编码）
class HuffEncoder {
private:
    Bitmap huffCode[26]; // 26个字母的编码（每个字母对应一个Bitmap）
    int codeLen[26];     // 每个字母编码的长度（比特数）

    // 递归遍历Huffman树，生成编码（左子树→0，右子树→1）
    void generateCode(HuffNode* node, Bitmap& tempCode, int len) {
        if (node == NULL) return;

        // 叶子节点：保存当前字符的编码
        if (node->left == NULL && node->right == NULL) {
            char c = node->data.first;
            int idx = c - 'a'; // 映射字母到索引0-25
            // 复制临时编码到目标Bitmap
            for (int i = 0; i < len; i++) {
                if (tempCode.test(i)) {
                    huffCode[idx].set(i);
                } else {
                    huffCode[idx].clear(i);
                }
            }
            codeLen[idx] = len; // 记录编码长度
            return;
        }

        // 遍历左子树：当前位设为0
        tempCode.clear(len);
        generateCode(static_cast<HuffNode*>(node->left), tempCode, len + 1);

        // 遍历右子树：当前位设为1
        tempCode.set(len);
        generateCode(static_cast<HuffNode*>(node->right), tempCode, len + 1);
    }

public:
    // 构造函数：基于Huffman树生成所有字母的编码
    HuffEncoder(HuffTree& tree) {
        // 初始化codeLen数组为0
        for (int i = 0; i < 26; i++) {
            codeLen[i] = 0;
        }
        
        Bitmap tempCode; // 临时存储当前路径的编码
        generateCode(static_cast<HuffNode*>(tree.getRoot()), tempCode, 0);
    }

    // 对外接口：对单个单词进行编码（忽略非字母字符，自动转小写）
    char* encodeWord(const char* word) {
        if (word == NULL) return NULL;

        // 1. 计算编码总长度（累加每个字母的编码长度）
        int totalLen = 0;
        for (int i = 0; word[i] != '\0'; i++) {
            char c = static_cast<char>(tolower(static_cast<unsigned char>(word[i]))); // 转为小写
            int idx = c - 'a';
            if (idx < 0 || idx >= 26) continue; // 跳过非字母字符
            totalLen += codeLen[idx];
        }

        // 2. 合并所有字母的编码到一个Bitmap
        Bitmap totalCode(totalLen);
        int pos = 0; // 当前写入的位置（比特）
        for (int i = 0; word[i] != '\0'; i++) {
            char c = static_cast<char>(tolower(static_cast<unsigned char>(word[i])));
            int idx = c - 'a';
            if (idx < 0 || idx >= 26) continue;

            // 复制当前字母的编码到总编码
            for (int j = 0; j < codeLen[idx]; j++) {
                if (huffCode[idx].test(j)) {
                    totalCode.set(pos + j);
                } else {
                    totalCode.clear(pos + j);
                }
            }
            pos += codeLen[idx];
        }

        // 3. 将Bitmap转为字符串返回（需外部释放内存）
        return totalCode.bits2string(totalLen);
    }

    // 对外接口：打印单个字符的编码（用于调试验证）
    void printCharCode(char c) {
        c = static_cast<char>(tolower(static_cast<unsigned char>(c)));
        int idx = c - 'a';
        if (idx < 0 || idx >= 26) {
            printf("无效字符：%c\n", c);
            return;
        }
        char* code = huffCode[idx].bits2string(codeLen[idx]);
        printf("%c 的编码：%s（长度：%d 比特）\n", c, code, codeLen[idx]);
        delete[] code; // 释放字符串内存
    }
};

#endif // HUFFTREE_H

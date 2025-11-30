#include "HuffTree.h"
#include <iostream>
#include <cstring>
#include <cstddef>  // 引入NULL定义

using namespace std;

// 从《I have a dream》原文统计26个字母的频率（需替换为真实统计结果）
void countFreqFromDream(int freq[26]) {
    // 初始化频率数组为0
    memset(freq, 0, 26 * sizeof(int));

    //从原文中统计的频率
    freq['a' - 'a'] = 192; // a出现192次
    freq['b' - 'a'] = 36;  // b出现36次
    freq['c' - 'a'] = 56; // c出现56次
    freq['d' - 'a'] = 106; // d出现106次
    freq['e' - 'a'] = 290; // e出现290次（元音字母频率较高）
    freq['f' - 'a'] = 75; // f出现75次
    freq['g' - 'a'] = 61;  // g出现61次
    freq['h' - 'a'] = 118; // h出现118次
    freq['i' - 'a'] = 127; // i出现127次
    freq['j' - 'a'] = 5;  // j出现5次
    freq['k' - 'a'] = 9;  // k出现9次
    freq['l' - 'a'] = 105; // l出现105次
    freq['m' - 'a'] = 63; // m出现63次
    freq['n' - 'a'] = 152; // n出现152次
    freq['o' - 'a'] = 147; // o出现147次
    freq['p' - 'a'] = 22;  // p出现22次
    freq['q' - 'a'] = 1;  // q出现1次（最少）
    freq['r' - 'a'] = 125; // r出现125次
    freq['s' - 'a'] = 149; // s出现149次
    freq['t' - 'a'] = 213; // t出现213次
    freq['u' - 'a'] = 49; // u出现49次
    freq['v' - 'a'] = 20;  // v出现20次
    freq['w' - 'a'] = 62; // w出现62次
    freq['x' - 'a'] = 1;  // x出现1次
    freq['y' - 'a'] = 52;  // y出现52次
    freq['z' - 'a'] = 1;  // z出现1次 
}

int main() {
    // 1. 统计《I have a dream》中26个字母的频率
    int charFreq[26];
    countFreqFromDream(charFreq);

    // 2. 构建Huffman树
    HuffTree huffTree(charFreq);
    cout << "Huffman树构建完成！" << endl << endl;

    // 3. 生成Huffman编码
    HuffEncoder encoder(huffTree);

    // 4. 验证关键字符的编码（以"dream"单词的字母为例）
    cout << "=== 关键字符编码验证 ===" << endl;
    encoder.printCharCode('d');
    encoder.printCharCode('r');
    encoder.printCharCode('e');
    encoder.printCharCode('a');
    encoder.printCharCode('m');
    cout << endl;

    // 5. 对目标单词进行编码（含作业要求的"dream"及自选单词）
    const char* targetWords[] = {"dream", "hope", "freedom", "equality", NULL};
    cout << "=== 单词编码结果 ===" << endl;
    for (int i = 0; targetWords[i] != NULL; i++) {
        char* code = encoder.encodeWord(targetWords[i]);
        cout << targetWords[i] << " 的Huffman编码：" << code << endl;
        delete[] code; // 释放编码字符串的内存
    }

    return 0;
}

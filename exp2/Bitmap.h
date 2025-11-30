#ifndef BITMAP_H
#define BITMAP_H

#include <cstdio>
#include <cstring>
#include <cstddef>  // 引入NULL定义

// 定义秩类型（用于索引位图的位）
typedef int Rank;

class Bitmap {
private:
    unsigned char* M;   // 存储位图数据的字节数组
    Rank N;             // 数组长度（字节数），对应 N*8 个比特
    Rank _sz;           // 有效位的个数（值为1的位的数量）

    // 初始化位图：指定初始容量（比特数）
    void init(Rank n) {
        // 计算所需字节数：(n+7)/8 向上取整
        M = new unsigned char[N = (n + 7) / 8];
        memset(M, 0, N); // 初始化为全0
        _sz = 0;         // 初始有效位为0
    }

    // 扩容操作：当访问的位超出当前容量时，加倍扩容
    void expand(Rank k) {
        if (k < 8 * N) return; // 未超出容量，无需扩容

        Rank oldN = N;
        unsigned char* oldM = M;
        init(2 * k);           // 新容量为 2*k 比特（加倍策略）
        memcpy(M, oldM, oldN); // 复制原数据到新空间
        delete[] oldM;         // 释放原空间，避免内存泄漏
    }

public:
    // 构造函数1：默认容量（8比特）
    Bitmap(Rank n = 8) {
        init(n);
    }

    // 构造函数2：从指定文件读取位图数据
    Bitmap(char* file, Rank n = 8) {
        init(n);
        FILE* fp = fopen(file, "r");
        if (fp != NULL) {
            fread(M, sizeof(char), N, fp);
            fclose(fp);
        }
        // 重新统计有效位个数（确保_sz准确）
        _sz = 0;
        for (Rank k = 0; k < n; k++) {
            if (test(k)) _sz++;
        }
    }

    // 析构函数：释放位图内存
    ~Bitmap() {
        delete[] M;
        M = NULL;
        _sz = 0;
    }

    // 对外接口：获取有效位个数
    Rank size() const {
        return _sz;
    }

    // 对外接口：将第k位设为1（添加元素）
    void set(Rank k) {
        expand(k); // 确保k在容量范围内
        // 位运算：定位字节与比特，设为1
        M[k >> 3] |= (0x80 >> (k & 0x07));
        _sz++;
    }

    // 对外接口：将第k位设为0（删除元素）
    void clear(Rank k) {
        expand(k); // 确保k在容量范围内
        if (test(k)) { // 仅当该位为1时，才减少有效位
            // 位运算：定位字节与比特，设为0
            M[k >> 3] &= ~(0x80 >> (k & 0x07));
            _sz--;
        }
    }

    // 对外接口：判断第k位是否为1（判别元素）
    bool test(Rank k) {
        expand(k); // 确保k在容量范围内
        // 位运算：检测目标位是否为1
        return M[k >> 3] & (0x80 >> (k & 0x07));
    }

    // 对外接口：将位图数据导出到指定文件
    void dump(char* file) {
        FILE* fp = fopen(file, "w");
        if (fp != NULL) {
            fwrite(M, sizeof(char), N, fp);
            fclose(fp);
        }
    }

    // 对外接口：将前n位转为字符串（便于显示编码结果）
    char* bits2string(Rank n) {
        expand(n - 1); // 确保最高位n-1在容量范围内
        char* s = new char[n + 1]; // 预留字符串结束符位置
        s[n] = '\0';
        for (Rank i = 0; i < n; i++) {
            s[i] = test(i) ? '1' : '0'; // 1→'1'，0→'0'
        }
        return s;
    }
};

#endif // BITMAP_H

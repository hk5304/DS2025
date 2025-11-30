#ifndef BINTREE_H
#define BINTREE_H

#include <cstddef>  // 引入NULL定义

// 二叉树节点类（模板类，支持任意数据类型）
template <typename T>
class BinNode {
public:
    T data;          // 节点存储的数据
    BinNode<T>* left; // 左子节点指针
    BinNode<T>* right;// 右子节点指针

    // 构造函数：初始化数据与子节点
    BinNode(T val) : data(val), left(NULL), right(NULL) {}
};

// 二叉树类（模板类，基于BinNode实现）
template <typename T>
class BinTree {
private:
    BinNode<T>* root; // 二叉树的根节点

    // 递归销毁二叉树（避免内存泄漏）
    void destroy(BinNode<T>* node) {
        if (node == NULL) return;
        destroy(node->left);  // 先销毁左子树
        destroy(node->right); // 再销毁右子树
        delete node;          // 最后销毁当前节点
    }

public:
    // 构造函数：默认创建空树
    BinTree() : root(NULL) {}

    // 析构函数：自动释放树内存
    ~BinTree() {
        destroy(root);
    }

    // 对外接口：获取根节点（只读）
    BinNode<T>* getRoot() const {
        return root;
    }

    // 对外接口：设置根节点（用于构建树结构）
    void setRoot(BinNode<T>* node) {
        root = node;
    }

    // 前序遍历（用于调试时验证树结构）
    void preOrder(BinNode<T>* node) const {
        if (node == NULL) return;
        preOrder(node->left);
        preOrder(node->right);
    }
};

#endif // BINTREE_H

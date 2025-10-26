#ifndef TREE_H
#define TREE_H
#include <iostream>
#include <cstdlib>
using namespace std;

// 二叉树节点模板
template <typename T>
struct TreeNode {
    T data;           // 节点数据
    TreeNode* left;   // 左子节点
    TreeNode* right;  // 右子节点
    TreeNode* parent; // 父节点

    // 构造函数
    TreeNode(const T& e = T(), TreeNode* l = NULL, TreeNode* r = NULL, TreeNode* p = NULL)
        : data(e), left(l), right(r), parent(p) {}
};

// 打印元素的函数（用于遍历）
template <typename T>
void printElem(T& e) {
    cout << e << " ";
}

// 二叉树类模板（邓俊辉《数据结构》风格实现）
template <typename T>
class Tree {
protected:
    TreeNode<T>* _root;  // 根节点
    int _size;           // 树的规模（节点数）

    // 递归删除以p为根的子树
    void clear(TreeNode<T>* p) {
        if (p == NULL) return;
        clear(p->left);
        clear(p->right);
        delete p;
        _size--;
    }

    // 递归复制以p为根的子树，返回新根节点
    TreeNode<T>* copy(TreeNode<T>* p, TreeNode<T>* parent) {
        if (p == NULL) return NULL;
        TreeNode<T>* newNode = new TreeNode<T>(p->data, NULL, NULL, parent);
        newNode->left = copy(p->left, newNode);
        newNode->right = copy(p->right, newNode);
        return newNode;
    }

public:
    // 构造函数：初始化空树
    Tree() : _root(NULL), _size(0) {}

    // 析构函数：释放所有节点
    ~Tree() {
        clear(_root);
    }

    // 拷贝构造函数：深拷贝
    Tree(const Tree& t) : _size(0) {
        _root = copy(t._root, NULL);
        _size = t._size;
    }

    // 赋值运算符：深拷贝
    Tree& operator=(const Tree& t) {
        if (this != &t) {
            clear(_root);
            _root = copy(t._root, NULL);
            _size = t._size;
        }
        return *this;
    }

    // 插入节点为p的左子节点，返回新节点
    TreeNode<T>* insertLeft(TreeNode<T>* p, const T& e) {
        if (p == NULL) return NULL;
        if (p->left != NULL) {
            cerr << "Error: Left child already exists." << endl;
            return NULL;
        }
        p->left = new TreeNode<T>(e, NULL, NULL, p);
        _size++;
        return p->left;
    }

    // 插入节点为p的右子节点，返回新节点
    TreeNode<T>* insertRight(TreeNode<T>* p, const T& e) {
        if (p == NULL) return NULL;
        if (p->right != NULL) {
            cerr << "Error: Right child already exists." << endl;
            return NULL;
        }
        p->right = new TreeNode<T>(e, NULL, NULL, p);
        _size++;
        return p->right;
    }

    // 插入根节点，返回新根
    TreeNode<T>* insertRoot(const T& e) {
        if (_root != NULL) {
            cerr << "Error: Root already exists." << endl;
            return NULL;
        }
        _root = new TreeNode<T>(e);
        _size++;
        return _root;
    }

    // 删除以p为根的子树，返回被删除节点的数量
    int remove(TreeNode<T>* p) {
        if (p == NULL) return 0;
        int count = 0;
        TreeNode<T>* parent = p->parent;
        clear(p);
        if (parent != NULL) {
            if (parent->left == p) parent->left = NULL;
            else parent->right = NULL;
        } else {
            _root = NULL;
        }
        count = _size;
        _size = 0; // clear函数已更新_size，此处仅为兼容
        return count;
    }

    // 查找元素e，从节点p开始（默认从根节点开始）
    TreeNode<T>* find(const T& e, TreeNode<T>* p = NULL) const {
        if (p == NULL) p = _root;
        if (p == NULL || p->data == e) return p;
        TreeNode<T>* left = find(e, p->left);
        if (left != NULL) return left;
        return find(e, p->right);
    }

    // 前序遍历：根-左-右
    void preOrder(TreeNode<T>* p, void (*visit)(T&)) const {
        if (p == NULL) return;
        visit(p->data);
        preOrder(p->left, visit);
        preOrder(p->right, visit);
    }

    void preOrder(void (*visit)(T&)) const {
        preOrder(_root, visit);
    }

    // 中序遍历：左-根-右
    void inOrder(TreeNode<T>* p, void (*visit)(T&)) const {
        if (p == NULL) return;
        inOrder(p->left, visit);
        visit(p->data);
        inOrder(p->right, visit);
    }

    void inOrder(void (*visit)(T&)) const {
        inOrder(_root, visit);
    }

    // 后序遍历：左-右-根
    void postOrder(TreeNode<T>* p, void (*visit)(T&)) const {
        if (p == NULL) return;
        postOrder(p->left, visit);
        postOrder(p->right, visit);
        visit(p->data);
    }

    void postOrder(void (*visit)(T&)) const {
        postOrder(_root, visit);
    }

    // 获取树的规模
    int size() const {
        return _size;
    }

    // 判断树是否为空
    bool empty() const {
        return _size == 0;
    }

    // 获取根节点
    TreeNode<T>* root() const {
        return _root;
    }

    // 打印树（前序遍历方式）
    void print() const {
        cout << "Tree [size=" << _size << "]: ";
        preOrder(printElem);
        cout << endl;
    }
};

// 测试树模块的函数（可选，可单独放在main.cpp中）
void testTree() {
    // 测试基本操作
    Tree<int> t;
    cout << "=== 测试树基本操作 ===" << endl;
    cout << "树是否为空：" << (t.empty() ? "是" : "否") << endl;

    // 插入根节点和子节点
    TreeNode<int>* root = t.insertRoot(1);
    TreeNode<int>* left = t.insertLeft(root, 2);
    TreeNode<int>* right = t.insertRight(root, 3);
    t.insertLeft(left, 4);
    t.insertRight(left, 5);
    t.insertLeft(right, 6);
    t.insertRight(right, 7);
    cout << "插入后树（前序遍历）：";
    t.print();

    // 查找元素
    TreeNode<int>* node = t.find(5);
    cout << "查找元素5的节点数据：" << (node ? node->data : -1) << endl;

    // 遍历
    cout << "前序遍历：";
    t.preOrder(printElem);
    cout << endl;
    cout << "中序遍历：";
    t.inOrder(printElem);
    cout << endl;
    cout << "后序遍历：";
    t.postOrder(printElem);
    cout << endl;

    // 拷贝构造与赋值
    Tree<int> t2 = t;
    cout << "\n=== 测试拷贝构造 ===" << endl;
    cout << "原树t：";
    t.print();
    cout << "拷贝树t2：";
    t2.print();

    Tree<int> t3;
    t3 = t2;
    cout << "\n=== 测试赋值运算符 ===" << endl;
    cout << "树t2：";
    t2.print();
    cout << "赋值后树t3：";
    t3.print();

    // 删除子树
    cout << "\n=== 测试删除子树 ===" << endl;
    int delCount = t.remove(left);
    cout << "删除左子树后树的规模：" << t.size() << "（删除" << delCount << "个节点）" << endl;
    t.print();
}

#endif  // TREE_H

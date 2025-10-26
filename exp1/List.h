#ifndef LIST_H
#define LIST_H
#include <iostream>
#include <cstdlib>
using namespace std;

// 列表节点模板
template <typename T>
struct ListNode {
    T data;           // 节点数据
    ListNode* pred;   // 前驱指针
    ListNode* succ;   // 后继指针

    // 构造函数
    ListNode(const T& e = T(), ListNode* p = NULL, ListNode* s = NULL)
        : data(e), pred(p), succ(s) {}
};

// 列表类模板（邓俊辉《数据结构》风格实现）
template <typename T>
class List {
private:
    int _size;        // 列表规模
    ListNode<T>* _head;  // 头哨兵节点
    ListNode<T>* _tail;  // 尾哨兵节点

    // 初始化空列表（仅含哨兵节点）
    void init() {
        _head = new ListNode<T>();
        _tail = new ListNode<T>();
        _head->succ = _tail;
        _tail->pred = _head;
        _size = 0;
    }

    // 清除所有节点（保留哨兵）
    void clear() {
        while (_size > 0) {
            remove(_head->succ);
        }
    }

public:
    // 构造函数：初始化空列表
    List() {
        init();
    }

    // 析构函数：释放所有节点
    ~List() {
        clear();
        delete _head;
        delete _tail;
    }

    // 拷贝构造函数：深拷贝
    List(const List& L) {
        init();
        for (ListNode<T>* p = L._head->succ; p != L._tail; p = p->succ) {
            insertAsLast(p->data);
        }
    }

    // 赋值运算符：深拷贝
    List& operator=(const List& L) {
        if (this != &L) {
            clear();
            for (ListNode<T>* p = L._head->succ; p != L._tail; p = p->succ) {
                insertAsLast(p->data);
            }
        }
        return *this;
    }

    // 插入节点为p的后继，返回新节点
    ListNode<T>* insertAfter(ListNode<T>* p, const T& e) {
        _size++;
        return p->succ = p->succ->pred = new ListNode<T>(e, p, p->succ);
    }

    // 插入节点为p的前驱，返回新节点
    ListNode<T>* insertBefore(ListNode<T>* p, const T& e) {
        _size++;
        return p->pred = p->pred->succ = new ListNode<T>(e, p->pred, p);
    }

    // 在头部插入节点，返回新节点
    ListNode<T>* insertAsFirst(const T& e) {
        return insertAfter(_head, e);
    }

    // 在尾部插入节点，返回新节点
    ListNode<T>* insertAsLast(const T& e) {
        return insertBefore(_tail, e);
    }

    // 删除节点p，返回其数据
    T remove(ListNode<T>* p) {
        T e = p->data;
        p->pred->succ = p->succ;
        p->succ->pred = p->pred;
        delete p;
        _size--;
        return e;
    }

    // 查找元素e，从位置p开始（默认从首节点开始）
    ListNode<T>* find(const T& e, ListNode<T>* p = NULL) const {
        if (p == NULL) p = _head->succ;
        while (p != _tail && p->data != e) {
            p = p->succ;
        }
        return p != _tail ? p : NULL;
    }

    // 去重：删除重复元素
    int deduplicate() {
        if (_size < 2) return 0;
        int oldSize = _size;
        ListNode<T>* p = _head->succ;
        int r = 0;
        while (p != _tail) {
            ListNode<T>* q = find(p->data, p->succ);
            if (q) {
                remove(q);
            } else {
                p = p->succ;
                r++;
            }
        }
        return oldSize - _size;
    }

    // 遍历：通过函数指针访问每个元素
    void traverse(void (*visit)(T&)) {
        for (ListNode<T>* p = _head->succ; p != _tail; p = p->succ) {
            visit(p->data);
        }
    }

    // 遍历：通过函数对象访问每个元素（更灵活）
    template <typename VST>
    void traverse(VST& visit) {
        for (ListNode<T>* p = _head->succ; p != _tail; p = p->succ) {
            visit(p->data);
        }
    }

    // 排序：插入排序
    void insertionSort(ListNode<T>* p, int n) {
        for (int i = 0; i < n; ++i) {
            ListNode<T>* q = p->succ;
            p = p->succ;
            while (q->pred != _head && q->pred->data > q->data) {
                swap(q->pred->data, q->data);
                q = q->pred;
            }
        }
    }

    void sort() {
        if (_size < 2) return;
        insertionSort(_head->succ, _size);
    }

    // 获取列表规模
    int size() const {
        return _size;
    }

    // 判断列表是否为空
    bool empty() const {
        return _size == 0;
    }

    // 打印列表
    void print() const {
        cout << "List [size=" << _size << "]: ";
        for (ListNode<T>* p = _head->succ; p != _tail; p = p->succ) {
            cout << p->data << " ";
        }
        cout << endl;
    }
};

// 测试列表模块的函数（可选，可单独放在main.cpp中）
void testList() {
    // 测试基本操作
    List<int> L;
    cout << "=== 测试列表基本操作 ===" << endl;
    cout << "列表是否为空：" << (L.empty() ? "是" : "否") << endl;

    // 插入元素
    L.insertAsFirst(1);
    L.insertAsLast(5);
    L.insertAfter(L.find(1), 2);
    L.insertBefore(L.find(5), 4);
    L.insertAfter(L.find(2), 3);
    cout << "插入后列表：";
    L.print();

    // 删除元素
    L.remove(L.find(3));
    cout << "删除元素3后列表：";
    L.print();

    // 去重
    L.insertAsLast(2);
    cout << "插入重复元素后列表：";
    L.print();
    int delCount = L.deduplicate();
    cout << "去重后列表（删除" << delCount << "个元素）：";
    L.print();

    // 排序
    L.sort();
    cout << "排序后列表：";
    L.print();

    // 拷贝构造与赋值
    List<int> L2 = L;
    cout << "\n=== 测试拷贝构造 ===" << endl;
    cout << "原列表L：";
    L.print();
    cout << "拷贝列表L2：";
    L2.print();

    List<int> L3;
    L3 = L2;
    cout << "\n=== 测试赋值运算符 ===" << endl;
    cout << "列表L2：";
    L2.print();
    cout << "赋值后列表L3：";
    L3.print();
}

#endif  // LIST_H

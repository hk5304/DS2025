#ifndef QUEUE_H
#define QUEUE_H
#include <iostream>
#include <cstdlib>
using namespace std;

// 队列类模板（邓俊辉《数据结构》风格实现，循环数组版）
template <typename T>
class Queue {
private:
    T* _elem;       // 存储队列元素的数组
    int _front;     // 队头指针（指向队头元素）
    int _rear;      // 队尾指针（指向队尾元素的下一个位置）
    int _capacity;  // 队列容量
    int _size;      // 队列当前元素个数

    // 扩容操作：容量加倍
    void expand() {
        if (_size < _capacity) return;  // 未满无需扩容
        int oldCapacity = _capacity;
        _capacity *= 2;
        T* oldElem = _elem;
        _elem = new T[_capacity];
        
        // 复制原队列元素（循环数组展开）
        int i = 0;
        for (int j = _front; j != _rear; j = (j + 1) % oldCapacity) {
            _elem[i++] = oldElem[j];
        }
        
        _front = 0;    // 新队头为0
        _rear = oldCapacity;  // 新队尾为原容量
        delete[] oldElem;
    }

public:
    // 构造函数：默认容量为10
    Queue(int capacity = 10) : _capacity(capacity), _size(0) {
        _elem = new T[_capacity];
        _front = _rear = 0;  // 初始队空
    }

    // 析构函数
    ~Queue() {
        delete[] _elem;
    }

    // 拷贝构造函数：深拷贝
    Queue(const Queue& q) : _capacity(q._capacity), _size(q._size) {
        _elem = new T[_capacity];
        _front = q._front;
        _rear = q._rear;
        // 复制元素
        for (int j = _front; j != _rear; j = (j + 1) % _capacity) {
            _elem[j] = q._elem[j];
        }
    }

    // 赋值运算符：深拷贝
    Queue& operator=(const Queue& q) {
        if (this != &q) {
            delete[] _elem;
            _capacity = q._capacity;
            _size = q._size;
            _elem = new T[_capacity];
            _front = q._front;
            _rear = q._rear;
            // 复制元素
            for (int j = _front; j != _rear; j = (j + 1) % _capacity) {
                _elem[j] = q._elem[j];
            }
        }
        return *this;
    }

    // 入队：在队尾插入元素
    void enqueue(const T& e) {
        expand();  // 确保容量足够
        _elem[_rear] = e;
        _rear = (_rear + 1) % _capacity;  // 循环移动队尾指针
        _size++;
    }

    // 出队：删除并返回队头元素
    T dequeue() {
        if (empty()) {
            cerr << "Error: Queue is empty, cannot dequeue." << endl;
            return T();
        }
        T e = _elem[_front];
        _front = (_front + 1) % _capacity;  // 循环移动队头指针
        _size--;
        return e;
    }

    // 获取队头元素（不删除）
    T& front() {
        if (empty()) {
            cerr << "Error: Queue is empty, no front element." << endl;
            exit(1);
        }
        return _elem[_front];
    }

    // 获取队头元素（const版本）
    const T& front() const {
        if (empty()) {
            cerr << "Error: Queue is empty, no front element." << endl;
            exit(1);
        }
        return _elem[_front];
    }

    // 判断队列是否为空
    bool empty() const {
        return _size == 0;
    }

    // 获取队列元素个数
    int size() const {
        return _size;
    }

    // 清空队列
    void clear() {
        _front = _rear = 0;
        _size = 0;
    }

    // 打印队列（从队头到队尾）
    void print() const {
        cout << "Queue [size=" << _size << "]: ";
        if (empty()) {
            cout << "(empty)";
        } else {
            int j = _front;
            for (int i = 0; i < _size; ++i) {
                cout << _elem[j] << " ";
                j = (j + 1) % _capacity;
            }
        }
        cout << endl;
    }
};

// 测试队列模块的函数
void testQueue() {
    // 测试基本操作
    Queue<int> q;
    cout << "=== 测试队列基本操作 ===" << endl;
    cout << "队列是否为空：" << (q.empty() ? "是" : "否") << endl;

    // 入队
    q.enqueue(1);
    q.enqueue(2);
    q.enqueue(3);
    q.enqueue(4);
    q.enqueue(5);
    cout << "入队后：";
    q.print();

    // 队头元素
    cout << "队头元素：" << q.front() << endl;

    // 出队
    cout << "出队元素：" << q.dequeue() << endl;
    cout << "出队后：";
    q.print();

    // 连续出队
    q.dequeue();
    q.dequeue();
    cout << "连续出队2个元素后：";
    q.print();

    // 继续入队（测试循环数组特性）
    q.enqueue(6);
    q.enqueue(7);
    q.enqueue(8);
    q.enqueue(9);
    q.enqueue(10);
    q.enqueue(11);  // 触发扩容
    cout << "多次入队后（含扩容）：";
    q.print();

    // 拷贝构造
    Queue<int> q2 = q;
    cout << "\n=== 测试拷贝构造 ===" << endl;
    cout << "原队列q：";
    q.print();
    cout << "拷贝队列q2：";
    q2.print();

    // 赋值运算符
    Queue<int> q3;
    q3 = q2;
    cout << "\n=== 测试赋值运算符 ===" << endl;
    cout << "队列q2：";
    q2.print();
    cout << "赋值后队列q3：";
    q3.print();

    // 清空队列
    q.clear();
    cout << "\n清空队列q后：";
    q.print();
}

#endif  // QUEUE_H

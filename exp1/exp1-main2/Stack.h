#ifndef STACK_H
#define STACK_H
#include <iostream>
#include <cstdlib>
using namespace std;

// 栈类模板
template <typename T>
class Stack {
private:
    T* _elem;  // 存储栈元素的数组
    int _top;   // 栈顶指针（指向栈顶元素的下一个位置）
    int _capacity;  // 栈的容量

    // 扩容操作：当栈满时，容量加倍
    void expand() {
        if (_top < _capacity) return;  // 栈未满，无需扩容
        _capacity = max(_capacity, 1) * 2;  // 容量加倍（初始容量为1时也能正确扩容）
        T* oldElem = _elem;
        _elem = new T[_capacity];
        for (int i = 0; i < _top; ++i) {
            _elem[i] = oldElem[i];  // 复制原有元素
        }
        delete[] oldElem;  // 释放原空间
    }

public:
    // 构造函数：初始化空栈，默认容量为10
    Stack(int capacity = 10) : _top(0), _capacity(capacity) {
        _elem = new T[_capacity];
    }

    // 析构函数：释放栈空间
    ~Stack() {
        delete[] _elem;
    }

    // 拷贝构造函数：深拷贝
    Stack(const Stack& s) : _top(s._top), _capacity(s._capacity) {
        _elem = new T[_capacity];
        for (int i = 0; i < _top; ++i) {
            _elem[i] = s._elem[i];
        }
    }

    // 赋值运算符：深拷贝
    Stack& operator=(const Stack& s) {
        if (this != &s) {
            delete[] _elem;
            _top = s._top;
            _capacity = s._capacity;
            _elem = new T[_capacity];
            for (int i = 0; i < _top; ++i) {
                _elem[i] = s._elem[i];
            }
        }
        return *this;
    }

    // 入栈操作：将元素e压入栈顶
    void push(const T& e) {
        expand();  // 确保空间足够
        _elem[_top++] = e;
    }

    // 出栈操作：弹出栈顶元素并返回
    T pop() {
        if (empty()) {
            cerr << "Error: Stack is empty, cannot pop." << endl;
            return T();  // 空栈返回默认值
        }
        return _elem[--_top];
    }

    // 取栈顶元素：返回栈顶元素（不弹出）
    T& top() {
        if (empty()) {
            cerr << "Error: Stack is empty, no top element." << endl;
            exit(1);  // 空栈直接退出程序
        }
        return _elem[_top - 1];
    }

    // 取栈顶元素（const版本，只读）
    const T& top() const {
        if (empty()) {
            cerr << "Error: Stack is empty, no top element." << endl;
            exit(1);  // 空栈直接退出程序
        }
        return _elem[_top - 1];
    }

    // 判断栈是否为空
    bool empty() const {
        return _top == 0;
    }

    // 获取栈的规模（元素个数）
    int size() const {
        return _top;
    }

    // 清空栈
    void clear() {
        _top = 0;
    }

    // 打印栈（从栈顶到栈底）
    void print() const {
        cout << "Stack [size=" << _top << "]: ";
        for (int i = _top - 1; i >= 0; --i) {
            cout << _elem[i] << " ";
        }
        cout << endl;
    }
};

// 测试栈模块的函数（可选，可单独放在main.cpp中）
void testStack() {
    // 测试基本操作
    Stack<int> s;
    cout << "=== 测试栈基本操作 ===" << endl;
    cout << "栈是否为空：" << (s.empty() ? "是" : "否") << endl;

    // 入栈
    s.push(1);
    s.push(2);
    s.push(3);
    s.push(4);
    s.push(5);
    cout << "入栈后：";
    s.print();

    // 取栈顶
    cout << "栈顶元素：" << s.top() << endl;

    // 出栈
    cout << "弹出元素：" << s.pop() << endl;
    cout << "出栈后：";
    s.print();

    // 拷贝构造与赋值
    Stack<int> s2 = s;
    cout << "\n=== 测试拷贝构造 ===" << endl;
    cout << "原栈s：";
    s.print();
    cout << "拷贝栈s2：";
    s2.print();

    Stack<int> s3;
    s3 = s2;
    cout << "\n=== 测试赋值运算符 ===" << endl;
    cout << "栈s2：";
    s2.print();
    cout << "赋值后栈s3：";
    s3.print();
}

#endif  // STACK_H

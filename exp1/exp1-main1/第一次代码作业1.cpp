#include <iostream>
#include <cstdlib>
#include <ctime>
#include "Vector.h"
#include "Complex.h"
using namespace std;

Vector<Complex> generateRandomVector(int size) {
    Vector<Complex> vec;
    srand(time(0));
    for (int i = 0; i < size; ++i) {
        double real = (rand() % 201) - 100;
        double imag = (rand() % 201) - 100;
        vec.push_back(Complex(real, imag));
        if (rand() % 5 < 2) {
            vec.push_back(Complex(real, imag));
        }
    }
    return vec;
}

void shuffleVector(Vector<Complex>& vec) {
    srand(time(0));
    for (int i = vec.size() - 1; i > 0; --i) {
        int j = rand() % (i + 1);
        swap(vec[i], vec[j]);
    }
}

Vector<Complex> rangeSearch(const Vector<Complex>& vec, double m1, double m2) {
    Vector<Complex> result;
    for (int i = 0; i < vec.size(); ++i) {
        double mod = vec[i].norm();
        if (mod >= m1 && mod < m2) {
            result.push_back(vec[i]);
        }
    }
    return result;
}

int main() {
    Vector<Complex> vec = generateRandomVector(20);
    vec.print("初始无序向量");

    shuffleVector(vec);
    vec.print("置乱后向量");

    if (!vec.empty()) {
        Complex target = vec[3];
        int index = vec.find(target);
        cout << "查找元素 " << target << "，索引: " << index << endl;
    }

    vec.insert(5, Complex(10, 20));
    vec.print("插入元素(10+20i)后");

    if (!vec.empty()) {
        Complex delElem = vec[2];
        vec.remove(2);
        cout << "删除元素" << delElem << "后: ";
        vec.print("");
    }

    vec.deduplicate();
    vec.print("唯一化后向量");

    const int SIZE = 5000;
    Vector<Complex> vec_order, vec_random, vec_reverse;

    for (int i = 0; i < SIZE; ++i) {
        vec_order.push_back(Complex(i, i));
    }

    vec_random = generateRandomVector(SIZE);
    shuffleVector(vec_random);

    vec_reverse = vec_order;
    for (int i = 0; i < SIZE / 2; ++i) {
        swap(vec_reverse[i], vec_reverse[SIZE - 1 - i]);
    }

    clock_t start, end;

    Vector<Complex> temp = vec_order;
    start = clock();
    temp.bubbleSort();
    end = clock();
    cout << "起泡排序(顺序): " << (double)(end - start) / CLOCKS_PER_SEC << "s" << endl;

    temp = vec_random;
    start = clock();
    temp.bubbleSort();
    end = clock();
    cout << "起泡排序(乱序): " << (double)(end - start) / CLOCKS_PER_SEC << "s" << endl;

    temp = vec_reverse;
    start = clock();
    temp.bubbleSort();
    end = clock();
    cout << "起泡排序(逆序): " << (double)(end - start) / CLOCKS_PER_SEC << "s" << endl;

    temp = vec_order;
    start = clock();
    temp.mergeSort();
    end = clock();
    cout << "归并排序(顺序): " << (double)(end - start) / CLOCKS_PER_SEC << "s" << endl;

    temp = vec_random;
    start = clock();
    temp.mergeSort();
    end = clock();
    cout << "归并排序(乱序): " << (double)(end - start) / CLOCKS_PER_SEC << "s" << endl;

    temp = vec_reverse;
    start = clock();
    temp.mergeSort();
    end = clock();
    cout << "归并排序(逆序): " << (double)(end - start) / CLOCKS_PER_SEC << "s" << endl;

    Vector<Complex> sorted_vec = generateRandomVector(100);
    sorted_vec.mergeSort();
    sorted_vec.print("排序后的向量");

    double m1 = 50, m2 = 100;
    Vector<Complex> range_result = rangeSearch(sorted_vec, m1, m2);
    cout << "模介于[" << m1 << "," << m2 << ")的元素: ";
    range_result.print("");

    return 0;
}

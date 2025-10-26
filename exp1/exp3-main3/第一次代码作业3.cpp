#include <iostream>
#include <cstdlib>
#include <ctime>
#include "Stack.h"
#include "Vector.h"
using namespace std;

// 计算柱状图最大矩形面积（使用栈模块）
int largestRectangleArea(Vector<int>& heights) {
    Stack<int> stk;
    heights.push_back(0);  // 哨兵，确保所有元素出栈
    int maxArea = 0;
    
    for (int i = 0; i < heights.size(); ++i) {
        while (!stk.empty() && heights[i] < heights[stk.top()]) {
            int height = heights[stk.pop()];
            int width = stk.empty() ? i : i - stk.top() - 1;
            maxArea = max(maxArea, height * width);
        }
        stk.push(i);
    }
    heights.remove(heights.size() - 1);  // 恢复原数组
    return maxArea;
}

// 生成随机测试数据并测试
void testRandomCases() {
    srand(time(0));  // 初始化随机数生成器
    
    for (int t = 1; t <= 10; ++t) {
        int n = rand() % 105 + 1;  // 数组长度：1~105
        Vector<int> heights;
        for (int i = 0; i < n; ++i) {
            heights.push_back(rand() % 105);  // 高度：0~104
        }
        
        cout << "测试用例 " << t << "：" << endl;
        cout << "高度数组：";
        for (int i = 0; i < heights.size(); ++i) {
            cout << heights[i] << " ";
        }
        cout << endl;
        
        int maxArea = largestRectangleArea(heights);
        cout << "最大矩形面积：" << maxArea << endl << endl;
    }
}

// 示例测试
void testExamples() {
    // 示例1
    Vector<int> example1;
    example1.push_back(2);
    example1.push_back(1);
    example1.push_back(5);
    example1.push_back(6);
    example1.push_back(2);
    example1.push_back(3);
    cout << "示例1：" << endl;
    cout << "高度数组：";
    for (int i = 0; i < example1.size(); ++i) {
        cout << example1[i] << " ";
    }
    cout << endl;
    cout << "最大矩形面积：" << largestRectangleArea(example1) << endl << endl;
    
    // 示例2
    Vector<int> example2;
    example2.push_back(2);
    example2.push_back(4);
    cout << "示例2：" << endl;
    cout << "高度数组：";
    for (int i = 0; i < example2.size(); ++i) {
        cout << example2[i] << " ";
    }
    cout << endl;
    cout << "最大矩形面积：" << largestRectangleArea(example2) << endl << endl;
}

int main() {
    testExamples();
    testRandomCases();
    return 0;
}

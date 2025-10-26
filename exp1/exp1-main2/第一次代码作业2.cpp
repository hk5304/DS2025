#include <iostream>
#include <cstring>
#include <cmath>
#include "Stack.h"  // 导入栈模块
using namespace std;

// 运算符定义（遵循邓俊辉《数据结构》代码4.6）
#define N_OPTR 9
typedef enum { ADD, SUB, MUL, DIV, POW, FAC, L_P, R_P, EOE } Operator;

// 优先级表（[栈顶运算符][当前运算符]）
const char pri[N_OPTR][N_OPTR] = {
    { '>', '>', '<', '<', '<', '<', '<', '>', '>' },  // +
    { '>', '>', '<', '<', '<', '<', '<', '>', '>' },  // -
    { '>', '>', '>', '>', '<', '<', '<', '>', '>' },  // *
    { '>', '>', '>', '>', '<', '<', '<', '>', '>' },  // /
    { '>', '>', '>', '>', '>', '<', '<', '>', '>' },  // ^
    { '>', '>', '>', '>', '>', '>', ' ', '>', '>' },  // !
    { '<', '<', '<', '<', '<', '<', '<', '=', ' ' },  // (
    { ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' },  // )
    { '<', '<', '<', '<', '<', '<', '<', ' ', '=' }   // \0（终止符）
};

// 字符转运算符枚举
Operator charToOp(char c) {
    switch (c) {
        case '+': return ADD;
        case '-': return SUB;
        case '*': return MUL;
        case '/': return DIV;
        case '^': return POW;
        case '!': return FAC;
        case '(': return L_P;
        case ')': return R_P;
        case '\0': return EOE;
        default: return (Operator)-1;  // 无效运算符
    }
}

// 阶乘计算（仅支持非负整数）
double factorial(int n) {
    if (n < 0) return NAN;  // 无效输入返回NaN
    double res = 1;
    for (int i = 2; i <= n; ++i) res *= i;
    return res;
}

// 执行运算（a 运算符 b，单目运算符b默认0）
double calculate(double a, Operator op, double b = 0) {
    switch (op) {
        case ADD: return a + b;
        case SUB: return a - b;
        case MUL: return a * b;
        case DIV: return (b == 0) ? NAN : a / b;  // 除零返回NaN
        case POW: return pow(a, b);
        case FAC: return factorial((int)a);       // 阶乘（单目运算符）
        default: return NAN;
    }
}

// 字符串表达式计算主函数
double stringCalculator(const char* expr) {
    Stack<double> numStack;    // 操作数栈
    Stack<Operator> opStack;   // 运算符栈
    opStack.push(EOE);         // 栈底压入终止符
    int i = 0;
    bool lastIsOp = true;      // 标记上一个 token 是否为运算符（用于语法检查）

    while (i <= strlen(expr)) {
        char c = (i < strlen(expr)) ? expr[i] : '\0';  // 处理结束符

        // 解析数字（包括小数）
        if (isdigit(c) || (c == '.' && i > 0 && isdigit(expr[i-1]))) {
            lastIsOp = false;  // 标记当前为数字
            double num = 0;
            // 整数部分
            while (isdigit(c)) {
                num = num * 10 + (c - '0');
                c = expr[++i];
            }
            // 小数部分
            if (c == '.') {
                c = expr[++i];
                double frac = 0.1;
                while (isdigit(c)) {
                    num += (c - '0') * frac;
                    frac *= 0.1;
                    c = expr[++i];
                }
            }
            numStack.push(num);  // 数字入栈
        } 
        // 处理运算符
        else {
            Operator currOp = charToOp(c);
            if (currOp == (Operator)-1) return NAN;  // 无效运算符

            // 处理结束符
            if (c == '\0' || i == strlen(expr)) {
                currOp = EOE;
            }

            // 语法检查：避免连续个非运算符连续出现（如 "3(4)" 或 "3+*5"）
            if (!lastIsOp && (currOp == L_P || isdigit(c))) {
                return NAN;
            }

            // 比较栈顶栈顶栈顶运算符优先级
            Operator topOp = opStack.top();
            char cmp = pri[topOp][currOp];

            if (cmp == '<') {  // 栈顶运算符优先级低，当前运算符入栈
                opStack.push(currOp);
                lastIsOp = (currOp != R_P);  // 右括号不算运算符
                i++;
            } 
            else if (cmp == '=') {  // 优先级相等相等（仅括号对）
                opStack.pop();  // 弹出左括号
                if (currOp == EOE) break;  // 终止符匹配，结束计算
                lastIsOp = true;
                i++;
            } 
            else if (cmp == '>') {  // 栈顶运算符优先级高，执行计算
                opStack.pop();
                if (topOp == FAC) {  // 单目运算符（阶乘）
                    if (numStack.empty()) return NAN;
                    double a = numStack.pop();
                    double res = calculate(a, topOp);
                    if (isnan(res)) return NAN;
                    numStack.push(res);
                } else {  // 双目运算符
                    if (numStack.empty()) return NAN;
                    double b = numStack.pop();
                    if (numStack.empty()) return NAN;
                    double a = numStack.pop();
                    double res = calculate(a, topOp, b);
                    if (isnan(res)) return NAN;
                    numStack.push(res);
                }
            } 
            else {  // 优先级表中为空格，无效表达式
                return NAN;
            }
        }
    }

    // 最终栈中应仅剩一个结果
    if (numStack.size() != 1) return NAN;
    return numStack.top();
}

// 测试计算器功能
void testCalculator() {
    const char* tests[] = {
        "3+5*2",          // 13
        "(3+5)*2",        // 16
        "10/2-3",         // 2
        "2^3+4",          // 12
        "5!",             // 120
        "3+4*2/(1-5)",    // 1
        "2.5+3.5*2",      // 9.5
        "5!+3*2",         // 126
        "3/0",            // 无效（除零）
        "((3+5)",         // 无效（括号不匹配）
        "3+*5",           // 无效（语法错误）
        NULL
    };

    for (int i = 0; tests[i] != NULL; i++) {
        cout << "表达式: " << tests[i] << "\t";
        double res = stringCalculator(tests[i]);
        if (isnan(res)) {
            cout << "结果: 式子无效" << endl;
        } else {
            cout << "结果: " << res << endl;
        }
    }
}

int main() {
    testCalculator();  // 运行测试
    return 0;
}

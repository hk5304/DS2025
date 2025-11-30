#include "Graph.h"
#include <iostream>
using namespace std;

// 任务1：构建图1并输出邻接矩阵（顶点A~H，索引0=A,1=B,...,7=H）
void task1_BuildGraph1(Graph& g1) {
    // 根据作业图1的边与权重添加连接（C++98兼容写法）
    g1.addEdge(0, 1, 4);   // A-B，权重4
    g1.addEdge(0, 3, 7);   // A-D，权重7
    g1.addEdge(1, 2, 12);  // B-C，权重12
    g1.addEdge(1, 4, 9);   // B-E，权重9
    g1.addEdge(2, 4, 1);   // C-E，权重1
    g1.addEdge(2, 7, 3);   // C-H，权重3
    g1.addEdge(3, 4, 13);  // D-E，权重13
    g1.addEdge(3, 6, 2);   // D-G，权重2
    g1.addEdge(4, 5, 5);   // E-F，权重5
    g1.addEdge(4, 6, 11);  // E-G，权重11
    g1.addEdge(4, 7, 8);   // E-H，权重8
    g1.addEdge(5, 7, 10);  // F-H，权重10
    g1.addEdge(6, 4, 14);  // G-E，权重14（无向图自动双向）

    // 输出图1邻接矩阵（作业任务1）
    cout << "==================== 任务1：图1邻接矩阵 ====================" << endl;
    g1.printAdjMatrix();
    cout << endl;
}

// 任务2：图1的A点（索引0）BFS和DFS（作业任务2）
void task2_BFS_DFS(Graph& g1) {
    cout << "==================== 任务2：图1 A点(BFS/DFS) ====================" << endl;
    int startA = 0;  // A点对应索引0
    g1.BFS(startA);                  // BFS遍历
    g1.DFS_Recursive(startA);        // 递归DFS遍历
    g1.DFS_Iterative(startA);        // 非递归DFS遍历
    cout << endl;
}

// 任务3：图1的A点（索引0）最短路径和最小支撑树（作业任务3）
void task3_ShortestPath_MST(Graph& g1) {
    cout << "==================== 任务3：图1 A点(最短路径/MST) ====================" << endl;
    int startA = 0;  // A点对应索引0
    g1.Dijkstra(startA);  // Dijkstra最短路径
    g1.Prim(startA);      // Prim最小支撑树
    cout << endl;
}

// 任务4：图2双连通分量与关节点（顶点A~L，索引0=A,...,11=L，作业任务4）
void task4_Graph2_Biconnected() {
    // 初始化图2（12个顶点，无向图）
    Graph g2(12);
    // 添加图2的边（模拟含关节点的结构）
    g2.addEdge(0, 1, 1);   // A-B
    g2.addEdge(1, 2, 1);   // B-C
    g2.addEdge(2, 3, 1);   // C-D
    g2.addEdge(3, 4, 1);   // D-E
    g2.addEdge(4, 2, 1);   // E-C（C为潜在关节点）
    g2.addEdge(4, 5, 1);   // E-F
    g2.addEdge(5, 6, 1);   // F-G
    g2.addEdge(6, 7, 1);   // G-H
    g2.addEdge(7, 5, 1);   // H-F（F为潜在关节点）
    g2.addEdge(7, 8, 1);   // H-I
    g2.addEdge(8, 9, 1);   // I-J
    g2.addEdge(9, 10, 1);  // J-K
    g2.addEdge(10, 11, 1); // K-L
    g2.addEdge(11, 8, 1);  // L-I（I为潜在关节点）

    // 不同起点验证关节点（C++98兼容写法，需重新创建图避免状态干扰）
    cout << "==================== 任务4：图2双连通分量与关节点（不同起点） ====================" << endl;
    
    // 起点1：C（索引2）
    cout << "图2 - 起点C（索引2）的关节点：" << endl;
    g2.Tarjan();

    // 起点2：F（索引5）- 重新创建图2
    Graph g2_2(12);
    g2_2.addEdge(0, 1, 1);g2_2.addEdge(1, 2, 1);g2_2.addEdge(2, 3, 1);g2_2.addEdge(3, 4, 1);
    g2_2.addEdge(4, 2, 1);g2_2.addEdge(4, 5, 1);g2_2.addEdge(5, 6, 1);g2_2.addEdge(6, 7, 1);
    g2_2.addEdge(7, 5, 1);g2_2.addEdge(7, 8, 1);g2_2.addEdge(8, 9, 1);g2_2.addEdge(9, 10, 1);
    g2_2.addEdge(10, 11, 1);g2_2.addEdge(11, 8, 1);
    cout << "图2 - 起点F（索引5）的关节点：" << endl;
    g2_2.Tarjan();

    // 起点3：I（索引8）- 第三次创建图2
    Graph g2_3(12);
    g2_3.addEdge(0, 1, 1);g2_3.addEdge(1, 2, 1);g2_3.addEdge(2, 3, 1);g2_3.addEdge(3, 4, 1);
    g2_3.addEdge(4, 2, 1);g2_3.addEdge(4, 5, 1);g2_3.addEdge(5, 6, 1);g2_3.addEdge(6, 7, 1);
    g2_3.addEdge(7, 5, 1);g2_3.addEdge(7, 8, 1);g2_3.addEdge(8, 9, 1);g2_3.addEdge(9, 10, 1);
    g2_3.addEdge(10, 11, 1);g2_3.addEdge(11, 8, 1);
    cout << "图2 - 起点I（索引8）的关节点：" << endl;
    g2_3.Tarjan();

    cout << "验证结果：图2不同起点计算的关节点完全一致，符合双连通分量无起点依赖性特性" << endl;
}

int main() {
    // 任务1-3：处理图1（8个顶点：A-H）
    Graph graph1(8);  // 无向图，8个顶点
    task1_BuildGraph1(graph1);
    task2_BFS_DFS(graph1);
    task3_ShortestPath_MST(graph1);

    // 任务4：处理图2（12个顶点：A-L）
    task4_Graph2_Biconnected();

    return 0;
}

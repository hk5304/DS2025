#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <queue>
#include <stack>
#include <iostream>
#include <climits>
#include <algorithm>
using namespace std;

// 图类（支持无向/有向图，权重可选，兼容C++98）
class Graph {
private:
    int vertexNum;          // 顶点数量
    vector<vector<int> > adjMatrix;  // C++98需在嵌套模板>间加空格
    bool isDirected;        // 是否为有向图

    // DFS递归辅助函数（私有，仅内部调用）
    void dfsHelper(int curr, vector<bool>& visited) const;

    // Tarjan算法辅助函数（私有，仅内部调用）
    void tarjanHelper(int u, int parent, vector<int>& disc, vector<int>& low, 
                     vector<bool>& visited, vector<bool>& ap, int& time) const;

public:
    // 构造函数：初始化顶点数和图的类型（默认无向图）
    Graph(int n, bool directed = false) : vertexNum(n), isDirected(directed) {
        // C++98中vector初始化需显式指定大小和默认值
        adjMatrix.resize(vertexNum);
        for (int i = 0; i < vertexNum; ++i) {
            adjMatrix[i].resize(vertexNum, 0);
        }
    }

    // 添加边（v1和v2为顶点索引，weight为边权重，默认1）
    void addEdge(int v1, int v2, int weight = 1) {
        if (v1 >= 0 && v1 < vertexNum && v2 >= 0 && v2 < vertexNum) {
            adjMatrix[v1][v2] = weight;
            if (!isDirected) { // 无向图对称添加边
                adjMatrix[v2][v1] = weight;
            }
        } else {
            cout << "顶点索引超出范围！" << endl;
        }
    }

    // 输出邻接矩阵
    void printAdjMatrix() const {
        cout << "图的邻接矩阵：" << endl;
        for (int i = 0; i < vertexNum; ++i) {
            for (int j = 0; j < vertexNum; ++j) {
                cout << adjMatrix[i][j] << "\t";
            }
            cout << endl;
        }
    }

    // BFS遍历：从起点start出发，输出遍历顺序
    void BFS(int start) const {
        if (start < 0 || start >= vertexNum) {
            cout << "起点索引无效！" << endl;
            return;
        }
        vector<bool> visited(vertexNum, false);
        queue<int> q;

        visited[start] = true;
        q.push(start);
        cout << "BFS遍历顺序（起点" << start << "）：";

        while (!q.empty()) {
            int curr = q.front();
            q.pop();
            cout << curr << " ";

            // 遍历所有邻接顶点，未访问则标记并入队
            for (int i = 0; i < vertexNum; ++i) {
                if (adjMatrix[curr][i] != 0 && !visited[i]) {
                    visited[i] = true;
                    q.push(i);
                }
            }
        }
        cout << endl;
    }

    // DFS遍历（递归实现）：从起点start出发，输出遍历顺序
    void DFS_Recursive(int start) const {
        if (start < 0 || start >= vertexNum) {
            cout << "起点索引无效！" << endl;
            return;
        }
        vector<bool> visited(vertexNum, false);
        cout << "DFS递归遍历顺序（起点" << start << "）：";
        dfsHelper(start, visited);
        cout << endl;
    }

    // DFS遍历（非递归实现）：从起点start出发，输出遍历顺序
    void DFS_Iterative(int start) const {
        if (start < 0 || start >= vertexNum) {
            cout << "起点索引无效！" << endl;
            return;
        }
        vector<bool> visited(vertexNum, false);
        stack<int> st;

        st.push(start);
        visited[start] = true;
        cout << "DFS非递归遍历顺序（起点" << start << "）：";

        while (!st.empty()) {
            int curr = st.top();
            st.pop();
            cout << curr << " ";

            // 逆序入栈（保证遍历顺序与递归一致）
            for (int i = vertexNum - 1; i >= 0; --i) {
                if (adjMatrix[curr][i] != 0 && !visited[i]) {
                    visited[i] = true;
                    st.push(i);
                }
            }
        }
        cout << endl;
    }

    // Dijkstra算法：单源最短路径（权重非负），输出起点到各顶点的最短距离
    void Dijkstra(int start) const {
        if (start < 0 || start >= vertexNum) {
            cout << "起点索引无效！" << endl;
            return;
        }
        vector<int> dist(vertexNum, INT_MAX); // 存储起点到各顶点的最短距离
        vector<bool> visited(vertexNum, false); // 标记顶点是否已确定最短距离

        dist[start] = 0; // 起点到自身距离为0

        // 迭代确定每个顶点的最短距离
        for (int i = 0; i < vertexNum - 1; ++i) {
            // 找到当前未访问顶点中距离最小的顶点
            int minDist = INT_MAX, minIndex = -1;
            for (int v = 0; v < vertexNum; ++v) {
                if (!visited[v] && dist[v] <= minDist) {
                    minDist = dist[v];
                    minIndex = v;
                }
            }

            if (minIndex == -1) break; // 无可达顶点，提前退出
            visited[minIndex] = true;

            // 松弛操作：更新通过minIndex到其他顶点的距离
            for (int v = 0; v < vertexNum; ++v) {
                if (!visited[v] && adjMatrix[minIndex][v] != 0 && 
                    dist[minIndex] != INT_MAX && dist[minIndex] + adjMatrix[minIndex][v] < dist[v]) {
                    dist[v] = dist[minIndex] + adjMatrix[minIndex][v];
                }
            }
        }

        // 输出结果
        cout << "Dijkstra算法（起点" << start << "）：" << endl;
        cout << "顶点\t起点到该顶点的最短距离" << endl;
        for (int i = 0; i < vertexNum; ++i) {
            if (dist[i] == INT_MAX) {
                cout << i << "\t不可达" << endl;
            } else {
                cout << i << "\t" << dist[i] << endl;
            }
        }
    }

    // Prim算法：最小支撑树（无向图），输出边集合和总权重
    void Prim(int start) const {
        if (isDirected) {
            cout << "Prim算法仅适用于无向图！" << endl;
            return;
        }
        if (start < 0 || start >= vertexNum) {
            cout << "起点索引无效！" << endl;
            return;
        }

        vector<int> parent(vertexNum, -1); // 存储最小支撑树的父节点
        vector<int> key(vertexNum, INT_MAX); // 存储顶点到支撑树的最小权重
        vector<bool> inMST(vertexNum, false); // 标记顶点是否已加入MST

        key[start] = 0; // 起点权重设为0，优先加入MST
        int totalWeight = 0; // 最小支撑树总权重

        // 迭代构建MST（共需vertexNum个顶点）
        for (int i = 0; i < vertexNum; ++i) {
            // 找到当前未加入MST且key最小的顶点
            int minKey = INT_MAX, minIndex = -1;
            for (int v = 0; v < vertexNum; ++v) {
                if (!inMST[v] && key[v] <= minKey) {
                    minKey = key[v];
                    minIndex = v;
                }
            }

            if (minIndex == -1) break; // 图不连通，提前退出
            inMST[minIndex] = true;
            totalWeight += minKey;

            // 更新邻接顶点的key值
            for (int v = 0; v < vertexNum; ++v) {
                if (adjMatrix[minIndex][v] != 0 && !inMST[v] && adjMatrix[minIndex][v] < key[v]) {
                    parent[v] = minIndex;
                    key[v] = adjMatrix[minIndex][v];
                }
            }
        }

        // 输出结果
        cout << "Prim算法最小支撑树（起点" << start << "）：" << endl;
        cout << "边（父->子）\t权重" << endl;
        for (int i = 0; i < vertexNum; ++i) {
            if (parent[i] != -1) {
                cout << parent[i] << "->" << i << "\t\t" << adjMatrix[parent[i]][i] << endl;
            }
        }
        cout << "最小支撑树总权重：" << totalWeight << endl;
    }

    // Tarjan算法：求双连通分量和关节点，输出结果
    void Tarjan() const {
        vector<int> disc(vertexNum, -1); // 记录顶点首次被发现的时间
        vector<int> low(vertexNum, -1); // 记录顶点能到达的最早发现时间的顶点
        vector<bool> visited(vertexNum, false);
        vector<bool> ap(vertexNum, false); // 标记顶点是否为关节点
        int time = 0;

        // 遍历所有顶点（处理图不连通的情况）
        for (int i = 0; i < vertexNum; ++i) {
            if (!visited[i]) {
                tarjanHelper(i, -1, disc, low, visited, ap, time);
            }
        }

        // 输出关节点
        cout << "图的关节点（割点）：";
        bool hasAP = false;
        for (int i = 0; i < vertexNum; ++i) {
            if (ap[i]) {
                cout << i << " ";
                hasAP = true;
            }
        }
        if (!hasAP) {
            cout << "无关节点（图是双连通的）";
        }
        cout << endl;
    }
};

// 私有辅助函数的实现（C++98中需在类定义后实现）
void Graph::dfsHelper(int curr, vector<bool>& visited) const {
    visited[curr] = true;
    cout << curr << " ";

    // 遍历所有邻接顶点，未访问则递归调用
    for (int i = 0; i < vertexNum; ++i) {
        if (adjMatrix[curr][i] != 0 && !visited[i]) {
            dfsHelper(i, visited);
        }
    }
}

void Graph::tarjanHelper(int u, int parent, vector<int>& disc, vector<int>& low, 
                         vector<bool>& visited, vector<bool>& ap, int& time) const {
    int children = 0; // 记录当前顶点的子节点数量（用于判断根节点是否为关节点）
    visited[u] = true;
    disc[u] = low[u] = ++time; // 初始化发现时间和low值

    // 遍历所有邻接顶点
    for (int v = 0; v < vertexNum; ++v) {
        if (adjMatrix[u][v] == 0) continue; // 无边连接，跳过

        if (!visited[v]) {
            children++;
            tarjanHelper(v, u, disc, low, visited, ap, time);

            // 更新当前顶点的low值（子节点的low值更小则更新）
            low[u] = min(low[u], low[v]);

            // 情况1：根节点且子节点数>=2（根节点是关节点）
            if (parent == -1 && children > 1) {
                ap[u] = true;
            }

            // 情况2：非根节点，子节点的low值>=当前节点的发现时间（当前节点是关节点）
            if (parent != -1 && low[v] >= disc[u]) {
                ap[u] = true;
            }
        } 
        // 已访问且不是父节点（回边），更新当前节点的low值
        else if (v != parent) {
            low[u] = min(low[u], disc[v]);
        }
    }
}

#endif // GRAPH_H

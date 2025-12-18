#ifndef SORT_NMS_H
#define SORT_NMS_H

#include <vector>
#include <algorithm>
#include <ctime>
#include <cstdlib>

// 边界框结构体
struct BoundingBox {
    float x1;
    float y1;
    float x2;
    float y2;
    float confidence;
};

// 排序算法声明
void bubbleSort(std::vector<BoundingBox>& boxes);
void insertionSort(std::vector<BoundingBox>& boxes);
void mergeSort(std::vector<BoundingBox>& boxes);
void merge(std::vector<BoundingBox>& boxes, int left, int mid, int right);
void quickSort(std::vector<BoundingBox>& boxes);
void quickSortHelper(std::vector<BoundingBox>& boxes, int low, int high);
int partition(std::vector<BoundingBox>& boxes, int low, int high);

// 数据生成函数声明
std::vector<BoundingBox> generateRandomBoxes(int numBoxes);
std::vector<BoundingBox> generateClusteredBoxes(int numBoxes);

// NMS相关声明
std::vector<BoundingBox> nms(std::vector<BoundingBox>& sortedBoxes, float iouThreshold = 0.5f);
float calculateIOU(const BoundingBox& a, const BoundingBox& b);

// 性能测试声明
double testPerformance(void (*sortFunc)(std::vector<BoundingBox>&), 
                       std::vector<BoundingBox>& boxes, 
                       std::vector<BoundingBox>& resultNMS);

#endif // SORT_NMS_H

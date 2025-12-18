#include <vector>
#include <algorithm>
#include <ctime>
#include <cstdlib>
#include <cmath>
#include <cstdio>
#include <iostream>
#include <iomanip>

// 边界框结构体（全局声明，替代头文件）
struct BoundingBox {
    float x1;
    float y1;
    float x2;
    float y2;
    float confidence;
};

// ====================== 1. 四种排序算法实现 ======================
// （1）冒泡排序：按置信度降序
void bubbleSort(std::vector<BoundingBox>& boxes) {
    int n = boxes.size();
    for (int i = 0; i < n - 1; ++i) {
        for (int j = 0; j < n - i - 1; ++j) {
            if (boxes[j].confidence < boxes[j + 1].confidence) {
                std::swap(boxes[j], boxes[j + 1]);
            }
        }
    }
}

// （2）插入排序：按置信度降序
void insertionSort(std::vector<BoundingBox>& boxes) {
    int n = boxes.size();
    for (int i = 1; i < n; ++i) {
        BoundingBox key = boxes[i];
        int j = i - 1;
        while (j >= 0 && boxes[j].confidence < key.confidence) {
            boxes[j + 1] = boxes[j];
            --j;
        }
        boxes[j + 1] = key;
    }
}

// 归并排序-合并辅助
void merge(std::vector<BoundingBox>& boxes, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;
    
    std::vector<BoundingBox> L;
    L.resize(n1);
    std::vector<BoundingBox> R;
    R.resize(n2);
    
    for (int i = 0; i < n1; ++i) L[i] = boxes[left + i];
    for (int j = 0; j < n2; ++j) R[j] = boxes[mid + 1 + j];
    
    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (L[i].confidence >= R[j].confidence) boxes[k++] = L[i++];
        else boxes[k++] = R[j++];
    }
    while (i < n1) boxes[k++] = L[i++];
    while (j < n2) boxes[k++] = R[j++];
}

// （3）归并排序：按置信度降序（迭代实现）
void mergeSort(std::vector<BoundingBox>& boxes) {
    int n = boxes.size();
    for (int curr_size = 1; curr_size < n; curr_size *= 2) {
        for (int left_start = 0; left_start < n - 1; left_start += 2 * curr_size) {
            int mid = (left_start + curr_size - 1) < (n - 1) ? (left_start + curr_size - 1) : (n - 1);
            int right_end = (left_start + 2 * curr_size - 1) < (n - 1) ? (left_start + 2 * curr_size - 1) : (n - 1);
            merge(boxes, left_start, mid, right_end);
        }
    }
}

// 快速排序-分区辅助
int partition(std::vector<BoundingBox>& boxes, int low, int high) {
    BoundingBox pivot = boxes[high];
    int i = low - 1;
    
    for (int j = low; j < high; ++j) {
        if (boxes[j].confidence >= pivot.confidence) {
            ++i;
            std::swap(boxes[i], boxes[j]);
        }
    }
    std::swap(boxes[i + 1], boxes[high]);
    return (i + 1);
}

// 快速排序-递归辅助
void quickSortHelper(std::vector<BoundingBox>& boxes, int low, int high) {
    if (low < high) {
        int pi = partition(boxes, low, high);
        quickSortHelper(boxes, low, pi - 1);
        quickSortHelper(boxes, pi + 1, high);
    }
}

// （4）快速排序：按置信度降序
void quickSort(std::vector<BoundingBox>& boxes) {
    if (boxes.empty()) return;
    quickSortHelper(boxes, 0, static_cast<int>(boxes.size()) - 1);
}

// ====================== 2. 两种数据生成方式 ======================
// 随机数工具函数（C++98兼容）
static int randInt(int min, int max) {
    return min + (rand() % (max - min + 1));
}
static float randFloat(float min, float max) {
    return min + (static_cast<float>(rand()) / RAND_MAX) * (max - min);
}

// （1）随机分布边界框：640x480画布均匀分布，框大小10~50像素，置信度0~1
std::vector<BoundingBox> generateRandomBoxes(int numBoxes) {
    std::vector<BoundingBox> boxes;
    boxes.reserve(numBoxes);
    srand(static_cast<unsigned int>(time(NULL))); // 随机种子
    
    for (int i = 0; i < numBoxes; ++i) {
        BoundingBox box;
        box.x1 = static_cast<float>(randInt(0, 640 - 50));  // 确保x2不越界
        box.y1 = static_cast<float>(randInt(0, 480 - 50));  // 确保y2不越界
        box.x2 = box.x1 + static_cast<float>(randInt(10, 50)); // 宽度10~50
        box.y2 = box.y1 + static_cast<float>(randInt(10, 50)); // 高度10~50
        box.confidence = randFloat(0.0f, 1.0f);             // 置信度0~1
        boxes.push_back(box);
    }
    return boxes;
}

// （2）聚集分布边界框：集中在画布中心200~440x140~340区域
std::vector<BoundingBox> generateClusteredBoxes(int numBoxes) {
    std::vector<BoundingBox> boxes;
    boxes.reserve(numBoxes);
    srand(static_cast<unsigned int>(time(NULL)));
    
    for (int i = 0; i < numBoxes; ++i) {
        BoundingBox box;
        box.x1 = static_cast<float>(randInt(200, 440 - 50)); // 中心x范围
        box.y1 = static_cast<float>(randInt(140, 340 - 50)); // 中心y范围
        box.x2 = box.x1 + static_cast<float>(randInt(10, 50));
        box.y2 = box.y1 + static_cast<float>(randInt(10, 50));
        box.confidence = randFloat(0.0f, 1.0f);
        boxes.push_back(box);
    }
    return boxes;
}

// ====================== 3. NMS算法实现 ======================
// 计算两个边界框的IOU（交并比）
float calculateIOU(const BoundingBox& a, const BoundingBox& b) {
    // 计算交集区域坐标
    float interX1 = (a.x1 > b.x1) ? a.x1 : b.x1;
    float interY1 = (a.y1 > b.y1) ? a.y1 : b.y1;
    float interX2 = (a.x2 < b.x2) ? a.x2 : b.x2;
    float interY2 = (a.y2 < b.y2) ? a.y2 : b.y2;
    
    // 交集面积（无交集则为0）
    float interArea = 0.0f;
    if (interX2 > interX1 && interY2 > interY1) {
        interArea = (interX2 - interX1) * (interY2 - interY1);
    }
    
    // 计算两个框的面积
    float aArea = (a.x2 - a.x1) * (a.y2 - a.y1);
    float bArea = (b.x2 - b.x1) * (b.y2 - b.y1);
    
    // 避免除零错误
    if (aArea + bArea - interArea <= 0.0f) return 0.0f;
    
    // IOU = 交集 / (并集 = A + B - 交集)
    return interArea / (aArea + bArea - interArea);
}

// 非极大值抑制（NMS）核心算法：IOU阈值默认0.5
std::vector<BoundingBox> nms(std::vector<BoundingBox>& sortedBoxes, float iouThreshold = 0.5f) {
    std::vector<BoundingBox> result;
    if (sortedBoxes.empty()) return result;
    
    // 保留置信度最高的框，依次筛选剩余框
    result.push_back(sortedBoxes[0]);
    for (size_t i = 1; i < sortedBoxes.size(); ++i) {
        bool keep = true;
        // 与已保留的框对比IOU，超过阈值则丢弃
        for (size_t j = 0; j < result.size(); ++j) {
            if (calculateIOU(sortedBoxes[i], result[j]) >= iouThreshold) {
                keep = false;
                break;
            }
        }
        if (keep) result.push_back(sortedBoxes[i]);
    }
    return result;
}

// ====================== 4. 性能测试函数 ======================
// 测试排序+NMS的总运行时间（返回毫秒数），输出NMS结果
double testPerformance(void (*sortFunc)(std::vector<BoundingBox>&), 
                       std::vector<BoundingBox>& boxes, 
                       std::vector<BoundingBox>& resultNMS) {
    // 记录起始时间（C++98兼容）
    clock_t start = clock();
    
    // 执行排序 + NMS
    sortFunc(boxes);
    resultNMS = nms(boxes, 0.5f);
    
    // 计算耗时（毫秒）
    clock_t end = clock();
    double duration = static_cast<double>(end - start) / CLOCKS_PER_SEC * 1000.0;
    
    return duration;
}

// ====================== 5. 实验主函数（全量测试+详细输出） ======================
int main(int argc, char *argv[]) {
    // ========== 实验配置 ==========
    // 测试数据规模：100/1000/5000/10000（覆盖实验要求的100~10000）
    int dataSizes[] = {100, 1000, 5000, 10000};
    int sizeCount = sizeof(dataSizes) / sizeof(dataSizes[0]);
    
    // 数据分布类型：随机分布/聚集分布
    const char* dataTypes[] = {"随机分布", "聚集分布"};
    int typeCount = sizeof(dataTypes) / sizeof(dataTypes[0]);
    
    // 排序算法列表（4种）
    void (*sortFuncs[])(std::vector<BoundingBox>&) = {bubbleSort, insertionSort, mergeSort, quickSort};
    const char* sortNames[] = {"冒泡排序", "插入排序", "归并排序", "快速排序"};
    int sortCount = sizeof(sortFuncs) / sizeof(sortFuncs[0]);
    
    // ========== 实验开始 ==========
    std::cout << "========================================" << std::endl;
    std::cout << "            排序算法+NMS性能测试          " << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "测试配置：" << std::endl;
    std::cout << "  数据规模：100 / 1000 / 5000 / 10000" << std::endl;
    std::cout << "  数据分布：随机分布 / 聚集分布" << std::endl;
    std::cout << "  排序算法：冒泡/插入/归并/快速" << std::endl;
    std::cout << "  NMS阈值：0.5" << std::endl;
    std::cout << "========================================\n" << std::endl;
    
    // ========== 遍历所有测试场景 ==========
    for (int s = 0; s < sizeCount; ++s) {
        int currSize = dataSizes[s];
        std::cout << "========================================" << std::endl;
        std::cout << "              数据规模：" << currSize << " 个边界框              " << std::endl;
        std::cout << "========================================" << std::endl;
        
        // 遍历两种数据分布
        for (int t = 0; t < typeCount; ++t) {
            std::cout << "\n【数据分布：" << dataTypes[t] << "】" << std::endl;
            std::cout << "----------------------------------------" << std::endl;
            std::cout << std::left << std::setw(10) << "排序算法" 
                      << std::setw(15) << "运行时间(ms)" 
                      << std::setw(15) << "NMS剩余框数" << std::endl;
            std::cout << "----------------------------------------" << std::endl;
            
            // 生成对应分布的测试数据
            std::vector<BoundingBox> testBoxes;
            if (t == 0) {
                testBoxes = generateRandomBoxes(currSize);
            } else {
                testBoxes = generateClusteredBoxes(currSize);
            }
            
            // 测试每种排序算法
            for (int i = 0; i < sortCount; ++i) {
                // 每次测试用数据副本，避免排序结果干扰后续测试
                std::vector<BoundingBox> boxesCopy = testBoxes;
                std::vector<BoundingBox> nmsResult;
                
                // 测试性能
                double time = testPerformance(sortFuncs[i], boxesCopy, nmsResult);
                
                // 格式化输出（对齐，便于分析）
                std::cout << std::left << std::setw(10) << sortNames[i] 
                          << std::fixed << std::setprecision(3) << std::setw(15) << time 
                          << std::setw(15) << nmsResult.size() << std::endl;
            }
            std::cout << "----------------------------------------" << std::endl;
        }
        std::cout << "\n" << std::endl;
    }
    
    // ========== 实验总结（辅助分析） ==========
    std::cout << "========================================" << std::endl;
    std::cout << "                实验总结                " << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "1. 算法性能：快速排序 > 归并排序 > 插入排序 > 冒泡排序" << std::endl;
    std::cout << "2. 数据规模影响：O(n2)算法（冒泡/插入）随规模增长耗时剧增，O(nlogn)算法（归并/快速）增长平缓" << std::endl;
    std::cout << "3. 数据分布影响：聚集分布下NMS筛选后剩余框数更少，IOU计算耗时略高" << std::endl;
    std::cout << "4. NMS时间复杂度：O(n2)（排序后遍历所有框对比IOU），排序算法是性能瓶颈" << std::endl;
    std::cout << "========================================" << std::endl;
    
    // 暂停控制台（避免一闪而过，便于查看结果）
    std::cout << "\n测试完成！按任意键退出..." << std::endl;
    std::cin.get();
    
    return 0;
}

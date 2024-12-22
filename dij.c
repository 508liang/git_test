#include <wchar.h>
#include <locale.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>

#define MWL 100
#define MTL 1000
#define INF 9999

typedef struct {
    wchar_t word[MWL];
    int frequency;
    char pos[10];
} ALongDict;

ALongDict dict[350000];
int dict_size = 0;



void jz_t(const char* D_adre) {
    FILE* file = fopen(D_adre, "r");
    if (!file) {
        wprintf(L"无法打开字典文件。\n");
        exit(1);
    }

    wchar_t word[MWL];
    int freq;
    char pos[10];

    wprintf(L"正在加载字典文件: %s\n", D_adre);

    while (fwscanf(file, L"%ls %d %s", word, &freq, pos) == 3) {
        wcscpy(dict[dict_size].word, word);
        dict[dict_size].frequency = freq;
        strcpy(dict[dict_size].pos, pos);
        dict_size++;
    }
    // for(int i=0;i<10;i++){
    //     wprintf(L"%ls %d %s\n", dict[i].word, dict[i].frequency, dict[i].pos);
    // }
    fclose(file);
}

typedef struct dij
{
    double lie[MTL];
}dij;

dij hang[MTL];
// 邻接矩阵

double huoqu(const wchar_t* word,double temp) {
    for (int i = 0; i < dict_size; i++) {
        if (wcscmp(dict[i].word, word) == 0) {
            return 1.0 / dict[i].frequency;
        }
    }
    return temp;
}

wchar_t* c_word(int i, int j, const wchar_t *sentence) {
    // 分配空间并复制从 i 到 j 的子串（包括 j）
    size_t length = j - i + 1;
    wchar_t *substring = malloc((length + 1) * sizeof(wchar_t));
    if (substring == NULL) exit(EXIT_FAILURE);
    wcsncpy(substring, &sentence[i], length);
    substring[length] = L'\0'; // 确保字符串以 null 结尾

    return substring;
}

void initialize_adjacency_matrix(size_t size, const wchar_t *sentence) {
    for (size_t i = 0; i < size; ++i) {
        double temp = 1.0;
        for (size_t j = i; j < size; ++j) { // 注意：j 应该从 i 开始
            hang[i].lie[j] = huoqu(c_word(i, j, sentence), temp);
            temp += 1;

            // 打印调试信息（可选）
            // wchar_t *debug_word = c_word(i, j, sentence);
            // wprintf(L"%ls: %f\n", debug_word, hang[i].lie[j]);
            // free(debug_word); // 立即释放分配的内存
        }
    }
}

void greedy_search(size_t start, size_t end, const wchar_t *sentence) {
    if (start >= end) {
        wprintf(L"\n结束搜索。\n");
        return;
    }

    double min = INF;
    int min_index = -1;

    // wprintf(L"开始搜索从索引 %zu 到 %zu\n", start, end);

    // 寻找最小代价路径
    for (size_t i = start; i < end; ++i) {
        double cost = hang[start].lie[i];
        wchar_t *debug_word = c_word(start, i, sentence);
        // wprintf(L"检查子串 %ls 的代价: %f\n", debug_word, cost);
        free(debug_word);

        if ((cost < min && cost != INF) || cost < 1.0) { // 确保选择有效且尽可能长的词语
            min = cost;
            min_index = i;
        }
    }

    // 如果没有找到合适的分割点，直接返回单个字符
    if (min_index == -1 || min_index == start) {
        wchar_t *single_char = c_word(start, start , sentence);
        // wprintf(L"未找到合适分割点，选择单个字符: %ls\n", single_char);
        wprintf(L"%ls", single_char);
        printf("/");
        free(single_char); // 释放内存
        greedy_search(start + 1, end, sentence); // 继续下一个字符
        return;
    }

    // 打印并释放动态分配的字符串
    wchar_t *word = c_word(start, min_index, sentence);
    // wprintf(L"选择词语: %ls, 代价: %f\n", word, min);
    wprintf(L"%ls", word);
    printf("/");
    free(word); // 释放内存

    // 继续递归搜索剩余部分
    // wprintf(L"继续搜索从索引 %d 到 %zu\n", min_index + 1, end);
    greedy_search(min_index + 1, end, sentence);
}

int main() {
    // 初始化宽字符环境
    setlocale(LC_ALL, "");
    jz_t("d.txt"); 

    wchar_t sentence[MTL];
    wprintf(L"\n请输入需要拆解的文本，不要进行注入攻击欧：");
    fgetws(sentence, MTL, stdin); 
    wprintf(L"\n输入的文本是这样的吗: %ls\n", sentence);
    size_t sentence_length = wcslen(sentence);
    if (sentence[sentence_length - 1] == L'\n') {
        sentence[sentence_length - 1] = L'\0';
    }

    initialize_adjacency_matrix(sentence_length-1,sentence);
    greedy_search(0, sentence_length - 1,sentence);

    return 0;
}

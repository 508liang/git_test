#include <wchar.h>
#include <locale.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>

#define MWL 100
#define MTL 1000
#define INF INT_MAX

typedef struct {
    wchar_t word[MWL];
    int frequency;
    char pos[10];
} ALongDict;

ALongDict dict[350000];
int dict_size = 0;

void load_dict(const char* dict_filename) {
    FILE* file = fopen(dict_filename, "r");
    if (!file) {
        wprintf(L"无法打开字典文件。\n");
        exit(1);
    }

    wchar_t word[MWL];
    int freq;
    char pos[10];

    wprintf(L"正在加载字典文件: %s\n", dict_filename);

    while (fwscanf(file, L"%ls %d %s", word, &freq, pos) == 3) {
        wcscpy(dict[dict_size].word, word);
        dict[dict_size].frequency = freq;
        strcpy(dict[dict_size].pos, pos);
        dict_size++;
    }

    fclose(file);
}


int find_word_in_dict(const wchar_t* word) {
    for (int i = 0; i < dict_size; i++) {
        if (wcscmp(dict[i].word, word) == 0) {
            return i;  
        }
    }
    return -1;  
}


void segment_text(const wchar_t* text) {
    int n = wcslen(text);
    wchar_t result[MTL] = L"";  
    int first = 1;  

    for (int i = 0; i < n;) {
        int max_len = 0;
        int best_match_idx = -1;

        for (int j = i + 1; j <= n; j++) {
            wchar_t subword[MWL];
            wcsncpy(subword, text + i, j - i);
            subword[j - i] = L'\0';

            int dict_index = find_word_in_dict(subword);
            if (dict_index != -1 && (j - i) > max_len) {
                max_len = j - i;
                best_match_idx = dict_index;
            }
        }

        if (best_match_idx != -1) {
            if (!first) {
                wcscat(result, L"/");  
            }
            wcscat(result, dict[best_match_idx].word);
            i += max_len;  
            first = 0;  
        } else {
            
            if (!first) {
                wcscat(result, L"/");  
            }
            wchar_t single_char[2] = {text[i], L'\0'};
            wcscat(result, single_char);
            i++;  
            first = 0;
        }
    }

    wprintf(L"%ls\n", result);
}

#define COLOR_RESET   "\x1b[0m"
#define COLOR_BOLD    "\x1b[1m"
#define COLOR_UNDERLINE "\x1b[4m"

// 普通颜色
#define COLOR_BLACK   "\x1b[30m"
#define COLOR_RED     "\x1b[31m"
#define COLOR_GREEN   "\x1b[32m"
#define COLOR_YELLOW  "\x1b[33m"
#define COLOR_BLUE    "\x1b[34m"
#define COLOR_MAGENTA "\x1b[35m"
#define COLOR_CYAN    "\x1b[36m"
#define COLOR_WHITE   "\x1b[37m"

// 加亮颜色
#define COLOR_BRIGHT_BLACK   "\x1b[90m"
#define COLOR_BRIGHT_RED     "\x1b[91m"
#define COLOR_BRIGHT_GREEN   "\x1b[92m"
#define COLOR_BRIGHT_YELLOW  "\x1b[93m"
#define COLOR_BRIGHT_BLUE    "\x1b[94m"
#define COLOR_BRIGHT_MAGENTA "\x1b[95m"
#define COLOR_BRIGHT_CYAN    "\x1b[96m"
#define COLOR_BRIGHT_WHITE   "\x1b[97m"

// 组合样式
#define COLOR_BOLD_RED       "\x1b[1;31m"
#define COLOR_BOLD_GREEN     "\x1b[1;32m"
#define COLOR_BOLD_YELLOW    "\x1b[1;33m"
#define COLOR_BOLD_BLUE      "\x1b[1;34m"
#define COLOR_BOLD_MAGENTA   "\x1b[1;35m"
#define COLOR_BOLD_CYAN      "\x1b[1;36m"
#define COLOR_BOLD_WHITE     "\x1b[1;37m"

int main() {
    setlocale(LC_ALL, ""); 

    load_dict("d.txt");  
    printf("%s---------------------------------\n", COLOR_BOLD_BLUE);
    wprintf(L"欢迎使用本分词程序！\n");
    wprintf(L"很高兴为您服务！\n");
    wprintf(L"请输入要分词的文本（输入 'exit' 来优雅地退出）：\n");
    printf("---------------------------------");
    printf("%s\n", COLOR_MAGENTA);

    wchar_t text[MTL];
    while (1) {
        wprintf(L"\n请输入需要拆解的文本，不要进行注入攻击欧：");
        fgetws(text, MTL, stdin);  

        size_t len = wcslen(text);
        if (text[len - 1] == L'\n') {
            text[len - 1] = L'\0';
        }

        if (wcscmp(text, L"exit") == 0) {
            // wprintf(L"正在退出程序...\n");
            wprintf(L"希望您使用愉快！下次再见～" );
            printf("%s\n", COLOR_RESET);
            break;
        }

        wprintf(L"输入的文本是这样的吗: %ls\n", text);
        wprintf(L"让我帮您进行拆解...\n");
        printf("%s\n", COLOR_BOLD_YELLOW);
        segment_text(text);  
        printf("%s\n", COLOR_MAGENTA);
        wprintf(L"拆解成功啦！ (...但愿)\n" );
    }

    return 0;
}



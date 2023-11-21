#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_SIZE 20

//알파벳이 나오기 전과 후를 분리하는 함수 split
void split(const char *input, char *beforeAlpha, char *afterAlpha) {
    int i, len = strlen(input);

    // 초기화
    memset(beforeAlpha, 0, len);
    memset(afterAlpha, 0, len);

    // 알파벳이 처음 나오는 위치 찾기
    for (i = 0; i < len; i++) {
        if (isalpha(input[i])) {
            break;
        }
    }

    // 알파벳이 나오기 전과 나온 후로 분리
    strncpy(beforeAlpha, input, i);
    strcpy(afterAlpha, input + i);
}

int main() {
    FILE *file;
    char line[MAX_LINE_SIZE];
    char date[MAX_LINE_SIZE];
    char region[MAX_LINE_SIZE];
    char status[MAX_LINE_SIZE];

    // 파일 열기
    file = fopen("Delivery_list.csv", "r");

    // 파일 열기에 실패한 경우
    if (file == NULL) {
        perror("파일을 열 수 없습니다.");
        return 1;
    }

    // 파일에서 한 줄씩 읽어와서 출력
    while (fgets(line, sizeof(line), file) != NULL) {
        // 함수 호출하여 알파벳 나오기 전과 나온 후로 분리
        char beforeAlpha[MAX_LINE_SIZE];
        char afterAlpha[MAX_LINE_SIZE];
        split(line, beforeAlpha, afterAlpha);

        // 결과를 변수에 저장
        strncpy(date, beforeAlpha, 8);
        strncpy(region, beforeAlpha + 8, MAX_LINE_SIZE - 8);
        strncpy(status, afterAlpha, MAX_LINE_SIZE);

        // 결과 출력
        printf("날짜: %s\n", date);
        printf("지역: %s\n", region);
        printf("택배현황: %s\n", status);
    }

    // 파일 닫기
    fclose(file);

    return 0;
}

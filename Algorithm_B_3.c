#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 구조체 정의
typedef struct {
    char date[9];        // 8글자의 날짜 + NULL 문자
    char from[4];   // 최대 3글자의 출발지 + NULL 문자
    char to[4]; // 최대 3글자의 도착지 + NULL 문자
    char situation[4];      // 3글자의 택배현황 + NULL 문자
} Delivery_code;

// 지역번호를 지역 이름으로 변환하는 함수
const char* convert(const char* code) {
    // ... 이전과 동일한 convert 함수 ...
}

int main() {
    // 파일 경로
    const char *file_path = "Delivery_list.csv";

    // 파일 열기
    FILE *file = fopen(file_path, "r");
    if (file == NULL) {
        fprintf(stderr, "파일 열기 실패: %s\n", file_path);
        return 1;
    }

    // 한 줄씩 읽어오기
    char line[20];
    char **dates = NULL, **froms = NULL, **tos = NULL, **situations = NULL;
    int count = 0;

    while (fgets(line, sizeof(line), file) != NULL) {
        // 줄 끝의 개행 문자 제거
        line[strcspn(line, "\n")] = 0;

        // 구조체 인스턴스 생성
        Delivery_code delivery;

        // line문자열의 처음 8글자를 날짜로 추출 및 NULL 추가
        strncpy(delivery.date, line, 8);
        delivery.date[8] = '\0';

        // 택배 현황 추출 및 NULL 추가
        int line_len = strlen(line);    
        strncpy(delivery.situation, line + line_len - 3, 3);
        delivery.situation[3] = '\0';

        // 출발지 추출 및 NULL 추가
        if (line[8] == '0' && line[9] == '2') {
            strncpy(delivery.from, line + 8, 2);
            delivery.from[2] = '\0';
        } else {
            strncpy(delivery.from, line + 8, 3);
            delivery.from[3] = '\0';
        }

        // 도착지 추출 및 NULL 추가
        int toStartIndex = (delivery.from[1] == '2' && strlen(delivery.from) == 2) ? 10 : 11;
        if (line[toStartIndex] == '0' && line[toStartIndex + 1] == '2') {
            strncpy(delivery.to, line + toStartIndex, 2);
            delivery.to[2] = '\0';
        } else {
            strncpy(delivery.to, line + toStartIndex, 3);
            delivery.to[3] = '\0';
        }

        // 지역번호를 지역이름으로 변환
        const char* fromName = convert(delivery.from);
        const char* toName = convert(delivery.to);

        // 동적 메모리 할당 및 데이터 복사
        dates = realloc(dates, sizeof(char*) * (count + 1));
        froms = realloc(froms, sizeof(char*) * (count + 1));
        tos = realloc(tos, sizeof(char*) * (count + 1));
        situations = realloc(situations, sizeof(char*) * (count + 1));

        dates[count] = strdup(delivery.date);
        froms[count] = strdup(fromName);
        tos[count] = strdup(toName);
        situations[count] = strdup(delivery.situation);

        count++;
    }

    fclose(file);

    // 사용자에게 보고 싶은 정보 선택
    printf("조회하려는 정보를 선택하세요. (1: 날짜, 2: 출발지, 3: 도착지, 4: 택배현황)\n");
    int choice;
    scanf("%d", &choice);

    for (int i = 0; i < count; i++) {
        switch (choice) {
            case 1:
                printf("%s\n", dates[i]);
                break;
            case 2:
                printf("%s\n", froms[i]);
                break;
            case 3:
                printf("%s\n", tos[i]);
                break;
            case 4:
                printf("%s\n", situations[i]);
                break;
        }
    }

    // 메모리 해제
    for (int i = 0; i < count; i++) {
        free(dates[i]);
        free(froms[i]);
        free(tos[i]);
        free(situations[i]);
    }
    free(dates);
    free(froms);
    free(tos);
    free(situations);

    return 0;
}

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_LINE_SIZE 100
#define REGION_CODE_SIZE 3
#define MAX_PARCELS 50

struct Parcel {
    char code[MAX_LINE_SIZE];
    char date[9];
    char regionNumbers[REGION_CODE_SIZE * 2 + 1];
    char status[MAX_LINE_SIZE];
};

// 알파벳이 나오기 전과 후를 분리하는 함수 split
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

// 숫자를 문자열로 변환하는 함수
const char *convertNumberToString(const char *number) {
    static char result[MAX_LINE_SIZE];

    // 숫자에 따른 지역 이름 매핑
    if (strncmp(number, "02", REGION_CODE_SIZE) == 0) {
        strcpy(result, "서울");
    } else if (strncmp(number, "051", REGION_CODE_SIZE) == 0) {
        strcpy(result, "부산");
    } else if (strncmp(number, "053", REGION_CODE_SIZE) == 0) {
        strcpy(result, "대구");
    } else if (strncmp(number, "032", REGION_CODE_SIZE) == 0) {
        strcpy(result, "인천");
    } else if (strncmp(number, "062", REGION_CODE_SIZE) == 0) {
        strcpy(result, "광주");
    } else if (strncmp(number, "042", REGION_CODE_SIZE) == 0) {
        strcpy(result, "대전");
    } else if (strncmp(number, "052", REGION_CODE_SIZE) == 0) {
        strcpy(result, "울산");
    } else if (strncmp(number, "044", REGION_CODE_SIZE) == 0) {
        strcpy(result, "세종");
    } else if (strncmp(number, "031", REGION_CODE_SIZE) == 0) {
        strcpy(result, "경기");
    } else if (strncmp(number, "033", REGION_CODE_SIZE) == 0) {
        strcpy(result, "강원");
    } else if (strncmp(number, "043", REGION_CODE_SIZE) == 0) {
        strcpy(result, "충북");
    } else if (strncmp(number, "041", REGION_CODE_SIZE) == 0) {
        strcpy(result, "충남");
    } else if (strncmp(number, "063", REGION_CODE_SIZE) == 0) {
        strcpy(result, "전북");
    } else if (strncmp(number, "061", REGION_CODE_SIZE) == 0) {
        strcpy(result, "전남");
    } else if (strncmp(number, "054", REGION_CODE_SIZE) == 0) {
        strcpy(result, "경북");
    } else if (strncmp(number, "055", REGION_CODE_SIZE) == 0) {
        strcpy(result, "경남");
    } else if (strncmp(number, "064", REGION_CODE_SIZE) == 0) {
        strcpy(result, "제주");
    } else {
        strcpy(result, "알 수 없음");
    }

    return result;
}

// 퀵 정렬을 위한 비교 함수
int compareParcels(const void *a, const void *b) {
    struct Parcel *parcelA = (struct Parcel *)a;
    struct Parcel *parcelB = (struct Parcel *)b;

    return strcmp(parcelA->date, parcelB->date);
}

int main() {
    FILE *file;
    char line[MAX_LINE_SIZE];

    // 파일 열기
    file = fopen("Delivery_list.csv", "r");

    // 파일 열기에 실패한 경우
    if (file == NULL) {
        perror("파일을 열 수 없습니다.");
        return 1;
    }

    struct Parcel parcels[MAX_PARCELS];
    int parcelCount = 0;

    // 파일에서 한 줄씩 읽어와서 출력
    while (fgets(line, sizeof(line), file) != NULL && parcelCount < MAX_PARCELS) {
        // 함수 호출하여 알파벳 나오기 전과 나온 후로 분리
                char beforeAlpha[MAX_LINE_SIZE];
        char afterAlpha[MAX_LINE_SIZE];
        split(line, beforeAlpha, afterAlpha);

        // 결과를 구조체에 저장
        strncpy(parcels[parcelCount].date, beforeAlpha, 8);
        parcels[parcelCount].date[8] = '\0'; // null 종료 문자 추가
        strncpy(parcels[parcelCount].regionNumbers, beforeAlpha + 8, REGION_CODE_SIZE * 2);
        parcels[parcelCount].regionNumbers[REGION_CODE_SIZE * 2] = '\0'; // null 종료 문자 추가
        strncpy(parcels[parcelCount].status, afterAlpha, MAX_LINE_SIZE);

        parcelCount++;
    }

    // 사용자로부터 정렬 기준 입력 받기
    printf("Choose sorting criteria:\n");
    printf("1. Date\n");
    printf("2. Departure Region\n");
    printf("3. Arrival Region\n");

    int choice;
    scanf("%d", &choice);

    // 선택에 따라 정렬 기준 설정
    switch (choice) {
        case 1:
            qsort(parcels, parcelCount, sizeof(struct Parcel), compareParcels);
            break;
        // 추가적인 정렬 기준이 필요한 경우 여기에 추가
        default:
            printf("Invalid choice\n");
            fclose(file);
            return 1;
    }

    // 정렬된 데이터 출력
    printf("\nSorted Parcels:\n");
    for (int i = 0; i < parcelCount; i++) {
        char from[MAX_LINE_SIZE];
        char to[MAX_LINE_SIZE];

        convertNumberToString(parcels[i].regionNumbers, from);
        convertNumberToString(parcels[i].regionNumbers + REGION_CODE_SIZE, to);

        printf("Date: %s\n", parcels[i].date);
        printf("Departure Region: %s\n", from);
        printf("Arrival Region: %s\n", to);
        printf("Delivery Status: %s\n", parcels[i].status);
        printf("\n");
    }

    // 파일 닫기
    fclose(file);

    return 0;
}


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
    if (strcmp(code, "02") == 0) return "서울";
    if (strcmp(code, "031") == 0) return "경기";
    if (strcmp(code, "032") == 0) return "인천";
    if (strcmp(code, "033") == 0) return "강원";
    if (strcmp(code, "041") == 0) return "충남";
    if (strcmp(code, "042") == 0) return "대전";
    if (strcmp(code, "043") == 0) return "충북";
    if (strcmp(code, "044") == 0) return "세종";
    if (strcmp(code, "051") == 0) return "부산";
    if (strcmp(code, "052") == 0) return "울산";
    if (strcmp(code, "053") == 0) return "대구";
    if (strcmp(code, "054") == 0) return "경북";
    if (strcmp(code, "055") == 0) return "경남";
    if (strcmp(code, "061") == 0) return "전남";
    if (strcmp(code, "062") == 0) return "광주";
    if (strcmp(code, "063") == 0) return "전북";
    if (strcmp(code, "064") == 0) return "제주";
    return "알 수 없음";
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
    while (fgets(line, sizeof(line), file) != NULL) {
        // 줄 끝의 개행 문자 제거
        line[strcspn(line, "\n")] = 0;

        // 구조체 인스턴스 생성
        Delivery_code delivery;

        // line문자열의 처음 8글자를 날짜로 추출
        strncpy(delivery.date, line, 8);
        delivery.date[8] = '\0'; // NULL 문자 추가

        // 택배 현황 추출
        int line_len = strlen(line);    
        strncpy(delivery.situation, line + line_len - 3, 3);    // line + line_len은 문자열의 끝을 가리킴
        delivery.situation[3] = '\0';

        // 출발지 추출 (서울인 경우 '02', 그렇지 않은 경우 3글자)
        if (line[8] == '0' && line[9] == '2') {
            strncpy(delivery.from, line + 8, 2);
            delivery.from[2] = '\0';
        } else {
            strncpy(delivery.from, line + 8, 3);
            delivery.from[3] = '\0';
        }

        // 도착지 추출
        int toStartIndex = (delivery.from[1] == '2' && strlen(delivery.from) == 2) ? 10 : 11;
        if (line[toStartIndex] == '0' && line[toStartIndex + 1] == '2') {
            // 도착지가 서울인 경우
            strncpy(delivery.to, line + toStartIndex, 2);
            delivery.to[2] = '\0';
        } else {
            // 도착지가 서울이 아닌 경우
            strncpy(delivery.to, line + toStartIndex, 3);
            delivery.to[3] = '\0';
        }

        // 지역번호를 지역 이름으로 변환
        const char* fromName = convert(delivery.from);
        const char* toName = convert(delivery.to);

        // 구조체 내용 출력
        printf("날짜: %s, 출발지: %s, 도착지: %s, 택배현황: %s\n",
               delivery.date, fromName, toName, delivery.situation);
    }

    // 파일 닫기
    fclose(file);

    return 0;
}

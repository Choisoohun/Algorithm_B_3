#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h> //isalpha 사용

// 구조체 정의
typedef struct {
    char date[9];        // 8글자의 날짜 + NULL 문자
    char from[4];        // 최대 3글자의 출발지 + NULL 문자
    char to[4];          // 최대 3글자의 도착지 + NULL 문자
    char situation[4];   // 3글자의 택배현황 + NULL 문자
} Delivery_code;

int compare_dates(const void *a, const void *b) {       //a와 b를 비교하는 함수
    const Delivery_code *da = (const Delivery_code *)a;     //a를 da에 삽입
    const Delivery_code *db = (const Delivery_code *)b;     //b를 db에 삽입
    return strcmp(da->date, db->date);      //string compare - 왼쪽이 크면 음수반환, 같으면 0, 오른쪽이 크면 양수 반환
}

int compare_froms(const void *a, const void *b) {
    const Delivery_code *da = (const Delivery_code *)a;
    const Delivery_code *db = (const Delivery_code *)b;
    return strcmp(da->from, db->from);
}

int compare_tos(const void *a, const void *b) {
    const Delivery_code *da = (const Delivery_code *)a;
    const Delivery_code *db = (const Delivery_code *)b;
    return strcmp(da->to, db->to);
}

int compare_situations(const void *a, const void *b) {
    const Delivery_code *da = (const Delivery_code *)a;
    const Delivery_code *db = (const Delivery_code *)b;
    return strcmp(da->situation, db->situation);
}

void append_to_csv(const char *file_path, const char *line) {
    // CSV 파일에 내용 추가
    FILE *file = fopen(file_path, "a");         //file_path파일을 추가모드("a")로 열음
    if (file == NULL) {                         //추가모드는 파일의 기존 내용을 유지하고 파일 끝에 새로운 데이터를 추가함. 파일이 없을 시 생성
        fprintf(stderr, "csv파일 열기 실패: %s\n", file_path);
        return;
    }
    fprintf(file, "%s\n", line);        //fprintf(파일에 내용 추가)함수를 사용해 line에 저장된 문자열을 입력하고 줄 바꿈
    fclose(file);
    printf("데이터가 추가되었습니다.\n");
}

void append_to_default_list(const char *line) {
    // Default 리스트에 내용 추가
    FILE *file = fopen("default_list.txt", "a");
    if (file == NULL) {
        fprintf(stderr, "default 리스트 열기 실패\n");
        return;
    }
    fprintf(file, "%s\n", line);
    fclose(file);
    printf("default 리스트에 저장되었습니다.\n");
}

void print_default_list() {
    // Default 리스트 내용 출력
    FILE *file = fopen("default_list.txt", "r");        //읽기 모드("r")이기 때문에 읽기만 가능
        if (file == NULL) {                             //파일 열기 실패할 경우 NULL 반환
        printf("Default 리스트 열기 실패.\n");
        return;
    }
    char line[18];      //최대 글자 17 + NULL종료문자
    while (fgets(line, sizeof(line), file)) {       //파일에서 한줄을 읽어 line을 저장, 17자까지 읽음
        printf("%s", line);
    }
    fclose(file);
}

void reset_default_list() {
    // Default 리스트 파일 열기
    FILE *file = fopen("default_list.txt", "w");        //쓰기 모드를 실행해 파일이 존재하지 않으면 새 파일을 생성하고 파일이 이미 존재하면 내용을 비워서 새로 작성
    if (file == NULL) {                                 //파일은 열기에 실패하면 NULL 반환
        fprintf(stderr, "파일 열기 실패\n");
        return;
    }
    fclose(file);  // 파일을 쓰기 모드로 열고 바로 닫으면 내용이 초기화됨
    printf("Default 리스트가 초기화되었습니다.\n");
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

    // 한 줄씩 읽어오기 위한 준비
    char line[20];
    Delivery_code *deliveries = NULL;
    int count = 0;

    while (fgets(line, sizeof(line), file) != NULL) {
        // 줄 끝의 개행 문자 제거
        line[strcspn(line, "\n")] = 0;

        // 구조체 인스턴스 생성 및 동적 메모리 할당
        deliveries = realloc(deliveries, sizeof(Delivery_code) * (count + 1));
        Delivery_code *delivery = &deliveries[count];

        // line 문자열에서 필드 추출 및 복사
        strncpy(delivery->date, line, 8);
        delivery->date[8] = '\0';

        // 택배 현황 추출
        int line_len = strlen(line);
        strncpy(delivery->situation, line + line_len - 3, 3);
        delivery->situation[3] = '\0';

        // 출발지 추출
        if (line[8] == '0' && line[9] == '2') {
            strncpy(delivery->from, line + 8, 2);
            delivery->from[2] = '\0';
        } else {
            strncpy(delivery->from, line + 8, 3);
            delivery->from[3] = '\0';
        }

        // 도착지 추출
        int toStartIndex = (delivery->from[1] == '2' && strlen(delivery->from) == 2) ? 10 : 11;
        if (line[toStartIndex] == '0' && line[toStartIndex + 1] == '2') {
            strncpy(delivery->to, line + toStartIndex, 2);
            delivery->to[2] = '\0';
        } else {
            strncpy(delivery->to, line + toStartIndex, 3);
            delivery->to[3] = '\0';
        }

        count++;
    }

    fclose(file);

    // 사용자에게 보고 싶은 정보 선택
    printf("조회하려는 정보를 선택하세요. (1: 배달리스트 출력, 2: 구분해서 출력, 3: 정렬해서 출력, 4: 특정 날짜 조회, 5: CSV에 추가, 6: Default 리스트 조회)\n");
    int choice;
    scanf("%d", &choice);

    if (choice == 1) {
        FILE *file = fopen("Delivery_list.csv", "r");
        if (file == NULL) {
            printf("파일을 열 수 없습니다.\n");
            return 1;
        }

        char line[1024];
        while (fgets(line, sizeof(line), file)) {
            printf("%s", line);
        }

        fclose(file);
    } else if (choice == 2) {
        for (int i = 0; i < count; i++) {
            printf("날짜: %s 출발지: %s 목적지: %s 택배현황: %s\n", deliveries[i].date, deliveries[i].from, deliveries[i].to, deliveries[i].situation);
        }
    } else if (choice == 3) {
        printf("정렬 기준 선택 (1: 날짜, 2: 출발지, 3: 도착지, 4: 택배현황)\n");
        int sort_choice;
        scanf("%d", &sort_choice);

        switch(sort_choice) {
            case 1:
                qsort(deliveries, count, sizeof(Delivery_code), compare_dates);
                break;
            case 2:
                qsort(deliveries, count, sizeof(Delivery_code), compare_froms);
                break;
            case 3:
                qsort(deliveries, count, sizeof(Delivery_code), compare_tos);
                break;
            case 4:
                qsort(deliveries, count, sizeof(Delivery_code), compare_situations);
                break;
        }

        for (int i = 0; i < count; i++) {
            printf("%s %s %s %s\n", deliveries[i].date, deliveries[i].from, deliveries[i].to, deliveries[i].situation);
        }
    } else if (choice == 4) {
        char search_date[9];
        printf("조회할 날짜를 입력하세요 (YYYYMMDD 형식): ");
        scanf("%s", search_date);

        printf("%s에 가야할 목적지와 택배현황:\n", search_date);
        for (int i = 0; i < count; i++) {
            if (strcmp(deliveries[i].date, search_date) == 0) {
                printf("\n목적지: %s, 택배현황%s", deliveries[i].to, deliveries[i].situation);
            }
        }
    } else if (choice == 5) {
    char new_entry[18];
    printf("추가할 내용을 입력하세요 (날짜,출발지,도착지,택배현황 형식): ");
    scanf("%s", new_entry);

    int len = strlen(new_entry);
    int is_valid = 1; // 유효성 검사 변수

    // 마지막에서 세 번째 문자가 알파벳이고, 나머지 문자가 숫자인지 확인
    if (!(len >= 15 && len <= 17 && isalpha(new_entry[len - 3]))) {
        is_valid = 0;
    }
    for (int i = 0; i < len; i++) {
        if (i != len - 3 && !isdigit(new_entry[i])) {
            is_valid = 0;
            break;
        }
    }

    if (is_valid) {
        append_to_csv("Delivery_list.csv", new_entry);
    } else {
        append_to_default_list(new_entry);
    }
} else if (choice == 6) {
        printf("--------------\nDefault 리스트\n--------------\n");
        print_default_list();
        printf("Default 리스트를 삭제하시겠습니까? (0: 예, 1: 아니오): ");
        int delete_choice;
        scanf("%d", &delete_choice);

        if (delete_choice == 0) {
            reset_default_list();
        }
    }

    // 메모리 해제
    free(deliveries);

    return 0;
}

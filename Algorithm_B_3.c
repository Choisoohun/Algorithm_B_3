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

// 배열의 가장 오른쪽 요소를 pivot으로 잡고 
int partition(Delivery_code data[], int left, int right) {  //int left = 0, int right = 배열의 개수 -1
    Delivery_code pivot = data[right]; // 피벗을 배열의 오른쪽 끝 요소로 선택
    int i = left - 1; // i = -1

    for (int j = left; j < right; j++) {  //j는 배열의 첫번째 요소, 배열의 가장 왼쪽 요소부터 피벗 전까지 순회(전부 순회)
        // 현재 요소(date[j])보다 pivot이 크면 교환
        if (strcmp(data[j].date, pivot.date) <= 0) {    //strcmp: 두 문자열이 같으면 0, 첫 번째 문자열이 사전 순으로 두 번째 문자열보다 앞에 있으면 음수, 뒤에 있으면 양수를 반환 => pivot이 더 크면
            i++;
            // data[i]와 data[j]를 교환
            Delivery_code temp = data[i];
            data[i] = data[j];
            data[j] = temp;
        }
    }

    // 피벗과 i+1 위치의 요소(data[i+1])를 교환하여 피벗을 정렬된 위치로 이동
    Delivery_code temp = data[i + 1];
    data[i + 1] = data[right];
    data[right] = temp;

    return i + 1; // 피벗의 최종 위치를 반환
}

int partition_by_from(Delivery_code data[], int left, int right) {
    Delivery_code pivot = data[right]; // 피벗을 배열의 오른쪽 끝 요소로 선택
    int i = left - 1; // i를 피벗보다 작은 요소들을 저장하기 위한 인덱스로 초기화

    for (int j = left; j < right; j++) {      
        // 현재 요소가 피벗보다 작거나 같으면
        if (strcmp(data[j].from, pivot.from) <= 0) { 
            i++; // i를 증가시키고
            // data[i]와 data[j]를 교환
            Delivery_code temp = data[i];
            data[i] = data[j];
            data[j] = temp;
        }
    }

    // 피벗과 i+1 위치의 요소(data[i+1])를 교환하여 피벗을 정렬된 위치로 이동
    Delivery_code temp = data[i + 1];
    data[i + 1] = data[right];
    data[right] = temp;

    return i + 1; // 피벗의 최종 위치를 반환
}

int partition_by_to(Delivery_code data[], int left, int right) {
    Delivery_code pivot = data[right];
    int i = left - 1;

    for (int j = left; j < right; j++) {
        if (strcmp(data[j].to, pivot.to) <= 0) {
            i++;
            Delivery_code temp = data[i];
            data[i] = data[j];
            data[j] = temp;
        }
    }

    Delivery_code temp = data[i + 1];
    data[i + 1] = data[right];
    data[right] = temp;

    return i + 1;
}

int partition_by_situation(Delivery_code data[], int left, int right) {
    Delivery_code pivot = data[right];
    int i = left - 1;

    for (int j = left; j < right; j++) {
        if (strcmp(data[j].situation, pivot.situation) <= 0) {
            i++;
            Delivery_code temp = data[i];
            data[i] = data[j];
            data[j] = temp;
        }
    }

    Delivery_code temp = data[i + 1];
    data[i + 1] = data[right];
    data[right] = temp;

    return i + 1;
}

void quick_sort(Delivery_code data[], int left, int right) {
    if (left < right) {     
        int q = partition(data, left, right);       //partition 배열을 q에 저장
        quick_sort(data, left, q - 1);              //피벗 기준 왼쪽 정렬
        quick_sort(data, q + 1, right);             //피벗 기준 오른쪽 정렬
    }
}

void quick_sort_by_from(Delivery_code data[], int left, int right) {
    if (left < right) {
        int q = partition_by_from(data, left, right);
        quick_sort_by_from(data, left, q - 1);
        quick_sort_by_from(data, q + 1, right);
    }
}

void quick_sort_by_to(Delivery_code data[], int left, int right) {
    if (left < right) {
        int q = partition_by_to(data, left, right);
        quick_sort_by_to(data, left, q - 1);
        quick_sort_by_to(data, q + 1, right);
    }
}

void quick_sort_by_situation(Delivery_code data[], int left, int right) {
    if (left < right) {
        int q = partition_by_situation(data, left, right);
        quick_sort_by_situation(data, left, q - 1);
        quick_sort_by_situation(data, q + 1, right);
    }
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

        switch (sort_choice) {
            case 1:
                quick_sort(deliveries, 0, count - 1); // 날짜를 기준으로 퀵 정렬 수행
                break;
            case 2:
                quick_sort_by_from(deliveries, 0, count - 1); // 출발지를 기준으로 퀵 정렬 수행
                break;
            case 3:
                quick_sort_by_to(deliveries, 0, count - 1); // 도착지를 기준으로 퀵 정렬 수행
                break;
            case 4:
                quick_sort_by_situation(deliveries, 0, count - 1); // 택배현황을 기준으로 퀵 정렬 수행
                break;
        }

        for (int i = 0; i < count; i++) {
            printf("%s %s %s %s\n", deliveries[i].date, deliveries[i].from, deliveries[i].to, deliveries[i].situation);
        }
    } else if (choice == 4) {
        char search_date[9];
        printf("조회할 날짜를 입력하세요 (ex20230101): ");
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
    int v = 1; // 유효성 검사 변수

    // 마지막에서 세 번째 문자가 알파벳이고, 나머지 문자가 숫자인지 확인
    if (!(len >= 15 && len <= 17 && isalpha(new_entry[len - 3]))) {     //입력 문자열의 길이가 15에서 17 사이인지 and 문자열의 마지막에서 세 번째 문자가 알파벳인지
        v = 0;
    }
    for (int i = 0; i < len; i++) {
        if (i != len - 3 && !isdigit(new_entry[i])) {           //마지막 3번째 문자빼고 모든 문자가 숫자인지 검사
            v = 0;
            break;
        }
    }

    if (v) {
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

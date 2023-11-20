#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Parcel {
    char code[15];
};

// Forward declaration
int partition(struct Parcel arr[], int low, int high);

// 문자열에서 날짜를 추출하여 정수로 반환
int extractDate(char *str) {
    char datePart[9];
    strncpy(datePart, str, 8);
    datePart[8] = '\0'; // 문자열 마지막에 널 문자 추가
    return atoi(datePart);
}

void quickSort(struct Parcel arr[], int low, int high);

int partition(struct Parcel arr[], int low, int high);

int main() {
    FILE *file = fopen("Delivery_list.csv", "r");
    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }

    struct Parcel parcels[1000]; // 택배 리스트를 저장할 배열
    int count = 0;

    // 파일에서 데이터 읽어오기
    while (fscanf(file, "%s", parcels[count].code) == 1 && count < 1000) {
        count++;
    }

    fclose(file);

    // 퀵 정렬 수행
    quickSort(parcels, 0, count - 1);

    // 정렬된 데이터를 새로운 CSV 파일에 쓰기
    FILE *outputFile = fopen("sorted_parcels.csv", "w");
    if (outputFile == NULL) {
        perror("Error opening output file");
        return 1;
    }

    for (int i = 0; i < count; i++) {
        fprintf(outputFile, "%s\n", parcels[i].code);
    }

    fclose(outputFile);

    // 정렬된 결과 출력
    for (int i = 0; i < count; i++) {
        printf("%s\n", parcels[i].code);
    }

    return 0;
}

void quickSort(struct Parcel arr[], int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);

        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

int partition(struct Parcel arr[], int low, int high) {
    struct Parcel pivot = arr[high];
    int i = low - 1;

    for (int j = low; j < high; j++) {
        // 수정된 부분: 문자열로 된 날짜를 정수로 변환하여 비교
        if (extractDate(arr[j].code) < extractDate(pivot.code)) {
            i++;
            struct Parcel temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
        }
    }

    struct Parcel temp = arr[i + 1];
    arr[i + 1] = arr[high];
    arr[high] = temp;

    return i + 1;
}

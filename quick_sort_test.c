#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Parcel {
    char code[15];
    int year;
    int date;
    int departureRegion;
    int arrivalRegion;
    char deliveryStatus[3];
};

// 비교 함수 포인터 타입 정의
typedef int (*CompareFunction)(struct Parcel, struct Parcel);

int extractYear(char *code);
int extractDate(char *code);
int extractDepartureRegion(char *code);
int extractArrivalRegion(char *code);

int compareByYear(struct Parcel a, struct Parcel b);
int compareByDate(struct Parcel a, struct Parcel b);
int compareByDepartureRegion(struct Parcel a, struct Parcel b);
int compareByArrivalRegion(struct Parcel a, struct Parcel b);

void quickSort(struct Parcel arr[], int low, int high, CompareFunction compareFunction);
int partition(struct Parcel arr[], int low, int high, CompareFunction compareFunction);

void sortParcels(struct Parcel parcels[], int count, CompareFunction compareFunction);
void printSortedParcels(struct Parcel parcels[], int count);

int main() {
    FILE *file = fopen("Delivery_list.csv", "r");
    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }

    // 정적 배열을 사용하여 데이터 저장
    struct Parcel parcels[50];
    int count = 0;

    // 파일에서 최대 50개의 데이터 읽어오기
    while (count < 50 && fscanf(file, "%s", parcels[count].code) == 1) {
        // 데이터를 파싱하여 구조체 멤버에 저장
        parcels[count].year = extractYear(parcels[count].code);
        parcels[count].date = extractDate(parcels[count].code);
        parcels[count].departureRegion = extractDepartureRegion(parcels[count].code);
        parcels[count].arrivalRegion = extractArrivalRegion(parcels[count].code);

        count++;
    }

    fclose(file);

    // 사용자로부터 정렬 기준 입력 받기
    printf("Choose sorting criteria:\n");
    printf("1. Year\n");
    printf("2. Date\n");
    printf("3. Departure Region\n");
    printf("4. Arrival Region\n");

    int choice;
    scanf("%d", &choice);

    CompareFunction compareFunction;

    // 선택에 따라 비교 함수 지정
    switch (choice) {
        case 1:
            compareFunction = compareByYear;
            break;
        case 2:
            compareFunction = compareByDate;
            break;
        case 3:
            compareFunction = compareByDepartureRegion;
            break;
        case 4:
            compareFunction = compareByArrivalRegion;
            break;
        default:
            printf("Invalid choice\n");
            return 1;
    }

    // 정렬
    quickSort(parcels, 0, count - 1, compareFunction);

    // 정렬된 데이터 출력
    printf("\nSorted Parcels:\n");
    printSortedParcels(parcels, count);

    return 0;
}

// 여기서부터 아래로 partition 함수와 다른 함수들의 정의가 있습니다.

// 비교 함수들
int compareByYear(struct Parcel a, struct Parcel b) {
    return a.year - b.year;
}

int compareByDate(struct Parcel a, struct Parcel b) {
    return a.date - b.date;
}

int compareByDepartureRegion(struct Parcel a, struct Parcel b) {
    return a.departureRegion - b.departureRegion;
}

int compareByArrivalRegion(struct Parcel a, struct Parcel b) {
    return a.arrivalRegion - b.arrivalRegion;
}

void quickSort(struct Parcel arr[], int low, int high, CompareFunction compareFunction) {
    if (low < high) {
        int pi = partition(arr, low, high, compareFunction);

        quickSort(arr, low, pi - 1, compareFunction);
        quickSort(arr, pi + 1, high, compareFunction);
    }
}

int partition(struct Parcel arr[], int low, int high, CompareFunction compareFunction) {
    struct Parcel pivot = arr[high];
    int i = low - 1;

    for (int j = low; j < high; j++) {
        if (compareFunction(arr[j], pivot) < 0) {
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

void sortParcels(struct Parcel parcels[], int count, CompareFunction compareFunction) {
    quickSort(parcels, 0, count - 1, compareFunction);
}

void printSortedParcels(struct Parcel parcels[], int count) {
    for (int i = 0; i < count; i++) {
        printf("%s\n", parcels[i].code);
    }
}

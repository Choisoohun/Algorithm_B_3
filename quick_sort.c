#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Parcel {
    char code[15];
};

int extractDate(char *str) {
    char datePart[9];
    strncpy(datePart, str, 8);
    datePart[8] = '\0'; // 문자열 마지막에 널 문자 추가
    return atoi(datePart);
}

int partition(struct Parcel arr[], int low, int high) {
    struct Parcel pivot = arr[high];
    int i = low - 1;

    for (int j = low; j < high; j++) {
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

void quickSort(struct Parcel arr[], int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);

        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

int main() {
    FILE *file = fopen("Delivery_list.csv", "r");
    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }

    struct Parcel *parcels = NULL;
    int count = 0;
    char buffer[15];

    // 파일에서 데이터 개수 확인
    while (fscanf(file, "%s", buffer) == 1) {
        count++;
    }

    if (count == 0) {
        fprintf(stderr, "No data in the file.\n");
        fclose(file);
        return 1;
    }

    rewind(file);

    parcels = (struct Parcel *)malloc(count * sizeof(struct Parcel));

    if (parcels == NULL) {
        perror("Memory allocation failed");
        fclose(file);
        return 1;
    }

    for (int i = 0; i < count; i++) {
        if (fscanf(file, "%s", parcels[i].code) != 1) {
            fprintf(stderr, "Error reading data from file.\n");
            fclose(file);
            free(parcels);
            return 1;
        }
    }

    fclose(file);

    quickSort(parcels, 0, count - 1);

    for (int i = 0; i < count; i++) {
        printf("%s\n", parcels[i].code);
    }

    free(parcels);

    return 0;
}


#include <stdio.h>

#define MAX_LINE_SIZE 20 //문자열의 최대 크기를 20으로 정의

int main() {
    FILE *file;                 //파일을 가리키기 위한 변수 file
    char line[MAX_LINE_SIZE];   //이름이 line인 문자 배열 선언 --> 문자열 선언

    // 파일 열기
    file = fopen("Delivery_list.csv", "r");

    // 파일 열기에 실패한 경우
    if (file == NULL) {                     //fopen은 파일을 열 수 없을 때 NULL 반환
        perror("파일을 열 수 없습니다.");    //오류 출력 (print error)
        return 1;
    }

    // 파일에서 한 줄씩 읽어와서 출력
    while (fgets(line, sizeof(line), file) != NULL) {  //파일에서 한 줄을 읽어와서 line문자배열에 저장
        printf("%s", line);                            //line 출력
    }

    // 파일 닫기
    fclose(file);

    return 0;
}
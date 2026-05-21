#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define MAX_LINE 1024

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "사용법: %s <파일명>\n", argv[0]);
        exit(1);
    }

    char *filename = argv[1];
    char temp_filename[256];
    
    // 안전 장치: 원본 파일명 앞에 마침표(.)와 뒤에 .tmp를 붙여 임시 파일명 생성
    snprintf(temp_filename, sizeof(temp_filename), ".%s.tmp", filename);

    FILE *source = fopen(filename, "r");
    FILE *target = fopen(temp_filename, "w");

    if (target == NULL) {
        perror("임시 파일 생성 실패");
        exit(1);
    }

    // 기능 1: 기존 파일이 존재하면 내용을 임시 파일로 복사 (백업)
    if (source != NULL) {
        char buffer[MAX_LINE];
        while (fgets(buffer, sizeof(buffer), source) != NULL) {
            fputs(buffer, target);
        }
        fclose(source);
    }

    // 기능 2: 사용자로부터 추가할 텍스트 입력 받기 (간이 편집 기능)
    printf("--- '%s' 편집 모드 (내용 입력 후 종료 시 Ctrl+D) ---\n", filename);
    char input[MAX_LINE];
    while (fgets(input, sizeof(input), stdin) != NULL) {
        fputs(input, target);
    }
    fclose(target);

    // 기능 3: 편집이 정상 종료되면 임시 파일을 원본 파일명으로 변경 (덮어쓰기)
    if (rename(temp_filename, filename) == 0) {
        printf("\n[성공] 파일이 안전하게 변경 및 저장되었습니다: %s\n", filename);
    } else {
        perror("파일 반영 실패");
        unlink(temp_filename); // 실패 시 임시 파일 삭제
        exit(1);
    }

    return 0;
}

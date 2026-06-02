#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_LINE 1024

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "사용법: %s <파일명>\n", argv[0]);
        exit(1);
    }

    char *filename = argv[1];
    char temp_filename[256];
    snprintf(temp_filename, sizeof(temp_filename), "%s.tmp", filename);

    FILE *target = fopen(temp_filename, "w");
    if (target == NULL) {
        perror("임시 파일 생성 실패");
        exit(1);
    }

    FILE *source = fopen(filename, "r");
    if (source != NULL) {
        char buffer[MAX_LINE];
        while (fgets(buffer, sizeof(buffer), source) != NULL) {
            fputs(buffer, target);
        }
        fclose(source);
    }

    printf("--- '%s' 편집 모드 (내용 입력 후 종료 시 Ctrl+D) ---\n", filename);
    char input[MAX_LINE];
    while (fgets(input, sizeof(input), stdin) != NULL) {
        fputs(input, target);
    }
    fclose(target);

    if (rename(temp_filename, filename) == 0) {
        printf("\n[성공] 파일이 안전하게 변경 및 저장되었습니다: %s\n", filename);
    } else {
        perror("파일 반영 실패");
        unlink(temp_filename);
        exit(1);
    }

    return 0;
}


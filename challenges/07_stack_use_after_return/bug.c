#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINES 8

typedef struct {
    char **lines;
    int    count;
} LineView;

static void view_set(LineView *out, char **arr, int n) {
    out->lines = arr;
    out->count = n;
}

static void split_lines(LineView *out, char *text) {
    int n = 0;

    for (char *ln = strtok(text, "\n");
         ln && n < MAX_LINES;
         ln = strtok(NULL, "\n")) {
        out->lines[n++] = ln;
    }

    out->count = n;
}

static void warm_stack(void) {
    char *scratch[MAX_LINES];

    for (int i = 0; i < MAX_LINES; i++)
        scratch[i] = (char *)0x4141414141414141ULL;

    __asm__ volatile("" :: "r"(scratch) : "memory");
}

int main(void) {
    char text[] = "alpha\nbeta\ngamma";

    char *lines[MAX_LINES];
    LineView v = {
        .lines = lines,
        .count = 0
    };

    split_lines(&v, text);
    warm_stack();

    long checksum = 0;

    for (int i = 0; i < v.count; i++)
        checksum += (unsigned char)v.lines[i][0];

    printf("lines = %d, checksum = %ld\n", v.count, checksum);
    return 0;
}
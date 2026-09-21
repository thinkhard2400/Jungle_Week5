#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ARENA_SIZE 4096

static unsigned char arena[ARENA_SIZE];
static size_t arena_off = 0;

static void *arena_alloc(size_t n) {
    if (n > sizeof(arena) - arena_off)
        return NULL;

    void *p = &arena[arena_off];
    arena_off += n;
    return p;
}

static char *intern(const char *s) {
    size_t n = strlen(s) + 1;
    char *dst = arena_alloc(n);

    if (dst == NULL)
        return NULL;

    memcpy(dst, s, n);
    return dst;
}

int main(void) {
    const char *words[] = {
        "insert", "delete", "search", "traverse", "balance",
        "rotate", "rehash", "compact", "serialize", "checkpoint",
    };

    int nwords = (int)(sizeof(words) / sizeof(words[0]));

    char *last = NULL;
    long total = 0;

    for (int i = 0; i < 100000; i++) {
        char buf[32];

        snprintf(buf, sizeof buf, "%s-%d", words[i % nwords], i);

        last = intern(buf);

        if (last == NULL) {
            fprintf(stderr, "arena full at i=%d\n", i);
            break;
        }

        total += (long)strlen(last);
    }

    if (last != NULL)
        printf("interned, last=%s total_len=%ld\n", last, total);

    return 0;
}
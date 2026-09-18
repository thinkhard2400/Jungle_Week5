/*
 * Challenge 03 — Heap Buffer Overflow (심화: 동적 배열 성장 버그)
 *
 * [시나리오]
 *   자동 성장하는 정수 동적 배열 IntList (init/ensure/push/sum). 용량이 부족하면
 *   list_ensure() 가 용량을 2배로 늘리고 realloc 한다. 이 리스트로 큰 수열을
 *   만들어 합을 구한다.
 *
 * [기대 동작]
 *   0..N-1 을 100 으로 나눈 나머지를 리스트에 넣고, 길이·용량·합을 출력한 뒤 정상 종료.
 * 
 * [gdb 로 잡기]
 *   make gdb NAME=03_heap_buffer_overflow
 *   (gdb) run                         → 크래시(SIGSEGV) 또는 abort
 *   (gdb) bt                          → list_push 의 l->data[l->len]=x 또는 realloc 내부
 *   (gdb) frame N ; print *l           → cap 은 큰데 실제 버퍼는 그보다 작음(불일치)
 *   (gdb) print l->len  / print l->cap → len 이 실제 확보량을 넘어섰는지 확인
 *   (gdb) break list_ensure           → newcap 과 realloc 에 넘기는 크기를 대조
 */

#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int   *data;
    size_t len;
    size_t cap;
} IntList;

static void list_init(IntList *l) //完
{
    l->cap  = 8;
    l->len  = 0;
    l->data = malloc(l->cap * sizeof(int));
    if (!l->data) { perror("malloc"); exit(1); }
}

static void list_ensure(IntList *l, size_t need) //完
{
    if (need <= l->cap) 
        return;

    size_t newcap = l->cap ? l->cap * 2 : 8;
    while (newcap < need) 
        newcap *= 2;

    int *p = realloc(l->data, newcap * sizeof(int));
    if (!p) { perror("realloc"); free(l->data); exit(1); }

    l->data = p;
    l->cap  = newcap;
}

static void list_push(IntList *l, int x) //完
{
    if (l->len == l->cap)
        list_ensure(l, l->cap + 1);
    l->data[l->len] = x;
    (l->len)++;
}

static long long list_sum(const IntList *l)
{
    long long s = 0;
    for (size_t i = 0; i < l->len; i++) 
        s += l->data[i];
    return s;
}

static void list_free(IntList *l)
{
    free(l->data);
    l->data = NULL;
    l->len = 0;
    l->cap = 0;
}

int main(void) {
    IntList l;
    list_init(&l);

    const int N = 2000000;
    for (int i = 0; i < N; i++) {
        list_push(&l, i % 100);        
    }

    printf("len=%zu cap=%zu sum=%lld\n", l.len, l.cap, list_sum(&l));
    list_free(&l);
    return 0;
}

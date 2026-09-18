/*
 * Challenge 04 — Double Free (심화: 두 인덱스가 같은 객체를 가리키는 별칭)
 *
 * [시나리오]
 *   직원 레코드(Rec)를 힙에 만들고, "ID 인덱스"(by_id)와 "이름 순 인덱스"(by_name)
 *   두 배열로 관리한다. 두 인덱스는 정렬만 다를 뿐, 결국 "같은 Rec 객체들"을 가리킨다
 *   (별칭, aliasing). 조회/출력 후 프로그램 끝에서 전부 정리한다.
 *
 * [기대 동작]
 *   레코드를 만들고 ID/이름으로 조회해 출력한 뒤, 누수 없이 정리하고 정상 종료.
 *
 * [gdb 로 잡기]
 *   make gdb NAME=04_double_free
 *   (gdb) run                       → abort
 *   (gdb) bt                        → directory_free() 의 두 번째 free 루프
 *   (gdb) frame N ; print d->by_name[i] → 이 주소가 앞서 by_id 로 이미 free 됐는지 확인
 *   (gdb) print d->by_id[0]          
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int   id;
    char *name;      
} Rec;

#define MAXN 16
typedef struct {
    Rec *by_id[MAXN];     
    Rec *by_name[MAXN];    
    int  count;
} Directory;

static Rec *rec_new(int id, const char *name) //完 
{
    Rec *r;
    r = malloc(sizeof *r);
    if (!r) { perror("malloc"); exit(1); }
    r->id = id;
    r->name = malloc(strlen(name) + 1);
    if (!r->name) { perror("malloc"); exit(1); }
    strcpy(r->name, name);
    return r;
}

static void directory_add(Directory *d, int id, const char *name) //完 
{
    Rec *r = rec_new(id, name);
    d->by_id[d->count]   = r;
    d->by_name[d->count] = r;      /* 같은 포인터를 두 인덱스에 함께 등록 */
    (d->count)++;
}

/* 이름 순 인덱스를 사전순으로 정렬(포인터만 재배치, 객체는 공유 그대로) */
static void directory_sort_by_name(Directory *d) //完 
{
    for (int i = 0; i < d->count; i++) 
    {
        for (int j = i + 1; j < d->count; j++) 
        {
            if (strcmp(d->by_name[i]->name, d->by_name[j]->name) > 0) 
            {
                Rec *t = d->by_name[i];
                d->by_name[i] = d->by_name[j];
                d->by_name[j] = t;
            }
        }
    }
}

static Rec *find_by_id(Directory *d, int id) 
{
    for (int i = 0; i < d->count; i++)
        if (d->by_id[i]->id == id) 
            return d->by_id[i];
    return NULL;
}

static void directory_dump(Directory *d) 
{
    printf("by id:  ");
    for (int i = 0; i < d->count; i++) 
        printf("%d:%s ", d->by_id[i]->id, d->by_id[i]->name);
    printf("\nby name:");
    for (int i = 0; i < d->count; i++) 
        printf(" %s(%d)", d->by_name[i]->name, d->by_name[i]->id);
    printf("\n");
}

static void directory_free(Directory *d) 
{
    for (int i = 0; i < d->count; i++) {
        free(d->by_name[i]);               
    }
    d->count = 0;
}

int main(void) {
    Directory dir;
    dir.count = 0;

    directory_add(&dir, 3, "carol");
    directory_add(&dir, 1, "alice");
    directory_add(&dir, 4, "dave");
    directory_add(&dir, 2, "bob");

    directory_sort_by_name(&dir);
    directory_dump(&dir);

    Rec *r = find_by_id(&dir, 2);
    if (r) printf("lookup id=2 -> %s\n", r->name);

    directory_free(&dir);                  
    printf("done\n");
    return 0;
}

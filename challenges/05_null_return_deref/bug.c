/*
 * Challenge 05 — NULL 반환 미확인 역참조 (심화: 설정 템플릿 확장기)
 *
 * [시나리오]
 *   key=value 설정 저장소(Config)와, "${key}" 자리표시자를 실제 값으로 치환하는
 *   템플릿 확장기 expand() 를 만든다. 예: "http://${host}:${port}/${path}".
 *
 * [기대 동작]
 *   템플릿의 모든 ${key} 를 설정값으로 치환한 최종 문자열을 출력.
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_KV 16
typedef struct {
    const char *keys[MAX_KV];
    const char *vals[MAX_KV];
    int n;
} Config;

static void cfg_set(Config *c, const char *k, const char *v) 
{
    if (c->n < MAX_KV) 
    {
         c->keys[c->n] = k; 
         c->vals[c->n] = v; 
         c->n++; 
    }
}

static const char *cfg_get(const Config *c, const char *k) 
{
    for (int i = 0; i < c->n; i++)
        if (strcmp(c->keys[i], k) == 0) 
            return c->vals[i];
    return NULL;
}

static void expand(const Config *c, const char *tmpl, char *out, size_t outcap) {
    size_t o = 0;
    for (const char *p = tmpl; *p; ) 
    {
        if (p[0] == '$' && p[1] == '{') 
        {
            const char *end = strchr(p, '}');
            if (!end) break;

            char key[32];
            size_t kl = (size_t)(end - (p + 2));

            if (kl >= sizeof key) 
                kl = sizeof key - 1;

            memcpy(key, p + 2, kl);
            key[kl] = '\0';

            const char *v = cfg_get(c, key);
            if (v == NULL)
                v = "";

            size_t vl = strlen(v);

            if (o + vl < outcap) 
            { 
                memcpy(out + o, v, vl); 
                o += vl; 
            }

            p = end + 1;
        } 
        else 
        {
            if (o + 1 < outcap) 
                out[o++] = *p;

            p++;
        }
    }

    out[o] = '\0';
}

int main(void) {
    Config cfg;
    cfg.n = 0;

    cfg_set(&cfg, "host", "example.com");
    cfg_set(&cfg, "port", "8080");

    const char *tmpl = "http://${host}:${port}/${path}/index.html";
    char out[256];

    expand(&cfg, tmpl, out, sizeof out);
    printf("url = %s\n", out);
    return 0;
}

#include <stdio.h>
#include <string.h>

static int append_field(char *buf, size_t cap, size_t *len,
                        const char *field, char sep)
{
    size_t flen = strlen(field);
    size_t extra = (*len > 0) ? 1 : 0;

    if (*len + extra + flen + 1 > cap)
        return 0;

    if (*len > 0)
        buf[(*len)++] = sep;

    for (size_t i = 0; i < flen; i++)
        buf[(*len)++] = field[i];

    buf[*len] = '\0';
    return 1;
}

static int build_record(char *rec, size_t cap)
{
    const char *fields[] = {
        "id=1042",
        "name=Jonathan",
        "department=Engineering",
        "role=maintainer",
    };

    int n = (int)(sizeof(fields) / sizeof(fields[0]));
    size_t len = 0;

    rec[0] = '\0';

    for (int i = 0; i < n; i++) {
        if (!append_field(rec, cap, &len, fields[i], '|'))
            return 0;
    }

    return 1;
}

int main(void)
{
    char rec[24];

    if (!build_record(rec, sizeof rec)) {
        fprintf(stderr, "error: record too large\n");
        return 1;
    }

    printf("record = %s\n", rec);
    return 0;
}
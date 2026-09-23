#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char *rx;
    char *tx;
    int  *state;
} Conn;

static int handshake_ok(const Conn *c)
{
    (void)c;
    return 0;
}

static int conn_open(Conn *c, size_t bufsz)
{
    c->rx = c->tx = NULL;
    c->state = NULL;

    c->rx = malloc(bufsz);
    if (!c->rx)
        goto fail_rx;

    c->tx = malloc(bufsz);
    if (!c->tx)
        goto fail_tx;

    c->state = malloc(sizeof(int) * 4);
    if (!c->state)
        goto fail_state;

    strcpy(c->rx, "rx-ready");
    strcpy(c->tx, "tx-ready");

    for (int i = 0; i < 4; i++)
        c->state[i] = i;

    if (!handshake_ok(c))
        goto fail_state;

    return 0;

fail_state:
    free(c->state);
    c->state = NULL;

fail_tx:
    free(c->tx);
    c->tx = NULL;

fail_rx:
    free(c->rx);
    c->rx = NULL;

    return -1;
}

int main(void)
{
    Conn c;

    int rc = conn_open(&c, 32);
    printf("conn_open rc=%d\n", rc);

    return 0;
}
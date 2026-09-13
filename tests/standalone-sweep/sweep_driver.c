
#include <stdio.h>

#include "sweep_contexts.h"

static int sweep_total = 0;
static int sweep_failed = 0;

#define RUN(P)                                              \
    do {                                                    \
        ++sweep_total;                                      \
        P##_ctx * ctx = P##_create();                       \
        if (ctx) {                                          \
            P##_destroy(ctx);                               \
        } else {                                            \
            ++sweep_failed;                                 \
            printf("FAIL %s: %s\n", #P, P##_last_error(0)); \
        }                                                   \
    } while (0)

#define HOLD(P) P##_ctx * anchor = P##_create()
#define DROP(P) if (anchor) P##_destroy(anchor)

int main ( void ) {
    SWEEP_ANCHOR(HOLD);
    SWEEP_CONTEXTS(RUN)
    SWEEP_ANCHOR(DROP);
    printf("standalone sweep (C++): %d contexts, %d failed\n", sweep_total, sweep_failed);
    return sweep_failed ? 1 : 0;
}

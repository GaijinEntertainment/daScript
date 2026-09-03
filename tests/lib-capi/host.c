/* Drives a library built by `daslang -lib` through its generated C header: no daScript.h, no
   Module, no compiler. Prints "ok" and exits 0 when every boundary shape survived the round trip. */
#include "fixture.h"

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <pthread.h>

static int failures = 0;

static void check ( int cond, const char * what ) {
    if ( !cond ) {
        printf("FAIL: %s\n", what);
        failures ++;
    }
}

static int near ( float have, float want ) {
    return fabsf(have - want) < 1e-5f;
}

/* daslang's environment is thread-local, so a second thread's create has to join the first's
   registration rather than start its own. */
static int worker_ok = 0;

static void * worker ( void * unused ) {
    (void) unused;
    fixture_ctx * w = fixture_create();
    if ( !w ) {
        printf("worker create failed: %s\n", fixture_last_error(NULL));
        return NULL;
    }
    worker_ok = fixture_bump(w) == 1;
    fixture_destroy(w);
    return NULL;
}

int main () {
    fixture_ctx * a = fixture_create();
    check(a != NULL, "create");
    if ( !a ) {
        printf("create failed: %s\n", fixture_last_error(NULL));
        return 1;
    }

    check(fixture_bump(a) == 1, "a global starts at its initializer");
    check(fixture_bump(a) == 2, "a global persists across calls");
    check(fixture_last_error(a) == NULL, "no error after a good call");

    fixture_ctx * b = fixture_create();
    check(b != NULL, "a second instance");
    check(fixture_bump(b) == 1, "the second instance has its own globals");
    check(fixture_bump(a) == 3, "the first instance kept its own");

    fixture_Pair p = { 10, 20 };
    fixture_Pair out = { 0, 0 };
    fixture_widen(a, &p, &out);
    check(out.lo == 9 && out.hi == 21, "a struct crosses in and out");

    fixture_float3 v = { 1.0f, 2.0f, 3.0f };
    fixture_float3 sv = { -1.0f, -1.0f, -1.0f };
    fixture_scale3(a, &v, 3.0f, &sv);
    check(near(sv.x, 3.0f) && near(sv.y, 6.0f) && near(sv.z, 9.0f), "a vector crosses in and out");

    const char * greeting = fixture_greet(a, "mundo");
    check(greeting != NULL && strcmp(greeting, "hola mundo") == 0, "a string crosses in and out");

    check(fixture_flip(a, true) == false, "bool in, bool out");
    check(fixture_bigsum(a, 5000000000LL, 7000000000ULL) == 12000000000ULL, "64-bit scalars keep their width");

    /* an 8-aligned result with a hole, written into an out param the caller only aligned to 1 */
    char raw[sizeof(fixture_Wide) + 1];
    memset(raw, 0, sizeof(raw));
    fixture_Wide * wide = (fixture_Wide *) (raw + 1);
    fixture_widen_wide(a, 7, wide);
    check(wide->a == 7 && wide->b == 700 && wide->c > 3.4f && wide->c < 3.6f,
          "an aligned structure crosses into an unaligned out param");

    /* the guard: a panic reaches C as a zero result plus a message, and leaves the instance usable */
    check(fixture_double_or_raise(a, 21) == 42, "the happy path before the panic");
    fixture_Pair kept = { 77, 88 };
    check(fixture_double_or_raise(a, -5) == 0, "a raising call returns zero");
    const char * err = fixture_last_error(a);
    check(err != NULL && strstr(err, "negative: -5") != NULL, "last_error carries the panic text");
    check(fixture_last_error(b) == NULL, "the other instance never saw it");
    fixture_widen(a, &p, &kept);
    check(kept.lo == 9 && kept.hi == 21, "the instance still works after a panic");
    check(fixture_last_error(a) == NULL, "a good call clears last_error");

    /* an instance created on another thread, while this one is live */
    pthread_t t;
    if ( pthread_create(&t, NULL, worker, NULL)==0 ) {
        pthread_join(t, NULL);
        check(worker_ok, "an instance created on a second thread works");
    } else {
        printf("could not start a thread; the second-thread check did not run\n");
    }

    fixture_destroy(b);
    fixture_destroy(a);
    printf(failures ? "%d failure(s)\n" : "ok\n", failures);
    return failures ? 1 : 0;
}

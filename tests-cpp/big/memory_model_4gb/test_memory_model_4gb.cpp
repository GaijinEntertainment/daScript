// Big test: regression for the 64-bit migration of the shoe allocator (das::Deck).
//
// Before the fix, Deck::total and Deck::totalBytes were uint32_t, so a size-class
// chunk whose byte total reached 2^32 wrapped to 0 -> das_aligned_alloc16(0) ->
// (Debug) assert / (Release) a 0-byte pointer the next write trampled (the
// "name string dereferenced as a pointer" crash the flatten fuzzer surfaced).
//
// Verifying the fix needs a real >4 GB allocation, so this is a local-only "big"
// test (`ninja test-big`), not part of CI.

#include "daScript/misc/platform.h"     // memory_model.h is not self-contained
#include "daScript/misc/memory_model.h"

#include <cstdio>
#include <cstring>

using namespace das;

static int g_failures = 0;
#define BIGCHECK(cond) do { if(!(cond)){ printf("FAIL line %d: %s\n", __LINE__, #cond); ++g_failures; } } while(0)

int main() {
    // ne * es crosses exactly 2^32: 2^28 entries * 16 bytes = 2^32 (the value that
    // wrapped to 0 in the lldb backtrace: Deck::Deck(ne=268435456, es=16)).
    const uint64_t ne = uint64_t(1) << 28;   // 268,435,456
    const uint32_t es = 16;
    {
        Deck d(ne, es, nullptr);
        BIGCHECK(d.total == ne);
        BIGCHECK(d.totalBytes == ne * uint64_t(es));
        BIGCHECK(d.totalBytes == (uint64_t(1) << 32));   // the pre-fix wrap target
        BIGCHECK(d.totalBytes != 0);
        BIGCHECK(d.data != nullptr);

        // A few entries from the chunk: distinct, 16-aligned, owned, writable.
        char * a = d.allocate();
        char * b = d.allocate();
        char * c = d.allocate();
        BIGCHECK(a != nullptr && b != nullptr && c != nullptr);
        BIGCHECK(a != b && b != c && a != c);
        BIGCHECK((reinterpret_cast<uintptr_t>(a) & 15) == 0);
        BIGCHECK(d.isOwnPtr(a) && d.isOwnPtr(b) && d.isOwnPtr(c));
        memset(a, 0xAB, es); memset(b, 0xCD, es); memset(c, 0xEF, es);
        BIGCHECK((unsigned char)a[0] == 0xAB);
        BIGCHECK((unsigned char)b[0] == 0xCD);
        BIGCHECK((unsigned char)c[es-1] == 0xEF);

        // The far end of the 4 GB buffer must be addressable and in-range — proves
        // totalBytes is the full 2^32, not a truncated value.
        char * tail = d.data + (d.totalBytes - es);
        BIGCHECK(d.isOwnPtr(tail));
        tail[0] = 0x5A; tail[es-1] = 0x5A;
        BIGCHECK((unsigned char)tail[0] == 0x5A && (unsigned char)tail[es-1] == 0x5A);
    }   // ~Deck releases the 4 GB buffer

    if ( g_failures ) {
        printf("memory_model_4gb: %d failure(s)\n", g_failures);
        return 1;
    }
    printf("memory_model_4gb: OK\n");
    return 0;
}

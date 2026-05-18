---
slug: qsort-byte-swap-implementations-survey
title: What byte-swap strategies do production qsorts use for arbitrary-width element exchange?
created: 2026-05-18
last_verified: 2026-05-18
links: []
---

Four canonical families across libc/kernel implementations:

**1. Word-at-a-time temp swap (Linux kernel, glibc).** Linux `lib/sort.c` and glibc `stdlib/qsort.c` both classify (size, alignment) once at qsort entry — `is_aligned(base, size, 8)` → `SWAP_WORDS_64`, `is_aligned(base, size, 4)` → `SWAP_WORDS_32`, else `SWAP_BYTES`. Dispatcher is a 3-way `if`, NOT an indirect call (glibc comment: "should help the branch predictor"). Each variant is a tiny do-while: `do { T t = *(T*)(a+(n-=k)); *(T*)(a+n) = *(T*)(b+n); *(T*)(b+n) = t; } while (n);` — no buffer, in-place, compiler vectorizes the loop body.

**2. Bentley-McIlroy "Engineering a Sort Function" (1993).** Same idea but older: `SWAPINIT` computes `swaptype = (a-(char*)0 | es) % sizeof(long) ? 2 : es > sizeof(long)`; `swaptype==0` means single-long inline exch (no call), `==1` means word-loop call, `==2` means byte-loop call. The single-long fast path inlined at the call site is the key trick — most sorts are 8-byte keys.

**3. memcpy 3-way with stack buffer (musl smoothsort `cycle()`).** `char tmp[256]; while (width) { l = min(256, width); memcpy(ar[n], ar[0], l); for (i..n) { memcpy(ar[i], ar[i+1], l); ar[i] += l; } width -= l; }`. Chunked at 256 bytes — small enough to fit L1, large enough to amortize memcpy overhead. Generalizes to n-cycle rotations (smoothsort's trinkle/sift needs this).

**4. Byte-by-byte only (FreeBSD).** `do { t = *a; *a++ = *b; *b++ = t; } while (--es > 0);` — no dispatch, no optimization. Simpler, but slow for anything >4 bytes. FreeBSD dropped Bentley-McIlroy's swaptype dispatch.

**Specialized fast paths.** All four production implementations (except FreeBSD) special-case the common width=8/16 case. Linux kernel via SWAP_WORDS_64 + do-while-unrollable, glibc same, B-M via swaptype==0 inline single-long exchange.

**pdqsort is C++-templated** — `std::iter_swap` on typed iterators, no byte primitive. Not applicable to byte-pointer ports.

**Bake-off candidates worth measuring:**
- Linux kernel SWAP_WORDS_64 (predicted winner for ≤64B aligned)
- Bentley-McIlroy single-long fast-path + word-loop fallback
- Current memcpy chunked-256 baseline
- memcpy 3-way unchunked with sized stack buffer (single memcpy per direction, no loop)
- Inline `__builtin_memcpy(a,b,W)` with width as compile-time constant via switch on common widths (4/8/16/32) — compiler emits direct SIMD load/store

Reference impls quoted at: torvalds/linux lib/sort.c, bminor/glibc stdlib/qsort.c, musl src/stdlib/qsort.c, cs.dartmouth.edu/~doug/qsort.c (B-M), freebsd-src lib/libc/stdlib/qsort.c.</body>
</invoke>

## Questions
- What byte-swap strategies do production qsorts use for arbitrary-width element exchange?

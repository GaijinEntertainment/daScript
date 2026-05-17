#pragma once

namespace das
{

//this is qsort_r/qsort_s like implementation, but with (potentially) inline comparator
//qsort_r/qsort_s is not portable, and all comparator calls are out-of-line
//from https://github.com/esmil/musl/blob/master/src/stdlib/qsort.c

/* Copyright (C) 2011 by Valentin Ochs
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

/* Minor changes by Rich Felker for integration in musl, 2011-04-27. */
//modified by Anton Yudintsev, to make inline comparator

static inline int ntz(unsigned long x)
{
    static const char debruijn64[64] = {
        0, 1, 2, 53, 3, 7, 54, 27, 4, 38, 41, 8, 34, 55, 48, 28,
        62, 5, 39, 46, 44, 42, 22, 9, 24, 35, 59, 56, 49, 18, 29, 11,
        63, 52, 6, 26, 37, 40, 33, 47, 61, 45, 43, 21, 23, 58, 17, 10,
        51, 25, 36, 32, 60, 20, 57, 16, 50, 31, 19, 15, 30, 14, 13, 12
    };
    return debruijn64[(x&-x)*0x022fdd63cc95386dull >> 58];
}

static inline int pntz(uint32_t p[2]) {
    int r = ntz(p[0] - 1);
    if(r != 0 || (r = 8*sizeof(uint32_t) + ntz(p[1])) != 8*sizeof(uint32_t)) {
        return r;
    }
    return 0;
}

static void cycle(uint32_t width, unsigned char* ar[], int n)
{
    unsigned char tmp[256];
    uint32_t l;
    int i;

    if(n < 2) {
        return;
    }

    ar[n] = tmp;
    while(width) {
        l = sizeof(tmp) < width ? sizeof(tmp) : width;
        memcpy(ar[n], ar[0], l);
        for(i = 0; i < n; i++) {
            memcpy(ar[i], ar[i + 1], l);
            ar[i] += l;
        }
        width -= l;
    }
}

/* shl() and shr() need n > 0 */
static inline void shl(uint32_t p[2], int n)
{
    if(n >= int(8 * sizeof(uint32_t))) {
        n -= 8 * sizeof(uint32_t);
        p[1] = p[0];
        p[0] = 0;
    }
    p[1] <<= n;
    p[1] |= p[0] >> (sizeof(uint32_t) * 8 - n);
    p[0] <<= n;
}

static inline void shr(uint32_t p[2], int n)
{
    if(n >= int(8 * sizeof(uint32_t))) {
        n -= 8 * sizeof(uint32_t);
        p[0] = p[1];
        p[1] = 0;
    }
    p[0] >>= n;
    p[0] |= p[1] << (sizeof(uint32_t) * 8 - n);
    p[1] >>= n;
}

template <typename Compare>
static void sift(unsigned char *head, uint32_t width, Compare cmp, int pshift, uint32_t lp[])
{
    unsigned char *rt, *lf;
    unsigned char *ar[14 * sizeof(uint32_t) + 1];
    int i = 1;

    ar[0] = head;
    while(pshift > 1) {
        rt = head - width;
        lf = head - width - lp[pshift - 2];

        if(!cmp(ar[0], lf) && !cmp(ar[0], rt)) {
            break;
        }
        if(!cmp(lf, rt)) {
            ar[i++] = lf;
            head = lf;
            pshift -= 1;
        } else {
            ar[i++] = rt;
            head = rt;
            pshift -= 2;
        }
    }
    cycle(width, ar, i);
}

template <typename Compare>
static void trinkle(unsigned char *head, uint32_t width, Compare cmp, uint32_t pp[2], int pshift, int trusty, uint32_t lp[])
{
    unsigned char *stepson,
                  *rt, *lf;
    uint32_t p[2];
    unsigned char *ar[14 * sizeof(uint32_t) + 1];
    int i = 1;
    int trail;

    p[0] = pp[0];
    p[1] = pp[1];

    ar[0] = head;
    while(p[0] != 1 || p[1] != 0) {
        stepson = head - lp[pshift];
        if(cmp(stepson, ar[0])) {
            break;
        }
        if(!trusty && pshift > 1) {
            rt = head - width;
            lf = head - width - lp[pshift - 2];
            if(!cmp(rt, stepson) || !cmp(lf, stepson)) {
                break;
            }
        }

        ar[i++] = stepson;
        head = stepson;
        trail = pntz(p);
        shr(p, trail);
        pshift += trail;
        trusty = 0;
    }
    if(!trusty) {
        cycle(width, ar, i);
        sift(head, width, cmp, pshift, lp);
    }
}

template <typename Compare>
inline void das_qsort_r(void *base, uint32_t nel, uint32_t width, Compare cmp)
{
    if (nel <= 1)
      return;
    uint32_t lp[12*sizeof(uint32_t)];
    uint32_t i, size = width * nel;
    unsigned char *head, *high;
    uint32_t p[2] = {1, 0};
    int pshift = 1;
    int trail;

    //if (!size) return;//we assume width isn't 0!
    head = (unsigned char *)base;
    high = head + size - width;

    /* Precompute Leonardo numbers, scaled by element width */
    for(lp[0]=lp[1]=width, i=2; (lp[i]=lp[i-2]+lp[i-1]+width) < size; i++);

    while(head < high) {
        if((p[0] & 3) == 3) {
            sift(head, width, cmp, pshift, lp);
            shr(p, 2);
            pshift += 2;
        } else {
            if(lp[pshift - 1] >= uint32_t(high - head)) {
                trinkle(head, width, cmp, p, pshift, 0, lp);
            } else {
                sift(head, width, cmp, pshift, lp);
            }

            if(pshift == 1) {
                shl(p, 1);
                pshift = 0;
            } else {
                shl(p, pshift - 1);
                pshift = 1;
            }
        }

        p[0] |= 1;
        head += width;
    }

    trinkle(head, width, cmp, p, pshift, 0, lp);

    while(pshift != 1 || p[0] != 1 || p[1] != 0) {
        if(pshift <= 1) {
            trail = pntz(p);
            shr(p, trail);
            pshift += trail;
        } else {
            shl(p, 2);
            pshift -= 2;
            p[0] ^= 7;
            shr(p, 1);
            trinkle(head - lp[pshift] - width, width, cmp, p, pshift + 1, 1, lp);
            shl(p, 1);
            p[0] |= 1;
            trinkle(head - width, width, cmp, p, pshift, 1, lp);
        }
        head -= width;
    }
}

// Expanded 2026-05-17 by Boris Batkin / Claude (Opus 4.7):
//   das_nth_element_r / das_partial_sort_r — introselect (median-of-3
//     quickselect with smoothsort fallback on depth blowup) + smoothsort tail
//   das_make_heap_r / das_push_heap_r / das_pop_heap_r — binary max-heap on a
//     contiguous byte buffer with explicit width
// Same inline-comparator template style as the existing das_qsort_r above.
// Motivated by the linq_fold splice project's BufferTopN emit mode — see
// benchmarks/sql/LINQ.md for context. Without these the partial_sort /
// nth_element / heap-op bindings would only cover workhorse types
// (std::partial_sort etc. template on iterator type, not void*+width).
// User-defined struct types route through the any-cblock path which lands here.

static inline void byte_swap(void *pa, void *pb, uint32_t width)
{
    unsigned char tmp[256];
    unsigned char *a = (unsigned char *)pa;
    unsigned char *b = (unsigned char *)pb;
    while (width) {
        uint32_t chunk = uint32_t(sizeof(tmp)) < width ? uint32_t(sizeof(tmp)) : width;
        memcpy(tmp, a, chunk);
        memcpy(a, b, chunk);
        memcpy(b, tmp, chunk);
        a += chunk;
        b += chunk;
        width -= chunk;
    }
}

template <typename Compare>
static inline void das_sift_down_r(unsigned char *data, uint32_t parent, uint32_t nel, uint32_t width, Compare cmp)
{
    while (true) {
        uint32_t left = 2u * parent + 1u;
        if (left >= nel) return;
        uint32_t right = left + 1u;
        uint32_t largest = parent;
        if (cmp(data + parent * width, data + left * width)) largest = left;
        if (right < nel && cmp(data + largest * width, data + right * width)) largest = right;
        if (largest == parent) return;
        byte_swap(data + parent * width, data + largest * width, width);
        parent = largest;
    }
}

template <typename Compare>
inline void das_make_heap_r(void *base, uint32_t nel, uint32_t width, Compare cmp)
{
    if (nel <= 1) return;
    unsigned char *data = (unsigned char *)base;
    // Floyd bottom-up: sift_down from the last non-leaf node.
    for (uint32_t i = nel / 2u; i-- > 0u;) {
        das_sift_down_r(data, i, nel, width, cmp);
    }
}

template <typename Compare>
inline void das_push_heap_r(void *base, uint32_t nel, uint32_t width, Compare cmp)
{
    // Assumes the caller has just appended the new element at index (nel-1)
    // and now wants the heap property restored.
    if (nel <= 1) return;
    unsigned char *data = (unsigned char *)base;
    uint32_t child = nel - 1u;
    while (child > 0u) {
        uint32_t parent = (child - 1u) / 2u;
        if (!cmp(data + parent * width, data + child * width)) return;
        byte_swap(data + parent * width, data + child * width, width);
        child = parent;
    }
}

template <typename Compare>
inline void das_pop_heap_r(void *base, uint32_t nel, uint32_t width, Compare cmp)
{
    // Swap root with last, then sift_down over the reduced range [0..nel-2].
    // Caller is expected to pop / drop the last slot after.
    if (nel <= 1) return;
    unsigned char *data = (unsigned char *)base;
    byte_swap(data, data + (nel - 1u) * width, width);
    das_sift_down_r(data, 0u, nel - 1u, width, cmp);
}

template <typename Compare>
inline void das_nth_element_r(void *base, uint32_t nel, uint32_t n, uint32_t width, Compare cmp)
{
    if (nel <= 1u || n >= nel) return;
    unsigned char *data = (unsigned char *)base;
    uint32_t lo = 0u;
    uint32_t hi = nel - 1u;
    // Introselect depth bound: 2 * floor(log2(nel)). On bound exhaustion,
    // fall back to smoothsort over the remaining range — O(n log n) but
    // immune to adversarial quickselect inputs.
    int depth_limit = 0;
    for (uint32_t x = nel; x > 0u; x >>= 1) depth_limit += 2;
    while (lo < hi) {
        if (depth_limit-- <= 0) {
            das_qsort_r(data + lo * width, hi - lo + 1u, width, cmp);
            return;
        }
        // Small range: insertion sort and we're done. 16 is a common cutoff.
        if (hi - lo < 16u) {
            for (uint32_t i = lo + 1u; i <= hi; i++) {
                for (uint32_t j = i; j > lo && cmp(data + j * width, data + (j - 1u) * width); j--) {
                    byte_swap(data + j * width, data + (j - 1u) * width, width);
                }
            }
            return;
        }
        // Median-of-3 pivot across lo, mid, hi; leaves median at `mid`.
        uint32_t mid = lo + (hi - lo) / 2u;
        unsigned char *plo  = data + lo  * width;
        unsigned char *pmid = data + mid * width;
        unsigned char *phi  = data + hi  * width;
        if (cmp(pmid, plo)) byte_swap(plo, pmid, width);
        if (cmp(phi,  plo)) byte_swap(plo, phi,  width);
        if (cmp(phi,  pmid)) byte_swap(pmid, phi, width);
        // Move pivot to `hi` and partition (Lomuto).
        byte_swap(pmid, phi, width);
        unsigned char *pivot = phi;
        uint32_t i = lo;
        for (uint32_t j = lo; j < hi; j++) {
            if (cmp(data + j * width, pivot)) {
                if (i != j) byte_swap(data + i * width, data + j * width, width);
                i++;
            }
        }
        byte_swap(data + i * width, pivot, width);
        if (i == n) return;
        if (i < n) lo = i + 1u;
        else       hi = i - 1u;  // i > n implies i >= 1
    }
}

template <typename Compare>
inline void das_partial_sort_r(void *base, uint32_t nel, uint32_t n, uint32_t width, Compare cmp)
{
    if (nel <= 1u || n == 0u) return;
    if (n > nel) n = nel;
    if (n < nel) {
        das_nth_element_r(base, nel, n, width, cmp);
    }
    das_qsort_r(base, n, width, cmp);
}

}

---
title: Do you even sort?
date: 2026-05-18 20:35:17
tags:
    - daScript
    - C++
---

The Art Of Computer Programming Volume 3. Kind of.

<!-- more -->

Sort and rescue? Also hashtag hash - but that is das_hash_map, and its a story for another day. It's an old story too, but I'll get to it.

Today is top_n_by day

    let rows <- _sql(db |> select_from(type<Car>) |> _order_by(_.price) |> take(TAKE_N))
    let rows <- _fold(arr |> _order_by(_.price) |> take(TAKE_N))
    let rows <- top_n_by(arr, TAKE_N, @@(c : Car -&) => c.price)

Its like take all the cars, sort by price, pick top N. Naive solution is to first sort, then take. Don't be naive.

But what's in box number 2?

    for (it in a) {
        if (length(buf) < n) {
            buf |> push_clone(it)
            sort_boost::push_heap(buf, $(v1, v2) => _::less(key(v1), key(v2)))
        } elif (_::less(key(it), key(buf[0]))) {
            sort_boost::pop_heap(buf, $(v1, v2) => _::less(key(v1), key(v2)))
            buf[length(buf) - 1] := it
            sort_boost::push_heap(buf, $(v1, v2) => _::less(key(v1), key(v2)))
        }
    }
    sort(buf, $(v1, v2) => _::less(key(v1), key(v2)))

There is also nth_element in there somewhere, but I will get to that one eventually.

It wasn't here this morning. I was working on making LINQ awesome. SQL version is excellent, but containers are lacking. Not for long.

Problem is - das has sort, sort, and only sort. No heap, no nth_element, no nada. Here is what I do when I see nada. I turn nothing into something. An hour later I had all this jazz there in the PR. Easy. Right? Right??

It never is. I can't just merge. I have to measure. Take the ruler, take it out, and see where it lands. This is how das became das. So a matrix was born. The matrix of the benchmarks, that is.

std::sort vs another std::sort (because Apple Clang is more awesome more often), vs qsort (which was later dropped for being dog slow), vs das_sort (and later vs `das_sort<T>`). repeat for nth_element, make_heap, heap_sort. Different sizes too.

Guess what? It instantly showed that what I had was not IT. A few hours later, what I had became IT enough. Did I mention the Apple Clang runtime? Its good.

It's bold when das wins.

### The Matrix vs libc++ std:: (thats Apple Clang)

| op | int32 | int64 | P32 | P128 |
|---|---:|---:|---:|---:|
| sort | 1.37x | 1.38x | **0.61x** | **0.91x** |
| partial_sort | 1.03x | 1.01x | 1.05x | **0.95x** |
| nth_element | **0.64x** | **0.65x** | **0.70x** | **0.74x** |
| make_heap | **0.95x** | 1.02x | 1.09x | 1.06x |
| heap_sort | **0.92x** | **0.94x** | 1.12x | 1.12x |

das is better 10/20. we win on larger structs, which I find more important.

### The Matrix vs libstdc++ std:: (thats regular Clang)

| op | int32 | int64 | P32 | P128 |
|---|---:|---:|---:|---:|
| sort | **0.66x** | **0.66x** | **0.68x** | **0.88x** |
| partial_sort | **0.99x** | **0.93x** | 1.02x | 1.01x |
| nth_element | **0.68x** | **0.71x** | **0.66x** | **0.74x** |
| make_heap | **0.67x** | **0.66x** | **0.56x** | **0.76x** |
| heap_sort | **0.89x** | **0.91x** | **0.93x** | 1.01x |

das is better 17/20.

That cars example on top - das is now on par with SQLITE, when operating on arrays. SQLITE has had years, its solid. Thats all it does.

All I do is make das awesome, one LINQ at a time.

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

std::sort vs another std::sort (because Apple Clang is more awesome more often), vs qsort (which was later dropped for being dog slow), vs das_sort (and later vs das_sort<T>). repeat for nth_element, make_heap, heap_sort. Different sizes too.

Guess what? It instantly showed that what I had was not IT. A few hours later, what I had became IT enough. Did I mention the Apple Clang runtime? Its good.

It's bold when das wins.

### The Matrix vs libc++ std:: (thats Apple Clang)

| op | int32 | int64 | P32 | P128 |
|---|---:|---:|---:|---:|
| sort | 1.37× | 1.38× | **0.61×** | **0.91×** |
| partial_sort | 1.03× | 1.01× | 1.05× | **0.95×** |
| nth_element | **0.64×** | **0.65×** | **0.70×** | **0.74×** |
| make_heap | **0.95×** | 1.02× | 1.09× | 1.06× |
| heap_sort | **0.92×** | **0.94×** | 1.12× | 1.12× |

das is better 10/20. we win on larger structs, which I find more important.

### The Matrix vs libstdc++ std:: (thats regular Clang)

| op | int32 | int64 | P32 | P128 |
|---|---:|---:|---:|---:|
| sort | **0.66×** | **0.66×** | **0.68×** | **0.88×** |
| partial_sort | **0.99×** | **0.93×** | 1.02× | 1.01× |
| nth_element | **0.68×** | **0.71×** | **0.66×** | **0.74×** |
| make_heap | **0.67×** | **0.66×** | **0.56×** | **0.76×** |
| heap_sort | **0.89×** | **0.91×** | **0.93×** | 1.01× |

das is better 17/20.

That cars example on top - das is now on par with SQLITE, when operating on arrays. SQLITE has had years, its solid. Thats all it does.

All I do is make das awesome, one LINQ at a time.

options gen2

require daslib/linq_das
require daslib/linq
require daslib/linq_boost
require daslib/linq_fold

// Tutorial language-56: C#-style LINQ query syntax (`%linq!`)
//
// This tutorial covers:
//   - the `%linq! … %%` reader macro: from / where / select
//   - orderby (single and multi-key, ascending / descending)
//   - group … by (IGrouping) and the `into` group-continuation (grouped aggregation)
//   - select … into (projection continuation) and multi-stage chains
//   - let bindings and a single inner join
//   - join … into g (group join — C# GroupJoin, outer)
//
// `%linq!` is a purely compile-time rewrite into a fused `_fold(...)` chain — it costs nothing over the
// hand-written pipe form, and the same query works over arrays, decs, SQL and XML (the typed `: Row` form).
//
// Run: daslang.exe tutorials/language/56_linq_query.das

struct Car {
    name  : string
    brand : string
    price : int
}

struct BrandHQ {
    brand   : string
    country : string
}

def private fleet() : array<Car> {
    return <- [Car(name = "i3",   brand = "bmw",  price = 100),
               Car(name = "m3",   brand = "bmw",  price = 250),
               Car(name = "a4",   brand = "audi", price = 200),
               Car(name = "a8",   brand = "audi", price = 400),
               Car(name = "rio",  brand = "kia",  price = 50)]
}

[export]
def main {
    let cars <- fleet()

    // ──────────────────────────────────────────────────────────────────────────
    // Section 1 — from / where / select
    // ──────────────────────────────────────────────────────────────────────────
    //
    // The range variable (`c`) is spliced verbatim as the lambda parameter. `select c` (the variable
    // alone) is the identity projection; any other projection emits a `_select`.
    let names <- %linq! from c in cars where c.price > 100 select c.name %%
    print("expensive: {names}\n")    // [ m3, a4, a8]

    // ──────────────────────────────────────────────────────────────────────────
    // Section 2 — orderby (multi-key, per-key direction)
    // ──────────────────────────────────────────────────────────────────────────
    //
    // Comma-separated keys, each with its own optional `ascending` / `descending`. More than one key
    // emits a single composite stable sort.
    let sorted <- %linq! from c in cars orderby c.brand, c.price descending select (b = c.brand, p = c.price) %%
    print("by brand, then price desc:\n")
    for (r in sorted) { print("  {r.b} {r.p}\n") }

    // ──────────────────────────────────────────────────────────────────────────
    // Section 3 — group … by  (IGrouping)
    // ──────────────────────────────────────────────────────────────────────────
    //
    // A bare `group c by k` yields one `(key, [members])` tuple per bucket — read the key as `._0` and
    // the members as `._1`.
    let buckets <- %linq! from c in cars group c by c.brand %%
    print("buckets:\n")
    for (g in buckets) { print("  {g._0}: {g._1 |> length} cars\n") }

    // ──────────────────────────────────────────────────────────────────────────
    // Section 4 — group … into g … select  (grouped aggregation)  ★ the headline
    // ──────────────────────────────────────────────────────────────────────────
    //
    // `into g` rebinds the group to a new range variable and continues the query. With the A2 convention
    // `g.key` is the bucket key and a bare `g` is the member collection — so the familiar LINQ aggregates
    // read naturally. This is the canonical C# grouped-aggregation idiom, and over a SQL source it pushes
    // straight down to `GROUP BY` + `COUNT/SUM/AVG/...`.
    let report <- %linq! from c in cars
                         group c by c.brand into g
                         select (brand = g.key,
                                 count = g |> length,
                                 total = g |> select($(u : Car) => u.price) |> sum,
                                 avg   = g |> select($(u : Car) => u.price) |> average) %%
    print("brand report:\n")
    for (r in report) { print("  {r.brand}: count={r.count} total={r.total} avg={r.avg}\n") }

    // The continuation may itself filter / order the groups. Here: drop singleton buckets, order by total.
    let bigBrands <- %linq! from c in cars
                            group c by c.brand into g
                            where g |> length > 1
                            orderby g |> select($(u : Car) => u.price) |> sum descending
                            select g.key %%
    print("multi-car brands, richest first: {bigBrands}\n")    // [ audi, bmw]

    // ──────────────────────────────────────────────────────────────────────────
    // Section 5 — select … into n …  (projection continuation, multi-stage)
    // ──────────────────────────────────────────────────────────────────────────
    //
    // `select <proj> into n` rebinds the projected value. Continuations chain, so a query can project,
    // filter, then re-order — all fused on one pass, no intermediate array.
    let dear <- %linq! from c in cars
                       select c.price into p
                       where p >= 200
                       orderby p descending
                       select p %%
    print("prices >= 200, desc: {dear}\n")    // [ 400, 250, 200]

    // ──────────────────────────────────────────────────────────────────────────
    // Section 6 — let bindings and a single inner join
    // ──────────────────────────────────────────────────────────────────────────
    //
    // `let` introduces a computed value reused downstream (inlined at compile time). `join … on … equals …`
    // is a single inner equi-join introducing a second range variable.
    let discounted <- %linq! from c in cars let net = c.price - 20 where net > 100 select (n = c.name, p = net) %%
    print("net (price-20) > 100:\n")
    for (r in discounted) { print("  {r.n}: {r.p}\n") }

    let hqs <- [BrandHQ(brand = "bmw", country = "DE"), BrandHQ(brand = "audi", country = "DE"),
                BrandHQ(brand = "kia", country = "KR")]
    let located <- %linq! from c in cars join h in hqs on c.brand equals h.brand select (car = c.name, country = h.country) %%
    print("car → country:\n")
    for (r in located) { print("  {r.car}: {r.country}\n") }

    // ──────────────────────────────────────────────────────────────────────────
    // Section 7 — join … into g  (group join, C# GroupJoin / outer)
    // ──────────────────────────────────────────────────────────────────────────
    //
    // `join … equals … into g` binds `g` to the array of *matching right rows* alongside the left variable —
    // the terminal `select` reads both. It is OUTER: every left row surfaces, an unmatched one with an empty
    // group. Here we group the cars under each brand HQ; "tesla" has no cars, so it surfaces with count 0.
    let hqs2 <- [BrandHQ(brand = "bmw", country = "DE"), BrandHQ(brand = "audi", country = "DE"),
                 BrandHQ(brand = "kia", country = "KR"), BrandHQ(brand = "tesla", country = "US")]
    let perHq <- %linq! from h in hqs2
                        join c in cars on h.brand equals c.brand into g
                        select (brand = h.brand,
                                country = h.country,
                                n     = g |> length,
                                total = g |> select($(u : Car) => u.price) |> sum) %%
    print("cars per brand HQ (outer — tesla has none):\n")
    for (r in perHq) { print("  {r.brand} ({r.country}): n={r.n} total={r.total}\n") }
}

options gen2

require dastest/testing_boost
require daslib/strings_boost
require daslib/faker
require daslib/fuzzer
require daslib/regex

[test(description = "levenshtein_distance and its fast variant give zero for equal strings and the textbook distances for classic pairs, and both survive fuzzed input")]
def test_levenshtein_distance(t : T?) {
    t |> run("zero distance") @@(t : T?) {
        t |> equal(levenshtein_distance("kitten", "kitten"), 0)
        t |> equal(levenshtein_distance_fast("kitten", "kitten"), 0)
    }

    t |> run("kitten vs sitting") @@(t : T?) {
        t |> equal(levenshtein_distance("kitten", "sitting"), 3)
        t |> equal(levenshtein_distance_fast("kitten", "sitting"), 3)
    }

    t |> run("kitten vs sitting") @@(t : T?) {
        t |> equal(levenshtein_distance("GUMBO", "GAMBOL"), 2)
        t |> equal(levenshtein_distance_fast("GUMBO", "GAMBOL"), 2)
    }

    t |> run("Saturday vs Sunday") @@(t : T?) {
        t |> equal(levenshtein_distance("Saturday", "Sunday"), 3)
        t |> equal(levenshtein_distance_fast("Saturday", "Sunday"), 3)
    }

    t |> run("fuzz levenshtein_distance") @@(t : T?) {
        var fake <- Faker()
        fuzz() {
            let s1 = fake |> any_string
            let s2 = fake |> any_string
            t |> accept(levenshtein_distance(s1, s2))
        }
    }

    t |> run("fuzz levenshtein_distance_fast") @@(t : T?) {
        var fake <- Faker()
        fuzz() {
            let s1 = fake |> any_string
            let s2 = fake |> any_string
            t |> accept(levenshtein_distance_fast(s1, s2))
        }
    }
}

def make_set(items : array<string>) : table<string> {
    var s : table<string>
    for (x in items) {
        s |> insert(x)
    }
    return <- s
}

[test(description = "jaccard over tables gives 1 for equal sets, 0 for disjoint or empty sets and the overlap ratio otherwise")]
def test_jaccard_table(t : T?) {
    t |> run("equal sets") @@(t : T?) {
        let a <- make_set(["foo", "bar", "baz"])
        let b <- make_set(["foo", "bar", "baz"])
        t |> equal(jaccard(a, b), 1.0f)
    }

    t |> run("disjoint sets") @@(t : T?) {
        let a <- make_set(["foo", "bar"])
        let b <- make_set(["baz", "qux"])
        t |> equal(jaccard(a, b), 0.0f)
    }

    t |> run("partial overlap") @@(t : T?) {
        let a <- make_set(["foo", "bar", "baz"])
        let b <- make_set(["foo", "bar", "qux"])
        t |> equal(jaccard(a, b), 0.5f)
    }

    t |> run("subset") @@(t : T?) {
        let a <- make_set(["foo", "bar"])
        let b <- make_set(["foo", "bar", "baz", "qux"])
        t |> equal(jaccard(a, b), 0.5f)
    }

    t |> run("empty either side returns 0") @@(t : T?) {
        let a <- make_set(["foo"])
        let b : table<string>
        t |> equal(jaccard(a, b), 0.0f)
        t |> equal(jaccard(b, a), 0.0f)
    }

    t |> run("both empty returns 0") @@(t : T?) {
        let a, b : table<string>
        t |> equal(jaccard(a, b), 0.0f)
    }
}

[test(description = "jaccard over arrays matches the table form and dedupes through set conversion")]
def test_jaccard_array(t : T?) {
    t |> run("array overload matches table form") @@(t : T?) {
        let a <- ["foo", "bar", "baz"]
        let b <- ["foo", "bar", "qux"]
        t |> equal(jaccard(a, b), 0.5f)
    }

    t |> run("array dedupes via set conversion") @@(t : T?) {
        let a <- ["foo", "foo", "bar"]   // dedupes to {foo, bar}
        let b <- ["foo", "bar"]
        t |> equal(jaccard(a, b), 1.0f)
    }

    t |> run("array empty returns 0") @@(t : T?) {
        let a <- ["foo"]
        let b : array<string>
        t |> equal(jaccard(a, b), 0.0f)
    }
}

[test(description = "levenshtein_distance_fast gives the textbook distances for classic pairs and survives fuzzed strings")]
def test_levenshtein_distance_fast(t : T?) {
    t |> equal(levenshtein_distance_fast("kitten", "sitting"), 3)
    t |> equal(levenshtein_distance_fast("Saturday", "Sunday"), 3)
    let limit = int(re_gen_get_rep_limit())
    var fake <- Faker()
    fuzz() {
        let ss = fake |> any_string
        let tt = fake |> any_string
        let dist = levenshtein_distance_fast(ss, tt)
        t |> success(dist <= limit)
        t |> equal(dist, levenshtein_distance(ss, tt))
    }
}

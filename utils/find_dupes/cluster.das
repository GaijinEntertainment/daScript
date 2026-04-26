options gen2

require strings
require math
require minhash


struct public MemberRef {
    file : string
    name : string
    line : int
    is_lambda : bool
}


struct public FuncRecord {
    ref : MemberRef
    canonical : string
    sig : array<uint64>
    token_count : int
    cluster_id : int = -1
}


struct public Cluster {
    canonical_sample : string
    token_count : int
    members : array<MemberRef>
}


struct public FuzzyPair {
    similarity : float
    a : MemberRef
    b : MemberRef
    canonical_a : string
    canonical_b : string
}


// Build exact-clone clusters by grouping records with identical
// canonical text. Mutates `records[i].cluster_id` so the fuzzy pass
// can skip same-cluster pairs. Skips clusters of size 1.
//
// Bucket on the canonical string itself (not its 64-bit hash) so a
// rare hash collision can't conflate two unrelated functions.
def public exact_buckets(var records : array<FuncRecord>; min_tokens : int) : array<Cluster> {
    var buckets : table<string; array<int>>
    for (i in range(length(records))) {
        if (records[i].token_count < min_tokens) {
            continue
        }
        let key = records[i].canonical
        if (key_exists(buckets, key)) {
            buckets[key] |> push(i)
        } else {
            var lst : array<int>
            lst |> push(i)
            buckets[key] <- lst
        }
    }
    var out : array<Cluster>
    var cluster_idx = 0
    for (idxs in values(buckets)) {
        if (length(idxs) < 2) {
            continue
        }
        var c = Cluster()
        c.canonical_sample := records[idxs[0]].canonical
        c.token_count = records[idxs[0]].token_count
        c.members |> reserve(length(idxs))
        for (i in idxs) {
            records[i].cluster_id = cluster_idx
            c.members |> push(records[i].ref)
        }
        out |> emplace(c)
        cluster_idx++
    }
    return <- out
}


// All-pairs MinHash comparison. Skips pairs already in the same exact
// cluster (those are reported as exact, not fuzzy). Skips pairs where
// either function has fewer than min_tokens tokens (noise floor).
//
// MinHash works on the SET of shingles. Highly periodic code (e.g. a
// boilerplate block repeated N times) has a tiny shingle set, so 4x
// and 7x repetitions both estimate Jaccard = 1.0 even though they're
// not the same function. Gate the report on a length ratio too.
def public fuzzy_pairs(records : array<FuncRecord>; threshold : float; min_tokens : int) : array<FuzzyPair> {
    var out : array<FuzzyPair>
    out |> reserve(64)
    let n = length(records)
    for (i in range(n)) {
        if (records[i].token_count < min_tokens) {
            continue
        }
        for (j in range(i + 1, n)) {
            if (records[j].token_count < min_tokens) {
                continue
            }
            if (records[i].cluster_id != -1 && records[i].cluster_id == records[j].cluster_id) {
                continue
            }
            // Same canonical text would already be in the exact-cluster
            // set (caught above when both have a cluster_id). Compare on
            // the string itself rather than the 64-bit hash to dodge any
            // theoretical collision.
            if (records[i].canonical == records[j].canonical) {
                continue
            }
            let ti = float(records[i].token_count)
            let tj = float(records[j].token_count)
            let len_ratio = ti < tj ? ti / tj : tj / ti
            if (len_ratio < threshold) {
                continue
            }
            let s = jaccard_estimate(records[i].sig, records[j].sig)
            // Geometric mean of MinHash similarity and length similarity:
            // periodic code with very different lengths drops out.
            let combined = sqrt(s * len_ratio)
            if (combined >= threshold) {
                var p = FuzzyPair()
                p.similarity = combined
                p.a = records[i].ref
                p.b = records[j].ref
                p.canonical_a := records[i].canonical
                p.canonical_b := records[j].canonical
                out |> emplace(p)
            }
        }
    }
    return <- out
}


def member_key(m : MemberRef) : string {
    return "{m.file}:{m.line}:{m.name}"
}


// Sort clusters by member count descending; tie-break on token_count
// desc, then canonical_sample, then first-member location. Without
// tie-breakers `values(buckets)` iteration order leaks into the report
// and diff-ing two runs becomes noisy.
def public sort_clusters(var clusters : array<Cluster>) {
    sort(clusters) $(a, b) {
        let na = length(a.members)
        let nb = length(b.members)
        if (na != nb) {
            return na > nb
        }
        if (a.token_count != b.token_count) {
            return a.token_count > b.token_count
        }
        if (a.canonical_sample != b.canonical_sample) {
            return a.canonical_sample < b.canonical_sample
        }
        if (na == 0) {
            return false
        }
        return member_key(a.members[0]) < member_key(b.members[0])
    }
}


// Sort fuzzy pairs by similarity descending; tie-break on (a, b)
// member locations so pair ordering is reproducible across runs.
def public sort_pairs(var pairs : array<FuzzyPair>) {
    sort(pairs) $(a, b) {
        if (a.similarity != b.similarity) {
            return a.similarity > b.similarity
        }
        let ka = member_key(a.a)
        let kb = member_key(b.a)
        if (ka != kb) {
            return ka < kb
        }
        return member_key(a.b) < member_key(b.b)
    }
}

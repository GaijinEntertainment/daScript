options gen2

require dastest/testing_boost public
require samples_store
require daslib/result
require daslib/sql_linq

def private test_policy : StorePolicy {
    return StorePolicy(max_source_bytes = 64, max_inserts_per_ip_hour = 3)
}

[test]
def test_migration(t : T?) {
    t |> run("fresh db migrates to v1") @(t : T?) {
        with_latest_sqlite(":memory:") $(db) {
            t |> equal(current_schema_version(db), 1)
        }
    }
}

[test]
def test_put_and_get(t : T?) {
    with_latest_sqlite(":memory:") $(db) {
        t |> run("put stores and returns the content hash") @(tt : T?) {
            let src = "print(\"hi\")"
            let r = put_sample(db, src, "10.0.0.1", test_policy(), 1000l)
            tt |> equal(r.status, PutStatus.ok)
            tt |> success(r.created, "first put creates")
            tt |> equal(r.hash, sha256_hex(src))
            tt |> success(sample_exists(db, r.hash), "exists after put")
            tt |> equal(get_source(db, r.hash), src)
        }
        t |> run("unknown hash reads as empty") @(tt : T?) {
            tt |> equal(get_source(db, "deadbeef"), "")
            tt |> success(!sample_exists(db, "deadbeef"), "absent hash does not exist")
        }
    }
}

[test]
def test_dedup_first_writer_wins(t : T?) {
    with_latest_sqlite(":memory:") $(db) {
        let src = "let x = 1"
        let first = put_sample(db, src, "10.0.0.1", test_policy(), 1000l)
        let second = put_sample(db, src, "10.0.0.2", test_policy(), 2000l)
        t |> run("second put dedups") @(tt : T?) {
            tt |> equal(second.status, PutStatus.ok)
            tt |> success(!second.created, "dedup hit does not create")
            tt |> equal(second.hash, first.hash)
        }
        t |> run("original row metadata is preserved") @(tt : T?) {
            let rows <- _sql(db |> select_from(type<Sample>)
                |> _where(_.Hash == first.hash))
            tt |> equal(length(rows), 1)
            tt |> equal(rows[0].ClientIp, "10.0.0.1")
            tt |> equal(rows[0].CreatedAt, 1000l)
            tt |> equal(rows[0].Origin, "user")
            tt |> success(!rows[0].Listed, "not listed by default")
        }
    }
}

[test]
def test_rejections(t : T?) {
    with_latest_sqlite(":memory:") $(db) {
        t |> run("empty source") @(tt : T?) {
            tt |> equal(put_sample(db, "", "10.0.0.1", test_policy(), 1000l).status, PutStatus.empty_source)
        }
        t |> run("over the size cap") @(tt : T?) {
            var big = ""
            for (_i in range(65)) {
                big += "x"
            }
            tt |> equal(put_sample(db, big, "10.0.0.1", test_policy(), 1000l).status, PutStatus.too_large)
        }
        t |> run("rejected puts store nothing") @(tt : T?) {
            let rows <- _sql(db |> select_from(type<Sample>) |> _select(_.Hash))
            tt |> equal(length(rows), 0)
        }
    }
}

[test]
def test_rate_ceiling(t : T?) {
    with_latest_sqlite(":memory:") $(db) {
        let pol = test_policy()   // 3 per hour
        t |> run("ceiling rejects the fourth insert") @(tt : T?) {
            tt |> equal(put_sample(db, "a1", "10.0.0.1", pol, 1000l).status, PutStatus.ok)
            tt |> equal(put_sample(db, "a2", "10.0.0.1", pol, 1010l).status, PutStatus.ok)
            tt |> equal(put_sample(db, "a3", "10.0.0.1", pol, 1020l).status, PutStatus.ok)
            tt |> equal(put_sample(db, "a4", "10.0.0.1", pol, 1030l).status, PutStatus.rate_limited)
        }
        t |> run("dedup hits bypass the ceiling") @(tt : T?) {
            tt |> equal(put_sample(db, "a1", "10.0.0.1", pol, 1040l).status, PutStatus.ok)
        }
        t |> run("another ip is unaffected") @(tt : T?) {
            tt |> equal(put_sample(db, "b1", "10.0.0.2", pol, 1050l).status, PutStatus.ok)
        }
        t |> run("the window expires") @(tt : T?) {
            tt |> equal(put_sample(db, "a5", "10.0.0.1", pol, 1000l + 3601l).status, PutStatus.ok)
        }
    }
}

[test]
def test_utf8_validation(t : T?) {
    t |> run("valid sequences") @(t : T?) {
        t |> success(is_valid_utf8("plain ascii"), "ascii")
        t |> success(is_valid_utf8("héllo wörld"), "two-byte")
        t |> success(is_valid_utf8("日本語"), "three-byte")
        t |> success(is_valid_utf8("🎉"), "four-byte")
        t |> success(is_valid_utf8(""), "empty")
    }
    t |> run("invalid sequences") @(t : T?) {
        var bad : array<uint8>
        bad |> push(uint8(0xC3))
        bad |> push(uint8(0x28))            // 2-byte lead + non-continuation
        t |> success(!is_valid_utf8(bad), "truncated two-byte")
        bad |> clear()
        bad |> push(uint8(0x80))            // bare continuation
        t |> success(!is_valid_utf8(bad), "bare continuation")
        bad |> clear()
        bad |> push(uint8(0xC0))
        bad |> push(uint8(0xAF))            // overlong '/'
        t |> success(!is_valid_utf8(bad), "overlong")
        bad |> clear()
        bad |> push(uint8(0xED))
        bad |> push(uint8(0xA0))
        bad |> push(uint8(0x80))            // UTF-16 surrogate D800
        t |> success(!is_valid_utf8(bad), "surrogate")
        bad |> clear()
        bad |> push(uint8(0xF5))
        bad |> push(uint8(0x80))
        bad |> push(uint8(0x80))
        bad |> push(uint8(0x80))            // > U+10FFFF
        t |> success(!is_valid_utf8(bad), "above U+10FFFF")
        bad |> clear()
        bad |> push(uint8(0xE2))
        bad |> push(uint8(0x82))            // truncated three-byte at end
        t |> success(!is_valid_utf8(bad), "truncated at end of input")
        delete bad
    }
}

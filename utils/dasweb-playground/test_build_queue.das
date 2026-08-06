options gen2

require dastest/testing_boost public
require samples_store
require build_queue
require daslib/sql_linq
require strings

// samples_store is required alongside build_queue so the migration registry
// carries the full v1..v4 stream, the way every real program that opens
// samples.db loads it.

def private test_cfg : BuildConfig {
    return BuildConfig(claim_timeout_seconds = 600, max_build_attempts = 3)
}

let H1 = "1111111111111111111111111111111111111111111111111111111111111111"
let H2 = "2222222222222222222222222222222222222222222222222222222222222222"
let H3 = "3333333333333333333333333333333333333333333333333333333333333333"
let TC = "abc1234def5678900000000000000000000000000"

[test]
def test_migration_v4(t : T?) {
    t |> run("fresh db migrates to v4") @(t : T?) {
        with_latest_sqlite(":memory:") $(db) {
            t |> equal(current_schema_version(db), 4)
        }
    }
}

[test]
def test_expected_artifact_names(t : T?) {
    // The server enforces this set rather than trusting the builder, so it is
    // a store behaviour and gets a store test, not only an HTTP one.
    t |> run("module publishes exactly one named artifact") @(tt : T?) {
        let names <- expected_artifact_names(BUILD_MODE_MODULE)
        tt |> equal(length(names), 1)
        tt |> equal(names[0], "sample.wasm")
    }
    t |> run("a mode with no declared set publishes nothing") @(tt : T?) {
        tt |> success(empty(expected_artifact_names(BUILD_MODE_PAGE)), "page not wired yet")
        tt |> success(empty(expected_artifact_names("bogus")), "unknown mode")
    }
}

[test]
def test_toolchain_meta(t : T?) {
    with_latest_sqlite(":memory:") $(db) {
        t |> run("no toolchain until announced") @(tt : T?) {
            tt |> equal(current_toolchain(db), "")
        }
        t |> run("announce sets, re-announce overwrites") @(tt : T?) {
            set_current_toolchain(db, TC)
            tt |> equal(current_toolchain(db), TC)
            set_current_toolchain(db, "feed5678deadbeef000000000000000000000000")
            tt |> equal(current_toolchain(db), "feed5678deadbeef000000000000000000000000")
        }
    }
}

[test]
def test_enqueue_idempotent(t : T?) {
    with_latest_sqlite(":memory:") $(db) {
        t |> run("first enqueue creates a queued job") @(tt : T?) {
            let r = enqueue_build(db, H1, TC, BUILD_MODE_MODULE, 1000l)
            tt |> success(r.created, "created")
            tt |> equal(r.state, BUILD_STATE_QUEUED)
        }
        t |> run("second enqueue is a no-op") @(tt : T?) {
            let r = enqueue_build(db, H1, TC, BUILD_MODE_MODULE, 1001l)
            tt |> success(!r.created, "not created twice")
            tt |> equal(r.state, BUILD_STATE_QUEUED)
        }
        t |> run("same hash under another toolchain is its own job") @(tt : T?) {
            let r = enqueue_build(db, H1, "feed5678deadbeef000000000000000000000000", BUILD_MODE_MODULE, 1002l)
            tt |> success(r.created, "created")
        }
    }
}

[test]
def test_claim_semantics(t : T?) {
    with_latest_sqlite(":memory:") $(db) {
        t |> run("empty queue claims nothing") @(tt : T?) {
            let c = claim_next_build(db, test_cfg(), TC, 1000l)
            tt |> equal(c.hash, "")
        }
        enqueue_build(db, H1, TC, BUILD_MODE_MODULE, 1000l)
        enqueue_build(db, H2, TC, BUILD_MODE_PAGE, 1001l)
        t |> run("claim takes the oldest queued job") @(tt : T?) {
            let c = claim_next_build(db, test_cfg(), TC, 1002l)
            tt |> equal(c.hash, H1)
            tt |> equal(c.mode, BUILD_MODE_MODULE)
            tt |> equal(c.attempts, 1)
            let st = build_status(db, H1, TC)
            tt |> equal(st.state, BUILD_STATE_BUILDING)
        }
        t |> run("claim is toolchain-scoped") @(tt : T?) {
            let c = claim_next_build(db, test_cfg(), "feed5678deadbeef000000000000000000000000", 1003l)
            tt |> equal(c.hash, "")
        }
        t |> run("next claim takes the next job, carrying its mode") @(tt : T?) {
            let c = claim_next_build(db, test_cfg(), TC, 1004l)
            tt |> equal(c.hash, H2)
            tt |> equal(c.mode, BUILD_MODE_PAGE)
        }
    }
}

[test]
def test_done_flow(t : T?) {
    with_latest_sqlite(":memory:") $(db) {
        enqueue_build(db, H1, TC, BUILD_MODE_MODULE, 1000l)
        t |> run("a result for an unclaimed job is refused") @(tt : T?) {
            tt |> success(!record_build_done(db, H1, TC, "x/y", "[]", 1001l), "refused before claim")
        }
        claim_next_build(db, test_cfg(), TC, 1002l)
        t |> run("a building job resolves to done with its artifact") @(tt : T?) {
            tt |> success(record_build_done(db, H1, TC, "{TC}/{H1}", "[\"a.wasm\"]", 1003l), "recorded")
            let st = build_status(db, H1, TC)
            tt |> equal(st.state, BUILD_STATE_DONE)
            tt |> equal(st.artifact_path, "{TC}/{H1}")
            tt |> equal(st.artifact_files, "[\"a.wasm\"]")
        }
        t |> run("a second result is refused") @(tt : T?) {
            tt |> success(!record_build_done(db, H1, TC, "x/y", "[]", 1004l), "refused after done")
        }
    }
}

[test]
def test_failed_flow(t : T?) {
    with_latest_sqlite(":memory:") $(db) {
        enqueue_build(db, H1, TC, BUILD_MODE_MODULE, 1000l)
        claim_next_build(db, test_cfg(), TC, 1001l)
        t |> run("failure stores the compiler error") @(tt : T?) {
            tt |> success(record_build_failed(db, H1, TC, "error[30101]: nope", 1002l), "recorded")
            let st = build_status(db, H1, TC)
            tt |> equal(st.state, BUILD_STATE_FAILED)
            tt |> equal(st.error, "error[30101]: nope")
        }
        t |> run("force re-enqueue resets the failed job") @(tt : T?) {
            let r = enqueue_build(db, H1, TC, BUILD_MODE_MODULE, 1003l, force = true)
            tt |> success(r.created, "recreated")
            let st = build_status(db, H1, TC)
            tt |> equal(st.state, BUILD_STATE_QUEUED)
            tt |> equal(st.attempts, 0)
            tt |> equal(st.error, "")
        }
        t |> run("plain re-request of a failed job stays failed") @(tt : T?) {
            claim_next_build(db, test_cfg(), TC, 1004l)
            record_build_failed(db, H1, TC, "still broken", 1005l)
            let r = enqueue_build(db, H1, TC, BUILD_MODE_MODULE, 1006l)
            tt |> success(!r.created, "not recreated")
            tt |> equal(r.state, BUILD_STATE_FAILED)
        }
    }
}

[test]
def test_error_text_is_capped(t : T?) {
    with_latest_sqlite(":memory:") $(db) {
        enqueue_build(db, H1, TC, BUILD_MODE_MODULE, 1000l)
        claim_next_build(db, test_cfg(), TC, 1001l)
        var huge = ""
        for (_i in range(1200)) {
            huge += "0123456789012345678901234567890123456789012345678901234567890123"   // 64 bytes
        }
        t |> run("oversized error is truncated with a marker") @(tt : T?) {
            record_build_failed(db, H1, TC, huge, 1002l)
            let st = build_status(db, H1, TC)
            tt |> success(length(st.error) < length(huge), "shorter than input")
            tt |> success(length(st.error) <= MAX_BUILD_ERROR_BYTES + 32, "near the cap")
            tt |> success(find(st.error, "truncated") >= 0, "marked truncated")
        }
    }
}

[test]
def test_stale_claim_requeue(t : T?) {
    with_latest_sqlite(":memory:") $(db) {
        let cfg = BuildConfig(claim_timeout_seconds = 600, max_build_attempts = 2)
        enqueue_build(db, H1, TC, BUILD_MODE_MODULE, 1000l)
        t |> run("an abandoned claim comes back to the queue") @(tt : T?) {
            let c1 = claim_next_build(db, cfg, TC, 1000l)
            tt |> equal(c1.attempts, 1)
            // 601s later the claim is stale: swept back and immediately reclaimed
            let c2 = claim_next_build(db, cfg, TC, 1601l)
            tt |> equal(c2.hash, H1)
            tt |> equal(c2.attempts, 2)
        }
        t |> run("attempts exhausted fails the job") @(tt : T?) {
            let c3 = claim_next_build(db, cfg, TC, 3202l)
            tt |> equal(c3.hash, "")
            let st = build_status(db, H1, TC)
            tt |> equal(st.state, BUILD_STATE_FAILED)
            tt |> success(find(st.error, "timed out") >= 0, "timeout recorded")
        }
        t |> run("a live claim is not swept") @(tt : T?) {
            enqueue_build(db, H2, TC, BUILD_MODE_MODULE, 4000l)
            claim_next_build(db, cfg, TC, 4000l)
            let c = claim_next_build(db, cfg, TC, 4100l)   // inside the timeout window
            tt |> equal(c.hash, "")
            tt |> equal(build_status(db, H2, TC).state, BUILD_STATE_BUILDING)
        }
    }
}

[test]
def test_queue_positions(t : T?) {
    with_latest_sqlite(":memory:") $(db) {
        enqueue_build(db, H1, TC, BUILD_MODE_MODULE, 1000l)
        enqueue_build(db, H2, TC, BUILD_MODE_MODULE, 1001l)
        enqueue_build(db, H3, TC, BUILD_MODE_MODULE, 1002l)
        t |> run("queued jobs report 1-based positions") @(tt : T?) {
            tt |> equal(build_status(db, H1, TC).position, 1)
            tt |> equal(build_status(db, H2, TC).position, 2)
            tt |> equal(build_status(db, H3, TC).position, 3)
        }
        t |> run("positions shift as the queue drains") @(tt : T?) {
            claim_next_build(db, test_cfg(), TC, 1003l)
            tt |> equal(build_status(db, H2, TC).position, 1)
            tt |> equal(build_status(db, H3, TC).position, 2)
            tt |> equal(build_status(db, H1, TC).position, 0)   // building, not queued
        }
        t |> run("unknown job is not found") @(tt : T?) {
            let st = build_status(db, "4444444444444444444444444444444444444444444444444444444444444444", TC)
            tt |> success(!st.found, "not found")
        }
    }
}

options gen2

require dastest/testing_boost public
require samples_store
require build_queue
require daslib/sql_linq
require strings

// samples_store is required alongside build_queue so the migration
// registry carries the full migration stream, the way every real
// program that opens samples.db loads it.

def private test_cfg : BuildConfig =>
    BuildConfig(claim_timeout_seconds = 600, max_build_attempts = 3)

[test(description = "a failed fault waits five minutes by default before a request re-queues it")]
def test_fault_retry_default(t : T?) {
    t |> equal(BuildConfig().fault_retry_seconds, 300)
}

let H1 = "1111111111111111111111111111111111111111111111111111111111111111"
let H2 = "2222222222222222222222222222222222222222222222222222222222222222"
let H3 = "3333333333333333333333333333333333333333333333333333333333333333"
let H4 = "4444444444444444444444444444444444444444444444444444444444444444"
let H5 = "5555555555555555555555555555555555555555555555555555555555555555"
let H6 = "6666666666666666666666666666666666666666666666666666666666666666"
let TC = "abc1234def5678900000000000000000000000000"

[test(description = "a fresh samples database migrates to schema v5, and a failure stored before the Fault column is marked a fault by the text only the build service writes")]
def test_migration_v5(t : T?) {
    t |> run("fresh db migrates to v5") @(t : T?) {
        with_latest_sqlite(":memory:") $(db) {
            t |> equal(current_schema_version(db), 5)
        }
    }
    t |> run("stored build-service faults are marked, a compiler error is not") @(tt : T?) {
        with_latest_sqlite(":memory:") $(db) {
            let cfg = BuildConfig(claim_timeout_seconds = 600, max_build_attempts = 1)
            enqueue_build(db, H1, TC, BUILD_MODE_MODULE, 1000l)
            claim_next_build(db, cfg, TC, 1001l)
            record_build_failed(db, H1, TC, "the builder could not publish this build's artifact",
                                1002l)
            enqueue_build(db, H2, TC, BUILD_MODE_MODULE, 1003l)
            claim_next_build(db, cfg, TC, 1004l)
            record_build_failed(db, H2, TC, "error[30101]: nope", 1005l)
            enqueue_build(db, H3, TC, BUILD_MODE_MODULE, 1006l)
            claim_next_build(db, cfg, TC, 1007l)
            record_build_failed(db, H3, TC, "builder could not allocate scratch space", 1008l)
            enqueue_build(db, H4, TC, BUILD_MODE_MODULE, 1009l)
            claim_next_build(db, cfg, TC, 1010l)
            record_build_failed(db, H4, TC, "build produced no artifacts\nsome build output", 1011l)
            enqueue_build(db, H5, TC, BUILD_MODE_MODULE, 1012l)
            claim_next_build(db, cfg, TC, 1013l)
            record_build_failed(db, H5, TC, "build timed out after 3 attempt(s)", 1014l)
            enqueue_build(db, H6, TC, BUILD_MODE_MODULE, 1015l)
            claim_next_build(db, cfg, TC, 1016l)
            record_build_failed(db, H6, TC, "sandbox image 'dasweb-builder:2' is missing", 1017l)
            tt |> equal(mark_stored_faults(db), 5)
            tt |> success(build_status(db, H5, TC).fault, "an abandoned build is a fault")
            tt |> success(build_status(db, H6, TC).fault, "a recipe check's message is a fault")
            tt |> success(build_status(db, H1, TC).fault, "a publish failure is a fault")
            tt |> success(!build_status(db, H2, TC).fault, "a compiler error is not")
            tt |> success(build_status(db, H3, TC).fault, "a scratch failure is a fault")
            tt |> success(build_status(db, H4, TC).fault, "a build with no artifact is a fault")
            tt |> equal(mark_stored_faults(db), 0)
        }
    }
}

[test(description = "module mode expects exactly one named artifact, page mode the html+js+wasm triple, and an undeclared mode nothing")]
def test_expected_artifact_names(t : T?) {
    // The server enforces this set rather than trusting the builder, so it is
    // a store behaviour and gets a store test, not only an HTTP one.
    t |> run("module publishes exactly one named artifact") @(tt : T?) {
        let names <- expected_artifact_names(BUILD_MODE_MODULE)
        tt |> equal(length(names), 1)
        tt |> equal(names[0], "sample.wasm")
    }
    t |> run("page publishes exactly the html+js+wasm triple") @(tt : T?) {
        let names <- expected_artifact_names(BUILD_MODE_PAGE)
        tt |> equal(length(names), 3)
        tt |> equal(names[0], "sample.html")
        tt |> equal(names[1], "sample.js")
        tt |> equal(names[2], "sample.wasm")
    }
    t |> run("a mode with no declared set publishes nothing") @(tt : T?) {
        tt |> success(empty(expected_artifact_names("bogus")), "unknown mode")
    }
}

[test(description = "a native graphics or audio require in any file of a source or bundle forces a page build, while plain compute code and lookalike names stay module builds")]
def test_detect_build_mode(t : T?) {
    t |> run("plain compute source is a module build") @(tt : T?) {
        tt |> equal(detect_build_mode("require math\n[export]\ndef main \{\n    print(\"hi\")\n\}\n"), BUILD_MODE_MODULE)
        tt |> equal(detect_build_mode("require daslib/random\nrequire strings\n"),
                    BUILD_MODE_MODULE)
    }
    t |> run("a native graphics/audio require forces a page build") @(tt : T?) {
        tt |> equal(detect_build_mode("require glfw/glfw_boost\nrequire math\n"), BUILD_MODE_PAGE)
        tt |> equal(detect_build_mode("require opengl/opengl_boost\n"), BUILD_MODE_PAGE)
        tt |> equal(detect_build_mode("require audio/audio_boost\n"), BUILD_MODE_PAGE)
        tt |> equal(detect_build_mode("require stbimage\n"), BUILD_MODE_PAGE)
        tt |> equal(detect_build_mode("require strudel/strudel_player\n"), BUILD_MODE_PAGE)
        tt |> equal(detect_build_mode("require live/glfw_live\n"), BUILD_MODE_PAGE)
        tt |> equal(detect_build_mode("require live_host\n"), BUILD_MODE_PAGE)
    }
    t |> run("optional-require marker and tabs still match") @(tt : T?) {
        tt |> equal(detect_build_mode("require ?audio\n"), BUILD_MODE_PAGE)
        tt |> equal(detect_build_mode("require\tglfw/glfw_boost\n"), BUILD_MODE_PAGE)
    }
    t |> run("lookalikes stay module builds") @(tt : T?) {
        tt |> equal(detect_build_mode("require live_stub   // the retired shim name\n"),
                    BUILD_MODE_MODULE)
        tt |> equal(detect_build_mode("require daslib/glfw_helpers.das\n"), BUILD_MODE_MODULE)   // daslib root, not glfw
        tt |> equal(detect_build_mode("requirements = 1\n"), BUILD_MODE_MODULE)
        tt |> equal(detect_build_mode("// require glfw\n"), BUILD_MODE_MODULE)
        tt |> equal(detect_build_mode("require glfwish\n"), BUILD_MODE_MODULE)
    }
    t |> run("bundle documents scan every file") @(tt : T?) {
        let page_bundle = "\{\"files\":\{\"main.das\":\"require helper\\n\",\"helper.das\":\"require opengl/opengl_boost\\n\"\},\"active\":\"main.das\"\}"
        tt |> equal(detect_build_mode(page_bundle), BUILD_MODE_PAGE)
        let mod_bundle = "\{\"files\":\{\"main.das\":\"require math\\n\"\},\"active\":\"main.das\"\}"
        tt |> equal(detect_build_mode(mod_bundle), BUILD_MODE_MODULE)
    }
}

[test(description = "there is no toolchain until one is announced, and a re-announce overwrites it")]
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

[test(description = "enqueueing a hash twice makes one queued job, while the same hash under another toolchain is its own job")]
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
            let r = enqueue_build(db, H1, "feed5678deadbeef000000000000000000000000",
                                  BUILD_MODE_MODULE, 1002l)
            tt |> success(r.created, "created")
        }
    }
}

[test(description = "a claim takes the oldest queued job of its own toolchain with its mode, and an empty queue claims nothing")]
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
            let c = claim_next_build(db, test_cfg(), "feed5678deadbeef000000000000000000000000",
                                     1003l)
            tt |> equal(c.hash, "")
        }
        t |> run("next claim takes the next job, carrying its mode") @(tt : T?) {
            let c = claim_next_build(db, test_cfg(), TC, 1004l)
            tt |> equal(c.hash, H2)
            tt |> equal(c.mode, BUILD_MODE_PAGE)
        }
    }
}

[test(description = "only a building job accepts a result, which resolves it to done with its artifact, and a second result is refused")]
def test_done_flow(t : T?) {
    with_latest_sqlite(":memory:") $(db) {
        enqueue_build(db, H1, TC, BUILD_MODE_MODULE, 1000l)
        t |> run("a result for an unclaimed job is refused") @(tt : T?) {
            tt |> success(!record_build_done(db, H1, TC, "x/y", "[]", 1001l),
                          "refused before claim")
        }
        claim_next_build(db, test_cfg(), TC, 1002l)
        t |> run("a building job resolves to done with its artifact") @(tt : T?) {
            tt |> success(record_build_done(db, H1, TC, "{TC}/{H1}", "[\"a.wasm\"]", 1003l),
                          "recorded")
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

[test(description = "a failure stores the compiler error, a forced re-enqueue resets the job, and a plain re-request leaves it failed")]
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

[test(description = "a build-service fault returns the job to the queue while it has attempts left and fails it as a fault after that, a request re-queues that failure only once the retry wait has passed, and a refused fault changes nothing")]
def test_fault_flow(t : T?) {
    with_latest_sqlite(":memory:") $(db) {
        let cfg = BuildConfig(claim_timeout_seconds = 600, max_build_attempts = 2,
                              fault_retry_seconds = 300)
        enqueue_build(db, H1, TC, BUILD_MODE_MODULE, 1000l)
        t |> run("a fault for an unclaimed job is refused") @(tt : T?) {
            tt |> equal(record_build_fault(db, cfg, H1, TC, "could not publish", 1000l), "")
            tt |> equal(build_status(db, H1, TC).state, BUILD_STATE_QUEUED)
        }
        t |> run("the first fault returns the job to the queue") @(tt : T?) {
            claim_next_build(db, cfg, TC, 1001l)
            tt |> equal(record_build_fault(db, cfg, H1, TC, "could not publish", 1002l),
                        BUILD_STATE_QUEUED)
            let st = build_status(db, H1, TC)
            tt |> equal(st.state, BUILD_STATE_QUEUED)
            tt |> equal(st.attempts, 1)
            tt |> success(!st.fault, "not a stored failure yet")
        }
        t |> run("a fault on the last attempt fails the job as a fault") @(tt : T?) {
            tt |> equal(claim_next_build(db, cfg, TC, 1003l).attempts, 2)
            tt |> equal(record_build_fault(db, cfg, H1, TC, "could not publish", 1004l),
                        BUILD_STATE_FAILED)
            let st = build_status(db, H1, TC)
            tt |> equal(st.state, BUILD_STATE_FAILED)
            tt |> equal(st.error, "could not publish")
            tt |> success(st.fault, "marked a fault")
        }
        t |> run("a request inside the retry wait leaves the failure") @(tt : T?) {
            let r = request_build(db, cfg, H1, TC, BUILD_MODE_MODULE, 1303l)
            tt |> success(!r.created, "not re-queued")
            tt |> equal(r.state, BUILD_STATE_FAILED)
        }
        t |> run("a request after the retry wait re-queues it with fresh attempts") @(tt : T?) {
            let r = request_build(db, cfg, H1, TC, BUILD_MODE_MODULE, 1304l)
            tt |> success(r.created, "re-queued")
            let st = build_status(db, H1, TC)
            tt |> equal(st.state, BUILD_STATE_QUEUED)
            tt |> equal(st.attempts, 0)
            tt |> equal(st.error, "")
            tt |> success(!st.fault, "fault mark cleared")
        }
        t |> run("a request never re-queues a failure that is the sample's own") @(tt : T?) {
            enqueue_build(db, H2, TC, BUILD_MODE_MODULE, 900l)
            tt |> equal(claim_next_build(db, cfg, TC, 2000l).hash, H2)
            record_build_failed(db, H2, TC, "error[30101]: nope", 2001l)
            let r = request_build(db, cfg, H2, TC, BUILD_MODE_MODULE, 999999l)
            tt |> success(!r.created, "not re-queued")
            tt |> equal(r.state, BUILD_STATE_FAILED)
        }
        t |> run("a request for a job that does not exist enqueues it") @(tt : T?) {
            tt |> success(request_build(db, cfg, H3, TC, BUILD_MODE_MODULE, 3000l).created,
                          "enqueued")
        }
    }
}

[test(description = "the operator's requeue returns a failed build under the current toolchain to the queue with fresh attempts, leaves a job in another state as it is, and reports a sample with no job")]
def test_requeue_failed_build(t : T?) {
    with_latest_sqlite(":memory:") $(db) {
        t |> run("no toolchain announced yet: nothing to requeue") @(tt : T?) {
            tt |> success(!requeue_failed_build(db, H1, 1000l), "no job")
        }
        set_current_toolchain(db, TC)
        t |> run("a sample with no job under the current toolchain") @(tt : T?) {
            tt |> success(!requeue_failed_build(db, H1, 1000l), "no job")
        }
        enqueue_build(db, H1, TC, BUILD_MODE_PAGE, 1000l)
        t |> run("a queued job stays as it is") @(tt : T?) {
            tt |> success(requeue_failed_build(db, H1, 1001l), "job found")
            tt |> equal(build_status(db, H1, TC).state, BUILD_STATE_QUEUED)
        }
        t |> run("a failed job returns to the queue with fresh attempts and its mode") @(tt : T?) {
            claim_next_build(db, test_cfg(), TC, 1002l)
            record_build_failed(db, H1, TC, "error[30101]: nope", 1003l)
            tt |> success(requeue_failed_build(db, H1, 1004l), "job found")
            let st = build_status(db, H1, TC)
            tt |> equal(st.state, BUILD_STATE_QUEUED)
            tt |> equal(st.attempts, 0)
            tt |> equal(st.error, "")
            tt |> equal(st.mode, BUILD_MODE_PAGE)
        }
    }
}

[test(description = "an oversized fault text is stored truncated with a marker, like a compiler error")]
def test_fault_text_is_capped(t : T?) {
    with_latest_sqlite(":memory:") $(db) {
        let last_attempt = BuildConfig(claim_timeout_seconds = 600, max_build_attempts = 1)
        enqueue_build(db, H1, TC, BUILD_MODE_MODULE, 1000l)
        claim_next_build(db, last_attempt, TC, 1001l)
        let huge = repeat("0123456789012345678901234567890123456789012345678901234567890123", 1200)
        t |> equal(record_build_fault(db, last_attempt, H1, TC, huge, 1002l), BUILD_STATE_FAILED)
        let st = build_status(db, H1, TC)
        t |> success(length(st.error) < length(huge), "shorter than input")
        t |> success(find(st.error, "truncated") >= 0, "marked truncated")
    }
}

[test(description = "an oversized compiler error is stored truncated with a marker")]
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

[test(description = "an abandoned claim returns to the queue until its attempts run out and the job fails, while a live claim is not swept")]
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
            tt |> success(st.fault, "an abandoned build is a build-service fault")
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

[test(description = "queued jobs report 1-based positions that shift as the queue drains, and an unknown job is not found")]
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

def private no_trees : array<string> {
    var none : array<string>
    return <- none
}

[test(description = "only stale-toolchain rows queued before the cutoff and with no tree left are purged, the current toolchain keeps its rows, and a purged sample re-enqueues fresh")]
def test_forget_stale_toolchain_jobs(t : T?) {
    with_latest_sqlite(":memory:") $(db) {
        let stale_tc = "feed5678deadbeef000000000000000000000000"
        let h2 = "2222222222222222222222222222222222222222222222222222222222222222"
        enqueue_build(db, H1, TC, BUILD_MODE_MODULE, 1000l)
        enqueue_build(db, H1, stale_tc, BUILD_MODE_MODULE, 1000l)
        enqueue_build(db, h2, stale_tc, BUILD_MODE_MODULE, 2000l)
        t |> run("no announced toolchain purges nothing") @(tt : T?) {
            tt |> equal(forget_stale_toolchain_jobs(db, "", no_trees(), 5000l), 0)
        }
        t |> run("a stale toolchain whose tree still stands keeps its rows") @(tt : T?) {
            tt |> equal(forget_stale_toolchain_jobs(db, TC, [stale_tc], 5000l), 0)
        }
        t |> run("without a tree, only stale rows queued before the cutoff go") @(tt : T?) {
            tt |> equal(forget_stale_toolchain_jobs(db, TC, no_trees(), 1500l), 1)
            tt |> equal(forget_stale_toolchain_jobs(db, TC, no_trees(), 1500l), 0)
            tt |> success(!enqueue_build(db, h2, stale_tc, BUILD_MODE_MODULE, 2500l).created,
                          "the newer stale row is still there")
            tt |> equal(forget_stale_toolchain_jobs(db, TC, no_trees(), 3000l), 1)
        }
        t |> run("the current toolchain keeps its rows; a purged sample re-enqueues fresh") @(tt : T?) {
            tt |> success(!enqueue_build(db, H1, TC, BUILD_MODE_MODULE, 4000l).created,
                          "current toolchain's job survives")
            tt |> success(enqueue_build(db, H1, stale_tc, BUILD_MODE_MODULE, 4001l).created,
                          "re-enqueues fresh")
        }
    }
}

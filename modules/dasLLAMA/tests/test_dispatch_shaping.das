options gen2
options _dasllama_internal = true

require dastest/testing_boost public
require dasllama/dasllama_math
require daslib/jobque_boost
require math

// Work-proportional dispatch shaping: lanes_for_work boundaries + per-regime caps, and the
// 3-arg matmul_chunks / matmul_chunks_gemv shapers (the inline signal, wave alignment, grain
// floors). Pure shaping math — no kernels, no model. The shapers read get_dispatch_lanes(),
// so every case runs inside with_job_que; knobs are restored after each case.

// pin the FIXTURE baseline every arm assumes (1M target, equal split) — deliberately NOT the
// shipped defaults (target 1, fine 8); the fine-grain arm covers those explicitly
def private pin_shaping_baseline() {
    set_target_chunk_work(1_000_000l)
    set_gemv_lane_cap(0)
    set_batch_lane_cap(0)
    set_matmul_min_chunk_rows(16)
    set_matmul_min_chunk_rows_gemv(64)
    set_gemv_chunks_per_lane(1)
    set_gemv_wave_fill(true)
    set_batch_grid_2d(0)
}

[test]
def test_lanes_for_work(t : T?) {
    with_job_que() {
        pin_shaping_baseline()
        let lanes = get_dispatch_lanes()
        t |> run("boundary: 1 lane below 2x target (the old 2M inline boundary)") @(t : T?) {
            t |> equal(lanes_for_work(1l, 0), 1)
            t |> equal(lanes_for_work(1_999_999l, 0), 1, "just under 2x target stays inline")
            t |> equal(lanes_for_work(2_000_000l, 0), min(2, lanes), "2x target = first parallel work")
            t |> equal(lanes_for_work(5_500_000l, 0), min(5, lanes), "work/target lanes in the mid band")
        }
        t |> run("huge work saturates to full dispatch lanes (structural guard)") @(t : T?) {
            t |> equal(lanes_for_work(1_000_000_000_000l, 0), lanes, ">=1e9-scale work invites every lane")
        }
        t |> run("caps clamp; 0 = uncapped; cap beyond lanes is inert") @(t : T?) {
            t |> equal(lanes_for_work(1_000_000_000_000l, 1), 1, "cap 1 forces the inline signal")
            t |> equal(lanes_for_work(1_000_000_000_000l, 2), min(2, lanes))
            t |> equal(lanes_for_work(1_000_000_000_000l, lanes + 100), lanes, "cap above lanes clamps to lanes")
        }
        t |> run("worker limit shrinks the live pool the shapers see") @(t : T?) {
            let jobs = get_total_hw_jobs()
            if (jobs >= 2) {
                set_jobque_worker_limit(1)
                t |> equal(get_dispatch_lanes(), 2, "1 live worker + the caller lane")
                t |> equal(lanes_for_work(1_000_000_000_000l, 0), 2, "huge work saturates to the LIVE pool")
                set_jobque_worker_limit(-1)
                t |> equal(get_dispatch_lanes(), lanes, "unlimited restores the full pool")
            }
        }
        t |> run("target knob moves the boundary; extremes force always-inline / always-parallel") @(t : T?) {
            set_target_chunk_work(1_000l)
            t |> equal(lanes_for_work(2_000l, 0), min(2, lanes), "smaller target parallelizes earlier")
            set_target_chunk_work(1l << 62l)
            t |> equal(lanes_for_work(1_000_000_000_000l, 0), 1, "huge target = never parallel")
            set_target_chunk_work(1l)
            t |> equal(lanes_for_work(int64(lanes) * 10l, 0), lanes, "target 1 = full lanes for any real matmul")
            pin_shaping_baseline()
        }
    }
}

[test]
def test_matmul_chunks_batch(t : T?) {
    with_job_que() {
        pin_shaping_baseline()
        let lanes = get_dispatch_lanes()
        let huge = 1_000_000_000_000l
        t |> run("sub-2x-target work returns the inline signal") @(t : T?) {
            t |> equal(matmul_chunks(4096, 4, 1_999_999l), 1)
        }
        t |> run("huge work: whole L-waves under the grain cap, oversplit waves wanted") @(t : T?) {
            // rows admit exactly 2 grain-waves: cap = rows/16 = 2*lanes < want = 4*lanes
            let chunks = matmul_chunks(16 * lanes * 2, 4, huge)
            t |> equal(chunks, 2 * lanes, "grain caps the wave count; result stays wave-aligned")
            // roomy rows: the full oversplit want survives
            t |> equal(matmul_chunks(16 * lanes * 8, 4, huge), 4 * lanes, "want = L x oversplit when grain allows")
        }
        t |> run("sub-grain rows still fill the admitted lanes (never underfill L)") @(t : T?) {
            t |> equal(matmul_chunks(8, 4, huge), min(8, lanes), "1-row chunks; rows < L is the only natural floor")
            t |> equal(matmul_chunks(16 * (lanes - 1), 4, huge), lanes, "grain-capped below L rounds up to one full wave")
        }
        t |> run("grain off = raw L x oversplit; batch cap bounds L") @(t : T?) {
            set_matmul_min_chunk_rows(0)
            t |> equal(matmul_chunks(8, 4, huge), lanes * 4, "grain disabled returns the raw want")
            pin_shaping_baseline()
            set_batch_lane_cap(2)
            let capped = matmul_chunks(16 * lanes * 8, 4, huge)
            t |> equal(capped, min(2, lanes) * 4, "cap bounds the wave width")
            pin_shaping_baseline()
        }
    }
}

[test]
def test_matmul_grid(t : T?) {
    with_job_que() {
        pin_shaping_baseline()
        let lanes = get_dispatch_lanes()
        let huge = 1_000_000_000_000l
        // the starved fixture: mr=4 units sized so the grain caps 1-D at lanes-1 chunks (< L)
        let sunits = 4 * (lanes - 1)
        t |> run("knob off (default): 1-D always, x = matmul_chunks") @(t : T?) {
            if (lanes >= 3) {
                let g = matmul_grid(sunits, 4l, 4, huge, 2048l, 4)
                t |> equal(g.x, lanes, "grain-starved shape wave-fills the 1-D count")
                t |> equal(g.y, 1)
            }
            t |> equal(matmul_grid(4 * lanes * 8, 4l, 4, huge, 2048l, 4).y, 1, "roomy shape 1-D")
        }
        t |> run("knob 1 (fine grid): starved shape goes 2-D at 16-token cells") @(t : T?) {
            set_batch_grid_2d(1)
            if (lanes >= 3) {
                let g = matmul_grid(sunits, 4l, 4, huge, 2048l, 4)
                t |> equal(g.x, lanes - 1, "row resolution = the 1-D grain cap")
                t |> equal(g.y, 128, "2048 tokens / 16-token cells")
                t |> equal(matmul_grid(sunits, 4l, 4, huge, 20l, 4).y, 2, "ntc = ceil(ntok/16), tile-clamped")
                t |> equal(matmul_grid(sunits, 4l, 4, huge, 4l, 4).y, 1, "no token axis (ntok < 2 tiles) stays 1-D")
            }
            t |> equal(matmul_grid(4 * lanes * 8, 4l, 4, huge, 2048l, 4).y, 1, "unstarved shape stays 1-D")
            t |> equal(matmul_grid(4 * lanes * 8, 4l, 4, 1_999_999l, 2048l, 4).y, 1, "inline signal stays 1-D")
            pin_shaping_baseline()
        }
        t |> run("knob 2 (wave-aligned): rc*ntc is whole L-waves, or falls back 1-D") @(t : T?) {
            set_batch_grid_2d(2)
            if (lanes >= 3) {
                let g = matmul_grid(sunits, 4l, 4, huge, 2048l, 4)
                t |> equal(g.x, lanes - 1)
                t |> success(g.y > 1, "starved shape with plenty of tokens goes 2-D")
                t |> equal((g.x * g.y) % lanes, 0, "the grid total is a whole number of L-waves")
                t |> success(g.x * g.y >= lanes * 4, "the grid covers the oversplit target")
                t |> equal(matmul_grid(sunits, 4l, 4, huge, 16l, 4).y, 1, "alignment that can't fit the tiles stays 1-D")
            }
            pin_shaping_baseline()
        }
        t |> run("row cells clamp to whole units; knob value clamps to [0,2]") @(t : T?) {
            set_batch_grid_2d(1)
            if (lanes > 4) {
                let g = matmul_grid(2, 32l, 4, huge, 2048l, 4)   // 64 rows = 4 grain chunks, but only 2 units
                t |> equal(g.x, 2, "rc never exceeds the unit count")
                t |> success(g.y > 1)
            }
            set_batch_grid_2d(7)
            t |> equal(get_batch_grid_2d(), 2)
            set_batch_grid_2d(-3)
            t |> equal(get_batch_grid_2d(), 0)
            pin_shaping_baseline()
        }
    }
}

[test]
def test_matmul_chunks_gemv(t : T?) {
    with_job_que() {
        pin_shaping_baseline()
        let lanes = get_dispatch_lanes()
        let huge = 1_000_000_000_000l
        t |> run("sub-2x-target work returns the inline signal") @(t : T?) {
            t |> equal(matmul_chunks_gemv(4096, 1, 1_999_999l), 1)
        }
        t |> run("huge work: one chunk per lane once rows clear the GEMV grain") @(t : T?) {
            t |> equal(matmul_chunks_gemv(64 * lanes, 1, huge), lanes)
            t |> equal(matmul_chunks_gemv(64 * lanes * 4, 8, huge), lanes, "oversplit never exceeds one chunk per lane")
        }
        t |> run("wave fill: grain-capped counts below L round up to one full wave (never underfill)") @(t : T?) {
            if (lanes > 2) {
                t |> equal(matmul_chunks_gemv(64, 1, huge), min(64, lanes), "sub-grain chunks fill the admitted lanes")
            }
            if (lanes >= 2) {
                t |> equal(matmul_chunks_gemv(64 * (lanes - 1), 1, huge), lanes, "one grain short of a wave rounds up to a full wave")
                t |> equal(matmul_chunks_gemv(128, 1, huge), min(128, lanes), "the 30B decode router shape (128 rows) fills every lane")
            }
            set_gemv_wave_fill(false)
            if (lanes >= 2) {
                t |> equal(matmul_chunks_gemv(64 * (lanes - 1), 1, huge), lanes - 1, "wave fill off keeps the grain count")
            }
            pin_shaping_baseline()
        }
        t |> run("gemv cap bounds the lanes; grain off = raw L x oversplit") @(t : T?) {
            set_gemv_lane_cap(2)
            t |> equal(matmul_chunks_gemv(64 * lanes, 1, huge), min(2, lanes))
            pin_shaping_baseline()
            set_matmul_min_chunk_rows_gemv(0)
            t |> equal(matmul_chunks_gemv(16, 3, huge), lanes * 3, "grain disabled returns the raw want")
            pin_shaping_baseline()
        }
        t |> run("fine-grain mode (the shipped default): whole grain waves, capped at the fine count") @(t : T?) {
            set_gemv_chunks_per_lane(8)
            if (lanes >= 2) {
                t |> equal(matmul_chunks_gemv(64 * lanes * 4, 8, huge), lanes * 4, "whole 64-row waves; the oversplit arg is inert in fine mode")
                t |> equal(matmul_chunks_gemv(64 * lanes * 100, 1, huge), lanes * 8, "wave count caps at gemv_chunks_per_lane")
            }
            if (lanes > 2) {
                t |> equal(matmul_chunks_gemv(32, 1, huge), min(32, lanes), "sub-wave rows fall through to the wave-filled split")
            }
            pin_shaping_baseline()
        }
    }
}

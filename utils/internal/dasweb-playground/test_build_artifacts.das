options gen2

require dastest/testing_boost public
require build_artifacts
require daslib/sha_256
require daslib/fio
require strings

let TC = "abc1234def5678900000000000000000000000000"
let H1 = "1111111111111111111111111111111111111111111111111111111111111111"

def private with_temp_blobs(blk : block<(blobs : string) : void>) {
    var err = ""
    let blobs = create_temp_directory("dasweb_blobs_", err)
    assert(!empty(blobs), "temp blobs dir created")
    invoke(blk, blobs)
    rmdir_rec(blobs)
}

[test]
def test_filename_validation(t : T?) {
    t |> run("known delivery names pass") @(tt : T?) {
        tt |> success(is_valid_artifact_filename("sample.wasm"), "wasm")
        tt |> success(is_valid_artifact_filename("index.html"), "html")
        tt |> success(is_valid_artifact_filename("arcanoid.js"), "js")
        tt |> success(is_valid_artifact_filename("assets.data"), "data")
        tt |> success(is_valid_artifact_filename("style.css"), "css")
        tt |> success(is_valid_artifact_filename("sample.wasm.br"), "br sidecar")
        tt |> success(is_valid_artifact_filename("a-b_c.9.wasm"), "boring charset")
    }
    t |> run("everything else is refused") @(tt : T?) {
        tt |> success(!is_valid_artifact_filename(""), "empty")
        tt |> success(!is_valid_artifact_filename("x.exe"), "unknown suffix")
        tt |> success(!is_valid_artifact_filename("x.br"), "bare br")
        tt |> success(!is_valid_artifact_filename(".hidden.wasm"), "leading dot")
        tt |> success(!is_valid_artifact_filename("a..b.wasm"), "double dot")
        tt |> success(!is_valid_artifact_filename("a/b.wasm"), "separator")
        tt |> success(!is_valid_artifact_filename("a\\b.wasm"), "backslash")
        tt |> success(!is_valid_artifact_filename("a b.wasm"), "space")
        tt |> success(!is_valid_artifact_filename("wasm"), "no dot at all")
        let long_name = repeat("aaaaaaaaaa", 13)
        tt |> success(!is_valid_artifact_filename(long_name + ".wasm"), "over length cap")
    }
}

[test]
def test_component_validation(t : T?) {
    t |> run("toolchain ids are 7..64 lowercase hex") @(tt : T?) {
        tt |> success(is_valid_toolchain_id("abc1234"), "short sha")
        tt |> success(is_valid_toolchain_id(TC), "long sha")
        tt |> success(!is_valid_toolchain_id("abc123"), "too short")
        tt |> success(!is_valid_toolchain_id("ABC1234"), "uppercase")
        tt |> success(!is_valid_toolchain_id("abc123g"), "non-hex")
        tt |> success(!is_valid_toolchain_id(""), "empty")
    }
    t |> run("artifact hashes are exactly 64 hex") @(tt : T?) {
        tt |> success(is_valid_artifact_hash(H1), "64 hex")
        tt |> success(!is_valid_artifact_hash(TC), "41 chars")
        tt |> success(!is_valid_artifact_hash(H1 + "1"), "65 chars")
    }
}

[test]
def test_finalize_roundtrip(t : T?) {
    with_temp_blobs() $(blobs) {
        t |> run("staged files with good shas land atomically") @(tt : T?) {
            let stage = create_stage_dir(blobs)
            tt |> success(!empty(stage), "stage created")
            fwrite(path_join(stage, "sample.wasm"), "fake wasm bytes")
            fwrite(path_join(stage, "index.html"), "<html></html>")
            let files <- [
                ArtifactFile(name = "sample.wasm", sha256 = sha256_hex("fake wasm bytes")),
                ArtifactFile(name = "index.html", sha256 = sha256_hex("<html></html>"))
            ]
            tt |> success(finalize_artifact(blobs, TC, H1, stage, files), "finalized")
            tt |> success(!stat(stage).is_valid, "stage consumed")
            let served = resolve_artifact_file(blobs, TC, H1, "sample.wasm")
            tt |> success(!empty(served), "artifact resolvable")
            tt |> equal(fread(served), "fake wasm bytes")
        }
        t |> run("an existing artifact absorbs a re-upload") @(tt : T?) {
            let stage = create_stage_dir(blobs)
            fwrite(path_join(stage, "sample.wasm"), "different bytes")
            let files <- [ArtifactFile(name = "sample.wasm", sha256 = sha256_hex("different bytes"))]
            tt |> success(finalize_artifact(blobs, TC, H1, stage, files), "accepted")
            tt |> success(!stat(stage).is_valid, "stage consumed")
            // content-addressed: the original content stays
            tt |> equal(fread(resolve_artifact_file(blobs, TC, H1, "sample.wasm")), "fake wasm bytes")
        }
    }
}

[test]
def test_finalize_refusals(t : T?) {
    with_temp_blobs() $(blobs) {
        t |> run("wrong sha keeps the artifact out") @(tt : T?) {
            let stage = create_stage_dir(blobs)
            fwrite(path_join(stage, "sample.wasm"), "payload")
            let files <- [ArtifactFile(name = "sample.wasm", sha256 = sha256_hex("not the payload"))]
            tt |> success(!finalize_artifact(blobs, TC, H1, stage, files), "refused")
            tt |> success(!stat(stage).is_valid, "stage cleaned up")
            tt |> equal(resolve_artifact_file(blobs, TC, H1, "sample.wasm"), "")
        }
        t |> run("missing staged file refuses the set") @(tt : T?) {
            let stage = create_stage_dir(blobs)
            fwrite(path_join(stage, "sample.wasm"), "payload")
            let files <- [
                ArtifactFile(name = "sample.wasm", sha256 = sha256_hex("payload")),
                ArtifactFile(name = "index.html", sha256 = sha256_hex("x"))
            ]
            tt |> success(!finalize_artifact(blobs, TC, H1, stage, files), "refused")
        }
        t |> run("empty manifest refuses") @(tt : T?) {
            let stage = create_stage_dir(blobs)
            let files : array<ArtifactFile>
            tt |> success(!finalize_artifact(blobs, TC, H1, stage, files), "refused")
        }
    }
}

[test]
def test_resolve_is_the_choke_point(t : T?) {
    with_temp_blobs() $(blobs) {
        let stage = create_stage_dir(blobs)
        fwrite(path_join(stage, "sample.wasm"), "bytes")
        let files <- [ArtifactFile(name = "sample.wasm", sha256 = sha256_hex("bytes"))]
        finalize_artifact(blobs, TC, H1, stage, files)
        t |> run("bad components never resolve") @(tt : T?) {
            tt |> equal(resolve_artifact_file(blobs, TC, H1, "other.wasm"), "")
            tt |> equal(resolve_artifact_file(blobs, TC, H1, "x.exe"), "")
            tt |> equal(resolve_artifact_file(blobs, "ABC1234", H1, "sample.wasm"), "")
            tt |> equal(resolve_artifact_file(blobs, TC, "deadbeef", "sample.wasm"), "")
            tt |> equal(resolve_artifact_file("", TC, H1, "sample.wasm"), "")
        }
    }
}

[test]
def test_stage_cleanup(t : T?) {
    with_temp_blobs() $(blobs) {
        t |> run("startup sweep removes leftover stages") @(tt : T?) {
            let stage = create_stage_dir(blobs)
            fwrite(path_join(stage, "sample.wasm"), "orphaned upload")
            clear_stage_dirs(blobs)
            tt |> success(!stat(stage).is_valid, "stage swept")
            // sweeping must not touch placed artifacts
            let stage2 = create_stage_dir(blobs)
            tt |> success(!empty(stage2), "staging still works after sweep")
        }
    }
}

def private place_artifact(blobs : string; toolchain : string; hash : string) {
    let stage = create_stage_dir(blobs)
    fwrite(path_join(stage, "sample.wasm"), "bytes")
    let files <- [ArtifactFile(name = "sample.wasm", sha256 = sha256_hex("bytes"))]
    finalize_artifact(blobs, toolchain, hash, stage, files)
}

[test]
def test_stale_toolchain_sweep(t : T?) {
    with_temp_blobs() $(blobs) {
        let stale_tc = "feed5678deadbeef000000000000000000000000"
        place_artifact(blobs, TC, H1)
        place_artifact(blobs, stale_tc, H1)
        let stage = create_stage_dir(blobs)
        let stray_file = path_join(blobs, "0badf00d0badf00d0badf00d0badf00d0badf00d")
        fwrite(stray_file, "a file named like a toolchain id")
        let now = int64(get_clock())
        let later = now + STALE_TOOLCHAIN_GRACE_SECONDS + 1l
        t |> run("the grace period is a day") @(tt : T?) {
            tt |> equal(STALE_TOOLCHAIN_GRACE_SECONDS, 86400l)
        }
        t |> run("a stale toolchain inside the grace period stays") @(tt : T?) {
            tt |> equal(sweep_stale_toolchains(blobs, TC, now), 0)
            tt |> success(!empty(resolve_artifact_file(blobs, stale_tc, H1, "sample.wasm")), "still served")
        }
        t |> run("the trees on disk are the two toolchains, not tmp or the stray file") @(tt : T?) {
            let on_disk <- toolchains_on_disk(blobs)
            tt |> equal(length(on_disk), 2)
            tt |> success(find_index(on_disk, TC) >= 0 && find_index(on_disk, stale_tc) >= 0, "both toolchains listed")
        }
        t |> run("past the grace period it goes; the current toolchain, blobs/tmp and a stray file stay") @(tt : T?) {
            tt |> equal(sweep_stale_toolchains(blobs, TC, later), 1)
            tt |> equal(length(toolchains_on_disk(blobs)), 1)
            tt |> equal(resolve_artifact_file(blobs, stale_tc, H1, "sample.wasm"), "")
            tt |> success(!empty(resolve_artifact_file(blobs, TC, H1, "sample.wasm")), "current toolchain intact")
            tt |> success(stat(stage).is_valid, "stage root untouched")
            tt |> success(stat(stray_file).is_valid, "a plain file is not a toolchain tree")
        }
        t |> run("no announced toolchain means nothing is swept") @(tt : T?) {
            place_artifact(blobs, stale_tc, H1)
            tt |> equal(sweep_stale_toolchains(blobs, "", later), 0)
            tt |> success(!empty(resolve_artifact_file(blobs, stale_tc, H1, "sample.wasm")), "kept")
        }
    }
}

[test]
def test_toolchain_sweep_cadence(t : T?) {
    t |> run("a fresh process sweeps at once, then once per interval") @(tt : T?) {
        tt |> success(toolchain_sweep_due(0l, 1000l), "first tick is due")
        tt |> success(!toolchain_sweep_due(1000l, 1000l + TOOLCHAIN_SWEEP_INTERVAL_SECONDS - 1l), "inside the interval")
        tt |> success(toolchain_sweep_due(1000l, 1000l + TOOLCHAIN_SWEEP_INTERVAL_SECONDS), "at the interval")
    }
}

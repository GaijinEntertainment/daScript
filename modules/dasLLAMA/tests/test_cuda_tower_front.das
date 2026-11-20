options gen2
options persistent_heap
options indenting = 4
options _dasllama_internal = true

// The audio fronts' CUDA stamps (`dasllama_cuda_classes_front`) on the case bodies the Vulkan twins' cells in
// `test_vulkan_tower_kernels.das` run (`_discrete_cells.das`), one cell a stamp; then the home's kernel prewarm under a
// stand-in compiler, its kernel texts and its cache folder knob. Model-free; every cell but the kernel texts, the prewarm,
// the knob and the closing leak gate skips without a CUDA device and NVRTC, and a stamp that does not compile on a device
// reds its cell. The leak gate never skips.

require dastest/testing_boost public
require dasllama/dasllama_lint public
require dasllama/dasllama_cuda_classes_front
require _cuda_cell
require _discrete_cells
require _child_run   // the cache folder knob is read at init: a child reads it
require daslib/fio   // path_join, stat: the prewarm's folder and the cubins in it
require strings   // find: the folder line in the child's output

[test(description = "The CUDA stamp of `tower_im2col` writes the CPU k3 s2 p1 im2col columns as halves over two chunks, pad and zero tail included, and leaves the sentinel lanes past the panel untouched")]
def test_cud_tower_im2col(t0 : T?) {
    t0 |> run("tower_im2col (CUDA) == the k3 s2 p1 columns over two chunks with the pad and the zero tail, as halves") <| @(t : T?) {
        return if (!cuda_up(t) || !stamps_built(t, ensure_tower_im2col_cls(), "tower_im2col_cls"))
        with_cu_planes() $(var cp) {
            im2col_case(t, cp)
        }
    }
}

[test(description = "The CUDA stamp of `tts_im2col16` matches the CPU 1-D im2col as halves on channel- and row-major sources, strides 1 and 2, off and on the four-channel lattice, keeping the halves before and past the panel")]
def test_cud_tts_im2col16(t0 : T?) {
    t0 |> run("tts_im2col16 (CUDA) == the stem's k3 columns on both source layouts and strides, off and on the four-channel lattice, as halves") <| @(t : T?) {
        return if (!cuda_up(t) || !stamps_built(t, ensure_tts_im2col16_cls(), "tts_im2col16_cls"))
        with_cu_planes() $(var cp) {
            im2col16_case(t, cp)
        }
    }
}

[test(description = "The CUDA stamp of `tower_q3a_shuffle` reproduces the CPU qwen3a feature rows off the third conv stage as halves and writes nothing past the rows")]
def test_cud_tower_q3a_shuffle(t0 : T?) {
    t0 |> run("tower_q3a_shuffle (CUDA) == the qwen3a feature rows off the third conv stage, as halves") <| @(t : T?) {
        return if (!cuda_up(t) || !stamps_built(t, ensure_tower_q3a_shuffle_cls(), "tower_q3a_shuffle_cls"))
        with_cu_planes() $(var cp) {
            q3a_shuffle_case(t, cp)
        }
    }
}

[test(description = "The CUDA stamp of `tower_feat_shuffle` turns the time-freq-ch image into time-ch-freq rows exactly, with zero and two pad rows, the pad rows exact zeros and the lanes past untouched")]
def test_cud_tower_feat_shuffle(t0 : T?) {
    t0 |> run("tower_feat_shuffle (CUDA) == the [time][ch][freq] rows off the [time][freq][ch] image, with and without zero pad rows") <| @(t : T?) {
        return if (!cuda_up(t) || !stamps_built(t, ensure_tower_feat_shuffle_cls(), "tower_feat_shuffle_cls"))
        with_cu_planes() $(var cp) {
            feat_shuffle_case(t, cp)
        }
    }
}

[test(description = "The CUDA stamp of `tower_ln_relu` applies channel LayerNorm then ReLU in place within the approximate bar of the CPU loop, actually moves live elements, and keeps the rows past the live ones")]
def test_cud_tower_ln_relu(t0 : T?) {
    t0 |> run("tower_ln_relu (CUDA) == the channel LayerNorm then ReLU in place, against the double form") <| @(t : T?) {
        return if (!cuda_up(t) || !stamps_built(t, ensure_tower_ln_relu_cls(), "tower_ln_relu_cls"))
        with_cu_planes() $(var cp) {
            ln_relu_case(t, cp)
        }
    }
}

[test(description = "The CUDA stamp of `tower_spec_dft` matches the double-precision DFT on both the magnitude and the power arm, with a control that the two arms differ and the lanes past the bins kept")]
def test_cud_tower_spec_dft(t0 : T?) {
    t0 |> run("tower_spec_dft (CUDA) == the frames' spectra on the magnitude and the power arm, against the double DFT") <| @(t : T?) {
        return if (!cuda_up(t) || !stamps_built(t, ensure_tower_spec_dft_cls(), "tower_spec_dft_cls"))
        with_cu_planes() $(var cp) {
            spec_dft_case(t, cp)
        }
    }
}

[test(description = "The CUDA stamp of `tower_mel_log` matches the CPU log filterbank in all four laws (gemma4a max floor, canary added floor, whisper log10, gigaam clamp), with quiet and loud frames proving each floor and ceiling bites")]
def test_cud_tower_mel_log(t0 : T?) {
    t0 |> run("tower_mel_log (CUDA) == the log filterbank in its four laws: gemma4a's max floor, canary's added floor, the whisper log10, gigaam's clamp") <| @(t : T?) {
        return if (!cuda_up(t) || !stamps_built(t, ensure_tower_mel_log_cls(), "tower_mel_log_cls"))
        with_cu_planes() $(var cp) {
            mel_log_floor_case(t, cp)
            mel_log_clamp_case(t, cp)
        }
    }
}

[test(description = "The CUDA stamp of `tower_mel_norm` applies canary per-feature normalization over the valid frames like the CPU loop, zeroes the tail exactly, and writes nothing past the plane an idle column lane reaches")]
def test_cud_tower_mel_norm(t0 : T?) {
    t0 |> run("tower_mel_norm (CUDA) == canary's per-feature normalization in place over the valid frames, the tail zero") <| @(t : T?) {
        return if (!cuda_up(t) || !stamps_built(t, ensure_tower_mel_norm_cls(), "tower_mel_norm_cls"))
        with_cu_planes() $(var cp) {
            mel_norm_case(t, cp)
        }
    }
}

[test(description = "The CUDA stamp of `tower_ga_frames` writes gigaam windowed frames bit for bit against the CPU frame loop, zero pad rows and a window read at an element base included")]
def test_cud_tower_ga_frames(t0 : T?) {
    t0 |> run("tower_ga_frames (CUDA) == gigaam's windowed frames bit for bit with the zero row pad, the window at an element base") <| @(t : T?) {
        return if (!cuda_up(t) || !stamps_built(t, ensure_tower_ga_frames_cls(), "tower_ga_frames_cls"))
        with_cu_planes() $(var cp) {
            ga_frames_case(t, cp)
        }
    }
}

[test(description = "The CUDA stamp of `tower_dw_conv` matches the CPU k3 s2 p1 depthwise conv with bias over channel-major taps on the 7 x 5 image behind a poison row and on the 11 x 10 image")]
def test_cud_tower_dw_conv(t0 : T?) {
    t0 |> run("tower_dw_conv (CUDA) == canary's k3 s2 p1 depthwise conv over channel-major taps with its bias, on the 7 x 5 and the 11 x 10 image") <| @(t : T?) {
        return if (!cuda_up(t) || !stamps_built(t, ensure_tower_dw_conv_cls(), "tower_dw_conv_cls"))
        with_cu_planes() $(var cp) {
            dw_conv_case(t, cp)
        }
    }
}

[test(description = "The CUDA stamp of `tower_pk_dw2d` matches the CPU parakeet depthwise conv over tap-major taps, both unmasked and with the rows past a shorter valid length masked")]
def test_cud_tower_pk_dw2d(t0 : T?) {
    t0 |> run("tower_pk_dw2d (CUDA) == parakeet's k3 s2 p1 depthwise conv over tap-major taps, the rows past the valid length masked") <| @(t : T?) {
        return if (!cuda_up(t) || !stamps_built(t, ensure_tower_pk_dw2d_cls(), "tower_pk_dw2d_cls"))
        with_cu_planes() $(var cp) {
            pk_dw2d_case(t, cp)
        }
    }
}

//! the leak gate: every device buffer the cells made freed, the tier shut down with nothing counted live
def private same_names(a, b : array<string>) : bool {
    return false if (length(a) != length(b))
    for (x, y in a, b) {
        return false if (x != y)
    }
    return true
}

[test(description = "The kernel texts hold one row a seeded kernel, no two with one text, the FP4 rung's arch-specific texts among them.")]
def test_cud_kernel_texts(t0 : T?) {
    t0 |> run("the kernel texts: one row a seeded kernel, every text distinct") <| @(t : T?) {
        let texts <- cud_kernel_texts()
        var names <- cud_kernel_names()
        t |> success(length(texts) >= 52, "the texts carry the home's kernels ({length(texts)} rows)")
        var row_names <- [for (k in texts); k.name]
        row_names |> sort()
        t |> success(same_names(row_names, names), "one row a seeded kernel, by name")
        names |> pop()
        t |> success(!same_names(row_names, names), "the control: a census one name short differs")
        var seen : table<string; string>
        var dupes : array<string>
        for (k in texts) {
            let other = seen?[k.text] ?? ""
            if (!empty(other)) {
                dupes |> push("{other} = {k.name}")
            }
            seen.insert(k.text, k.name)
        }
        t |> equal(length(dupes), 0, "no two kernels share a text: {dupes}")
        let rung = length([for (k in texts); k; where k.is_arch_specific])
        t |> success(rung > 0, "the FP4 rung's arch-specific texts are rows ({rung})")
    }
}

let STUB_NVRTC = 12080   //! the NVRTC version the stand-in prewarms key under
let STUB_DRIVER = 12090   //! the driver version they key under

//! the prewarm's stand-in compiler: a 1 to 7 byte image, never empty, so every text compiles
def private stub_compile(source, name : string; compile_opts : array<string>; var log : string&) : array<uint8> {
    log = "{name} {length(compile_opts)}"
    return <- [for (b in range(length(source) % 7 + 1)); uint8(b)]
}

[test(description = "The kernel prewarm compiles every kernel a card has a target for - the FP4 rung's only from compute capability 120 - at the paths a load reads, and a second prewarm compiles none.")]
def test_cud_kernel_prewarm(t0 : T?) {
    t0 |> run("the kernel prewarm over the kernel texts: the rung filter, the load's paths, then nothing left to compile") <| @(t : T?) {
        let texts <- cud_kernel_texts()
        let rung = length([for (k in texts); k; where k.is_arch_specific])
        let dir = path_join(test_temp_dir(), "cubin_prewarm_{ref_time_ticks()}")
        let below = cud_prewarm_for(FP4_RUNG_SM - 1, dir, STUB_NVRTC, STUB_DRIVER, @@stub_compile)
        t |> equal(below.compiled, length(texts) - rung, "below the rung every text but the rung's compiles")
        t |> equal(below.cached, 0, "a fresh folder holds nothing")
        t |> equal(below.failed, 0, "the stand-in refuses nothing")
        let at = cud_prewarm_for(FP4_RUNG_SM, dir, STUB_NVRTC, STUB_DRIVER, @@stub_compile)
        t |> equal(at.compiled, length(texts), "at the rung every text compiles, under the card's own options")
        t |> equal(at.cached, 0, "another card's options key other files")
        let missing <- [for (k in texts); k.name;
            where !cubin_is_hit(stat(cubin_cache_path(dir, k.text, cuda_text_compile_options(FP4_RUNG_SM, k.text), STUB_NVRTC, STUB_DRIVER)))]
        t |> equal(length(missing), 0, "every text's cubin is at the path a load reads: {missing}")
        let again = cud_prewarm_for(FP4_RUNG_SM, dir, STUB_NVRTC, STUB_DRIVER, @@stub_compile)
        t |> equal(again.compiled, 0, "the second prewarm compiles nothing")
        t |> equal(again.cached, length(texts), "it reads every kernel from the cache")
    }
}

[test(description = "DASLLAMA_CUDA_CACHE_DIR names the folder the home's loads and prewarm use, in a child that reads it at init, and unset maps to CUDA_CACHE_DIR.")]
def test_cud_cache_dir_knob(t0 : T?) {
    t0 |> run("the cache folder knob reaches the folder the home uses") <| @(t : T?) {
        t |> equal(cud_cache_dir_of(""), CUDA_CACHE_DIR, "unset: the shared folder")
        let dir = path_join(test_temp_dir(), "cubin_knob")
        var out : string
        let rc = run_verb_child(child_verb_cmd("cuda-cache", [(name = "DASLLAMA_CUDA_CACHE_DIR", value = dir), (name = "DASLLAMA_GPU", value = ""),
            (name = "DASLLAMA_CUDA", value = "")], "dir"), out)
        t |> equal(rc, 0, "the child ran:\n{out}")
        t |> success(find(out, "ROOT_DONE") >= 0, "the child reached its end:\n{out}")
        t |> success(find(out, "CACHE_DIR {dir}\n") >= 0, "the home's folder is the knob's:\n{out}")
    }
}

[test(description = "After the front cells, shutting the CUDA tier down leaves zero live CUDA objects or allocations; this leak gate never skips")]
def test_cud_front_shutdown_leaks(t0 : T?) {
    t0 |> run("the CUDA tier shuts down with nothing live after the front cells") <| @(t : T?) {
        cud_shutdown()
        t |> equal(cuda_log_leaks_if_any(), 0l, "no CUDA object or allocation outlives the tier")
    }
}

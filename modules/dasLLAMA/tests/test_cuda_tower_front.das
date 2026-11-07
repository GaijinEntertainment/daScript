options gen2
options persistent_heap
options indenting = 4
options _dasllama_internal = true

// The audio fronts' CUDA stamps (`dasllama_cuda_classes_front`) on the case bodies the Vulkan twins' cells in
// `test_vulkan_tower_kernels.das` run (`_discrete_cells.das`), one cell a stamp. Model-free; every cell but the closing leak
// gate skips without a CUDA device and NVRTC, and a stamp that does not compile on a device reds its cell. The leak gate
// never skips.

require dastest/testing_boost public
require dasllama/dasllama_lint public
require dasllama/dasllama_cuda_classes_front
require _cuda_cell
require _discrete_cells

[test]
def test_cud_tower_im2col(t0 : T?) {
    t0 |> run("tower_im2col (CUDA) == the k3 s2 p1 columns over two chunks with the pad and the zero tail, as halves") <| @(t : T?) {
        return if (!cuda_up(t) || !stamps_built(t, ensure_tower_im2col_cls(), "tower_im2col_cls"))
        with_cu_planes() $(var cp) {
            im2col_case(t, cp)
        }
    }
}

[test]
def test_cud_tts_im2col16(t0 : T?) {
    t0 |> run("tts_im2col16 (CUDA) == the stem's k3 columns on both source layouts and strides, off and on the four-channel lattice, as halves") <| @(t : T?) {
        return if (!cuda_up(t) || !stamps_built(t, ensure_tts_im2col16_cls(), "tts_im2col16_cls"))
        with_cu_planes() $(var cp) {
            im2col16_case(t, cp)
        }
    }
}

[test]
def test_cud_tower_q3a_shuffle(t0 : T?) {
    t0 |> run("tower_q3a_shuffle (CUDA) == the qwen3a feature rows off the third conv stage, as halves") <| @(t : T?) {
        return if (!cuda_up(t) || !stamps_built(t, ensure_tower_q3a_shuffle_cls(), "tower_q3a_shuffle_cls"))
        with_cu_planes() $(var cp) {
            q3a_shuffle_case(t, cp)
        }
    }
}

[test]
def test_cud_tower_feat_shuffle(t0 : T?) {
    t0 |> run("tower_feat_shuffle (CUDA) == the [time][ch][freq] rows off the [time][freq][ch] image, with and without zero pad rows") <| @(t : T?) {
        return if (!cuda_up(t) || !stamps_built(t, ensure_tower_feat_shuffle_cls(), "tower_feat_shuffle_cls"))
        with_cu_planes() $(var cp) {
            feat_shuffle_case(t, cp)
        }
    }
}

[test]
def test_cud_tower_ln_relu(t0 : T?) {
    t0 |> run("tower_ln_relu (CUDA) == the channel LayerNorm then ReLU in place, against the double form") <| @(t : T?) {
        return if (!cuda_up(t) || !stamps_built(t, ensure_tower_ln_relu_cls(), "tower_ln_relu_cls"))
        with_cu_planes() $(var cp) {
            ln_relu_case(t, cp)
        }
    }
}

[test]
def test_cud_tower_spec_dft(t0 : T?) {
    t0 |> run("tower_spec_dft (CUDA) == the frames' spectra on the magnitude and the power arm, against the double DFT") <| @(t : T?) {
        return if (!cuda_up(t) || !stamps_built(t, ensure_tower_spec_dft_cls(), "tower_spec_dft_cls"))
        with_cu_planes() $(var cp) {
            spec_dft_case(t, cp)
        }
    }
}

[test]
def test_cud_tower_mel_log(t0 : T?) {
    t0 |> run("tower_mel_log (CUDA) == the log filterbank in its four laws: gemma4a's max floor, canary's added floor, the whisper log10, gigaam's clamp") <| @(t : T?) {
        return if (!cuda_up(t) || !stamps_built(t, ensure_tower_mel_log_cls(), "tower_mel_log_cls"))
        with_cu_planes() $(var cp) {
            mel_log_floor_case(t, cp)
            mel_log_clamp_case(t, cp)
        }
    }
}

[test]
def test_cud_tower_mel_norm(t0 : T?) {
    t0 |> run("tower_mel_norm (CUDA) == canary's per-feature normalization in place over the valid frames, the tail zero") <| @(t : T?) {
        return if (!cuda_up(t) || !stamps_built(t, ensure_tower_mel_norm_cls(), "tower_mel_norm_cls"))
        with_cu_planes() $(var cp) {
            mel_norm_case(t, cp)
        }
    }
}

[test]
def test_cud_tower_ga_frames(t0 : T?) {
    t0 |> run("tower_ga_frames (CUDA) == gigaam's windowed frames bit for bit with the zero row pad, the window at an element base") <| @(t : T?) {
        return if (!cuda_up(t) || !stamps_built(t, ensure_tower_ga_frames_cls(), "tower_ga_frames_cls"))
        with_cu_planes() $(var cp) {
            ga_frames_case(t, cp)
        }
    }
}

[test]
def test_cud_tower_dw_conv(t0 : T?) {
    t0 |> run("tower_dw_conv (CUDA) == canary's k3 s2 p1 depthwise conv over channel-major taps with its bias, on the 7 x 5 and the 11 x 10 image") <| @(t : T?) {
        return if (!cuda_up(t) || !stamps_built(t, ensure_tower_dw_conv_cls(), "tower_dw_conv_cls"))
        with_cu_planes() $(var cp) {
            dw_conv_case(t, cp)
        }
    }
}

[test]
def test_cud_tower_pk_dw2d(t0 : T?) {
    t0 |> run("tower_pk_dw2d (CUDA) == parakeet's k3 s2 p1 depthwise conv over tap-major taps, the rows past the valid length masked") <| @(t : T?) {
        return if (!cuda_up(t) || !stamps_built(t, ensure_tower_pk_dw2d_cls(), "tower_pk_dw2d_cls"))
        with_cu_planes() $(var cp) {
            pk_dw2d_case(t, cp)
        }
    }
}

//! the leak gate: every device buffer the cells made freed, the tier shut down with nothing counted live
[test]
def test_cud_front_shutdown_leaks(t0 : T?) {
    t0 |> run("the CUDA tier shuts down with nothing live after the front cells") <| @(t : T?) {
        cud_shutdown()
        t |> equal(cuda_log_leaks_if_any(), 0l, "no CUDA object or allocation outlives the tier")
    }
}

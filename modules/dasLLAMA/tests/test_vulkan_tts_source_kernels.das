options gen2
options _comment_hygiene = true
options _dasllama_internal = true
options persistent_heap   // a dastest root frees what it deletes only on this heap; the linear heap holds every carrier a cell deleted until the context ends

module test_vulkan_tts_source_kernels

require dastest/testing_boost public
require ?vulkan dasllama/dasllama_vulkan_common
require ?vulkan dasllama/dasllama_vulkan_classes
require ?vulkan _vkd_oracles
require _discrete_tts_source_cells   // nolint:STYLE030,LINT019 — the case bodies both discrete homes run, inside the vulkan arms alone

//! The TTS tower's decoder tail on Vulkan against the CPU chain (a Vulkan device, else the cells skip, and a stamp that
//! does not build reds its cell): the stage sum's axpy and the last stage's reflect pad, exact; the harmonic source on
//! both resample laws - the phase frames, the mixed signal, the captured noise and the source's own draw - held to the
//! CPU `sine_source`; the STFT on both pad laws and the inverse STFT with and without the envelope held to their
//! double-precision forms; every output under a NaN fill, every compare with its poisoned element. Every cell runs the
//! case body the CUDA home's cell runs (`_discrete_tts_source_cells.das`).

//! every src stamp built
def private src_ready : bool {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        return tts_src_ready(VtRail())
    } else {
        return false
    }
}

[test(description = "The stage-sum axpy equals o + 0.25 y in f32 bit for bit over 1030 elements when accumulating, and 0.25 y over a NaN-filled o when overwriting, with the elements outside the run untouched")]
def test_vkt_axpy(t0 : T?) {
    t0 |> run("the axpy == o + 0.25 y in f32, exact, accumulating (zero = 0) and overwriting (zero = 1)") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            return if (!vk_up(t) || !stamps_built(t, ensure_tts_axpy_cls(), "tts_axpy_cls"))
            with_cell_planes() $(var cp : CellPlanes) {
                tts_axpy_case(t, cp)
            }
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

[test(description = "The reflect pad built from two row copies equals `pad_reflect_left_rows_inplace` bit for bit: x [37][24] shifted down one row behind its own row 1, nothing written past the 38 rows")]
def test_vkt_reflect1(t0 : T?) {
    t0 |> run("the reflect pad as the driver's two row copies: y [38][24] == x [37][24] behind x's row 1, exact") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            return if (!vk_up(t) || !stamps_built(t, ensure_tts_pk_rows_cls(), "tts_pk_rows_cls"))
            with_cell_planes() $(var cp : CellPlanes) {
                tts_reflect1_case(t, cp)
            }
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

[test(description = "The harmonic source on the torch and onnx laws holds phase frames to one ulp and the mixed signal to 2e-6 of the CPU `sine_source`, a poisoned f0 frame reds both; at up 300 the 40000-frame cumsum and the seeded own noise draw match too")]
def test_vkt_source(t0 : T?) {
    for (torch in fixed_array(true, false)) {
        t0 |> run("the harmonic source, {torch ? "torch" : "onnx"} law, up 300 x 700 frames: phase frames, mixed signal, long cumsum, own draw") <| @(t : T?) {
            static_if (typeinfo builtin_module_exists(vulkan)) {
                return if (!vk_up(t) || !stamps_built(t, src_ready(), "the source stamps"))
                with_cell_planes() $(var cp : CellPlanes) {
                    tts_source_case(t, cp, torch, 300, 700, true)
                }
            } else {
                t |> skip("dasVulkan not present")
            }
        }
        t0 |> run("the harmonic source, {torch ? "torch" : "onnx"} law, up 2 x 37 frames: the first taps read sample 0, the initial phases in") <| @(t : T?) {
            static_if (typeinfo builtin_module_exists(vulkan)) {
                return if (!vk_up(t) || !stamps_built(t, src_ready(), "the source stamps"))
                with_cell_planes() $(var cp : CellPlanes) {
                    tts_source_case(t, cp, torch, 2, 37, false)
                }
            } else {
                t |> skip("dasVulkan not present")
            }
        }
    }
}

[test(description = "The STFT on reflect and edge pads produces 10 frames x 4 bins of magnitude and phase within 1e-4 of the double form, the zero-imaginary bin reading +pi or 0, and a poisoned sample reds the compare")]
def test_vkt_stft(t0 : T?) {
    for (reflect in fixed_array(true, false)) {
        t0 |> run("the STFT, {reflect ? "reflect" : "edge"} pad, 10 frames x 4 bins == the double form") <| @(t : T?) {
            static_if (typeinfo builtin_module_exists(vulkan)) {
                return if (!vk_up(t))
                let built = ensure_tts_stft_reflect_cls() && ensure_tts_stft_edge_cls()
                return if (!stamps_built(t, built, "tts_stft_reflect_cls and tts_stft_edge_cls"))
                with_cell_planes() $(var cp : CellPlanes) {
                    tts_stft_case(t, cp, reflect)
                }
            } else {
                t |> skip("dasVulkan not present")
            }
        }
    }
}

[test(description = "The inverse STFT with and without the window envelope turns 9 frames x 3 bins into 36 samples within 1e-4 of each sample's envelope (the sum of its terms' magnitudes) of the double overlap-add, some samples under a zero envelope, and a poisoned magnitude reds the compare")]
def test_vkt_istft(t0 : T?) {
    for (envelope in fixed_array(true, false)) {
        t0 |> run("the inverse STFT, envelope {envelope ? "on" : "off"}, 9 frames x 3 bins into 36 samples == the double overlap-add") <| @(t : T?) {
            static_if (typeinfo builtin_module_exists(vulkan)) {
                return if (!vk_up(t) || !stamps_built(t, ensure_tts_istft_cls(), "tts_istft_cls"))
                with_cell_planes() $(var cp : CellPlanes) {
                    tts_istft_case(t, cp, envelope)
                }
            } else {
                t |> skip("dasVulkan not present")
            }
        }
    }
}

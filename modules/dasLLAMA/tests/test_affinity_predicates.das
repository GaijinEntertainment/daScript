options gen2
options _dasllama_internal = true
require dastest/testing_boost public
require ../performance/profile_common.das

// affinity_on() (das-side scheduling classification: Windows pin, darwin QoS) and
// lcpp_mask_on() (llama-bench --cpu-mask argv, Windows-only) split in the QoS arc.
// This pins the observable: mask argv exists exactly when lcpp_mask_on() says so,
// and the platform the test runs on classifies as expected.

[test]
def test_affinity_predicates(t : T?) {
    t |> run("mask argv matches lcpp_mask_on") @(t : T?) {
        let argv <- lcpp_affinity_argv(8)
        if (lcpp_mask_on()) {
            t |> success(!empty(argv), "windows: mask argv present")
        } else {
            t |> equal(0, length(argv))
        }
    }
    t |> run("platform classification") @(t : T?) {
        if (get_platform_name() == "darwin") {
            t |> success(affinity_on(), "darwin classifies")
            t |> success(!lcpp_mask_on(), "darwin never emits lcpp mask argv")
        }
        if (get_platform_name() == "windows") {
            t |> success(affinity_on(), "windows pins")
            t |> success(lcpp_mask_on(), "windows emits lcpp mask argv")
        }
    }
}

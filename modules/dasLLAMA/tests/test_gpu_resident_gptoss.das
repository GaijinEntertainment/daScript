options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options persistent_heap   // + explicit deletes in the cells: the model and sessions free before exit
options _dasllama_internal = true

require dastest/testing_boost public
require _moe_resident

// gpt-oss-20b (the native-MXFP4 routed stacks on the mx4 expert rail, the biased router's softmax over its four picks,
// the gate / up / down bias rows at the act and the combine under the clamped swiglu, the per-head attention sinks in
// the flash tiles and the token command's pass, the output bias on the residual step, sliding windows on the even
// layers, YaRN folded at load) in _moe_resident's forced-feed form: one window, then two. Large-tier
// (DASLLAMA_PARITY_FULL=1); -jit only; skips without the file, the armed tier, or a device with no cm2 tile family;
// on a cm2 device the driver's admission is asserted - a decline (a seat, the sink arm, a plane's format) is a red
// that sends the reader to the load log.
//! gpt-oss's logits bar: the routed chain's noise reads 0.06-0.15 of a 7-15 max logit on the 5060 Ti (0.9-2.2%) at 40 and 600 tokens and
//! every step, the argmax agreeing everywhere - the four picks over 32 experts on a biased softmax sit far from a near-tie on
//! this fixture - while the one-step-off controls read 4.2-18.9, so a 5% bar keeps a 2x margin over the noise and 12x under the control
let private GPTOSS_LOGIT_BAR_REL = 0.05
let private GPTOSS = MoeFixture(file = "gpt-oss-20b-mxfp4.gguf", shexp = false, bar_rel = GPTOSS_LOGIT_BAR_REL)   //! the pinned upstream file (its row in ../performance/model_specs.das)

[test]
def test_gpu_resident_gptoss(t : T?) {
    t |> run("gpt-oss-20b: the resident mx4 MoE prefill window + the biased, sinked token command vs the CPU chain, one window") @(t : T?) {
        moe_forced_feed(t, 40l, GPTOSS)
    }
    t |> run("gpt-oss-20b: the same across two windows (the 128-key sliding layers masking below their window on both eras)") @(t : T?) {
        assert_pf_window(t)
        moe_forced_feed(t, PF_WINDOW_ROWS + 88l, GPTOSS)
    }
}

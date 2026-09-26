# dasLLAMA tests - Media Cells Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
doc: `CLAUDE.md`. Planned work: `../followup_general.md`, `../followup_vulkan.md`,
`../followup_metal.md`.

A cell is one `t |> run` subtest, or a `[test]` function that runs no subtest; a helper's
asserts belong to every cell that calls it.

**A cell that runs with no model loaded and encodes, preprocesses, or asserts on media bytes
an encoder consumes - pixels or audio samples, not a `.dlim` model image - builds its fixture
procedurally and pins its expectations in-repo.**

**An image a test feeds an embedder that the test does not build, and that
`DASLLAMA_VISION_DUMP` cannot preview, is a defect** - a failure never requires adding
instrumentation before a human can see what the model consumed.

**An audio clip a test feeds an embedder that the test does not build, that the repository does
not track, and that is not one of the clips stocked beside the models (`jfk.wav`,
`gemma4a_test2.wav`), is a defect** - a clip nobody else can play makes a red unreadable.

**A diff that stocks a new clip beside the models adds it to the stocked-clip list above in the
same change.**

**A media fixture an embedder-parity cell regenerates in-test and compares against an oracle
dump, with no exact-value generator - one whose values are exactly representable floats, so
every box produces the same bytes - is a defect.** A generator running libm transcendentals is
not exact-value: it is not float-portable.

**An embedder-parity cell - one comparing an encoder's output rows against a second source, another
encode chain (the CPU exact, the CPU q8 or the device chain) or an oracle dump - that does not
name its fixture in its label or a logged line, or does not log the measured maxdiff on green as
well as red, is a defect.**

**An ASR cell comparing transcripts across two serving lanes - a lane is a weight format the
family serves, one chain (CPU or device) over one format, or one kernel form of one format -
asserts WORD equality when the pair is a crowned kernel form and its tensor twin, and TOKEN
equality otherwise.** A crowned kernel form is the one the tuner measured fastest and armed as the serving
one; its tensor twin is the same kernel written on Metal's tensor primitives, and the twins'
rounding legitimately flips tokens.

**An ASR transcript cell that cannot assert the equality its comparison calls for converts to
a forced-feed logits compare within a tolerance bar, or to equal transcript text with the encoder
output rows held within a tolerance of the other side's or by the twin bar - the device chain's
distance from the exact chain within a stated multiple of the CPU q8 chain's own - never to a
text compare alone.**

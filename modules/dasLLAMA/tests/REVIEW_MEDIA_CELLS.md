# dasLLAMA tests - Media Cells Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
doc: `CLAUDE.md`. Planned work: `../followup_general.md`, `../followup_vulkan.md`,
`../followup_metal.md`.

A cell is one `t |> run` subtest, or a `[test]` function that runs no subtest; a helper's
asserts belong to every cell that calls it. An encoder is any stage of a vision tower, an audio
tower or a speech synthesizer - never the language model that reads their rows.

**A cell that runs with no model loaded and encodes, preprocesses, or asserts on media bytes
an encoder consumes - pixels or audio samples, not a `.dlim` model image - builds its fixture
procedurally, and its expected values live in the repository: in-test constants, a tracked
file, or a value the test computes itself.**

**An image a test feeds an encoder that the test does not build, and that
`DASLLAMA_VISION_DUMP` cannot preview, is a defect** - a failure never requires adding
instrumentation before a human can see what the model consumed.

**An audio clip a test feeds an encoder that is not built by the test, tracked in the
repository, or fetched by `../performance/setup_asr_rig.das` is a defect** - a clip nobody else
can fetch makes a red unreadable.

**A media fixture an encoder-parity cell regenerates in-test and compares against an oracle
dump, with no exact-value generator - one whose values are exactly representable floats, so
every box produces the same bytes - is a defect.** A generator running libm transcendentals is
not exact-value: it is not float-portable.

**An encoder-parity cell - one comparing an encoder's output rows against a second source,
another encode chain (the CPU exact, the CPU q8 or the device chain) or an oracle dump - that
does not name its fixture in its label or a logged line, or does not log the measured distance
it compares to its bar, whether the cell passes or fails, is a defect.**

**An ASR cell comparing transcripts across two serving lanes - a lane is a weight format the
family serves, one chain (CPU or device) over one format, or one kernel form of one format -
asserts WORD equality when the pair is a crowned kernel form and its tensor twin, and TOKEN
equality otherwise.** A crowned kernel form is the one the tuner measured fastest and armed as
the serving one; its tensor twin is the same kernel written on Metal's tensor primitives, and the
twins' rounding legitimately flips tokens.

**An ASR transcript cell that cannot assert the equality its comparison calls for converts to
one of: a forced-feed logits compare within a tolerance bar; equal transcript text with the
encoder output rows held within a tolerance of the other side's; or equal transcript text with
the encoder output rows held by the twin bar - never to a text compare alone.** The twin bar holds
the device chain's distance from the CPU exact chain within a stated multiple of the CPU q8
chain's distance from it.

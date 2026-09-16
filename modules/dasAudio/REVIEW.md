# dasAudio Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture doc:
`ARCHITECTURE.md`.

**A diff that makes `REVIEW.das` (beside this file) report less - fewer roots walked, fewer calls
reported, more exempt paths, a deleted or loosened check - is a defect.**

**The browser mixer thread must wake on the worklet, never on a sleep timeout.** A `sleep_for`,
a `sleep`, or a `futex_wait` whose timeout is the thing expected to end the wait is a defect
there, whatever the number, because only a timeout is quantized to the host's timer resolution
and that differs 15x between macOS and Windows. A timeout may remain ONLY as a backstop against
a lost wake.

**A diff that changes the browser ring's depth must state, in `ARCHITECTURE.md`, the latency it
buys and the producer turnaround it covers, and must never deepen it to make underruns go
away.** Buffered audio IS output latency: the producer renders ahead, so in steady state a newly
triggered sound waits behind everything already in the ring. When underruns appear the
reviewable question is what stalled the producer, not what number would have hidden it, and
`sound_playback_underrun_frames` is the evidence - a browser-only count, so a native zero from it
proves nothing.

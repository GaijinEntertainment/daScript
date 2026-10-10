# dasLLAMA Hooks, Overrides and Lane Pins Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
doc: `ARCHITECTURE.md`. Planned work: `followup_general.md`.

**A diff that adds or changes a function in `dasllama/dasllama_common.das` that performs work
through a hook another module registers makes it run its own CPU code for that work when the hook
is unset or, where it has no CPU code for that work, panic with a message naming the module to
require.** A function that returns quietly when the hook is unset leaves the program running
without saying which module it failed to require.

**A diff that adds or changes a function in `dasllama/dasllama_common.das` that reports whether a
hook another module registers is installed makes it return false when the hook is unset - never
panic.**

**A diff that moves a family encode stage - a `dasllama/dasllama_<family>.das` stage that turns
input into embeddings or fills a cache the next stage reads - onto a GPU hook leaves the CPU form
in place and changes no value that code after the stage reads.** The CPU form serves every
machine with no GPU driver.

**A diff that adds an override, or changes what one does - a value it now clamps or ignores
included - without the announce is a defect.** An announce is the line the run prints where the
override changes the outcome. An override is an environment knob, a public setter reachable from
`dasllama/dasllama.das` through `public` requires, or an on-disk state file - one a run writes or
a user places, never data a build ships - that moves a gate, policy, or threshold off its default
and so changes what the run executes, writes, reads, mints (writes as a prepared image), or
computes. A measured time, the run's own duration, or a different moment at which the same work
happens is not such a change; a CLI flag is never an override.

**An announce names the override by the spelling a user would set - the env variable, the sidecar
or file key, the setter's name - and, for an override that is on by default, the spelling that
turns it off, or, when nothing turns it off, a statement that nothing does.**

**A call to a `set_*_q8` lane setter - one that picks whether a model family's weights run the
q8 or the float path - in a file under `dasllama/` or `harness/`, outside the body of another
`set_*_q8` setter, is followed at once by a `defer()` calling its `reset_*_q8` twin.** A q8 lane
choice still set after its caller returns silently changes the lane of the next model the process
loads.

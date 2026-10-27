# dasLLAMA Architecture - the procedure documents

Companion to `ARCHITECTURE.md`; a section is cited by its anchor. This document carries the
HOW_TO document type: what a HOW_TO is, the form of its steps and traps, and the gate that holds
its mechanical half.

### The HOW_TO document {#howto-documents}

A `HOW_TO_*.md` beside this file is the procedure for one recurring task - adding a weight
format, adding a speech-recognition family, taking a box to a committed tune profile. Its reader
is the next author of that task, who reads it before the first edit; `CLAUDE.md`'s HOW_TO table
names each document and the task that opens it.

- **Steps.** A HOW_TO walks the code in the order the work lands. A step 0 may ask the shape
  questions whose answers decide which existing path the work takes; their answers open the PR
  body. Every `## N.` step, step 0 included, names the files it touches and cites the
  `ARCHITECTURE*.md#<anchor>` section that owns the mechanism it changes: the HOW_TO says what to
  do in what order, the cited section says why the shape is what it is. The last step names the
  gates that prove the work end to end.
- **Validation.** A HOW_TO is validated by execution: its author walks it on a real instance of
  the task.
- **Traps.** A place where the documented procedure does not hold is one sentence at the step it
  applies to, stating the consequence the author meets ("the compile fails", "the cells read
  garbage").
- **Tense.** A HOW_TO is timeless: no record of what happened - no past-tense narrative, no date,
  no PR or issue number, no box log. Git keeps the history, and a live fact a record carried is
  restated in the present tense at its step.
- **Length.** A HOW_TO carries no line cap: LINT027 binds only `REVIEW*.md` and
  `ARCHITECTURE*.md`.

`REVIEW.das`'s `check_howto_docs` walks every `HOW_TO_*.md` under the module and fails a line
carrying a past-tense marker outside backtick spans (the gate's `HOWTO_PAST_WORDS` and `HOWTO_PAST_PHRASES`), a `20YY-MM`
date, or a `#` followed by three digits, and a `## N.` step carrying no `ARCHITECTURE*.md#` cite.
It licenses no names. Whether a step is an instruction and whether its cite names the owning
section are a reviewer's call (`REVIEW_DOCS.md`).

# ARCHITECTURE.md - the common contract

Every folder-scoped `ARCHITECTURE.md` states how that folder's system is built and why. It has
two readers: the writer, who reads it BEFORE writing code in the folder, and the reviewer, who
follows a rule's citation into it. This file is the part all architecture docs share; its
sibling `REVIEW_COMMON.md` (repo root) is the checklists' contract. The two split one grammar:
a ban or duty on a diff lives in the folder's `REVIEW.md`; a statement of what is true lives
in its `ARCHITECTURE.md`; what happened lives in `/history`.

**An architecture doc is written in the present tense: what is true of the current tree, and
why that shape wins.** The mood test is mechanical. A sentence that binds a diff with
must/never/"is a defect" is a rule - move it to the folder's `REVIEW.md`. A dated or
past-tense passage is history - archive it to `/history` (`skills/internal/doc_archiving.md`).
A promise, phase, or plan is planned work - move it to the folder's follow-up ledger.

**What belongs:**

- **File charters** - one line per file: what it owns, what never lands in it.
- **Mechanisms** - how a subsystem works, and the reason its shape wins over the obvious
  alternative.
- **Definitions** - terms the folder's rules and code cite, each stated once.
- **Inventories** - the folder's override knobs, registered families, sanctioned cases: lists
  a rule checks a diff against.
- **The exception ledger** - the cases a `REVIEW.md` rule fires on that are ruled acceptable,
  each with its reason. A rule stays absolute; this is where its sanctioned violations live.

**Sections are numbered, and rules cite them by section.** A section number, once cited, is
never reused for different content - append new sections, do not renumber.

**A fact that a rule or a code comment cites is load-bearing: it must stay true.** The
same-change duty that keeps it true belongs in the folder's `REVIEW.md`, not here.

**Plain English - short sentences, common words, no idioms.** A term of art is defined where
it first appears. A passage that needs a second read is a defect of the passage.

**A statement earns its place by changing what someone writes or concludes.** Restating what
the code says in the same words, or describing what a reader can see in one glance at the
file, is clutter - cut it.

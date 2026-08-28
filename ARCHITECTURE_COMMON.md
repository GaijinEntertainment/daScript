# ARCHITECTURE.md - the common contract

Every folder-scoped `ARCHITECTURE.md` states how that folder's system is built and why. It has
two readers: the writer, who reads it BEFORE writing code in the folder, and the reviewer, who
follows a rule's citation into it. This file is the part all architecture docs share; its
sibling `REVIEW_COMMON.md` (repo root) is the checklists' contract.

**An architecture doc is written in the present tense: what is true of the current tree, and
why that shape wins.** The mood test: a sentence that binds a diff with must/never/"is a
defect" is a rule - move it to the folder's `REVIEW.md`. A passage that narrates what
happened - past tense, or dated as a record of an event - is history - archive it to
`/history`; a `probe-verified <date>` tag or a version stamp records no event and stays. A promise, phase, or plan is planned work - move it to the
folder's follow-up ledger.

**What belongs in an architecture doc: file charters, mechanisms, definitions, inventories,
and the exception ledger. Nothing else.**

- **File charters** - one line per file: what it owns, what never lands in it.
- **Mechanisms** - how a subsystem works, and the reason its shape wins over the obvious
  alternative.
- **Definitions** - terms the folder's rules and code cite, each stated once.
- **Inventories** - lists a rule checks a diff against: override knobs (an env variable or
  setting that changes what a run compiles, tunes, or emits), registries a diff adds a row to,
  sanctioned cases.
- **The exception ledger** - the cases a `REVIEW.md` rule fires on that are ruled acceptable,
  each with its reason.

**Sections are numbered, and rules cite them by section.** A section number is never reused
for different content: append new sections, never renumber.

**A section that code implements carries a `{#anchor}` on its heading, and every anchor is
cited by an `[arch(at="<doc>#<anchor>")]` on a function in the document's own folder tree.**
One anchor per heading; the anchor name is stable across rewording, like the section number.
An anchor no function cites, a citation naming no anchor, and a citation reaching a document
outside the citer's folder tree are all lint findings (LINT026), in every folder. A mechanism
another folder's document states is restated here in prose - a paragraph, not a resolved link -
and the code cites this document. A section no function implements is narrative and carries no
anchor.

**A fact that a rule or a code comment cites is load-bearing: it must stay true.** The
same-change duty that keeps it true belongs in the folder's `REVIEW.md`, not here.

**Plain English - short sentences, common words, no idioms.** A term of art is defined where
it first appears. A passage that needs a second read is a defect of the passage.

**A statement earns its place by changing what someone writes or concludes.** Restating what
the code says in the same words, or describing what a reader can see in one glance at the
file, is clutter - cut it.

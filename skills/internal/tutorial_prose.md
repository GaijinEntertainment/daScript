# Tutorial Prose (REQUIRED for general-reader docs)

How to WRITE tutorial and doc prose — voice, vocabulary, structure. The RST
mechanics (labels, toctrees, Sphinx workflow) live in
`skills/internal/documentation_rst.md`; this file is about the words. It was distilled
from the tutorial-00 reader-feedback sessions (2026-07); the worked example of
every rule is `doc/source/reference/tutorials/dasLLAMA_00_problem_statement.rst`.

## The audience

Software engineers, often non-native English speakers. They know programming
and they know daslang. They do not know your domain, and every English idiom
is a parse stall that teaches nothing. The voice is playful-direct, never
academic — a hacker showing a friend a neat trick, not a thesis defense.

## Sentence rules

1. **SVO, short sentences.** No cleft sentences ("What we build is…" → "We
   build…"). No throat-clearing, no meta-frames ("worth remembering", "one
   more fact", "needs a definition first") — cut the frame, keep the fact.
2. **No idioms, no literary constructions.** The bar: does a B1/B2-English
   engineer parse it on first read? "Settled", "traded for", "deserves a
   pause", "not by decree" all failed real readers.
3. **No Latinate connectives.** therefore/thus/hence/moreover → "so", "and",
   plain "is".
4. **We-voice.** The writer and reader do things together ("we build", "we
   compare"); the reader is "you". Never "this page does X". Author-intent
   meta dies ("that is deliberate" — the reader needs options, not
   intentions). Prefer invitations over lectures: "try different prompts",
   "run it".
5. **Never define by negation.** "There is no require X", "it returns no
   text", "not a hypothetical" — say what a thing IS.
6. **No forward promises.** "Explained below / in the next section / later"
   is a non-explanation — future promises are for politicians. Either say it
   here in one line, or say nothing and let the section arrive. Backward
   recalls ("the dot-product fact from earlier") are fine. Teaser clauses
   ("…all of it rests on one operation") are forward promises in disguise.

## Term discipline

7. **A term is taught where it appears, or it does not appear yet.** Plain
   words until the teaching point ("the model file" until "checkpoint" gets
   its save-point story). This also applies to CODE IDENTIFIERS: `forward`
   does not appear in prose before the reader has met it in code or been
   introduced ("the heart of the file is one function, `forward`").
8. **Flip it — when in doubt, flip it.** Mechanism/story first, the NAME
   lands last as a conclusion: "…a running sum of differences — residuals —
   is why `x` is called the residual stream." Never "X is called Y. The name
   means…".
9. **Every name gets its why, in one clause.** Classifier (classifies among
   32,000 candidates), heads (read heads on a tape drive), checkpoint (a
   save point of training), feed-forward (values flow one way). Expand every
   acronym once: BPE, RMS, SiLU, SwiGLU. If the honest why is "the shamans
   tried incantations and this one worked", say that — empirical truth beats
   fake derivation.
10. **Reinforce on reuse.** A term taught long ago gets a pocket reminder at
    its next distant use: "three projections — three matrices, each asking
    its own questions —". If a reader could ask "what's X?" at a use site,
    the reminder was missing.
11. **NO glossary.** If a term needs a separate explanation section, the
    teaching failed. Re-uses link to the FIRST explanation: put an inline
    anchor before the teaching paragraph (`.. _t00-term:`, page-unique
    prefix) and link with `:ref:`tokens <t00-term>``. `custom.css` styles
    these quiet via the anchor prefix (`[href^="#t00-"]`) so real cross-page
    links stay amber.
12. **Watch overloaded words.** "Weights" (parameters vs attention weights)
    must be split explicitly the moment both senses are in play.

## Structure rules

13. **Explain at the point of use.** No glossary-chapter of operations up
    front: each function's explanation AND its code block sit exactly where
    the narrative first needs them. A roll-call list ("five functions do all
    the math") may inventory; it may not define.
14. **Concrete over abstract.** Ground every number at its use (48 = one
    head's slice; "floats 0 and 1 are the first pair"). No algebra symbols
    (`W`, `d`) in prose without an on-ramp. No "N-dimensional space" talk —
    "a vector of 288 floats", "closer", "the pattern of all 288".
15. **Intuition owns the flow; rigor gets one marked sentence.** When a
    passage must satisfy both a cold reader and a verifier, the airtight step
    (e.g. a sum regroups freely) is one clearly separated sentence, never
    fused into the teaching sentence.
16. **No language teaching.** Readers know daslang (`#`, `array_view`) or
    can read the language tutorials. Cut every das-explainer aside from
    domain docs.
17. **Cut noise.** Line counts, "who cares" qualifiers, flexes that teach
    nothing, cross-reference bookkeeping in figure captions. A caption says
    what the figure is; the sections do the explaining.
18. **Answer the reader's next question.** If an engineer would ask "why not
    `x >= 0 ? x : 0`?", the page asks and answers it. If the answer is
    empirical, admit it.

## Process

- **Verify with persona agents** before shipping: a blind read by an agent
  role-playing the target reader (e.g. smart 15-year-old, AP math/CS, zero
  domain knowledge), then after fixes a second pass whose bar is
  "reproduce the reasoning, not locate a sentence". Give agents the rendered
  figure PNGs, not just source.
- **Term audit** when a page is term-heavy: table of every term × first use ×
  where explained; any explanation below its first use is a bug. Include
  figure text — figures use terms too.
- **Iterate with the owner**: nitpick rounds item by item (ledger, discuss,
  counter-draft in the owner's register), then batch "restart" passes that
  apply everything and rebuild HTML for the next read.
- Blog posts are a DIFFERENT register (deliberately rough — see the blog
  style conventions); do not apply this file to `site/blog/`.

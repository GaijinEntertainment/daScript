# LAWS.md - Boris's rulings

Append-only intent provenance for rule-document edits in this folder's documents (the
mechanism: CLAUDE.md sec. "Boris's rulings get a `LAWS.md` sidecar"). Never groomed,
compacted, or cited as rules.

- **2026-09-01** (`REVIEW.md`, new): the cookie audit of daslang.io left the served doc tree
  ungated - its one off-site load is declared in `conf.py`, which no page-level gate reads.
  Told that closing it meant a new checklist folder, Boris ruled "why not. lets add gates".
  The checklist exists to carry the gate's two rules; RST prose review stays in its skills.

- **2026-09-01** (`REVIEW.md`): dragon round on the new checklist. `conf.py` resolved
  against `doc/`, where no such file exists, and naming `html_css_files`/`html_js_files`
  understated a gate that reads every absolute URL in the file - both replaced by the
  property. The opening now routes the prose dimension rather than reading as if a
  prose-only diff escapes the gate.

- **2026-09-01** (`REVIEW.md`): the two gate rules took the site checklist's dragon wording -
  the weakening rule re-plained, the disclosure rule naming `ALLOWED_HOSTS` directly.

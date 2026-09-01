# LAWS.md - Boris's rulings

Append-only intent provenance for rule-document edits in this folder's documents (the
mechanism: CLAUDE.md sec. "Boris's rulings get a `LAWS.md` sidecar"). Never groomed,
compacted, or cited as rules.

- **2026-09-01** (`REVIEW.md`, new): the cookie audit's gate walk found the playground shell
  deployed from `examples/ui/src` (gitignored where it lands under `site/`, so no site-rooted
  gate sees it in a fresh checkout) and the canvas shell users ship with. Boris ruled "why
  not. lets add gates"; both surfaces became this folder's gated set.

- **2026-09-01** (`REVIEW.md`): dragon round on the new checklist. The census of served
  surfaces was wrong - `examples/ui/samples` is staged four ways and is gitignored where it
  lands, the same reason `examples/ui/src` needed a gate - so the opening states the served
  property and a new rule makes a diff that serves a tree add it to the gate's root list.
  The empty-list WHY was moment-pinned and went; "off-site" became "a host its allowed list
  does not name", which is what the gate actually tests.

- **2026-09-01** (`REVIEW.md`): the two gate rules took the site checklist's dragon wording -
  the weakening rule re-plained, the disclosure rule naming `ALLOWED_HOSTS` directly.

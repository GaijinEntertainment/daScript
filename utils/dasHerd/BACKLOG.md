# dasHerd backlog and plan

Status: plan of record, 2026-07-26. History and per-note forensics live in
HERDER_FIXROUND.md; arc designs in PTY_HOST_DESIGN.md and
AGENT_REVIEW_WORKFLOWS.md. Rule of the round stands: bugs over everything;
review capacity is the bottleneck the arcs exist to fix.

## Phase 0 — deploy and prove — PROVEN LIVE 2026-07-26 (accidentally)

The goal-round rig ran the current branch; the watcher died mid-flight
(ungraceful — a closed stdout pipe) with a live shell AND a live Claude
Code session in detached hosts. The restarted watcher logged "adopted 2
detached PTY host session(s)"; both records resurrected running (no
watcher_restart scar), the client reconnected on its own, and the adopted
shell echoed input ("SURVIVED-THE-WATCHER"). Remaining for Boris: see it
with his own eyes once, then the note-50 operational rule retires.
40/41 are deployed with their round-time verification; dogfood confirms.

## Phase 1 — dogfood bug burn-down — DONE 2026-07-26 (goal round)

Every note fixed or verified in-build; per-note detail + proof lives in
HERDER_FIXROUND.md:

- 42 FIXED — Claude runs alt-screen + mouse reporting; the widget now
  forwards wheel/clicks to the child when reporting is active (dasTerminal
  terminal_encode_mouse + imgui_terminal forwarding; Shift = local).
- 44 FIXED — focus via the terminal's own model (the tabbing request fell
  through the InvisibleButton), surviving the checkpoint reset.
- 47 FIXED — the "stray column" was Sessions-panel text clipped at the
  window edge; card/context/summary rows now elide to the content region.
- 43 FIXED — herd-level selection pick; dead-card clicks stick.
- 48/45/46/49 VERIFIED in the current build (fix-round work, was pending
  deploy).
- Drive-by: herder_close_session rail now rejects unknown herd ids
  instead of ok=true.

Carried remainders that slot here when touched:

- 39 portal file in sticky Diff mode: silent empty inspector (fix shape
  decided: force View or state "nested repository - no diff vs parent")
- 20 token palette adoption on herd cards + repositories panel
- 13 diff-mode search (needs text-line -> display-row map); terminal +
  history search rides the scrollback/history work
- dasImgui-internal QoL: combo imgui_force_set after live reload;
  ClickState.click_count on disabled widgets

## Phase 2 — external sessions arc

A session whose terminal lives OUTSIDE the herder (the dev session building
dasHerd), claimed from outside, coordination-only, outliving the watcher.
Mechanically: a host/stamp the watcher never spawned — adoption already does
not care who spawned a stamp. Payoff: the sessions doing the real work become
visible, coordinated participants; "Repositories and worktrees" migrates
right, sessions-first left (Boris, 2026-07-25).

## Phase 3 — GitHub observer + PR assistant (Track B)

The review-capacity payoff. Design decided (AGENT_REVIEW_WORKFLOWS.md):

- Watcher-side observer polls PRs/CI/reviews; agents never poll.
- Tool-driven lifecycle: arm ("PR about to open, sit tight") then summon
  ("look at that" + delta briefing) on transitions.
- Hard signals (CI red = truth) summon; soft signals (Copilot verdicts =
  advice) rank. Attention ranks by deviation from the declared bundle.
- Escalation policy attaches at arm time per PR/bundle.
- With it: flip auto-Copilot-review off on GitHub; rework skills/babysit.md
  (it assumes Copilot feedback arrives on every PR).
- 26 "agent needs me" (blocked-at-prompt heuristics) folds into the same
  attention machinery.

## Phase 4 — reviewer agent / tool

Rides Phases 2-3 with zero new machinery: a session (built), armed on
bundle -> ready (observer), scoped by the bundle (built), reporting into
attention (built). Disciplines: reviewer gets the bundle, never the author's
conversation; opinions rank, reproductions escalate; deterministic tools
(lint, perf_lint, detect-dupe) join one trust rung up.

## Phase 5 — ssh / watcher-per-box

Decided shape (PTY_HOST_DESIGN.md): one watcher per box, client federates
via a watcher list; ssh = tunnel + control channel only; git stays
watcher-local. Pull forward if a second box becomes real before Phase 3-4
finish; nothing else depends on it.

## Structural, when the scenario demands

- 25 multiple terminals (or instant switching with preserved state) — the
  several-agents-at-once scenario; pairs with 26.
- 35 human/agent UI input lease UX ("input is leased to X - click to
  claim") — the mute + badges landed with 38; the arbitration UX remains.
- 37 follow-up: registered-repository synthesis for gitignored nested
  repos; portal into its own Project view.
- Icon-set expansion (consult Claude Design first; proposal list in
  HERDER_FIXROUND.md).

## Deferred (standing: never over real work)

- Agent env via host DASHERD_CHILD_* instead of the powershell wrapper
  (token visible in host command line today, as it was before; harden
  later, way later).
- WebSocketServer handle leak at rich-client exit.
- Lease heartbeat starvation under multi-second frame stalls (no repro).
- Diff BEFORE/AFTER one-frame scroll desync (cosmetic).
- ImGui Install/RestoreCallbacks vs chain prev caches (latent, zero
  in-tree callers).
- mcp_supervisor.py ping while a tool call blocks; URL-encoding of query
  values (watcher ids/tokens are URL-safe).
- Watcher-ring transcript/history view for repainting TUIs (the eventual
  answer to "scrollback" for Ink-style agents).

## Decision points for Boris

1. **The PR.** Sequencing settled 2026-07-25 says: whole round on this
   branch, one PR for the record, big review on that PR. The branch now
   carries the fix round AND the host arc (~25 unpushed commits). Cut the
   PR after Phase 0 proves the deploy, or keep riding until Phase 1 lands?
   Preflight budget unchanged: one full run, none used.
2. **Phase 2 vs Phase 3 order.** External sessions makes today's work
   visible; the observer pays down review capacity. Plan assumes external
   first (it is smaller and Phase 3's reviewer summons benefit from it).
3. **When ssh becomes real** — pull Phase 5 forward or leave it parked.

## Design: worktree/session deletion safety (note 58, 2026-07-27)

Boris hit a wall twice trying to retire an unused worktree: the app knows
nothing he needs and shows nothing he can act on. The answer is not a
label — it is a computed VERDICT carrying its reasons.

### The question a user actually asks

"If I delete this, do I lose anything?" It decomposes into five facts,
every one of them cheap:

| Fact | Source | Meaning when bad |
|---|---|---|
| in use | herd registry + running PTYs (already known) | someone is working there NOW |
| dirty | status porcelain (already counted per worktree) | uncommitted edits die |
| untracked | same (already counted) | files git never saw die |
| stashed | stash list filtered to that worktree | stashed work dies |
| unmerged | rev-list count BASE..head | commits exist nowhere else |

### CORRECTION (2026-07-27, after Boris asked "worktree yes, branch no is how?")

The tiers below were written for the wrong operation. Proven in an isolated
probe: `git worktree remove` — the only thing dasHerd runs — never touches
the branch, so committed work is never at risk from it. What the probe DID
destroy: an ignored file, silently, while `status --porcelain` reported the
tree clean (note 59). So for worktree removal:

- committed work: never at risk (the branch keeps it)
- uncommitted / untracked: already blocked by the dirty guard
- IGNORED files: silently destroyed — the only real loss, and unguarded

The unmerged count therefore is NOT a safety gate for removing a worktree;
it answers "is this workspace still carrying unfinished work" — an
abandonment signal, which is what "is this still needed?" really asks.
The destructive/recoverable/safe ladder below is correct only for BRANCH
deletion, which dasHerd does not offer yet. If we add it (retiring a
branch+worktree pair together is the natural user intent), the ladder and
its base-selection trap apply exactly as written.

### The trap: which BASE

WorktreeState's ahead/behind are measured against the branch's own
UPSTREAM — that answers "is it pushed", NOT "is it merged". Both matter,
and together they give a three-tier verdict:

- unmerged AND unpushed: deleting DESTROYS work. Red.
- unmerged but pushed: the remote still has it; the worktree is
  disposable, the branch is not. Amber, with the recovery command.
- merged into the integration ref: nothing to lose. Green.

The base must be the AUTHORITATIVE ref (origin/master), never local
master. Measured live: codex/fix-ci-30233791631 reads 45 unmerged against
local master and 0 against origin/master, because local master sat 108
behind. The convenient base inverts the answer.

### Proposal

1. Extend WorktreeState with unmerged_commits, merge_base_ref,
   has_upstream, stash_count, claimed_by (session name or ""), and a
   derived delete_safety = safe | recoverable | destructive | in_use.
2. Put the verdict where the decision happens: a badge on every worktree
   row and session card, colored by tier, tooltip listing the reasons.
   Absence of a session marker must READ as "no session" — an empty space
   is indistinguishable from an unimplemented feature, which is exactly
   the confusion Boris hit.
3. Make delete a CHECKLIST, not a confirm: the five facts with pass/fail,
   the tier verdict, and the literal recovery command for the amber case.
   Blocked cases keep note 41's resolver-session offer.
4. Tree/History: mark commits already contained in the base, so "3
   commits on a purple line" reads as "3 commits, all in origin/master".
5. Recompute on the git observer's cadence, and ALWAYS name the base in
   the UI ("vs origin/master") so no number is ever ambiguous.

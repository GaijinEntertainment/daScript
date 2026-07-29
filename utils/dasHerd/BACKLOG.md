# dasHerd backlog and plan

Status: plan of record, 2026-07-26. History and per-note forensics live in
HERDER_FIXROUND.md; arc designs in PTY_HOST_DESIGN.md and
AGENT_REVIEW_WORKFLOWS.md. Rule of the round stands: bugs over everything;
review capacity is the bottleneck the arcs exist to fix.

## Filed 2026-07-29 (live review round 2)

- **Search by file name, across worktrees.** The Project tab's filter only
  narrows the SELECTED worktree's tree. Boris looked for
  LANGUAGE_SUPPORT_PLAN.md and could not find it — it existed one worktree
  over. Wanted: a name search over ALL worktrees of the repository (each
  worktree is a branch, so this is cross-branch by construction), results
  labeled with the branch @ folder identity, click = select that worktree
  and open the file.
- **Which Changelist tab is home?** Boris's mental model of "the
  changelist" includes the branch's COMMITTED files (the PR result view);
  the arc's tab split made Changes (working-tree dirt) the default, so a
  committed file "disappeared" from the window he was looking at. Interim:
  the clean-tree message now points at the PR result tab. To discuss:
  default to PR result when the tree is clean, merge the two views into
  one grouped list (CONFLICTS/STAGED/MODIFIED/UNTRACKED + COMMITTED vs
  base), or keep tabs and remember the last choice.
- **Comment on a selection, addressed to an agent.** Reading a doc in the
  inspector, Boris can select text but cannot SAY anything about it. Wanted:
  select (in View, rich markdown or plain source) → right-click → "Leave a
  comment" → typed note travels with the byte-range focus target to an
  agent, riding the existing Look-at-that mailbox shape. Prerequisite worth
  doing at the same time: the session DRIVING dasHerd development (this
  Claude Code session) is not herd-managed, so there is nobody to address —
  adopt the already-running external terminal as a herd session so the
  reviewer can talk back to the builder. Filed 2026-07-29, deliberately not
  built yet — more feedback incoming.
- **STANDING RULE: every text view gets IDE-quality search.** Rich or
  plain, diff or not. VSCode find-widget feature set (regex/case/word,
  count, prev/next, Ctrl+F/Esc). Current gaps: the markdown View branch
  skips draw_inspector_search entirely (where Boris reviewed the plan),
  and Diff mode has no search at all. Editors additionally get
  search/replace/replace-all from day one; per-tree search is the
  follow-up slice. Declared 2026-07-29 after the first full in-app plan
  review — the first real win.
- **Inspector auto-reload on change.** The inspector captures content at
  inspect time and never watches the disk; a refresh icon on the header
  ships today (2026-07-29). The real fix: when a repository refresh lands
  and the INSTALLED file's status row changed (code or stats), re-inspect
  automatically with the installed identity — stale-while-revalidate
  already keeps the swap flicker-free. Watch generation guards: a user
  click mid-auto-reload must win.
- **Terminal file-link navigation (PLAN T3, now with concrete shape).**
  When a session's terminal shows a path (agents print them constantly),
  clicking it opens the file in the inspector, resolved against THAT
  session's worktree — many trees exist, the session's own tree is the
  answer. Detection: worktree-relative or absolute paths in terminal text,
  the linkifying done in the renderer's visible viewport only.
- **Right-click → Find in the terminal (the fallback door).** Select text
  in any terminal (ours or an adopted external one) → right-click → Find:
  treat the selection as a file-name query against the session's worktree
  (the cross-worktree search above, seeded and scoped). Deliberately
  duplicates the click-a-link path — selection survives even when the
  linkifier fails to recognise a path.
- **Drag-and-drop: the herder HOLDS the payload (Boris's design turn,
  2026-07-29).** Do not squeeze anything through the PTY beyond a
  reference. A drop (or paste) onto a session becomes a herder-held
  artifact: stored in that session's space (beside events/mailbox/
  bundles, same durability rules), then DELIVERED as a reference — the
  path typed into the PTY, or an inbox message carrying it with
  provenance. Images just work everywhere because agents read files by
  path (Claude Code reads PNGs natively); "or anything else" holds — any
  payload is a held file plus a reference. This also unlocks: clipboard
  image paste into a session, Look-at-that with an attachment, and
  agent→agent artifact handoff through the same store. Baseline
  fallback for plain shells: quoted path typed into the PTY. GLFW gives
  the host the drop callback; the watcher owns storage + delivery.
- **"Where is the file, really?"** A file's location is three-dimensional
  (repository, worktree/branch, path) and the app shows only the path. Any
  file surface should answer: which worktrees contain this path, which is
  the one I am looking at, and does the content differ between them. First
  slice: the search above listing every worktree that has the file.

- **Input lock must be visible in the title bar.** When @live synth input
  holds the app (`set_user_control(false)` — which `imgui_click` takes
  implicitly), the window title must read
  "dasHerd - is being controlled by @live"; today the app just feels dead
  until control is handed back, and Boris hit exactly that. Title reverts
  when `user_control` returns true. Consider the same marker in
  `herder_client_state`. (Also a driver-discipline rule for Claude:
  every synthetic interaction ends with `set_user_control {enabled:true}`.)

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

## NEXT ARC — the Changelist becomes a working surface (Boris, 2026-07-27)

PLANNED IN FULL: **CHANGELIST_ACTIONS_PLAN.md** — read that before starting;
it carries the git command table, the discard semantics per group, the test
plan, and the ordered checkpoints.

Where the retire scenario LEADS. Boris, coming out of it: "im going from
delete scenario - but it brings 'ok, unstaged files, uncommited files - what do
i do'." The delete flow answers "is it safe"; it then hands the user a dirty
worktree and no way to act on it. Today the Changelist has exactly one control:
Refresh.

His list, in his order: stage/unstage/discard with multiselect and
confirmations; commit; fetch/push/pull/sync.

Three things the plan settles that are easy to get wrong:

- A prerequisite refactor comes FIRST: `advance_operation`'s per-kind phase
  ladder becomes a step list with a per-step failure policy. Nine more kinds on
  the current shape would rot it.
- **Discard is the first genuinely unrecoverable action in the app** — retire
  never loses a commit, archive comes back, worktree removal keeps the branch.
  Discard destroys the only copy, so it names the count, offers the WIP-commit
  rescue first, and arms in two steps. Stage/unstage get NO confirmation.
- `git pull` is never bare — `--ff-only`, because a surprise merge commit is
  the same class of silent lie this round kept removing.

## Retiring a workspace — BUILT 2026-07-27 (design below stands as the record)

Both doors, the verdict, and the archive rule are in the build:

- WorktreeState carries the delete-safety facts: `upstream_ref` (free, parsed
  off the status branch line), `unmerged_commits` / `unmerged_base_ref` /
  `unmerged_known` measured in a new refresh phase against the AUTHORITATIVE
  base (`origin/master`, one retry on `origin/main`, then "unknown" — never
  local master, never a silent zero).
- `repository_worktree_delete_tier` is the settled ladder: in_use / main /
  dirty / warn / green. `repository_worktree_only_here` needs BOTH unmerged
  and unpushed, and an unknown count never manufactures a warning.
- Door 1 (worktree): "Retire worktree..." always opens the CHECKLIST — the
  verdict comes before the attempt, not after a refusal. Escapes: commit WIP
  to the branch (`repository_commit_worktree_wip`, refused on detached HEAD),
  the note-41 resolver session, and an explicit "delete anyway" that names the
  files it loses (`--force`, never defaulted).
- Door 2 (session): "Retire session..." archives the record and OFFERS each
  worktree with its own verdict; blocked ones are checkboxes you cannot tick.
- `herd_archive` / `herd_restore` replace `herd_delete` — note 60 closed. The
  record survives archived, hidden behind a "Show N archived" toggle, and
  holds no worktree while archived.
- Rails, so the verdict is inspectable and every action commandable:
  `herder_worktree_retire_state` (tier + every fact + the base ref each was
  measured against), `herder_worktree_retire`, `herder_worktree_commit_wip`,
  `herder_session_retire`, `herder_session_restore`.
- Rows answer the question where it is asked: a muted "no session" mark (an
  empty space read as an unbuilt feature) plus a row tooltip carrying the
  verdict in words.

SETTLED (Boris, 2026-07-27): archiving does NOT move the session's
`events.jsonl` / `mailbox.jsonl` / `bundles.jsonl`. They stay where they are.
Note 60 asked for two things — keep the record AND move the files beside the
archived host files — and only the first one was load-bearing: the files were
orphaned BECAUSE the record was erased, so a surviving record gives them an
owner again. The move was tidying, and it would fight restore, which is the
point of archiving: the watcher holds live paths to those files, so moving
them can break reads and restore would have to move them back. Leaving them
costs nothing. (The host stamp, journal and log still self-archive on host
exit, as they always did.)

NOT PROVEN LIVE yet: unit tests + lint are green and the base-ref trap is
reproduced (this worktree reads 81 unmerged vs local master, 9 vs
origin/master, local master 108 behind), but no dialog has been driven in a
running herder. UI is never proven from logic — that is the next session.

## Retiring a workspace — the two scenarios (Boris, 2026-07-27)

Boris named the shape: two entry doors into ONE operation, because a
session and its worktree are a pair and a live session's worktree is
hard-blocked anyway.

### Scenario 1 — worktree first: "is it safe to delete?"

The question is "is there work in here that will be lost". Two layers,
and only one of them is loss:

- WORKING TREE (uncommitted + untracked) — genuinely lost on delete.
- BRANCH (commits not in the base) — NOT lost; the branch survives
  `git worktree remove`. It means "unfinished", not "at risk".

So the offer is a verdict with an escape hatch per tier:

- clean -> **Delete** (green; nothing to lose).
- dirty -> a MECHANICAL rescue that needs no judgment, then delete. The
  cheapest complete one is committing the WIP to the branch: the branch
  outlives the worktree, keeps its name, and stays visible in the branch
  list. (`git stash -u` also survives — stashes are repo-level, not
  per-worktree — but it is easy to forget a stash and hard to find it
  later; prefer the named branch.)
- dirty and interesting -> **"Start a session to sort it out"**, which
  ALREADY EXISTS (note 41): the blocked-delete dialog briefs a resolver
  session with the exact blocking state. What is missing is that it is
  REACTIVE — you only meet it after a delete attempt is refused. It
  should be offered from the verdict, before you try.
- unknown -> **"Start a session to figure out what is here"** — the same
  launcher with an investigate brief instead of a resolve brief.

Open question for Boris: "move changes to main" — main = the master
BRANCH (put the work on master) or the main CHECKOUT (move the files to
D:/Work/daScript)? Deciding what belongs on master is judgment, so that
one reads like the agent path rather than a button; committing to the
branch is the button.

### Scenario 2 — session first: "delete this session"

Offer to delete its worktree in the same act. Mostly built already:
herd_delete removes the record and then attempts each worktree under the
same guards, reporting removed/kept. What is missing is that it ACTS
instead of OFFERING — it should present the same verdict/checklist and
let the user choose, and it should say what happens to the session's own
artifacts (records, journal, host log): deleted with it, or archived.

Consequence Boris drew, and it retires the note-59 alarm: herder files
belong to the SESSION, live in the watcher's tree, and a live session's
worktree cannot be deleted — so worktree deletion never endangers them.

### SETTLED RULES (Boris, 2026-07-27)

Two entry doors, one operation:
1. kill/close the session, which then offers to delete its worktree;
2. delete a worktree directly — the same operation, blocked while a
   session holds it.

| condition | outcome |
|---|---|
| a session holds the worktree | BLOCK — retire the session first (door 1) |
| uncommitted OR untracked present | BLOCK, with rescue: commit WIP to the branch, launch a resolver session, or an explicit "delete anyway, N files lost" |
| commits exist only on this machine (unmerged AND unpushed) | WARN only; a push helper comes later |
| otherwise | GREEN |

ARTIFACTS — settled, and it is a standing rule, not a preference for this
feature (Boris, 2026-07-27): "if u ever hear 'delete' from me - slap me.
boris never delets. boris may forget to save, but stories stay." So
retiring a session ARCHIVES it: the record survives in an archived state,
hidden from the default list but retrievable, and its artifacts move
beside the other archived host files. Nothing about retiring a workspace
destroys history. See note 60 — today's herd_delete violates this by
erasing the record and orphaning the files it leaves on disk. The wording
follows too: a control that archives must not be labelled "Delete".

SCOPE (Boris, 2026-07-27): no "move to main" button — if someone wants the
work they check the branch out. Two adjacent scenarios are explicitly
SEPARATE and LATER, not part of this one:

- "show me uncommitted branches" (which branches carry work that exists
  nowhere else);
- "delete local branches no longer in use" — where the destructive /
  recoverable / safe ladder further down finally applies.

Two wording rules that keep it honest:

- The unpushed WARNING is about REDUNDANCY, not risk. Deleting a worktree
  cannot lose commits — the branch keeps them. Phrase it "N commits here
  exist only on this machine; deleting this folder will not lose them,
  nothing else has a copy." A warning that implies the delete destroys
  them would be a lie, which is the class of bug this round keeps finding.
- Untracked-as-blocker needs a way through. Untracked is usually junk, and
  a block with no escape sends the user to clean by hand or spawn an agent
  for nothing. List the files and let them proceed explicitly.

### The leftover

Neither scenario deletes the BRANCH, so retired workspaces leave their
branches behind. That is where the destructive / recoverable / safe
ladder below actually applies, if we ever offer it.

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

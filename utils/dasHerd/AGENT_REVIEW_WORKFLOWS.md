# Agent-assisted review workflows

Status: bidirectional local Review Focus and multi-repository debug-session
foundation implemented; GitHub PR surface remains next

This note records the two agreed product tracks that follow a usable local Git
inspector. Both tracks will be implemented. The local Review Focus vertical
slice comes first, followed by the GitHub PR surface.

1. Make local Git review useful for preparing and reviewing a PR through
   explicit human-agent attention handoff.
2. Add a GitHub PR surface for status, body, CI, comments, logs, and supervised
   review operations.

They are not separate interaction models. Both should use one inspectable
`Review Focus` contract.

## Implemented first slice

The watcher now owns an append-only `mailbox.jsonl` in every session artifact
directory plus authenticated localhost HTTP and WebSocket projections. Inbox
and Outbox are visible in the web control UI with sender, lifecycle, reply
identity, delivery/fetch state, and raw JSON. The watcher assigns provenance
from the routed direction/session instead of trusting a payload sender string.

`utils/dasHerd/dasherd.ps1` implements `whoami`, Inbox list/get/ack/complete,
and Outbox send/reply. URL, token, and session routing come from
`DASHERD_URL`, `DASHERD_TOKEN`, and `DASHERD_SESSION_ID`, with explicit flags
available for diagnostics. `utils/dasHerd/dasherder.md` is the agent-facing
protocol note. Fetching a specific Inbox item records `fetched`; the one-step
human action stores before sending an atomic TTY wake and records notification
success or failure.

The rich client consumes Outbox messages without changing repository,
worktree, installed file, Diff/View mode, scroll, raised window, terminal, or
keyboard focus. Its existing Sessions window contains persistent Attention;
agent-targeted files carry an `[AGENT]` marker in changelists. Only clicking a
message or file target navigates. Active source focus supports multiple
disjoint UTF-8 byte ranges, dimmed surrounding text, direct file navigation,
previous/next target navigation, and a show/hide toggle. A code selection's
right-click **Look at that** action atomically posts the exact installed
repository/worktree/comparison/path/range to the selected session Inbox.

The watcher can now create a dedicated branch/worktree in a visible Git task
session and launch Codex there only after creation succeeds, or launch on an
explicitly selected existing worktree. It authors a durable `context.md`, injects the watcher route,
watcher-assigned session ID, purpose, and context path into the process, and
retains purpose/task/origin in session state. The launch token is deliberately
removed from persisted `session.json`.

An agent may declaratively sync one logical Review Bundle spanning several
registered repository/worktree participants. Each participant owns its own
claimed file set and revision metadata; focus targets may override repository,
worktree, comparison, and revision per file. A ready bundle is projected to one
retry-safe Attention item. The Sessions UI shows the debug context, immutable
origin, bundle status, and every participant; selecting a participant switches
the ordinary Git surfaces to that exact repository/worktree without changing
the session origin.

The deterministic multi-repository acceptance scenario launches Codex only
after its dedicated worktree is observed, keeps that launch worktree as the
session origin, registers a second repository, and publishes one bundle with
participants and exact focus in both. It also proves terminal ownership is a
lease rather than process ownership: a second client can observe while control
is held, explicit detach hands control over, and an ungraceful socket close
releases it for the next client without terminating the agent.

Live commands expose Attention state and explicit target opening for semantic
verification. The next product track is the GitHub PR surface described below;
MCP remains deliberately deferred.

## Shared contract: Review Focus

A Review Focus is a visible, clearable set of things that a human or agent asks
the other party to inspect. It must be structured state, not prose inferred
from a terminal and not an invisible filter.

A focus item identifies:

- repository, workspace/worktree, and revision or comparison;
- a target such as file, symbol/function, source range, ref/SHA, PR, check,
  review thread, or log range;
- a role such as primary, risk, question, failure, context, or plumbing;
- a short explanation of why it matters;
- origin (human, agent, or tool), session, and creation time;
- acknowledgement or resolution state; and
- enough snapshot identity to report when the target has become stale.

Source targets use the existing canonical identity: workspace, worktree-
relative path, revision/comparison, and UTF-8 byte or line range. Symbol names
help a human read a focus item but do not replace the source range and revision.
GitHub targets add stable repository, PR, check-run, thread/comment, and log
identities without weakening the local source anchor.

The UI may subdue unrelated material, but it always shows who supplied the
focus, why each item is present, and how to clear it or return to the ordinary
Auto/All view. Focus never changes Git state by itself.

## Human-agent communication protocol

The rendering is deliberately simple: focused files/ranges keep their ordinary
or brighter treatment, unrelated material is darkened, and a toggle restores
the unfiltered view. The protocol that delivers focus state is the harder
contract.

### Agent to dasHerd

An agent-facing `dasherder.md` skill explains the available operations and how
to discover the current session. The agent publishes structured focus sets,
notes, acknowledgements, and results to dasHerd. The first supported agent
surface is a small `dasherd` CLI. It speaks to the local watcher protocol, reads
the current session context from its environment or explicit flags, and offers
stable human-readable and JSON output. MCP is explicitly deferred until the mailbox and focus
contract have enough real use to justify its support cost. Raw localhost HTTP
is an implementation/debugging substrate, not the initial agent-facing
contract.

The agent should not have to synthesize UI input or emit terminal prose for
dasHerd to scrape. A typical operation is semantically:

```text
dasherd outbox send --kind focus --payload focus.json
```

### dasHerd to agent: durable payload plus wake-up

Local mailbox state cannot wake a passive agent model. Conversely, pasting the
complete request into a TTY is fragile, noisy, difficult to acknowledge, and
unsafe for large source selections. The proposed transport therefore splits
notification from payload:

1. dasHerd stores the complete structured request in a durable per-session
   inbox and assigns a stable message ID.
2. The same explicit UI action immediately injects one short natural-language
   notification through the agent's normal TTY input path.
3. The notification tells the agent to use the dasHerd skill and fetch that
   exact message ID.
4. The agent fetches the payload through the CLI and acknowledges it.
5. dasHerd shows new, notification success/failure, fetched, acknowledged, and
   completed state rather than assuming that pasted text was read. Cancel and
   stale transitions remain extensions of the same record.

For example, the visible TTY message can be:

> dasHerd review request `hf_42` is ready (3 source anchors). Use the dasHerd
> skill to fetch and acknowledge it.

The nudge is intentionally sufficient for the model to choose the skill but
does not duplicate the source payload in terminal scrollback or context.

A future direct agent callback may remove the TTY wake-up for agents that
support true push. It is an optimization of the wake transport, not a
replacement for the durable inbox and acknowledgement model. Pure polling is
not the default because an idle agent has no reason to take another turn.

### Per-session Inbox and Outbox

Inbox and Outbox are named relative to the agent session:

- **Inbox** contains human or dasHerd-system requests addressed to that agent.
- **Outbox** contains focus sets, replies, notes, and results published by that
  agent for the human/dasHerd.

The first implementation includes both views in the web UI for every session.
This is not deferred until rich focus rendering. At minimum each mailbox shows
message ID, time, sender, kind/subject, lifecycle state, reply/correlation ID,
notification/fetch state, and a readable payload. A raw structured-payload
view is available from the start so protocol mistakes can be diagnosed by eye.

There is no required compose, preview, or confirmation stage for a contextual
request. The baseline interaction is: select code in Diff/View, right-click,
choose **Look at that**, and return to work. That menu action immediately
creates the Inbox message from the selected revision/path/range and queues the
TTY notification. Selection alone sends nothing. If notification fails, the
durable message remains visible and retryable in the mailbox.

The CLI begins with operations equivalent to:

```text
dasherd whoami
dasherd inbox list
dasherd inbox get <message-id>
dasherd inbox ack <message-id>
dasherd inbox complete <message-id> [--message <text>]
dasherd outbox send --kind <kind> --payload <file>
dasherd outbox reply <message-id> --kind <kind> --payload <file>
```

### Sender identity and provenance

Sender is a routing/provenance label, not an authenticated principal. It is
assigned by dasHerd from the mailbox operation rather than copied from an
arbitrary message-body field:

- Posting to session `<id>`'s Outbox stamps the sender as
  `agent_session:<id>`. The CLI defaults `<id>` from `DASHERD_SESSION_ID`.
- The local web UI stamps the sender as `human:local` in the single-user first
  slice and records its client/connection ID separately for diagnostics.
- Watcher-generated lifecycle notices use `system:dasHerd`.

A later multi-user or cross-agent design can add more provenance labels without
changing message identity. Cross-session agent messaging is not exposed in the
first CLI because it is outside the initial workflow, not because dasHerd
claims to prevent it. Replies carry `reply_to`/correlation identity rather than
copying or overwriting the original sender.

### No Herder permission model

dasHerd deliberately does not implement agent permissions, per-operation
approval gates, or a capability sandbox. The local agent already has the
machine/worktree authority to edit files, run Git and `gh`, and call localhost.
Adding nominal Herder restrictions would not create a meaningful security
boundary and would import support cost and permission theater from other agent
clients.

The CLI accepts launch context containing the localhost endpoint and default
session ID through environment variables; tighter launcher injection can be
added without changing the protocol. A connection token may still exist to
prevent accidental cross-talk with unrelated local pages/processes, as the watcher already does;
it is transport hygiene, not authorization or a claim that the agent lacks
authority.

Sender labels, mailbox direction, and event history make behavior observable
and debuggable. They are not security attestations. Likewise, a human clicking
a proposed GitHub action is a product interaction, not a permission boundary:
the same trusted agent could perform that operation directly through its shell.

### Delivery rules

- Human-to-agent delivery is explicit but one-step. Invoking **Look at that**
  is the send action; there is no mandatory composer or second confirmation.
- The TTY nudge uses the same serialized input ownership as ordinary terminal
  input; it must not interleave with a human's partial line or paste.
- Message IDs keep notification retries tied to the existing Inbox payload so
  acknowledgement state is not lost when retry UI is added.
- The inbox is authoritative. TTY text is only a wake signal and never proof
  that the agent received, understood, or completed the request.
- Agent profiles may replace TTY notification with a stronger native wake
  transport, but every profile exposes the same message lifecycle.
- A Markdown review brief remains the portable export. It can reference the
  same message/focus IDs, but it does not become the authoritative mutable
  inbox record.

## Next vertical slice: one scenario each way

The next implementation slice is complete only when the real PR-ready focus
scenario works agent-to-human and one corrective source-range handoff works
human-to-agent against a running agent session.

### Agent points human at code

The agent publishes one Outbox Focus Set with the summary:

> Ready for PR. Check it out; I've highlighted the important bits.

The set contains several revision-aware targets at once: whole files that are
important as units and multiple source ranges/sections inside other files. A
file target with no ranges means the whole file matters. A file target may
contain several disjoint ranges. dasHerd adds one durable entry to a non-modal
**Attention** panel backed by the session Outbox and marks every affected file
in the Changelist.

Arrival never steals the human's workflow. It does not select a repository or
worktree, replace the inspected file, switch Diff/View, scroll, raise a window,
or take keyboard focus. If a targeted document is already displayed, all of its
ranges may be highlighted in place without moving the viewport. Other targeted
files receive Changelist/Attention markers but are not opened.

At a time of the human's choosing, clicking the Attention entry activates that
Focus Set and navigates to its first important target. The Attention panel also
lists every file/section for direct navigation. Focus mode keeps targeted files
and ranges ordinary/brighter while subduing unrelated PR plumbing; a toggle
restores the normal undimmed view without discarding the set. Previous/next
file navigation walks the set's targets; previous/next highlight navigation
walks the ranges in the current file. An optional range `caption` appears as a
hover hint explaining why that section matters. The entry remains inspectable
and can be dismissed or resolved independently.

### Human points agent at code

The human selects one source range, right-clicks, and invokes **Look at that**.
That one action stores the exact revision/path/range in the session Inbox and
sends the short TTY nudge. The agent uses the CLI to fetch and acknowledge the
message, then can publish a reply or focus result through its Outbox.

### Acceptance evidence

- Before/after live state proves agent-message arrival did not change selected
  worktree, installed file, mode, scroll position, active window/widget, or
  keyboard focus.
- The test Focus Set contains multiple files and multiple disjoint ranges in at
  least one file. The Attention panel and mailbox expose its summary, every
  anchor, sender, lifecycle, and raw payload immediately.
- Clicking the Attention entry is what changes navigation. All targeted files
  are marked, all ranges for the opened file are highlighted, previous/next
  walks the set, and the focus toggle restores the ordinary undimmed view.
- The reverse request contains the human's exact selected source anchor; the
  Inbox records notification, fetch, and acknowledgement state.
- The demonstration uses the real CLI, watcher, web UI mailbox, rich client,
  and agent TTY rather than a test-only shortcut.

## Track A: local PR preparation and review

### Agent to human

The agent can publish a review focus equivalent to:

> Look at these two files and these three functions. The remaining changes are
> mechanical PR plumbing.

The Changelist emphasizes selected files and labels supporting/plumbing files.
Opening Diff or View emphasizes exact symbols and ranges while retaining the
complete file. Tree applies the same idea to topology: it shows the refs, SHAs,
and ancestry paths named by the agent, with unrelated branches subdued.

This supports:

- a PR-preparation summary of important implementation and risk areas;
- a reviewer guide through semantic changes hidden among generated or
  mechanical files;
- questions and caveats attached to exact source ranges; and
- an agent-directed Tree focus for primary, incoming, and base/context paths.

### Human to agent

A human can select files, functions, source ranges, commits, or Tree paths and
turn that selection into either:

- an immediate **Look at that** Inbox request plus the short visible TTY nudge;
  or
- an exported Markdown review brief that the human can ask the agent to read.

The Markdown artifact is the portable human-readable projection of the same
structured focus. It lists canonical anchors, selected text only when useful,
and the human's notes/questions. A machine-readable representation may travel
beside or inside it, but the `.md` must remain sufficient to inspect, edit, and
discuss without dasHerd.

Selecting text never sends by itself. Invoking the named context-menu action is
the deliberate send; the resulting request and delivery state are immediately
visible in the session mailbox. Markdown export remains the editable path for a
larger curated brief.

## Track B: GitHub PR observation and supervised operation

When a session/worktree has an open PR, dasHerd indicates that association and
offers a PR Status surface containing:

- PR identity, state, base/head, mergeability, and current review summary;
- rendered PR body plus a locally autosaved editable Markdown draft and an
  explicit conflict-aware publish action;
- CI/check rows with live state, timestamps, and links;
- review comments and threads, including replies and resolution state; and
- failure logs that can be opened, searched, selected, and attached to a
  Review Focus.

The human can point the agent at a failing check, a red log range, or a review
thread using the same handoff as source code: generated visible prompt or
exported review brief. The agent can answer questions such as "why is this
broken?" against exact observed state rather than a pasted approximation.

Copilot and other automated-review interaction should use supervised
suggestions: dasHerd presents a proposed accept, reject, reply, resolve, rerun,
or merge operation as a ghost action with its rationale and exact target. The
human may approve, reject, or add a comment. Read/monitor work can be automatic;
remote mutations remain explicit and observable.

The existing babysit workflow should ultimately consume this state directly:
monitor checks and reviews, prepare proposed responses/actions, surface only
decisions that need a human, and merge when the user's declared policy permits.

### The observer model: tool-driven, not agent-driven (decided 2026-07-26)

Watching is software's job; judgment is the agent's. Agents must never poll —
an agent context re-running `gh pr view` on a timer is the most expensive
polling loop ever devised, and honest software does the same watch better for
100500x less. The watcher grows a **GitHub observer**: dumb, cheap, tireless —
it polls PRs/CI/reviews, diffs observed state, and acts only on transitions.

The lifecycle is inverted — the tool drives the agent ("PR slave driver"):

- **Arm**: when a PR is about to open or opens, the observer tells the
  session's agent "we are about to do a PR, sit tight" — context delivered
  once, no watching duties assigned.
- **Summon**: on a meaningful transition the observer wakes the agent with a
  delta briefing — "CI lane X went red after commit Y", "review question Z
  arrived" — so tokens are spent on the judgment call, never on discovering
  that nothing changed.

Signal hierarchy, hard before soft: **a CI lane going red is a 100% real
failure** and outranks every advisory signal. "Copilot says it's bad" is
cute — an input to judgment; "test failed" is truth — an event that summons.
Attention ranking follows deviation, not activity: changes outside the
declared Review Bundle focus, failures appearing after a force-push, repeated
failed fix attempts on the same lane escalate to "needs looking into, for
reals"; everything else stays auto-trusted with a one-line receipt.

## Proposed order

Implement Track A first, beginning with the shared Review Focus model and its
two-way handoff, then build Track B on that contract.

Reasons:

- The local Git inspector has just become usable, so this is the shortest path
  from a viewer to a genuinely useful review tool.
- File/symbol/range/ref anchoring is the common semantic foundation. CI lines,
  comments, and logs can later become additional focus target kinds instead of
  inventing a second selection and communication system.
- The complete human-agent loop can be tested deterministically against local
  repository state before adding authentication, API limits, changing remote
  state, and provider-specific behavior.
- It establishes the agent-facing publish/consume contract and the human-facing
  prompt/Markdown export contract that GitHub babysitting will need anyway.
- GitHub work can then concentrate on observation and supervised operations
  rather than simultaneously discovering the underlying interaction language.

The strongest argument for Track B first is immediate operational value for
already-open PRs: status, CI, comments, and babysitting are useful before rich
source focus exists. That remains a valid priority if remote operations are the
near-term bottleneck. The tradeoff is a higher chance of building GitHub-
specific selection/actions that must later be reconciled with source and Tree
focus.

## Suggested implementation sequence

1. Define the generic message envelope, lifecycle/event history, routing/
   provenance labels, and per-session Inbox/Outbox storage.
2. Implement the session-aware CLI context and `dasherder.md` skill for
   list/get/ack/complete/send/reply.
3. Expose Inbox and Outbox in the web UI, including readable and raw payloads,
   lifecycle state, cancel, and retry-notification controls.
4. Add the short serialized TTY wake-up and prove store-before-notify,
   idempotent retry, acknowledgement, and failure behavior.
5. Define the Review Focus payload, revision/staleness rules, and live
   inspection surface on top of the mailbox.
6. Render agent-to-human file, source, and Tree focus, then capture human
   selections as immediate contextual requests and optional Markdown review
   briefs.
7. Add read-only GitHub PR association, body, checks, comments, and logs;
   extend focus targets to GitHub state; then add supervised ghost actions and
   babysit/merge policy.

Each stage must expose semantic state to live commands. Screenshots are useful
for presentation, never the correctness oracle.

## Discussion record

- 2026-07-21: Both tracks are committed product direction; only order is open.
- 2026-07-21: Local review should support agent-selected important files,
  functions/ranges, and Tree branches while treating the rest as plumbing.
- 2026-07-21: Human selections must travel back to an agent through a visible
  generated prompt or an editable/exportable Markdown review brief.
- 2026-07-21: GitHub review should cover PR body, CI, comments, logs, questions
  about exact failures, Copilot review, supervised actions, and eventual
  babysit-to-merge operation.
- 2026-07-26: Track B runs on a watcher-side GitHub observer with a
  tool-driven agent lifecycle (arm/summon); agents never poll. Hard signals
  (CI red) outrank soft ones (Copilot verdicts); attention ranks by deviation
  from the declared bundle, not by activity. Boris: review capacity is the
  bottleneck — the PR-list rate resembles a medium-size startup — so the
  next big UI after the terminal arc is the GitHub PR assistant, and key
  views are the regular workflow (human in terminals, human directing agents
  into terminals) plus reviewing copilot interaction with an honest
  "this one needs looking into, for reals" escalation instead of forced trust.
- 2026-07-21: Proposed preference is local Review Focus first, GitHub second,
  because Review Focus is the shared interaction foundation.
- 2026-07-21: Adopt local Review Focus first and begin protocol design before
  UI implementation.
- 2026-07-21: Proposed human-to-agent transport is a durable per-session inbox
  fetched through the CLI, paired with a short visible TTY wake-up and an
  explicit acknowledgement lifecycle.
- 2026-07-21: Make the CLI the first and only supported agent transport; defer
  MCP until real mailbox usage justifies its support cost.
- 2026-07-21: Ship visible per-session Inbox/Outbox views in the web UI with the
  first protocol implementation, including raw payload inspection for manual
  debugging.
- 2026-07-21: Assign sender provenance from mailbox direction/session routing:
  `agent_session:<id>`, `human:local`, or `system:dasHerd`. These are observable
  workflow labels, not authenticated security principals.
- 2026-07-21: Human contextual delivery is one-step: select code, invoke
  **Look at that**, and immediately create the Inbox message plus TTY nudge.
  The visible mailbox is an audit/debug surface, not a mandatory composer or
  confirmation step.
- 2026-07-21: Herder has no agent permission or capability model. Session
  context supplies routing defaults, not authority; connection tokens are only
  transport hygiene against accidental local cross-talk.
- 2026-07-21: Scope the next implementation slice to the real PR-ready Focus
  Set (several important files and several sections per file) agent-to-human,
  plus one corrective source-range handoff human-to-agent. Arrival is non-modal
  and never changes navigation/focus; the human navigates only by clicking its
  Attention entry.
- 2026-07-22: Bind a debug session to a watcher-created worktree while allowing
  the cooperating agent to declare additional repository/worktree participants
  in one logical Review Bundle. Treat the launch origin as immutable routing
  identity, not as a limit on the work presented for review.
- 2026-07-22: Require a real black-box protocol test for the slice: two Git
  repositories, dedicated worktree and agent bootstrap, explicit detach and
  reattach, controller contention, disconnect release, declarative bundle
  replacement, and ordered durable lifecycle logs. Preserve diagnostics only
  on failure.

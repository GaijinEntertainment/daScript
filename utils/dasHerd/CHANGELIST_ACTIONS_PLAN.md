# The Changelist becomes a working surface — plan of record

Status: PLANNED 2026-07-27, not started. Written to be implementable from cold
context: every file, function and git command it names was verified against the
tree on the day it was written.

Where this comes from. Boris, coming out of the retire-a-worktree scenario:
"im going from delete scenario - but it brings 'ok, unstaged files, uncommited
files - what do i do'." The retire flow answers *is it safe to delete* and then
hands the user a dirty worktree with no way to act on it. The Git Changelist
window has exactly one control today: Refresh.

His list, in his order: stage / unstage / discard, with multiselect and
confirmations; commit; then fetch / push / pull / sync.

## What already exists (verified 2026-07-27)

- `repository_build_file_action_argv` (repository_core.das:577) handles exactly
  two actions, `stage` and `unstage`, ONE path each, and returns
  "file action is not implemented" for anything else. It validates the path
  through `repository_valid_relative_file_path` — that guard is load-bearing,
  every path in this arc reaches an argv.
- Per-row stage/unstage icons exist ONLY in the PR views
  (`draw_pr_not_included_files` / `draw_pr_included_files`, rich_git_ui.das).
  The status groups — `draw_git_file_group` (rich_git_ui.das:363), rendering
  CONFLICTS / STAGED / MODIFIED / UNTRACKED — have no action affordance at all.
- Groups are classified by `git_file_in_group` (rich_state.das:994) and mapped
  to a comparison by `git_group_scope` (staged for group 1, working otherwise).
- The single file action is single-flight, guarded by `git_action_loading` on
  the client, and it is reset to "interrupted" on disconnect (rich_net.das:115).
- The `WorktreeOperation` rail (repository_core.das) runs multi-step git on the
  task-session rail with progress, cancel, a command log, failure
  classification and the "start a session to fix this" escape. Built for
  retire; this arc is its second consumer.

## Step 0 (PREREQUISITE) — generalise the operation rail

`advance_operation` currently hard-codes a phase machine per `kind`
("remove" / "commit_wip") with `if (g_operation_phase == 1)` ladders. Adding
seven more kinds that way will rot. Replace the phase integer with a STEP LIST
before adding anything.

- `struct WorktreeOperationStep { argv : array<string>; label : string; kind : string }`
- `g_operation_steps : array<WorktreeOperationStep>` plus a cursor.
- `advance_operation` becomes: poll → log → on non-zero exit consult a per-step
  failure policy → advance the cursor → launch the next step or finish.
- Failure policy per step, because retire already needs three different ones:
  `fail` (stop), `recover` (a named follow-up step list — this is how
  "not a git repository" ➜ `remove --force` ➜ `prune` is expressed), and
  `tolerate` (non-zero is a normal outcome, e.g. `git diff --quiet`).
- Port `remove` and `commit_wip` onto it FIRST and re-run their tests green
  before writing any new action. This is a refactor with existing coverage —
  do not mix it with new behaviour in one commit.

Keep `step_index` / `step_count` semantics intact; the dialog already renders
them and the retire tests assert `step 2/2` and `3` after the prune recovery.

## Step 1 — a real selection model

Today "selected" means *the file the inspector has installed* — there is no
selection set. Add one in rich_state.das:

- `struct GitChangelistSelection { repository_id, worktree_path : string; keys : table<string; bool> }`
  where a key is `"{comparison}:{path}"`, matching the id the rows already
  build at rich_git_ui.das:386.
- A selection is scoped to ONE worktree. Changing worktree or repository clears
  it — a staged path from another worktree in the same argv would be a
  cross-tree write, which is the class of bug note 10 was about.
- Mouse contract, matching every file manager: plain click = select only this
  row AND open it in the inspector (today's behaviour, preserved);
  Ctrl+click = toggle this row, do not change the inspector; Shift+click =
  range from the last plain click within the same group.
- Ranges do not cross group boundaries. STAGED and UNTRACKED need different git
  commands, so a range spanning them cannot map to one action.
- The group header gains a tri-state "select all in group" checkbox.

Selection lives on the client only — the watcher receives explicit path lists.

## Step 2 — stage / unstage / discard on a selection

Extend `repository_build_file_action_argv` to take `paths : array<string>` and
add the discard actions. Git needs a DIFFERENT command per group, and getting
this wrong destroys work, so it is spelled out:

| action | applies to | command |
|---|---|---|
| `stage` | modified, untracked, conflicted | `git add -- <paths>` |
| `unstage` | staged | `git reset --quiet HEAD -- <paths>` |
| `discard_worktree` | MODIFIED (unstaged tracked edits) | `git checkout -- <paths>` — restores from the index, so a staged part survives |
| `discard_all` | STAGED (throw the change away entirely) | `git checkout HEAD -- <paths>` — resets index AND worktree to HEAD |
| `discard_untracked` | UNTRACKED | `git clean -f -- <paths>` (add `-d` only when the path is a directory) |

Rules that must hold:

- Every path keeps the `repository_valid_relative_file_path` check, per path,
  before it reaches an argv. One bad path fails the whole action — never a
  partial argv.
- `--literal-pathspecs` on every command, as the existing two already do, or a
  filename containing a glob character becomes a pathspec.
- CONFLICTED files get stage (that is how a resolution is marked) but NO
  discard in this arc. Discarding one side of a conflict is a separate decision
  with its own semantics; out of scope, and say so in the UI rather than
  silently omitting the button.
- Discard runs on the operation rail, not the single-flight file action: it can
  touch thousands of files.

### The confirmation, and why discard is different

Discard is the FIRST genuinely unrecoverable action in the app. Everything
retire touches is recoverable — a branch keeps its commits, an archived session
comes back, a removed worktree loses nothing. Discard destroys the only copy.

It therefore gets the strictest treatment, modelled on the retire checklist:

- Never a bare "are you sure". Name the count and the bytes, list the paths
  (elided past ~20 with "+N more"), and say the sentence plainly: *these
  changes exist nowhere else and cannot be recovered.*
- Offer the non-destructive alternative FIRST, because it exists and is one
  command: commit the WIP to the branch (`repository_operation_commit_wip`,
  already built). A user who wanted the folder clean, not the work gone,
  should not have to know the difference.
- Two-step confirm, same shape as `g_worktree_retire.confirming`: the first
  button arms, the second acts, and the armed state resets whenever the dialog
  opens or the selection changes.
- Stage and unstage are recoverable and get NO confirmation. Confirming a
  reversible action teaches users to click through confirmations.

## Step 3 — commit

- A multiline message input in the Changelist, under the groups.
- Commits what is STAGED: `git commit -m <message>`. When nothing is staged,
  the button is disabled and says why — never silently `-a`.
- No `--no-verify` here. `commit_wip` uses it because it is a rescue that must
  not be blocked by a hook; a real commit must run the repository's hooks.
  A hook failure is a normal outcome and lands in the failure UX with git's own
  words.
- Runs on the operation rail; the message is carried in, not re-read from UI
  state when the step launches.
- After success the input clears and the repository refreshes.

## Step 4 — fetch / push / pull / sync

All four are network operations: the rail is mandatory, cancel matters, and the
log is the only way to see what a remote said.

| action | steps | notes |
|---|---|---|
| fetch | `git fetch --prune` | read-only, always safe |
| pull | `git pull --ff-only` | NEVER a bare `git pull`. A surprise merge commit in someone's worktree is exactly the class of silent lie this round keeps removing. When it fails as non-fast-forward, say so and offer rebase as a SEPARATE explicit action |
| push | `git push` | when the branch has no upstream, the button reads "Publish branch" and runs `git push -u origin <branch>`, naming the remote it will create |
| sync | fetch → pull --ff-only → push | one operation, three steps, so the progress panel shows which one is running |

`WorktreeState.upstream_ref` and `ahead` / `behind` already exist (added for
the retire verdict) — use them for the button labels and enablement, so push
reads "Push 3" and pull reads "Pull 12" instead of being blind verbs.

New `classify_failure` patterns to add with this step:

- "non-fast-forward" / "rejected" → the remote moved; fetch and rebase or
  merge first. Retryable only after that, so NOT retryable.
- "no upstream" / "has no upstream branch" → publish the branch instead.
- "could not read Username" / "Authentication failed" → credentials, not code;
  retry will fail identically.
- "Connection" / "unable to access" / "timed out" → transient, retryable.

## Tests

The rule for this arc: **anything that can destroy a file is tested only in a
temporary scratch repository**, created and torn down by the test. Never point
a discard or clean test at the working tree.

### Unit — `utils/dasHerd/watcher/tests/test_repository_core.das`

1. `repository_build_file_action_argv` for every action in the table above:
   the exact argv, `--literal-pathspecs` present, multi-path form, and that the
   paths land after `--`.
2. Path validation: a path containing `..`, an absolute path, and an empty
   path each fail the WHOLE action and leave `argv` empty.
3. `discard_worktree` vs `discard_all` produce different commands — a
   regression here silently destroys staged work, so assert both spellings.
4. Unknown action still returns an error rather than an empty argv.
5. The network step lists: fetch/pull/push/sync build the expected steps in
   order, and push with no upstream produces the `-u origin <branch>` form.
6. `repository_classify_failure` for each new pattern, asserting the hint is
   non-empty AND that `retryable` is false for auth and non-fast-forward. The
   existing test for this (the locked-folder case) is the template.

### Unit — selection model, new `tests/test_changelist_selection.das`

Pure logic, no ImGui: toggle, range within a group, range refusing to cross
groups, select-all tri-state, and — most important — that changing worktree or
repository clears the set. Assert a selection can never contain two
worktree paths.

### Integration

The operation rail is already exercised end to end by the retire flow. For this
arc, add one scratch-repo test that runs a REAL stage → commit → discard cycle
through `repository_*` entry points and asserts the resulting
`git status --porcelain` — that is the only way to prove the argv table is
right rather than merely self-consistent.

Not mirrorable locally: nothing here needs CI-only infrastructure. The network
four cannot be tested against a real remote in CI; test their step-list
construction only, and verify the live behaviour by hand against
`borisbat/*` once.

### Live verification (the parity rule)

Every action must be commandable and every new panel state inspectable, or it
is a bug by standing rule. Rails to add, matching the retire set:

- `herder_changelist_state` — groups, counts, the selection set, and what each
  action would do to it.
- `herder_changelist_select` — set / toggle / range / clear by path.
- `herder_changelist_action` — stage / unstage / discard / commit by name, on
  the current selection or an explicit path list.
- `herder_git_sync` — fetch / pull / push / sync by name.

Then prove it the way this round proved retire: drive the rails on a throwaway
worktree, read `herder_worktree_operation_state` for the log and the step
count, and confirm the resulting `git status`. Screenshots show it renders;
only the rails show it is right.

## Order of work, with checkpoints

1. Step 0 refactor, retire tests green, commit. **Nothing new in this commit.**
2. Selection model + its tests, commit.
3. stage / unstage on the status groups (recoverable, no confirm), commit.
4. discard with the full confirmation, scratch-repo test, commit.
5. commit-from-view, commit.
6. fetch / push / pull / sync + the classifier patterns, commit.

Each step ends with lint clean on every changed `.das`, the watcher test suite
green, and one live drive through the rails.

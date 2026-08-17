# Git activity and topology plan

This plan records the agreed direction for the rich client's Git review surface. Each stage is reviewed before the next one starts.

## Stage 1: stable review layout

- Split the current combined Git window into independently dockable `Git Changelist` and `Git Activity` windows.
- Keep worktree identity, status, refresh state, errors, and the selected PR/commit file list in Changelist.
- Keep only the PR, History, and Tree perspectives in Activity.
- Use real resizable tables for commits: SHA, title, and author are separate columns and colors.
- Show Tree with the same readable table as History until topology has a trustworthy implementation.
- Reserve a permanent bottom status bar in each Git window; loading and error text replaces the stable summary there without moving content.

This prevents asynchronous file-list replacement from resizing or shifting the activity view.

## Stage 2: Git-owned topology stream

- Treat `git log --graph --topo-order --full-history` as the topology oracle.
- Preserve Git's physical output rows, including transition-only rows between commits.
- Parse the graph prefix separately from commit metadata into a small headless model.
- Add fixtures for forks, merges, octopus merges, crossings, truncation, and decorations before drawing anything.

Tree observes all refs available to the worktree so branch topology is visible;
History remains the focused single-branch view.

## Stage 3: topology rendering

- Extend the Tree table to four columns: SHA, title, author, topology.
- Render the parsed graph prefix as vector lanes, nodes, and transitions in the fourth (rightmost) column.
- Assign stable colors by active lane identity, not commit-hash randomness.
- Keep commit metadata aligned as a table while the topology column explains ancestry.

## Stage 4: selection contract

- PR selects the complete outgoing range against the detected base.
- History and Tree commit rows select one commit and populate Changelist with that commit's files.
- A Tree ref badge selects that branch's outgoing range against its base.
- SHA cells retain hover-copy behavior and copy the full hash.

## Stage 5: robustness and scale

- Add pending and failure states per window without changing dock geometry.
- Stress-test large histories, many refs, rapid selection, and repeated async replacement.
- Measure parsing, rendering, and selection latency before optimizing.

## Endgame Tree view contract

Tree is a semantic branch and PR topology view, not a decorated ASCII Git log.
The current `git log --graph` stream remains a useful ordering oracle and test
fallback, while the final view uses a headless commit DAG and draws continuous
vector edges and nodes.

PR remains the default perspective and the primary review answer to "what is
going out from this worktree?" Tree is an explicit escalation surface for work
that involves multiple branches, PRs, bases, or incoming sources. It must make
the role of every emphasized source clear rather than presenting an undirected
mass of refs.

- An ordinary commit is a hollow circle. Every visible branch or PR head is a
  filled circle: it represents the thing that would go out.
- Every pushed commit has a mildly bolder circle outline. Pushed means reachable
  from that branch's configured upstream remote head, not merely present on an
  unrelated remote.
- Multiple local, remote, worktree, or PR refs at one commit share one filled
  node with multiple badges.
- Real Git parent relationships use continuous solid edges. A branch color stays
  continuous while its lane moves, and stable per-repository assignments survive
  restarts. Shared history uses the base branch context rather than pretending a
  commit belongs exclusively to one branch.
- Selecting a branch or PR emphasizes the outgoing range
  `merge-base(base, head)..head`, subdues parallel work, and populates Changelist
  with that same range. For stacked PRs the declared immediate base is the
  default; comparing against the repository base is an option.
- `Auto` is the default cognitive-load filter: show branches with outgoing
  changes, the current branch, necessary base context, and a small recent set of
  merged branches. `All` adds all relevant local branches, coalesced upstream
  refs, worktrees, and available PR heads.
- Keep approximately the current vertical commit density. Unselected topology
  remains visible but subdued rather than disappearing.
- Normal merges are visible through their real Git parent edge. Squash and
  rebase merges may have no ancestry edge back to the original PR commits; how
  optional GitHub metadata should visualize that association remains a later,
  explicitly labelled display decision.

### Future agent-directed Tree focus

A later herder skill can let an agent publish a structured Tree focus instead
of describing topology only in terminal prose. The focus identifies exact
refs/SHAs and assigns visible roles such as:

- primary: this worktree's PR and the thing going out;
- incoming: the branch or PR being evaluated or brought in;
- base/context: the integration branch and any required shared ancestry.

An agent-facing view can then say, in effect, "this is your PR; this is the
other branch; this is what we are bringing in," emphasize those paths, and
subdue unrelated topology. The focus is inspectable user-visible state, not an
implicit filter: the user can see the selected identities, clear the focus, or
switch back to `Auto`/`All`. This is future direction, not part of the current
view-only PR. The shared file/source/Tree/GitHub focus and two-way human-agent
handoff are specified in
[`AGENT_REVIEW_WORKFLOWS.md`](AGENT_REVIEW_WORKFLOWS.md).

## Deferred operational direction -- not this PR

This PR is view-only. It may select and identify commits, refs, branches, PRs,
bases, and outgoing ranges, but it does not rewrite or synchronize Git state.

Future tools can use those exact semantic selections for operations such as
squash, rebase, cherry-pick onto another branch, fetch, pull, synchronize to a
PR, or bring a GitHub PR into a local worktree. Potentially destructive history
operations should carry explicit head/base/ref/SHA identity into an agent-owned
terminal workflow. The graph must refresh from observed Git and GitHub state
after an operation rather than optimistically inventing the result.

## Immediate follow-up: stable File Inspector replacement

- Keep the currently prepared document visible while a newly selected file is
  fetched and prepared; selection must not tear down and recreate the view.
- Track requested identity separately from displayed identity. Replace the
  document atomically only when the newest request has a complete prepared
  result, and discard stale worker generations.
- Give File Inspector a permanent bottom status bar. Loading, preparation,
  errors, and the stable file summary share that reserved area; no transient
  status is inserted above the document.
- Preserve dock geometry and controls throughout replacement. Apply the chosen
  new-file scroll policy only at the atomic swap, not during pending work.
- Expose retained-content identity, pending identity, generation, footer
  position, and swap revision through live inspection so tests can prove that
  selection never blanks or shifts the view.

## Research basis

- Git's graph output and topological ordering are the behavioral oracle: <https://git-scm.com/docs/git-log>
- Git's implementation documents the graph state machine and transition rows: <https://github.com/git/git/blob/master/graph.c>
- The target information layout follows the reviewed Git GUI examples: commit metadata remains tabular while topology is a dedicated graph column.

# Terminal application compatibility corpus

This directory compares `dasTerminal` with a pinned `@xterm/headless` oracle on
the byte streams that matter to the terminal applications we actually run:
Codex, Claude Code, Pi, and OpenCode.

The oracle also pins xterm's Unicode 11 addon. Stock xterm 5.5 defaults to
Unicode 6 cell widths (including one-cell widths for modern emoji), which is not
an appropriate compatibility target for these applications.

`scenarios.json` is the shared boundary. Terminal writes are committed as JSON
arrays of byte values. The Node oracle passes those values to xterm as
`Uint8Array`; the daScript test passes the same values to `terminal_feed_bytes`.
Strings are deliberately not used as the transport model.

Run the focused checks from this directory:

```powershell
npm ci
npm run oracle:check
```

Regenerate `oracle-snapshots.json` only when intentionally changing the pinned
oracle or the scenario corpus:

```powershell
npm run oracle:update
```

The application-derived cases are reduced to stable byte fixtures; none of the
applications is a test dependency. Pi cases follow its renderer and its own
xterm-backed virtual-terminal tests. Codex cases follow its pinned Ratatui
scrolling-region implementation, and OpenCode cases follow OpenTUI's capability
probes. The Claude Code case was reduced from a short Windows ConPTY capture of
version 2.1.215 because its implementation is not public.

## Deliberately deferred conformance scope

Broad DEC/xterm conformance (`libvterm`, `vttest`, `esctest`, VTE parser suites,
and similar projects) is a follow-up, not a release gate for this phase. Start
that work after two independent terminal-semantic bug reports, or when an
application outside the four-app corpus becomes a supported target. One report
is fixed and covered here; the second is the signal that the corpus is no
longer broad enough.

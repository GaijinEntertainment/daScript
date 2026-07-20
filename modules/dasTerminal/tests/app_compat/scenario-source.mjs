const encoder = new TextEncoder();
const ESC = "\x1b";

function bytes(text) {
  return [...encoder.encode(text)];
}

export const corpus = {
  version: 1,
  scenarios: [
    {
      id: "existing/style-and-wide",
      source: "modules/dasTerminal/tests/terminal_semantics.das",
      columns: 12,
      rows: 3,
      steps: [
        { feed: bytes(`hello\r\n${ESC}[31mA${ESC}[1mB${ESC}[0m \u4e2d`) },
        { checkpoint: "final" }
      ]
    },
    {
      id: "existing/scroll-history",
      source: "modules/dasTerminal/tests/terminal_semantics.das",
      columns: 4,
      rows: 2,
      steps: [
        { feed: bytes("a\r\nb\r\nc\r\nd") },
        { checkpoint: "final" }
      ]
    },
    {
      id: "existing/erase-display",
      source: "modules/dasTerminal/tests/terminal_semantics.das",
      columns: 6,
      rows: 3,
      steps: [
        { feed: bytes(`abcdef\r\nghijkl\r\nmnopqr${ESC}[2;3H${ESC}[1J`) },
        { checkpoint: "final" }
      ]
    },
    {
      id: "existing/alternate-screen",
      source: "modules/dasTerminal/tests/terminal_semantics.das",
      columns: 10,
      rows: 3,
      steps: [
        { feed: bytes(`main${ESC}]2;PowerShell\x07`) },
        { checkpoint: "normal" },
        { feed: bytes(`${ESC}[?25l${ESC}[?1049halt`) },
        { checkpoint: "alternate" },
        { feed: bytes(`${ESC}[?1049l`) },
        { checkpoint: "restored" }
      ]
    },
    {
      id: "pi/synchronized-full-redraw",
      source: "https://github.com/earendil-works/pi/blob/94373d815d2b4a3a48864d5341afc824b8db45e3/packages/tui/src/tui.ts",
      columns: 16,
      rows: 4,
      steps: [
        {
          feed: bytes(
            `${ESC}[?2026h${ESC}[2J${ESC}[H${ESC}[3J` +
            `Status\r\nworking \u4e2d\r\n${ESC}[32m\u2713 ready${ESC}[0m` +
            `${ESC}[?2026l`
          )
        },
        { checkpoint: "final" }
      ]
    },
    {
      id: "pi/differential-middle-line",
      source: "https://github.com/earendil-works/pi/blob/94373d815d2b4a3a48864d5341afc824b8db45e3/packages/tui/src/tui.ts",
      columns: 18,
      rows: 4,
      steps: [
        { feed: bytes("Header\r\nold value\r\nFooter") },
        { checkpoint: "initial" },
        {
          feed: bytes(
            `${ESC}[?2026h${ESC}[1A\r${ESC}[2K` +
            `new ${ESC}[32mvalue${ESC}[0m${ESC}[1B\r${ESC}[?2026l`
          )
        },
        { checkpoint: "updated" }
      ]
    },
    {
      id: "pi/shrink-and-clear-tail",
      source: "https://github.com/earendil-works/pi/blob/94373d815d2b4a3a48864d5341afc824b8db45e3/packages/tui/src/tui.ts",
      columns: 14,
      rows: 5,
      steps: [
        { feed: bytes("one\r\ntwo\r\nthree\r\nfour") },
        { checkpoint: "initial" },
        {
          feed: bytes(
            `${ESC}[3A\r${ESC}[2KONE\r\n${ESC}[2KTWO` +
            `${ESC}[2B\r${ESC}[2K${ESC}[1A\r${ESC}[2K`
          )
        },
        { checkpoint: "shrunk" }
      ]
    },
    {
      id: "pi/unicode-cursor-accounting",
      source: "https://github.com/earendil-works/pi/blob/94373d815d2b4a3a48864d5341afc824b8db45e3/packages/tui/test/tui-render.test.ts",
      columns: 16,
      rows: 2,
      steps: [
        { feed: bytes("A\u4e2d\ud83d\ude42e\u0301") },
        { checkpoint: "final" }
      ]
    },
    {
      id: "pi/keyboard-negotiation-fallback",
      source: "https://github.com/earendil-works/pi/blob/94373d815d2b4a3a48864d5341afc824b8db45e3/packages/tui/src/terminal.ts",
      columns: 20,
      rows: 2,
      steps: [
        { feed: bytes(`prefix${ESC}[>7u${ESC}[?u${ESC}[c`) },
        { checkpoint: "negotiated" },
        { feed: bytes(`${ESC}[>4;2mfallback`) },
        { checkpoint: "fallback-enabled" },
        { feed: bytes(`${ESC}[<u${ESC}[>4;0m`) },
        { checkpoint: "stopped" }
      ]
    },
    {
      id: "codex/inline-scrolling-regions",
      source: "https://github.com/openai/codex/blob/678157acaa819d5510adfe359abb5d0392cfe461/codex-rs/tui/src/tui.rs",
      columns: 8,
      rows: 5,
      steps: [
        { feed: bytes("zero\r\none\r\ntwo\r\nview\r\nprompt") },
        { checkpoint: "initial" },
        { feed: bytes(`${ESC}[1;3r${ESC}[1S${ESC}[r`) },
        { checkpoint: "scrolled-up" },
        { feed: bytes(`${ESC}[1;3r${ESC}[1T${ESC}[r`) },
        { checkpoint: "scrolled-down" }
      ]
    },
    {
      id: "opencode/opentui-capability-probes",
      source: "https://github.com/anomalyco/opentui/blob/34e78b2fbf18fd969efdf5f3e2589d17d1f536f1/packages/core/src/zig/ansi.zig",
      columns: 20,
      rows: 3,
      steps: [
        { feed: bytes("probe") },
        {
          feed: bytes(
            `${ESC}P+q4d73${ESC}\\` +
            `${ESC}_Gi=31337,s=1,v=1,a=q,t=d,f=24;AAAA${ESC}\\` +
            `${ESC}[?2;1;0S${ESC}[=c${ESC}[>0q`
          )
        },
        { checkpoint: "probes-consumed" }
      ]
    },
    {
      id: "claude-code/captured-startup-and-teardown",
      source: "Claude Code 2.1.215, Windows ConPTY capture, 2026-07-19",
      columns: 100,
      rows: 30,
      steps: [
        {
          feed: bytes(
            `shell${ESC}[?9001h${ESC}[?1004h${ESC}[?25l${ESC}[2J${ESC}[m${ESC}[H` +
            `${ESC}]0;claude\x07${ESC}[?25h${ESC}[?25l${ESC}[?2004h${ESC}[?1004h` +
            `${ESC}[?2031h${ESC}[<u${ESC}[>1u${ESC}[>4;2m${ESC}[>0q` +
            `${ESC}[?1049h${ESC}[2J${ESC}[<u${ESC}[>1u${ESC}[>4;2m` +
            `${ESC}[?1000h${ESC}[?1002h${ESC}[?1003h${ESC}[?1006h`
          )
        },
        { checkpoint: "initialized" },
        {
          feed: bytes(
            `${ESC}]0;\u2733 Claude Code\x07${ESC}[?2026h${ESC}[?2026l` +
            `${ESC}[38;2;215;119;87m${ESC}[2;1H \u2590` +
            `${ESC}[48;2;0;0;0m\u259b\u2588\u2588\u2588\u259c${ESC}[49m\u258c${ESC}[m` +
            `${ESC}[1m${ESC}[3CClaude Code${ESC}[38;2;153;153;153m${ESC}[22m` +
            `${ESC}[1Cv2.1.215`
          )
        },
        { checkpoint: "rendered" },
        {
          feed: bytes(
            `${ESC}[?1006l${ESC}[?1003l${ESC}[?1002l${ESC}[?1000l` +
            `${ESC}[?2026h${ESC}[?2026l${ESC}[<u${ESC}[28;3H${ESC}[?25h` +
            `${ESC}[?1049l${ESC}[>4m${ESC}[?2031l${ESC}[?2004l`
          )
        },
        { checkpoint: "restored" }
      ]
    }
  ]
};

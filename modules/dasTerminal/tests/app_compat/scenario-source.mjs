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
      source: "https://github.com/earendil-works/pi/tree/main/packages/tui",
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
      source: "https://github.com/earendil-works/pi/tree/main/packages/tui",
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
      source: "https://github.com/earendil-works/pi/tree/main/packages/tui",
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
      source: "https://github.com/earendil-works/pi/tree/main/packages/tui",
      columns: 16,
      rows: 2,
      steps: [
        { feed: bytes("A\u4e2d\ud83d\ude42e\u0301") },
        { checkpoint: "final" }
      ]
    }
  ]
};

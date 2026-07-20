import { readFile, writeFile } from "node:fs/promises";
import { dirname, join } from "node:path";
import { fileURLToPath } from "node:url";
import unicode11Package from "@xterm/addon-unicode11";
import xtermHeadless from "@xterm/headless";
import { corpus } from "./scenario-source.mjs";

const { Terminal } = xtermHeadless;
const { Unicode11Addon } = unicode11Package;
const encoder = new TextEncoder();

const root = dirname(fileURLToPath(import.meta.url));
const streamsPath = join(root, "scenarios.json");
const snapshotsPath = join(root, "oracle-snapshots.json");
const mode = process.argv[2];

if (mode !== "--check" && mode !== "--update") {
  throw new Error("usage: node oracle.mjs --check|--update");
}

function color(cell, foreground) {
  const isDefault = foreground ? cell.isFgDefault() : cell.isBgDefault();
  const isPalette = foreground ? cell.isFgPalette() : cell.isBgPalette();
  const value = foreground ? cell.getFgColor() : cell.getBgColor();
  if (isDefault) return { kind: "default_color", index: 0, red: 0, green: 0, blue: 0 };
  if (isPalette) return { kind: "indexed", index: value, red: 0, green: 0, blue: 0 };
  return {
    kind: "rgb",
    index: 0,
    red: (value >> 16) & 255,
    green: (value >> 8) & 255,
    blue: value & 255
  };
}

function attributes(cell) {
  return (
    (cell.isBold() ? 1 : 0) |
    (cell.isDim() ? 2 : 0) |
    (cell.isItalic() ? 4 : 0) |
    (cell.isUnderline() ? 8 : 0) |
    (cell.isBlink() ? 16 : 0) |
    (cell.isInverse() ? 32 : 0) |
    (cell.isInvisible() ? 64 : 0) |
    (cell.isStrikethrough() ? 128 : 0)
  );
}

function normalizedCells(buffer, rows, columns) {
  const result = [];
  const reusable = buffer.getNullCell();
  for (let row = 0; row < rows; ++row) {
    const line = buffer.getLine(buffer.baseY + row);
    if (!line) continue;
    for (let column = 0; column < columns; ++column) {
      const cell = line.getCell(column, reusable);
      if (!cell) continue;
      const grapheme = cell.getChars() || (cell.getWidth() === 0 ? "" : " ");
      const attrs = attributes(cell);
      const foreground = color(cell, true);
      const background = color(cell, false);
      if (
        grapheme === " " &&
        cell.getWidth() === 1 &&
        attrs === 0 &&
        foreground.kind === "default_color" &&
        background.kind === "default_color"
      ) {
        continue;
      }
      result.push({
        row,
        column,
        grapheme,
        width: cell.getWidth(),
        attributes: attrs,
        foreground,
        background
      });
    }
  }
  return result;
}

function snapshot(terminal, title, checkpoint, replies) {
  const buffer = terminal.buffer.active;
  const screen = [];
  const history = [];
  for (let row = 0; row < terminal.rows; ++row) {
    screen.push(buffer.getLine(buffer.baseY + row)?.translateToString(true) ?? "");
  }
  for (let row = 0; row < buffer.baseY; ++row) {
    history.push(buffer.getLine(row)?.translateToString(true) ?? "");
  }
  return {
    checkpoint,
    screen,
    history,
    cursor: { row: buffer.cursorY, column: buffer.cursorX },
    alternate_active: buffer.type === "alternate",
    title,
    replies,
    cells: normalizedCells(buffer, terminal.rows, terminal.cols)
  };
}

async function runScenario(scenario) {
  const terminal = new Terminal({
    cols: scenario.columns,
    rows: scenario.rows,
    allowProposedApi: true,
    scrollback: 10000
  });
  terminal.loadAddon(new Unicode11Addon());
  terminal.unicode.activeVersion = "11";
  let title = "";
  const replies = [];
  terminal.onTitleChange(value => {
    title = value;
  });
  terminal.onData(value => {
    replies.push(...encoder.encode(value));
  });
  const checkpoints = [];
  for (const step of scenario.steps) {
    if (step.feed) {
      await new Promise(resolve => terminal.write(Uint8Array.from(step.feed), resolve));
    }
    if (step.resize) terminal.resize(step.resize.columns, step.resize.rows);
    if (step.checkpoint) {
      checkpoints.push(snapshot(terminal, title, step.checkpoint, replies.splice(0)));
    }
  }
  terminal.dispose();
  return { id: scenario.id, checkpoints };
}

async function generateSnapshots() {
  const scenarios = [];
  for (const scenario of corpus.scenarios) scenarios.push(await runScenario(scenario));
  return {
    oracle: "@xterm/headless@5.5.0 + @xterm/addon-unicode11@0.8.0",
    version: corpus.version,
    scenarios
  };
}

async function checkedText(path) {
  try {
    return await readFile(path, "utf8");
  } catch (error) {
    if (error.code === "ENOENT") return "";
    throw error;
  }
}

const streams = `${JSON.stringify(corpus, null, 2)}\n`;
const snapshots = `${JSON.stringify(await generateSnapshots(), null, 2)}\n`;

if (mode === "--update") {
  await writeFile(streamsPath, streams);
  await writeFile(snapshotsPath, snapshots);
  console.log("updated scenarios.json and oracle-snapshots.json");
} else {
  const committedStreams = await checkedText(streamsPath);
  const committedSnapshots = await checkedText(snapshotsPath);
  const stale = [];
  if (committedStreams !== streams) stale.push("scenarios.json");
  if (committedSnapshots !== snapshots) stale.push("oracle-snapshots.json");
  if (stale.length) {
    throw new Error(`${stale.join(" and ")} stale; run npm run oracle:update`);
  }
  console.log(`oracle fixtures current (${corpus.scenarios.length} scenarios)`);
}

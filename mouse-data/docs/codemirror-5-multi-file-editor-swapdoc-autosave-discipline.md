---
slug: codemirror-5-multi-file-editor-swapdoc-autosave-discipline
title: How do I implement a multi-file editor in CodeMirror 5 with per-tab undo history and localStorage autosave?
created: 2026-05-13
last_verified: 2026-05-13
links: []
---

**Core pattern: one `CodeMirror.Doc` per file, `editor.swapDoc(doc)` to switch the visible buffer. The editor instance is shared; each Doc carries its own text, undo stack, and scroll position.**

```js
const editor = CodeMirror(el, opts);          // one editor

const state = {
    files: {                                  // one Doc per file
        'main.das': editor.getDoc(),
        'utils.das': new CodeMirror.Doc('', 'daslang'),
    },
    active: 'main.das',
};

function switchFile(name) {
    state.active = name;
    editor.swapDoc(state.files[name]);        // history preserved
    autosave();                               // ← see below
}
```

## Autosave discipline — the trap

The instinct is to autosave on content change only:
```js
doc.on('change', autosave);   // covers content edits
```

But that misses **every other state mutation** — tab switch, add, delete, rename. If the user picks tab B, types nothing, and reloads, persistence restores tab A. Fix: call `autosave()` from *every* state-modifying entry point, not just `change`:

- `switchFile` → `autosave()`
- `addFile` → `autosave()`
- `deleteFile` → `autosave()`
- `renameFile` → `autosave()`
- Initial `change` listener stays (per-Doc, attached when Doc is created)

Debounce the writer (~250ms) so rapid edits coalesce. Redundant calls during rapid state changes are free — `clearTimeout(autosaveTimer)` then `setTimeout(..., 250)`.

## Serialization

Each Doc stringifies via `.getValue()`. Whole state:
```js
function getStateJson() {
    return {
        files: Object.fromEntries(
            Object.entries(state.files).map(([k, d]) => [k, d.getValue()])
        ),
        active: state.active,
    };
}
```
Persist `JSON.stringify(getStateJson())` into `localStorage`. Restore by parsing then reconstructing fresh Docs.

## URL share format

LZ-string the JSON, encode URL-safely:
```js
const url = location.origin + location.pathname
    + '#z=' + LZString.compressToEncodedURIComponent(JSON.stringify(getStateJson()));
```
Decode with `LZString.decompressFromEncodedURIComponent`. ~50-line program → ~250B encoded → fits in any URL bar.

## Where this came from

PR #2648 (daslang.io playground): `web/ui` shipped a multi-file `files: [...]` JSON schema in samples since forever, but the loader only ever read `files[0]`. Adding tabs was a Doc map + `swapDoc` per click. The autosave-on-tab-switch bug landed in round 6 of review — `pgSwitchFile` updated state but didn't call `autosave()`. Three rounds of testing later, Copilot caught it.

## Bonus

`editor.refresh()` after layout changes (e.g. splitter resize) — CM caches gutter widths and won't redraw unless told.

## Questions
- How do I implement a multi-file editor in CodeMirror 5 with per-tab undo history and localStorage autosave?

// node --test  (run from this directory)
import test from 'node:test';
import assert from 'node:assert/strict';
import { readFile } from 'node:fs/promises';
import path from 'node:path';
import { fileURLToPath } from 'node:url';
import * as P from './protocol.mjs';

const HERE = path.dirname(fileURLToPath(import.meta.url));

const TABLE = {
    defaults: {
        kind: 'graphics', modes: ['interpreter', 'wasm'], action: 'run',
        budget_ms: 1000, build_budget_ms: 5000, min_observe_ms: 500, min_rafs: 30, min_draws: 10,
    },
    samples: {
        'Gfx': {},
        'Console': { kind: 'console', stdout: 'hello' },
        'Audio': { kind: 'audio' },
        'InterpOnly': { kind: 'console', stdout: 'x', modes: ['interpreter'] },
    },
};

const line = (text, color = '#ffffff') => ({ text, color });

test('normalizeColor maps the pane\'s rgb() back to hex', () => {
    assert.equal(P.normalizeColor('rgb(255, 45, 45)'), '#ff2d2d');
    assert.equal(P.normalizeColor('rgb(255, 147, 147)'), '#ff9393');
    assert.equal(P.normalizeColor(''), '');
});

test('classifyOutput separates GL lines from failures and dedupes GL', () => {
    const { gl, errors } = P.classifyOutput([
        line('hello'),
        line('GL error: GL_INVALID_OPERATION at frame 3', 'rgb(255, 147, 147)'),
        line('GL error: GL_INVALID_OPERATION at frame 4', 'rgb(255, 147, 147)'),
        line('GL error: GL_INVALID_OPERATION at frame 3', 'rgb(255, 147, 147)'),
    ]);
    assert.equal(errors.length, 0);
    assert.equal(gl.length, 2);
});

test('classifyOutput trusts the pane colour for severity', () => {
    const { errors } = P.classifyOutput([line('something went wrong', 'rgb(255, 45, 45)')]);
    assert.deepEqual(errors, ['something went wrong']);
});

test('classifyOutput catches runtime markers that arrived uncoloured', () => {
    const cases = [
        'EXCEPTION: division by zero at main.das:4',
        'runtime aborted: RuntimeError',
        'main.das:3:1: error[30301] cannot infer type',
        'assert failed, expected 4',
        'Aborted(OOM)',
        'asset /a/b.png: 404',
    ];
    for (const text of cases) {
        const { errors } = P.classifyOutput([line(text)]);
        assert.deepEqual(errors, [text], text);
    }
});

test('classifyOutput honours a sample\'s ignore list', () => {
    const { errors } = P.classifyOutput(
        [line('WebGL: too many contexts', 'rgb(255, 45, 45)')], ['too many contexts']);
    assert.deepEqual(errors, []);
});

test('resolveSpec merges defaults and fails closed on an unlisted sample', () => {
    const ok = P.resolveSpec(TABLE, 'Gfx');
    assert.equal(ok.ok, true);
    assert.equal(ok.spec.min_rafs, 30);
    assert.equal(ok.spec.kind, 'graphics');

    const missing = P.resolveSpec(TABLE, 'Brand New Sample');
    assert.equal(missing.ok, false);
    assert.match(missing.error, /expectations/);
});

test('resolveSpec rejects rows the runner could not execute', () => {
    assert.equal(P.resolveSpec({ defaults: {}, samples: { A: { kind: 'video' } } }, 'A').ok, false);
    assert.equal(P.resolveSpec({ defaults: TABLE.defaults, samples: { A: { kind: 'console' } } }, 'A').ok, false);
    assert.equal(P.resolveSpec({ defaults: TABLE.defaults, samples: { A: { modes: [] } } }, 'A').ok, false);
    assert.equal(P.resolveSpec({ defaults: TABLE.defaults, samples: { A: { budget_ms: 0 } } }, 'A').ok, false);
    assert.equal(P.resolveSpec({ defaults: TABLE.defaults, samples: { A: { min_observe_ms: 0 } } }, 'A').ok, false);
    // An observation window longer than the budget could never close.
    assert.equal(P.resolveSpec({ defaults: TABLE.defaults, samples: { A: { min_observe_ms: 99999 } } }, 'A').ok, false);
    // Only wasm rows pay the build; an interpreter-only row needs no build budget.
    assert.equal(P.resolveSpec({ defaults: TABLE.defaults, samples: { A: { build_budget_ms: 0 } } }, 'A').ok, false);
    assert.equal(P.resolveSpec(
        { defaults: { ...TABLE.defaults, build_budget_ms: 0 }, samples: { A: { modes: ['interpreter'] } } }, 'A').ok, true);
});

test('artifactUrlFrom takes only a real cross-origin page artifact', () => {
    assert.equal(
        P.artifactUrlFrom('https://run.daslang.io/b/abc/sample.html', 'https://daslang.io'),
        'https://run.daslang.io/b/abc/sample.html');
    assert.equal(P.artifactUrlFrom('', 'https://daslang.io'), '');
    assert.equal(P.artifactUrlFrom('about:blank', 'https://daslang.io'), '');
    // The frame src before an artifact lands, and non-page artifacts, are not URLs to open.
    assert.equal(P.artifactUrlFrom('https://run.daslang.io/b/abc/sample.wasm', 'https://daslang.io'), '');
    // Same-origin is accepted ON PURPOSE: the separate run origin is a production
    // deployment choice, and --base-url may point at a staging box without one.
    assert.equal(
        P.artifactUrlFrom('/api/build/artifact/abc/def/sample.html', 'https://daslang.io'),
        'https://daslang.io/api/build/artifact/abc/def/sample.html');
});

test('planRows expands modes, applies the filter, and keeps unknown samples as FAIL', () => {
    const samples = [{ name: 'Gfx' }, { name: 'InterpOnly' }, { name: 'Unlisted' }];
    const rows = P.planRows(samples, TABLE);
    assert.deepEqual(rows.map((r) => `${r.name}/${r.mode}`),
        ['Gfx/interpreter', 'Gfx/wasm', 'InterpOnly/interpreter', 'Unlisted/-']);
    assert.equal(rows[3].status, 'FAIL');

    const wasmOnly = P.planRows(samples, TABLE, { modes: ['wasm'] });
    assert.deepEqual(wasmOnly.map((r) => `${r.name}/${r.mode}`), ['Gfx/wasm', 'Unlisted/-']);

    const filtered = P.planRows(samples, TABLE, { filter: 'interp' });
    assert.deepEqual(filtered.map((r) => r.name), ['InterpOnly']);
});

test('verdict: a wedge outranks everything', () => {
    const spec = P.resolveSpec(TABLE, 'Gfx').spec;
    const v = P.verdict(spec, { wedge: true, rafs: 999, draws: 999, lines: [] });
    assert.equal(v.status, 'FAIL');
    assert.match(v.detail, /wedge/);
});

test('verdict: a crashed graphics program fails even with the host page painting', () => {
    const spec = P.resolveSpec(TABLE, 'Gfx').spec;
    // The exact blind spot a bare rAF counter has: the frame keeps ticking after
    // the emscripten loop dies, so only the draw counter and the error line see it.
    const v = P.verdict(spec, { rafs: 500, draws: 0, lines: [line('runtime aborted: RuntimeError')] });
    assert.equal(v.status, 'FAIL');
    assert.match(v.detail, /runtime aborted/);

    const silent = P.verdict(spec, { rafs: 500, draws: 0, lines: [] });
    assert.equal(silent.status, 'FAIL');
    assert.match(silent.detail, /draws 0\/10/);
});

test('verdict: a live graphics program passes', () => {
    const spec = P.resolveSpec(TABLE, 'Gfx').spec;
    assert.equal(P.verdict(spec, { rafs: 60, draws: 60, lines: [] }).status, 'PASS');
});

test('verdict: GL errors fail a sample that is otherwise alive', () => {
    const spec = P.resolveSpec(TABLE, 'Gfx').spec;
    const v = P.verdict(spec, { rafs: 60, draws: 60, lines: [line('GL error: GL_INVALID_OPERATION')] });
    assert.equal(v.status, 'FAIL');
    assert.match(v.detail, /GL_INVALID_OPERATION/);
});

test('verdict: console matches the pane, audio only needs a boot', () => {
    const console_ = P.resolveSpec(TABLE, 'Console').spec;
    assert.equal(P.verdict(console_, { lines: [line('hello world')] }).status, 'PASS');
    assert.equal(P.verdict(console_, { lines: [line('goodbye')] }).status, 'FAIL');

    const audio = P.resolveSpec(TABLE, 'Audio').spec;
    assert.equal(P.verdict(audio, { started: true, lines: [] }).status, 'PASS');
    assert.equal(P.verdict(audio, { started: false, lines: [] }).status, 'FAIL');
});

test('verdict: a page error fails the row', () => {
    const spec = P.resolveSpec(TABLE, 'Gfx').spec;
    const v = P.verdict(spec, { rafs: 60, draws: 60, lines: [], pageErrors: ['TypeError: x is not a function'] });
    assert.equal(v.status, 'FAIL');
    assert.match(v.detail, /page error/);
});

test('verdict: the ignore list covers page errors too', () => {
    // Emscripten's blocking-on-main-thread advisory arrives as console.error while
    // the sample runs fine — a threaded sample's row ignores it by pattern.
    const spec = { ...P.resolveSpec(TABLE, 'Gfx').spec, ignore: ['^Blocking on the main thread is very dangerous'] };
    const alive = { rafs: 60, draws: 60, lines: [] };
    const ignored = P.verdict(spec, { ...alive, pageErrors: [
        'Blocking on the main thread is very dangerous, see https://emscripten.org/docs/porting/pthreads.html#blocking-on-the-main-browser-thread',
    ] });
    assert.equal(ignored.status, 'PASS');
    const other = P.verdict(spec, { ...alive, pageErrors: ['TypeError: x is not a function'] });
    assert.equal(other.status, 'FAIL');
});

test('driftWarnings reports both directions and never fails', () => {
    const rows = P.driftWarnings(['a', 'b'], ['b', 'c']);
    assert.deepEqual(rows.map((r) => `${r.name}:${r.status}`), ['a:WARN', 'c:WARN']);
});

test('formatSummary escapes pipes and exitCodeFor gates on FAIL alone', () => {
    const rows = [
        { name: 'A|B', mode: 'interpreter', status: 'PASS', ms: 12, detail: 'rafs 60' },
        { name: 'C', mode: 'wasm', status: 'WARN', ms: 0, detail: 'drift' },
    ];
    const md = P.formatSummary(rows);
    assert.match(md, /A\\\|B/);
    assert.match(md, /1 passed, 0 failed, 1 warnings/);
    assert.equal(P.exitCodeFor(rows), 0);
    assert.equal(P.exitCodeFor([...rows, { status: 'FAIL' }]), 1);
});

test('every expectations row resolves, and the shipped manifest is fully covered', async () => {
    const table = JSON.parse(await readFile(path.join(HERE, 'expectations.json'), 'utf8'));
    for (const name of Object.keys(table.samples)) {
        const r = P.resolveSpec(table, name);
        assert.equal(r.ok, true, `${name}: ${r.error}`);
    }
    const manifest = JSON.parse(await readFile(
        path.join(HERE, '..', '..', '..', 'web', 'examples', 'ui', 'samples', 'data.json'), 'utf8'));
    const rows = P.planRows(manifest.examples, table);
    const unresolved = rows.filter((r) => r.status === 'FAIL').map((r) => r.name);
    assert.deepEqual(unresolved, [], 'samples with no expectations row');
});

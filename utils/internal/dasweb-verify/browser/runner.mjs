#!/usr/bin/env node
// Browser leg of the curated-sample verifier: drive the LIVE playground in
// headless Chromium and report, per sample and per engine, whether the program
// actually runs.
//
// It targets production on purpose. This arc's worst bugs were production state
// no local rebuild could see — artifacts truncated behind caddy, wasm archives
// missing on one box, a build failure cached under its content hash. Artifacts
// are content-addressed (source hash x toolchain id), so a night with no
// toolchain bump and no sample edit is mostly cache hits.
//
//   node runner.mjs [--mode interpreter|wasm] [--filter substr] [--base-url URL]
//
// Exit code 0 = every planned row passed; 1 = at least one FAIL (each named).

import { chromium } from 'playwright';
import { readFile, writeFile, mkdir, appendFile } from 'node:fs/promises';
import path from 'node:path';
import { fileURLToPath } from 'node:url';
import * as P from './protocol.mjs';
import { installProbe } from './probe.mjs';

const HERE = path.dirname(fileURLToPath(import.meta.url));
const REPO_MANIFEST = path.join(HERE, '..', '..', '..', '..', 'web', 'examples', 'ui', 'samples', 'data.json');

const POLL_MS = 500;
// Build polling is deliberately slow: the queue is one shared compute box and
// a fresh page build runs for minutes. Serial pacing also keeps the verifier
// under the playground's POST rate limits.
const BUILD_POLL_MS = 4500;
// Cap for a single page.evaluate outside a sample's own budget (page setup,
// sample selection). Inside the run loop the cap is whatever is LEFT of the
// budget instead: a daslang program owns the main thread while it runs, so a
// console benchmark computing for 40s looks exactly like a wedge until its
// budget is actually spent. Declaring one sooner is a false alarm — it bit the
// tree benchmark on the first live sweep.
const EVALUATE_MS = 20000;
// Floor under that derived cap, so the last poll of a budget still gets a fair
// chance to answer instead of reporting a wedge a millisecond before the end.
const EVALUATE_FLOOR_MS = 5000;
// The playground tab itself must come up well inside this or the run is moot.
const PAGE_READY_MS = 90000;

function parseArgs(argv) {
    const cfg = {
        baseUrl: 'https://daslang.io',
        modes: [],
        filter: '',
        summary: process.env.GITHUB_STEP_SUMMARY || '',
        artifacts: path.join(HERE, 'artifacts'),
        headed: false,
        list: false,
    };
    for (let i = 0; i < argv.length; i++) {
        const a = argv[i];
        const next = () => argv[++i];
        if (a === '--base-url') cfg.baseUrl = next().replace(/\/+$/, '');
        else if (a === '--filter' || a === '-f') cfg.filter = next();
        else if (a === '--mode' || a === '-m') cfg.modes.push(...next().split(',').map((s) => s.trim()).filter(Boolean));
        else if (a === '--summary') cfg.summary = next();
        else if (a === '--artifacts') cfg.artifacts = next();
        else if (a === '--headed') cfg.headed = true;
        else if (a === '--console') cfg.console = true;
        else if (a === '--list') cfg.list = true;
        else if (a === '--help' || a === '-?') cfg.help = true;
        else throw new Error(`unknown argument: ${a}`);
    }
    return cfg;
}

const HELP = `dasweb-verify browser leg

  --base-url URL     site to verify (default https://daslang.io)
  --mode M[,M]       interpreter | wasm (default: whatever each sample lists)
  --filter SUBSTR    only samples whose name contains SUBSTR
  --summary PATH     append the markdown report here (default $GITHUB_STEP_SUMMARY)
  --artifacts DIR    where failure screenshots go
  --headed           run with a visible browser
  --console          echo every browser console message (the only way to read the
                     WHY behind a GL error — the browser logs it where no page can)
  --list             print the plan and exit, no browser
`;

async function fetchJson(url) {
    const resp = await fetch(url);
    if (!resp.ok) throw new Error(`GET ${url} -> ${resp.status}`);
    return resp.json();
}

// Every call into the page is raced: a wedged renderer never answers, and a
// verifier that waits forever reports nothing at all.
async function withDeadline(promise, ms, label) {
    let timer;
    const guard = new Promise((resolve) => {
        timer = setTimeout(() => resolve({ wedge: true, label }), ms);
    });
    try {
        const value = await Promise.race([promise.then((v) => ({ value: v })), guard]);
        return value;
    } finally {
        clearTimeout(timer);
    }
}

async function evaluateIn(target, fn, arg, ms = EVALUATE_MS) {
    const r = await withDeadline(target.evaluate(fn, arg), ms, 'evaluate');
    if (r.wedge) throw new WedgeError();
    return r.value;
}

class WedgeError extends Error {
    constructor() {
        super('page stopped answering evaluate');
        this.wedge = true;
    }
}

// ── the playground tab ────────────────────────────────────────────────────

class Playground {
    constructor(browser, cfg) {
        this.browser = browser;
        this.cfg = cfg;
        this.context = null;
        this.page = null;
        this.pageErrors = [];
    }

    async open() {
        this.context = await this.browser.newContext({ viewport: { width: 1440, height: 1000 } });
        // Before any page script: the probe must not race the program's start.
        await this.context.addInitScript(installProbe, { pollGl: false });
        this.page = await this.context.newPage();
        this.page.on('pageerror', (e) => this.pageErrors.push(String(e && e.message ? e.message : e)));
        this.page.on('console', (msg) => {
            // Chrome writes the WHY of a rejected GL call to its own console
            // ("WebGL: INVALID_ENUM: <api>: <reason>") and nothing else can read
            // it — including the page. --console is how a failing sample gets
            // diagnosed without reproducing it by hand.
            if (this.cfg.console) process.stderr.write(`  [${msg.type()}] ${msg.text()}\n`);
            // A failed request is NOT a program failure here: the playground
            // probes `<sample>.das.assets.json` for every sample and only two
            // have one, so a 404 per run is the designed normal case. Assets
            // that fail once a run frame does want them surface as an
            // `asset <url>: …` line in the output pane, which the verdict reads.
            if (msg.type() === 'error' && !/^Failed to load resource:/.test(msg.text())) {
                this.pageErrors.push(msg.text());
            }
        });
        await this.page.goto(`${this.cfg.baseUrl}/playground/index.html`, { waitUntil: 'load', timeout: PAGE_READY_MS });
        // The interpreter engine gates Run on the local runtime finishing its
        // load; the wasm radio unlocks only once the build service answers.
        await this.page.waitForFunction(
            () => { const b = document.getElementById('run'); return b && !b.disabled; },
            null, { timeout: PAGE_READY_MS });
    }

    async close() {
        // A wedged renderer can refuse to tear down; the browser outlives it.
        await withDeadline(this.context.close(), 15000, 'close');
        this.context = null;
        this.page = null;
    }

    async restart() {
        try { await this.close(); } catch { /* the point of restarting */ }
        await this.open();
    }

    async selectMode(mode) {
        if (mode === 'wasm') {
            await this.page.waitForFunction(
                () => { const r = document.querySelector('input[name=engine][value=wasm]'); return r && !r.disabled; },
                null, { timeout: PAGE_READY_MS });
        }
        await evaluateIn(this.page, (m) => {
            const radio = document.querySelector(`input[name=engine][value=${m}]`);
            if (!radio) throw new Error('no engine radio ' + m);
            radio.checked = true;
            radio.dispatchEvent(new Event('change', { bubbles: true }));
        }, mode);
    }

    // Load a sample by its index in the deployed manifest — that is exactly the
    // picker's own indexing, so this clicks what a visitor clicks.
    //
    // Readiness is "the editor holds THIS sample's bytes", compared against the
    // source fetched straight from the site. "The text changed" would be the
    // obvious check and is wrong: the page preloads the first sample, so
    // verifying it would wait forever for a change that already happened.
    async selectSample(index, sample) {
        const entryUrl = `${this.cfg.baseUrl}/playground/samples/${sample.files[0]}`;
        const resp = await fetch(entryUrl);
        if (!resp.ok) throw new Error(`entry file ${entryUrl} -> ${resp.status}`);
        const expected = (await resp.text()).replace(/\r\n/g, '\n').trimEnd();

        await evaluateIn(this.page, (i) => window.selectSample('examples', i), index);
        const loaded = await withDeadline(this.page.waitForFunction(({ want, count }) => {
            const st = window.pgState;
            if (!st || !st.files) return false;
            const names = Object.keys(st.files);
            if (names.length !== count) return false;
            const doc = st.files['main.das'] || st.files[names[0]];
            const text = doc ? doc.getValue() : '';
            return text.replace(/\r\n/g, '\n').trimEnd() === want;
        }, { want: expected, count: sample.files.length }, { timeout: 30000, polling: 200 }), 35000, 'sample-load');
        if (loaded.wedge) throw new WedgeError();
    }

    async clearAndRun(action, mode) {
        // The interpreter runs in a pre-warmed frame, and the frame that
        // replaces a torn-down one needs a moment to report itself ready. Run
        // before that and the page answers "daslang is still loading" instead
        // of running the sample — a verifier failure with no bug behind it.
        if (mode !== 'wasm') {
            const ready = await withDeadline(this.page.waitForFunction(
                () => window.PlaygroundRunner && window.PlaygroundRunner.isReady(),
                null, { timeout: 60000, polling: 200 }), 65000, 'frame-ready');
            if (ready.wedge) throw new WedgeError();
        }
        await evaluateIn(this.page, () => {
            if (typeof window.clearOutput === 'function') window.clearOutput();
        });
        this.pageErrors.length = 0;
        // Deliberately not awaited inside the page: runCode resolves only after
        // the whole wasm build round-trip, and a graphics program never stops.
        await evaluateIn(this.page, (a) => {
            const fn = a === 'test' ? window.runTests : window.runCode;
            Promise.resolve(fn()).catch((e) => {
                const out = document.getElementById('output');
                if (out) out.appendChild(Object.assign(document.createElement('div'), {
                    className: 'output_line', innerText: 'runner: run threw ' + e,
                }));
            });
        }, action);
    }

    async readOutput(ms) {
        return evaluateIn(this.page, () => {
            const rows = document.querySelectorAll('#output .output_line');
            return Array.from(rows, (row) => {
                const p = row.querySelector('.output_line_text');
                return { text: p ? p.innerText : row.innerText, color: row.style.backgroundColor };
            });
        }, null, ms);
    }

    // The playground keeps a pre-warmed spare frame, so identity matters: the
    // one that was handed a program is the one that saw the run message.
    async readRunFrameProbe(ms) {
        for (const frame of this.page.frames()) {
            if (!/run-frame\.html/.test(frame.url())) continue;
            let probe = null;
            try {
                probe = await evaluateIn(frame, () => (window.__dwv ? { ...window.__dwv } : null), null, ms);
            } catch (e) {
                if (e.wedge) throw e;
                continue;   // frame was swapped out mid-read
            }
            if (probe && probe.ran) return probe;
        }
        return null;
    }

    async reset() {
        await evaluateIn(this.page, () => {
            if (window.PlaygroundRunner) window.PlaygroundRunner.reset();
        });
    }

    // The wasm page rail embeds the artifact cross-origin — a deliberate
    // security boundary, so the frame cannot be probed. Its src is the whole
    // point: the artifact opens as a top-level page instead.
    async readArtifactUrl() {
        const src = await evaluateIn(this.page, () => {
            const frame = document.querySelector('iframe.pg-page-frame');
            return frame ? frame.src : '';
        });
        return P.artifactUrlFrom(src, this.cfg.baseUrl);
    }

    // Stop the embedded copy before the top-level one starts: two instances of
    // a threaded GL program fighting for the same box makes both look slow.
    async dropArtifactFrame() {
        await evaluateIn(this.page, () => {
            if (typeof window.showCanvas === 'function') window.showCanvas(false);
            const frame = document.querySelector('iframe.pg-page-frame');
            if (frame && frame.parentNode) frame.parentNode.removeChild(frame);
        });
    }
}

// A page-shape artifact runs in its own context so the GL-error poll can be
// installed there and only there: run-frame.html already polls glGetError on
// the playground side, and getError CLEARS the flag, so a second poller would
// steal half the errors.
class Artifacts {
    constructor(browser, cfg) {
        this.browser = browser;
        this.cfg = cfg;
        this.context = null;
        this.page = null;
        this.pageErrors = [];
    }

    async open() {
        this.context = await this.browser.newContext({ viewport: { width: 1280, height: 900 } });
        await this.context.addInitScript(installProbe, { pollGl: true });
        this.page = await this.context.newPage();
        this.page.on('pageerror', (e) => this.pageErrors.push(String(e && e.message ? e.message : e)));
        this.page.on('console', (msg) => {
            // Chrome writes the WHY of a rejected GL call to its own console
            // ("WebGL: INVALID_ENUM: <api>: <reason>") and nothing else can read
            // it — including the page. --console is how a failing sample gets
            // diagnosed without reproducing it by hand.
            if (this.cfg.console) process.stderr.write(`  [${msg.type()}] ${msg.text()}\n`);
            if (msg.type() === 'error' && !/^Failed to load resource:/.test(msg.text())) {
                this.pageErrors.push(msg.text());
            }
        });
    }

    async close() {
        if (this.context) await withDeadline(this.context.close(), 15000, 'close');
        this.context = null;
        this.page = null;
    }

    async restart() {
        try { await this.close(); } catch { /* the point of restarting */ }
        await this.open();
    }

    async load(url) {
        if (!this.page) await this.open();
        this.pageErrors.length = 0;
        const nav = await withDeadline(
            this.page.goto(url, { waitUntil: 'commit', timeout: 120000 }), 130000, 'artifact-goto');
        if (nav.wedge) throw new WedgeError();
    }

    async readProbe(ms) {
        return evaluateIn(this.page, () => (window.__dwv ? { ...window.__dwv } : null), null, ms);
    }

    // Leave nothing running between samples: an artifact page keeps its render
    // loop (and its worker pool) alive for as long as the tab exists.
    async blank() {
        if (this.page) await withDeadline(this.page.goto('about:blank'), 15000, 'blank');
    }
}

// ── one sample, one engine ────────────────────────────────────────────────

function satisfied(spec, obs) {
    if (spec.kind === 'console') return P.stdoutMatches(obs.lines, spec.stdout);
    if (spec.kind === 'audio') return obs.started;
    return obs.rafs >= spec.min_rafs && obs.draws >= spec.min_draws;
}

// Stopping the moment the thresholds are met makes the whole verdict depend on
// machine speed: an error line that lands a beat later is simply never read, so
// the same sample passes on a fast box and fails on a slow one. The contract is
// "runs cleanly for this long", so the window is held open for min_observe_ms
// regardless of how quickly the thresholds fell.
function done(spec, obs, startedAt) {
    return satisfied(spec, obs) && Date.now() - startedAt >= spec.min_observe_ms;
}

async function verifyInterpreter(pg, row, index) {
    const spec = row.spec;
    const obs = { wedge: false, rafs: 0, draws: 0, started: false, lines: [], pageErrors: [] };
    await pg.selectMode('interpreter');
    await pg.selectSample(index, row.sample);
    await pg.clearAndRun(spec.action, 'interpreter');

    const startedAt = Date.now();
    const deadline = startedAt + spec.budget_ms;
    const left = () => Math.max(EVALUATE_FLOOR_MS, deadline - Date.now());
    while (Date.now() < deadline) {
        obs.lines = await pg.readOutput(left());
        const probe = await pg.readRunFrameProbe(left());
        if (probe) {
            obs.rafs = probe.raf;
            obs.draws = probe.draws;
            obs.started = probe.gl || probe.audio || obs.lines.length > 0;
        }
        obs.pageErrors = [...pg.pageErrors];
        const { errors, gl } = P.classifyOutput(obs.lines, spec.ignore);
        if (errors.length || gl.length) break;      // fail fast; the detail is already captured
        if (done(spec, obs, startedAt)) break;
        await new Promise((r) => setTimeout(r, POLL_MS));
    }
    return obs;
}

// Wasm mode has two deliveries, decided by the build: a console program comes
// back as a bare wasi module and prints into the same output pane; a graphics or
// audio program comes back as a page-shape artifact on the run origin.
async function verifyWasm(pg, artifacts, row, index) {
    const spec = row.spec;
    const obs = { wedge: false, rafs: 0, draws: 0, started: false, lines: [], pageErrors: [] };
    await pg.selectMode('wasm');
    await pg.selectSample(index, row.sample);
    await pg.clearAndRun('run', 'wasm');

    // A fresh build takes minutes (the toolchain embeds declared assets and
    // relinks); an unchanged source is a cache hit and resolves in seconds.
    // Both are correct outcomes — a cached FAILED build is a correct verdict
    // too, so the verifier never cache-busts.
    let artifactUrl = '';
    const buildDeadline = Date.now() + spec.build_budget_ms;
    while (Date.now() < buildDeadline) {
        obs.lines = await pg.readOutput(Math.max(EVALUATE_FLOOR_MS, buildDeadline - Date.now()));
        obs.pageErrors = [...pg.pageErrors];
        if (P.classifyOutput(obs.lines, spec.ignore).errors.length) return obs;
        if (spec.kind === 'console' && P.stdoutMatches(obs.lines, spec.stdout)) return obs;
        artifactUrl = await pg.readArtifactUrl();
        if (artifactUrl) break;
        await new Promise((r) => setTimeout(r, BUILD_POLL_MS));
    }
    if (!artifactUrl) {
        obs.lines.push({ text: `build did not deliver an artifact within ${spec.build_budget_ms}ms`, color: '#ff2d2d' });
        return obs;
    }

    await pg.dropArtifactFrame();
    await artifacts.load(artifactUrl);
    const startedAt = Date.now();
    const deadline = startedAt + spec.budget_ms;
    while (Date.now() < deadline) {
        const probe = await artifacts.readProbe(Math.max(EVALUATE_FLOOR_MS, deadline - Date.now()));
        if (probe) {
            obs.rafs = probe.raf;
            obs.draws = probe.draws;
            obs.started = probe.gl || probe.audio;
            // The artifact page has no output pane, so its GL-error poll feeds
            // the same line list the verdict already reads.
            for (const text of probe.glErrors) {
                if (!obs.lines.some((l) => l.text === text)) obs.lines.push({ text, color: '' });
            }
        }
        obs.pageErrors = [...pg.pageErrors, ...artifacts.pageErrors];
        if (P.classifyOutput(obs.lines, spec.ignore).errors.length) break;
        if (done(spec, obs, startedAt)) break;
        await new Promise((r) => setTimeout(r, POLL_MS));
    }
    obs.artifactUrl = artifactUrl;
    return obs;
}

async function verifySample(pg, artifacts, row, index, cfg) {
    const started = Date.now();
    let obs;
    try {
        if (row.mode === 'interpreter') obs = await verifyInterpreter(pg, row, index);
        else if (row.mode === 'wasm') obs = await verifyWasm(pg, artifacts, row, index);
        else throw new Error(`unknown mode "${row.mode}"`);
    } catch (e) {
        if (e instanceof WedgeError || e.wedge) {
            const shot = await screenshot(shotPage(pg, artifacts, row), row, cfg);
            await pg.restart();
            await artifacts.restart();
            return { ...row, status: 'FAIL', ms: Date.now() - started, detail: `wedge${shot ? ' — ' + shot : ''}` };
        }
        return { ...row, status: 'FAIL', ms: Date.now() - started, detail: String(e.message || e) };
    }
    const v = P.verdict(row.spec, obs);
    const out = { ...row, status: v.status, ms: Date.now() - started, detail: v.detail };
    if (v.status === 'FAIL') {
        const shot = await screenshot(shotPage(pg, artifacts, row), row, cfg);
        if (shot) out.detail = `${out.detail} — ${shot}`;
    }
    try {
        await pg.reset();
        await artifacts.blank();
    } catch {
        await pg.restart();
        await artifacts.restart();
    }
    return out;
}

// A page-rail failure is a picture of the ARTIFACT (the black canvas), not of
// the playground tab that merely embedded it.
function shotPage(pg, artifacts, row) {
    if (row.mode === 'wasm' && artifacts.page && !/^about:/.test(artifacts.page.url())) return artifacts.page;
    return pg.page;
}

function slug(text) {
    return text.toLowerCase().replace(/[^a-z0-9]+/g, '-').replace(/^-|-$/g, '').slice(0, 60);
}

async function screenshot(page, row, cfg) {
    if (!page) return '';
    const file = path.join(cfg.artifacts, `${slug(row.name)}-${row.mode}.png`);
    try {
        await mkdir(cfg.artifacts, { recursive: true });
        // A wedged renderer cannot paint, so this is best-effort by design.
        const shot = await withDeadline(page.screenshot({ path: file, timeout: 10000 }), 15000, 'screenshot');
        if (shot.wedge) return '';
        return path.relative(process.cwd(), file);
    } catch {
        return '';
    }
}

// ── main ──────────────────────────────────────────────────────────────────

async function main() {
    const cfg = parseArgs(process.argv.slice(2));
    if (cfg.help) { process.stdout.write(HELP); return 0; }

    const table = JSON.parse(await readFile(path.join(HERE, 'expectations.json'), 'utf8'));
    const deployed = await fetchJson(`${cfg.baseUrl}/playground/samples/data.json`);
    if (!deployed.examples || !deployed.examples.length) {
        throw new Error('deployed manifest lists no samples');
    }

    const rows = [];
    try {
        const repo = JSON.parse(await readFile(REPO_MANIFEST, 'utf8'));
        rows.push(...P.driftWarnings(deployed.examples.map((e) => e.name), repo.examples.map((e) => e.name)));
    } catch (e) {
        rows.push({ name: '(drift check)', mode: '-', status: 'WARN', ms: 0, detail: String(e.message || e) });
    }

    const plan = P.planRows(deployed.examples, table, { modes: cfg.modes, filter: cfg.filter });
    const indexOf = new Map(deployed.examples.map((e, i) => [e.name, i]));

    if (cfg.list) {
        for (const row of plan) {
            process.stdout.write(`${row.status || 'plan'}\t${row.mode}\t${row.name}\n`);
        }
        return 0;
    }

    const browser = await chromium.launch({
        headless: !cfg.headed,
        channel: 'chromium',    // the new headless mode — the old one throttles rAF harder
        args: ['--autoplay-policy=no-user-gesture-required'],
    });
    const pg = new Playground(browser, cfg);
    const artifacts = new Artifacts(browser, cfg);
    try {
        await pg.open();
        await artifacts.open();
        for (const row of plan) {
            if (row.status) { rows.push(row); continue; }   // already decided (no expectations row)
            process.stderr.write(`… ${row.mode}\t${row.name}\n`);
            const result = await verifySample(pg, artifacts, row, indexOf.get(row.name), cfg);
            process.stderr.write(`${result.status}\t${result.ms}ms\t${result.name} [${result.mode}] ${result.detail}\n`);
            rows.push(result);
        }
    } finally {
        await browser.close();
    }

    const report = P.formatSummary(rows.map((r) => ({ ...r, detail: r.detail || '' })));
    process.stdout.write(report);
    if (cfg.summary) await appendFile(cfg.summary, report);
    const counts = P.countStatuses(rows);
    process.stderr.write(`\n${counts.PASS} passed, ${counts.FAIL} failed, ${counts.WARN} warnings\n`);
    return P.exitCodeFor(rows);
}

main().then((code) => process.exit(code), (e) => {
    process.stderr.write(`dasweb-verify: ${e && e.stack ? e.stack : e}\n`);
    process.exit(1);
});

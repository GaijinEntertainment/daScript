// Pure half of the browser leg: expectations lookup, output-pane classification,
// verdict assembly, reporting. No playwright, no network, no DOM — everything
// here takes plain data and returns plain data, so node:test can cover it.
//
// The impure half (driving the live playground) lives in runner.mjs.

// printOutput() in web/examples/ui/src/main.js stamps the severity as the row's
// background colour, so the page itself tells us which lines it considered
// failures: #ff9393 = stderr / compiler error, #ff2d2d = abort / infrastructure.
export const ERROR_COLORS = new Set(['#ff9393', '#ff2d2d']);

// run-frame.html's per-frame glGetError poll (#3640). A failed GL call neither
// throws nor stops the program, so this line is the only in-page evidence.
export const GL_ERROR_RE = /^GL error:/;

// Lines that are failures regardless of how they were coloured. `EXCEPTION` is
// how utils/daScript/main.cpp reports an uncaught panic; `error[NNNNN]` is a
// compile diagnostic; the rest are the run frame's own and emscripten's.
export const ERROR_MARKERS = [
    /\bEXCEPTION\b/,
    /^runtime aborted:/,
    /\berror\[\d{5}\]/,
    /\bassert failed\b/,
    /\bAborted\(/,
    /^FS error writing /,
    /^asset .+: /,
    /^build failed/,
    /^build request failed/,
    /^build status failed/,
    /^build timed out/,
    /^build reported done with no artifact/,
    /^page build reported no html artifact/,
    /^wasm build error:/,
    /^wasm builds are not available/,
    /^sample store returned/,
];

// The output pane reports colours back as `rgb(r, g, b)`.
export function normalizeColor(color) {
    if (!color) return '';
    const m = /^rgba?\(\s*(\d+)\s*,\s*(\d+)\s*,\s*(\d+)/.exec(color);
    if (!m) return String(color).toLowerCase();
    const hex = (n) => Number(n).toString(16).padStart(2, '0');
    return '#' + hex(m[1]) + hex(m[2]) + hex(m[3]);
}

function ignoreMatcher(ignore) {
    const res = (ignore || []).map((p) => new RegExp(p));
    return (text) => res.some((re) => re.test(text));
}

// Split the output pane into the three things a verdict cares about. GL lines
// are pulled out before the error sweep so they report as their own class —
// "the program ran but the driver refused a call" is a different bug from
// "the program died".
export function classifyOutput(lines, ignore) {
    const ignored = ignoreMatcher(ignore);
    const gl = [];
    const errors = [];
    for (const line of lines) {
        const text = String(line.text ?? '');
        if (ignored(text)) continue;
        if (GL_ERROR_RE.test(text)) {
            if (!gl.includes(text)) gl.push(text);
            continue;
        }
        const color = normalizeColor(line.color);
        if (ERROR_COLORS.has(color) || ERROR_MARKERS.some((re) => re.test(text))) {
            errors.push(text);
        }
    }
    return { gl, errors };
}

export function stdoutMatches(lines, pattern) {
    const re = new RegExp(pattern);
    return lines.some((line) => re.test(String(line.text ?? '')));
}

// Production serves page-shape artifacts from a separate cookie-less run origin,
// but that is a property of the DEPLOYMENT, not a rule to hard-code here —
// --base-url points at staging boxes that may serve them from one origin. So this
// validates only what must always hold: an absolute http(s) URL naming an .html
// artifact. The src can come from nowhere else — main.js sets iframe.pg-page-frame
// from the build result alone.
export function artifactUrlFrom(src, siteUrl) {
    if (!src) return '';
    let url;
    try {
        url = new URL(src, siteUrl);
    } catch {
        return '';
    }
    if (!/^https?:$/.test(url.protocol)) return '';
    if (!/\.html?$/.test(url.pathname)) return '';
    return url.href;
}

export const KINDS = new Set(['graphics', 'console', 'audio']);

// Merge a sample's row over the table defaults. Fails closed twice over: an
// unlisted sample is an error (so a newly deployed sample cannot skip
// verification), and so is a row the runner would not know how to execute.
export function resolveSpec(table, name) {
    const row = table.samples ? table.samples[name] : undefined;
    if (row === undefined) {
        return { ok: false, error: 'no expectations row — add one to expectations.json' };
    }
    const spec = { ...(table.defaults || {}), ...row, name };
    if (!KINDS.has(spec.kind)) {
        return { ok: false, error: `unknown kind "${spec.kind}"` };
    }
    if (spec.kind === 'console' && !spec.stdout) {
        return { ok: false, error: 'console sample needs a "stdout" pattern' };
    }
    if (!Array.isArray(spec.modes) || spec.modes.length === 0) {
        return { ok: false, error: 'no modes listed' };
    }
    if (!(spec.budget_ms > 0)) {
        return { ok: false, error: 'budget_ms must be positive' };
    }
    // Missing would not misjudge anything, but it would silently hold every
    // sample open for its whole budget — minutes of nightly, no signal.
    if (!(spec.min_observe_ms > 0) || spec.min_observe_ms > spec.budget_ms) {
        return { ok: false, error: 'min_observe_ms must be positive and within budget_ms' };
    }
    if (spec.modes.includes('wasm') && !(spec.build_budget_ms > 0)) {
        return { ok: false, error: 'build_budget_ms must be positive' };
    }
    return { ok: true, spec };
}

// One row per sample x mode, in manifest order. An unresolvable sample yields a
// single FAIL row rather than being dropped.
export function planRows(samples, table, opts = {}) {
    const wanted = opts.modes && opts.modes.length ? new Set(opts.modes) : null;
    const filter = opts.filter ? String(opts.filter).toLowerCase() : '';
    const rows = [];
    for (const sample of samples) {
        const name = sample.name;
        if (filter && !name.toLowerCase().includes(filter)) continue;
        const resolved = resolveSpec(table, name);
        if (!resolved.ok) {
            rows.push({ name, mode: '-', status: 'FAIL', detail: resolved.error, ms: 0 });
            continue;
        }
        for (const mode of resolved.spec.modes) {
            if (wanted && !wanted.has(mode)) continue;
            rows.push({ name, mode, spec: resolved.spec, sample });
        }
    }
    return rows;
}

// The liveness protocol, in one place. Order matters: a wedge hides everything
// else, and a dead program's error line is a better report than "too few
// frames" — which is only the symptom.
export function verdict(spec, obs) {
    if (obs.wedge) {
        return { status: 'FAIL', detail: 'wedge: page stopped answering evaluate' };
    }
    const { gl, errors } = classifyOutput(obs.lines || [], spec.ignore);
    if (errors.length) {
        return { status: 'FAIL', detail: errors[0] };
    }
    const ignored = ignoreMatcher(spec.ignore);
    const pageErrors = (obs.pageErrors || []).filter((e) => !ignored(e));
    if (pageErrors.length) {
        return { status: 'FAIL', detail: 'page error: ' + pageErrors[0] };
    }
    if (gl.length) {
        return { status: 'FAIL', detail: gl.join(' | ') };
    }
    if (spec.kind === 'console') {
        if (stdoutMatches(obs.lines || [], spec.stdout)) return { status: 'PASS', detail: '' };
        return { status: 'FAIL', detail: `no output line matched /${spec.stdout}/` };
    }
    if (spec.kind === 'audio') {
        if (obs.started) return { status: 'PASS', detail: '' };
        return { status: 'FAIL', detail: 'program never started' };
    }
    // graphics: the page must keep painting (not wedged) AND the program's own
    // render loop must issue draws — a crashed emscripten loop leaves the host
    // page's rAF ticking, so the frame counter alone would call it alive.
    const rafs = obs.rafs || 0;
    const draws = obs.draws || 0;
    if (rafs < spec.min_rafs || draws < spec.min_draws) {
        return {
            status: 'FAIL',
            detail: `rafs ${rafs}/${spec.min_rafs}, draws ${draws}/${spec.min_draws}`,
        };
    }
    return { status: 'PASS', detail: `rafs ${rafs}, draws ${draws}` };
}

// Deploy lag mid-merge is legitimate, so manifest drift is reported, never
// failed. A sample only on the deployed side still gets verified (and fails
// closed if it has no expectations row).
export function driftWarnings(deployedNames, repoNames) {
    const repo = new Set(repoNames);
    const deployed = new Set(deployedNames);
    const rows = [];
    for (const name of deployedNames) {
        if (!repo.has(name)) {
            rows.push({ name, mode: '-', status: 'WARN', detail: 'deployed but not in the repo manifest', ms: 0 });
        }
    }
    for (const name of repoNames) {
        if (!deployed.has(name)) {
            rows.push({ name, mode: '-', status: 'WARN', detail: 'in the repo manifest but not deployed', ms: 0 });
        }
    }
    return rows;
}

function escapeCell(text) {
    return String(text ?? '').replace(/\|/g, '\\|').replace(/\r?\n/g, ' ').slice(0, 300);
}

export function formatSummary(rows, title = 'dasweb sample verifier') {
    const counts = countStatuses(rows);
    const out = [
        `## ${title}`,
        '',
        `${counts.PASS} passed, ${counts.FAIL} failed, ${counts.WARN} warnings`,
        '',
        '| sample | mode | status | ms | detail |',
        '| --- | --- | --- | ---: | --- |',
    ];
    for (const r of rows) {
        out.push(`| ${escapeCell(r.name)} | ${r.mode} | ${r.status} | ${r.ms ?? 0} | ${escapeCell(r.detail)} |`);
    }
    return out.join('\n') + '\n';
}

export function countStatuses(rows) {
    const counts = { PASS: 0, FAIL: 0, WARN: 0 };
    for (const r of rows) counts[r.status] = (counts[r.status] || 0) + 1;
    return counts;
}

export function exitCodeFor(rows) {
    return rows.some((r) => r.status === 'FAIL') ? 1 : 0;
}

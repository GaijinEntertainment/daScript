// Forge interactivity: hero sample switcher + (later) install-tab toggle +
// bench renderer. Phase 1 ships only the hero pieces.

(function () {
    const SAMPLES = {
        hello: {
            label: 'hello.das',
            code:
`[export]
def main() {
    let name = "world"
    print("hello, {name}\\n")
    for (i in range(3)) {
        print("  tick {i}\\n")
    }
}`,
            output: [
                { kind: 'cmd',    text: 'daslang run hello.das' },
                { kind: 'stdout', text: 'hello, world' },
                { kind: 'hint',   text: '  tick 0 · tick 1 · tick 2' },
                { kind: 'ok',     text: '✓ 0.42 ms · interpreted' },
            ],
        },
        fib: {
            label: 'fib.das',
            code:
`def fib_rec(n : int) : int {
    return n < 2 ? n : fib_rec(n-1) + fib_rec(n-2)
}

def fib_iter(n : int) : int {
    var a = 0
    var b = 1
    for (_ in range(n)) {
        let t = a + b; a = b; b = t
    }
    return a
}

[export]
def main() {
    for (i in range(10)) {
        print("fib({i})  rec={fib_rec(i)}  iter={fib_iter(i)}\\n")
    }
}`,
            output: [
                { kind: 'cmd',    text: 'daslang run fib.das' },
                { kind: 'stdout', text: 'fib(0)  rec=0  iter=0' },
                { kind: 'stdout', text: 'fib(1)  rec=1  iter=1' },
                { kind: 'hint',   text: '…through fib(9)  rec=34  iter=34' },
                { kind: 'ok',     text: '✓ 0.21 ms · interpreted' },
            ],
        },
        generic: {
            label: 'generic.das',
            code:
`struct Pet {
    name : string
    age  : int
}

// Generic: compile-time dispatched per type via static_if + typeinfo.
def show(v : auto(T)) : string {
    static_if (typeinfo is_numeric(type<T>)) {
        return "{typeinfo typename(type<T>)}: {v}"
    } elif (typeinfo is_string(type<T>)) {
        return "string: {v}"
    } elif (typeinfo is_ref_type(type<T>)) {
        static_if (typeinfo has_field<name>(v)) {
            return "{typeinfo typename(type<T>)}: {v.name}"
        }
        return "{typeinfo typename(type<T>)}: <opaque>"
    } else {
        return "?: {v}"
    }
}

// Partial specialization: wins for any array<T>, regardless of T.
def show(v : array<auto(T)>) : string {
    return "array<{typeinfo typename(type<T>)}>[{length(v)}]"
}

[export]
def main() {
    let nums <- [1, 2, 3, 4]
    print(show(42) + "\\n")
    print(show(3.14) + "\\n")
    print(show("hello") + "\\n")
    print(show(Pet(name="ada", age=3)) + "\\n")
    print(show(nums) + "\\n")
}`,
            output: [
                { kind: 'cmd',    text: 'daslang run generic.das' },
                { kind: 'stdout', text: 'int const: 42' },
                { kind: 'stdout', text: 'float const: 3.14' },
                { kind: 'stdout', text: 'string: hello' },
                { kind: 'stdout', text: 'Pet const: ada' },
                { kind: 'stdout', text: 'array<int>[4]' },
                { kind: 'ok',     text: '✓ 0.13 ms · interpreted' },
            ],
        },
    };

    function renderOutput(outputEl, lines) {
        const html = lines.map(l => {
            if (l.kind === 'cmd') {
                return `<div class="forge-output__line"><span class="forge-output__prompt">$</span> ${escapeHtml(l.text)}</div>`;
            }
            const cls = 'forge-output__' + (l.kind === 'cmd' ? 'stdout' : l.kind);
            return `<div class="forge-output__line ${cls}">${escapeHtml(l.text)}</div>`;
        }).join('');
        outputEl.innerHTML = html;
    }

    function escapeHtml(s) {
        return s.replace(/&/g, '&amp;').replace(/</g, '&lt;').replace(/>/g, '&gt;');
    }

    let currentSample = 'hello';
    let heroEditor = null;
    // User edits per sample key — preserved across tab switches.
    const editedCode = {};

    function initHeroEditor() {
        const el = document.getElementById('hero-editor');
        if (!el || !window.CodeMirror) return;
        heroEditor = window.CodeMirror(el, {
            mode: 'daslang',
            theme: 'forge',
            lineNumbers: true,
            matchBrackets: true,
            indentWithTabs: false,
            tabSize: 4,
            indentUnit: 4,
            styleActiveLine: true,
        });
    }

    function showSample(key) {
        const sample = SAMPLES[key];
        if (!sample) return;
        // Save any in-flight edit for the outgoing tab before switching.
        if (heroEditor && currentSample && currentSample !== key) {
            const live = heroEditor.getValue();
            if (live && live !== SAMPLES[currentSample]?.code) {
                editedCode[currentSample] = live;
            }
        }
        currentSample = key;
        const text = editedCode[key] ?? sample.code;
        if (heroEditor) heroEditor.setValue(text);
        const fileEl = document.getElementById('hero-file');
        const outEl  = document.getElementById('hero-output');
        if (fileEl) fileEl.textContent = sample.label;
        if (outEl)  renderOutput(outEl, sample.output);
        document.querySelectorAll('.forge-sample-tab').forEach(t => {
            t.classList.toggle('is-active', t.dataset.sample === key);
        });
    }

    function wireSampleTabs() {
        document.querySelectorAll('.forge-sample-tab').forEach(tab => {
            tab.addEventListener('click', () => showSample(tab.dataset.sample));
        });
    }

    // ↗ playground — open the current hero buffer in the full IDE.
    // Code travels via URL hash; playground-init.js applies it after the CM
    // instance comes up.
    function wirePlaygroundButton() {
        const btn = document.getElementById('hero-playground');
        if (!btn) return;
        btn.addEventListener('click', () => {
            const code = heroEditor ? heroEditor.getValue() : '';
            const url = 'playground/index.html#code=' + encodeURIComponent(code);
            window.open(url, '_blank');
        });
    }

    async function wireRunButton() {
        const btn = document.getElementById('hero-run');
        const outEl = document.getElementById('hero-output');
        if (!btn || !outEl) return;
        // Enable as soon as the runner.js is loaded — even if WASM isn't yet
        // available, we let the click try to load it and degrade.
        btn.disabled = false;
        btn.addEventListener('click', async () => {
            if (!window.daslangRun) {
                renderOutput(outEl, [{ kind: 'stderr', text: 'runner unavailable — try the full /playground/' }]);
                return;
            }
            btn.disabled = true;
            const prev = btn.textContent;
            btn.textContent = '… running';
            renderOutput(outEl, [
                { kind: 'cmd',  text: 'daslang run ' + (SAMPLES[currentSample]?.label || 'hero.das') },
                { kind: 'hint', text: '…' },
            ]);
            try {
                const r = await window.daslangRun(heroEditor ? heroEditor.getValue() : '');
                if (r.unavailable) {
                    renderOutput(outEl, [
                        { kind: 'stderr', text: 'WASM runtime not deployed at ./files/wasm/ — see site/README.md' },
                        { kind: 'hint',   text: 'open the full IDE at /playground/ for now' },
                    ]);
                } else {
                    const lines = [{ kind: 'cmd', text: 'daslang run ' + (SAMPLES[currentSample]?.label || 'hero.das') }];
                    if (r.stdout) lines.push({ kind: 'stdout', text: r.stdout });
                    if (r.stderr) lines.push({ kind: 'stderr', text: r.stderr });
                    const tag = r.exitCode === 0 ? 'ok' : 'stderr';
                    lines.push({ kind: tag, text: (r.exitCode === 0 ? '✓ ' : '✗ ') + r.durationMs.toFixed(2) + ' ms · interpreted' });
                    renderOutput(outEl, lines);
                }
            } finally {
                btn.disabled = false;
                btn.textContent = prev;
            }
        });
    }

    // ─── Install-section tab toggle ────────────────────────────────
    function wireInstallTabs() {
        const tabs  = document.querySelectorAll('.forge-install__tab');
        const panes = document.querySelectorAll('.forge-install__pane');
        tabs.forEach(tab => {
            tab.addEventListener('click', () => {
                const key = tab.dataset.pane;
                tabs.forEach(t  => t.classList.toggle('is-active', t === tab));
                panes.forEach(p => p.classList.toggle('is-active', p.dataset.pane === key));
            });
        });
    }

    // ─── Benchmarks (§ 01) ─────────────────────────────────────────
    //
    // Source: site/files/profile_results.json, vendored from
    // github.com/borisbat/dasProfile. Refreshed by .github/workflows/pages.yml.

    // Column order + display headers per category. Ported from
    // dasProfile/update_readme_benchmarks.py:SECTION_CONFIGS so the chart
    // mirrors the README exactly.
    const BENCH_COLS = {
        'Interpreted': [
            { id: 'DAS INTERPRETER',     label: 'Daslang · interp',  group: 'das' },
            { id: 'LUAU',                label: 'Luau',              group: 'rival' },
            { id: 'LUA',                 label: 'Lua',               group: 'rival' },
            { id: 'LUAJIT -joff',        label: 'LuaJIT -joff',      group: 'rival' },
            { id: 'QUIRREL',             label: 'Quirrel',           group: 'rival' },
            { id: 'QUICKJS',             label: 'QuickJS',           group: 'rival' },
            { id: 'MONO --interpreter',  label: 'Mono --interpreter',group: 'rival' },
        ],
        'AOT or JIT': [
            { id: 'DAS AOT',             label: 'Daslang · AOT',     group: 'das' },
            { id: 'DAS JIT',             label: 'Daslang · JIT',     group: 'das' },
            { id: 'C++',                 label: 'C++ · -O2',         group: 'native' },
            { id: 'LUAU --codegen',      label: 'Luau --codegen',    group: 'rival' },
            { id: 'LUAJIT',              label: 'LuaJIT',            group: 'rival' },
            { id: 'MONO',                label: 'Mono',              group: 'rival' },
            { id: '.NET',                label: '.NET',              group: 'rival' },
        ],
    };

    let benchData = null;
    let benchCat  = 'Interpreted';
    let benchBm   = 'sha256';

    async function loadBench() {
        try {
            const r = await fetch('./files/profile_results.json');
            benchData = await r.json();
        } catch (e) {
            const cap = document.getElementById('bench-caption');
            if (cap) cap.textContent = 'benchmark data unavailable';
            return;
        }
        populateBmDropdown();
        renderBench();
        wireBenchControls();
    }

    function populateBmDropdown() {
        const sel = document.getElementById('bench-bm');
        if (!sel || !benchData) return;
        const cat = benchData[benchCat];
        if (!cat) return;
        sel.innerHTML = Object.keys(cat).map(name =>
            `<option${name === benchBm ? ' selected' : ''}>${escapeHtml(name)}</option>`
        ).join('');
    }

    function wireBenchControls() {
        document.querySelectorAll('.forge-bench__cat').forEach(btn => {
            btn.addEventListener('click', () => {
                benchCat = btn.dataset.cat;
                document.querySelectorAll('.forge-bench__cat').forEach(b =>
                    b.classList.toggle('is-active', b.dataset.cat === benchCat));
                if (!(benchBm in (benchData[benchCat] || {}))) {
                    benchBm = Object.keys(benchData[benchCat] || { sha256: [] })[0];
                }
                populateBmDropdown();
                renderBench();
            });
        });
        const sel = document.getElementById('bench-bm');
        if (sel) sel.addEventListener('change', () => {
            benchBm = sel.value;
            renderBench();
        });
    }

    function renderBench() {
        const rowsEl = document.getElementById('bench-rows');
        const capEl  = document.getElementById('bench-caption');
        if (!rowsEl || !benchData) return;
        const cat = benchData[benchCat] || {};
        const series = cat[benchBm] || [];
        const byLang = Object.fromEntries(series.map(e => [e.language, e.time]));
        let cols = BENCH_COLS[benchCat] || [];

        // Experiment: on the AOT·JIT chart, splice in Daslang's interpreter
        // time (from the Interpreted dataset) as a reference row so readers
        // can see how the interp tier stacks against the compiled tiers. It's
        // an outlier — clearly slower — but informative.
        if (benchCat === 'AOT or JIT') {
            const interpSeries = (benchData['Interpreted'] || {})[benchBm] || [];
            const interpEntry = interpSeries.find(e => e.language === 'DAS INTERPRETER');
            if (interpEntry) {
                byLang['DAS INTERP (ref)'] = interpEntry.time;
                cols = [
                    ...cols.slice(0, 2),  // DAS AOT, DAS JIT
                    { id: 'DAS INTERP (ref)', label: 'Daslang · interp (ref)', group: 'das-ref' },
                    ...cols.slice(2),
                ];
            }
        }

        const times = cols.map(c => byLang[c.id]).filter(t => typeof t === 'number' && t > 0);
        if (times.length === 0) {
            rowsEl.innerHTML = '<div class="forge-bench__caption">no data for this combination</div>';
            return;
        }
        const fastest = Math.min(...times);
        const slowest = Math.max(...times);

        rowsEl.innerHTML = cols.map(c => {
            const t = byLang[c.id];
            if (typeof t !== 'number' || t <= 0) {
                return `<div class="forge-bench__row forge-bench__row--rival">
                    <div class="forge-bench__label">${escapeHtml(c.label)}</div>
                    <div class="forge-bench__bar"></div>
                    <div class="forge-bench__num">—</div>
                </div>`;
            }
            const rel = t / fastest;
            const pct = (t / slowest) * 100;
            return `<div class="forge-bench__row forge-bench__row--${c.group}">
                <div class="forge-bench__label">${escapeHtml(c.label)}</div>
                <div class="forge-bench__bar"><div class="forge-bench__bar-fill" style="width: ${pct.toFixed(1)}%"></div></div>
                <div class="forge-bench__num">${rel.toFixed(2)}×</div>
            </div>`;
        }).join('');

        if (capEl) {
            const cpu = benchData.cpu || 'unknown CPU';
            const v   = benchData.versions || {};
            const ts  = (benchData.timestamp || '').split(' ').slice(0, 4).join(' ');
            capEl.textContent = `${cpu} · daslang ${v.daslang || '?'} · LLVM ${v.llvm || '?'} · captured ${ts} · source: github.com/borisbat/dasProfile`;
        }
    }

    // ─── § 05 News feed (top 5 from news.json) ─────────────────────
    async function loadNews() {
        const rowsEl = document.getElementById('news-rows');
        if (!rowsEl) return;
        let news;
        try {
            const r = await fetch('./files/news.json');
            news = await r.json();
        } catch (e) {
            return; // leave the hardcoded placeholders
        }
        const top = (news || []).slice(0, 5);
        if (!top.length) return;
        rowsEl.innerHTML = top.map(n => {
            const title = escapeHtml(n.title);
            const titleHtml = n.link
                ? `<a href="${escapeHtml(n.link)}">${title}</a>`
                : title;
            return `<div class="forge-news__row">
                <div class="forge-news__date">${escapeHtml(n.date)}</div>
                <div class="forge-news__tag">${escapeHtml(n.tag)}</div>
                <div class="forge-news__title">${titleHtml}</div>
            </div>`;
        }).join('');
    }

    // Mobile shows a static highlighted hero (in index.html, .forge-hero__code).
    // CSS hides the .forge-editor / sample tabs / run + playground buttons, so
    // we skip the heavy CM init + sample wiring entirely — keeps the static
    // block from getting a hidden CodeMirror layered on top of it.
    function isMobile() {
        return window.matchMedia('(max-width: 767px)').matches;
    }

    function init() {
        if (!isMobile()) {
            wireSampleTabs();
            initHeroEditor();
            showSample('hello');
            wireRunButton();
            wirePlaygroundButton();
        }
        wireInstallTabs();
        loadBench();
        loadNews();
    }

    if (document.readyState === 'loading') {
        document.addEventListener('DOMContentLoaded', init);
    } else {
        init();
    }
})();

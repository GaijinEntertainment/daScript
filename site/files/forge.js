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
        return s.replace(/&/g, '&amp;').replace(/</g, '&lt;').replace(/>/g, '&gt;')
                .replace(/"/g, '&quot;').replace(/'/g, '&#39;');
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
    // Multi-platform cycler. One profile JSON per platform; left rail lists
    // platforms, clicking a row swaps the bars on the right. Mode tabs
    // (Interpreted / AOT or JIT) and a test-selector chip live in the card
    // header; a "try <test> on the playground →" deep-link in the footer
    // jumps into /playground/?example=<slug>.
    //
    // Source: site/files/profile_results_<platform>.json, vendored from
    // github.com/borisbat/dasProfile and fetched by pages.yml.

    // Hardcoded platform list. Mirrors the curl loop in pages.yml — when
    // Linux numbers exist, append 'linux' to both lists.
    const PLATFORMS      = ['darwin', 'windows'];
    const PLATFORM_ORDER = { darwin: 0, linux: 1, windows: 2 };
    const PLATFORM_OS    = { darwin: 'macOS', linux: 'Linux', windows: 'Windows' };

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

    let profiles = [];          // normalized BenchProfile[] (see normalizeProfile)
    let pIdx     = 0;
    let benchCat = 'Interpreted';
    let benchBm  = 'sha256';

    async function loadBench() {
        const results = await Promise.allSettled(PLATFORMS.map(p =>
            fetch('./files/profile_results_' + p + '.json')
                .then(r => { if (!r.ok) throw new Error(r.status); return r.json(); })
                .then(json => ({ platform: p, json }))
        ));
        for (const r of results) {
            if (r.status === 'fulfilled') {
                profiles.push(normalizeProfile(r.value.platform, r.value.json));
            }
            // Rejected fetches drop silently — pages.yml controls which
            // files ship; a missing JSON is a build-time issue, not UX.
        }
        if (!profiles.length) {
            const rowsEl = document.getElementById('bench-rows');
            if (rowsEl) rowsEl.innerHTML = '<div class="forge-bench__caption">benchmark data unavailable</div>';
            return;
        }
        profiles.sort((a, b) =>
            (PLATFORM_ORDER[a.platform] ?? 99) - (PLATFORM_ORDER[b.platform] ?? 99)
        );

        // Optional ?bench=<test> URL param — symmetric with the playground's
        // ?example=<slug>. Lets the cycler land on a specific workload when
        // the visitor follows a deep-link from elsewhere.
        try {
            const params = new URLSearchParams(window.location.search);
            const wantedBench = params.get('bench');
            if (wantedBench && profiles.some(p =>
                wantedBench in (p.sections['Interpreted'] || {}) ||
                wantedBench in (p.sections['AOT or JIT']  || {})
            )) {
                benchBm = wantedBench;
            }
        } catch (e) { /* swallow */ }

        renderPlatformList();
        renderHeader();
        renderBench();
        renderFooter();
        wireBenchControls();
    }

    function normalizeProfile(platform, json) {
        const cpu = json.cpu || '(unknown CPU)';
        const os  = PLATFORM_OS[platform] || platform;
        const v   = json.versions || {};
        const ts  = (json.timestamp || '').split(' ').slice(0, 4).join(' ');
        return {
            platform,
            cpu, os,
            label: `${cpu} · ${os}`,
            meta:  `daslang ${v.daslang || '?'} · LLVM ${v.llvm || '?'} · captured ${ts}`,
            sections: {
                'Interpreted': json['Interpreted'] || {},
                'AOT or JIT':  json['AOT or JIT']  || {},
            },
        };
    }

    function renderPlatformList() {
        const root = document.getElementById('bench-platforms');
        if (!root) return;
        root.innerHTML = profiles.map((p, i) =>
            `<div class="forge-bench__platform${i === pIdx ? ' is-active' : ''}" data-idx="${i}">
                <div class="forge-bench__platform-label">${escapeHtml(p.label)}</div>
                <div class="forge-bench__platform-meta">${escapeHtml(p.meta)}</div>
            </div>`
        ).join('');
        root.querySelectorAll('.forge-bench__platform').forEach(el => {
            el.addEventListener('click', () => {
                pIdx = parseInt(el.dataset.idx, 10);
                root.querySelectorAll('.forge-bench__platform').forEach(x =>
                    x.classList.toggle('is-active', parseInt(x.dataset.idx, 10) === pIdx));
                // If the active workload doesn't exist on the new platform/cat,
                // fall back to the first available so the chart doesn't render
                // empty. Current dasProfile data has identical workload sets
                // across platforms; this guards against future asymmetry.
                const cat = profiles[pIdx]?.sections[benchCat] || {};
                if (!(benchBm in cat)) {
                    const first = Object.keys(cat)[0];
                    if (first) benchBm = first;
                }
                renderHeader();
                renderBench();
                renderFooter();
            });
        });
    }

    function renderHeader() {
        // Decorative chip in single-workload mode; when 2+ workloads exist we
        // show the <select> and populate it. Use inline `style.display`
        // rather than the `hidden` attribute because `.forge-bench__test-chip
        // { display: inline-flex }` overrides the UA `[hidden] { display:
        // none }` rule.
        const chip = document.getElementById('bench-chip');
        const sel  = document.getElementById('bench-bm');
        const cat  = profiles[pIdx]?.sections[benchCat] || {};
        const workloads = Object.keys(cat);
        if (workloads.length <= 1) {
            if (chip) {
                chip.style.display = '';   // restore inline-flex from stylesheet
                chip.firstChild && (chip.firstChild.nodeValue = benchBm + ' ');
            }
            if (sel) sel.style.display = 'none';
        } else {
            if (chip) chip.style.display = 'none';
            if (sel) {
                sel.style.display = '';
                sel.innerHTML = workloads.map(name =>
                    `<option${name === benchBm ? ' selected' : ''}>${escapeHtml(name)}</option>`
                ).join('');
            }
        }
    }

    function wireBenchControls() {
        document.querySelectorAll('.forge-bench__cat').forEach(btn => {
            btn.addEventListener('click', () => {
                benchCat = btn.dataset.cat;
                document.querySelectorAll('.forge-bench__cat').forEach(b =>
                    b.classList.toggle('is-active', b.dataset.cat === benchCat));
                // Make sure the active test still exists in the new category.
                const cat = profiles[pIdx]?.sections[benchCat] || {};
                if (!(benchBm in cat)) {
                    const first = Object.keys(cat)[0];
                    if (first) benchBm = first;
                }
                renderHeader();
                renderBench();
                renderFooter();
            });
        });
        const sel = document.getElementById('bench-bm');
        if (sel) sel.addEventListener('change', () => {
            benchBm = sel.value;
            renderBench();
            renderFooter();
        });
    }

    function renderBench() {
        const rowsEl = document.getElementById('bench-rows');
        if (!rowsEl) return;
        const profile = profiles[pIdx];
        if (!profile) {
            rowsEl.innerHTML = '<div class="forge-bench__caption">benchmark data unavailable</div>';
            return;
        }
        const cat = profile.sections[benchCat] || {};
        const series = cat[benchBm] || [];
        const byLang = Object.fromEntries(series.map(e => [e.language, e.time]));
        let cols = BENCH_COLS[benchCat] || [];

        // Experiment: on the AOT·JIT chart, splice in Daslang's interpreter
        // time (from the Interpreted dataset) as a reference row so readers
        // can see how the interp tier stacks against the compiled tiers. It's
        // an outlier — clearly slower — but informative.
        if (benchCat === 'AOT or JIT') {
            const interpSeries = (profile.sections['Interpreted'] || {})[benchBm] || [];
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
    }

    // dasProfile workload names use hyphens (`spectral-norm`, `table-sort`);
    // playground sample basenames use underscores (`spectral_norm.das`,
    // `table_sort.das`). The cross-language harness keeps hyphens upstream
    // so its scripts (`spectral-norm.lua`, etc.) keep working. Normalize
    // here when building the playground URL.
    function benchToSampleSlug(name) {
        return name.replace(/-/g, '_');
    }

    function renderFooter() {
        const el = document.getElementById('bench-footer');
        if (!el) return;
        const slug = encodeURIComponent(benchToSampleSlug(benchBm));
        el.innerHTML =
            `<a href="/playground/index.html?example=${slug}">try <span class="forge-bench__playground-test">${escapeHtml(benchBm)}</span> on the playground →</a>`;
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

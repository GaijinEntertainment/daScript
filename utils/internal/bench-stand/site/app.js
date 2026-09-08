// daslang benchmark stand viewer. Reads data.json (built by bench-stand `report`) and
// status.json (written by nightly.sh) from the same directory and renders everything
// client-side: the latest night, the group index, one chart per benchmark arm, run history.
"use strict";

const LANES = ["interp", "jit"];
const state = {
    data: null,
    status: null,
    runWindow: 365,
    lanes: new Set(LANES),
    group: "",
    search: "",
    onlyChanged: false,
};

const $ = (sel, root) => (root || document).querySelector(sel);
const el = (tag, cls, text) => {
    const e = document.createElement(tag);
    if (cls) e.className = cls;
    if (text !== undefined) e.textContent = text;
    return e;
};

function fmtNs(v) {
    if (v >= 1e6) return (v / 1e6).toFixed(2) + " ms";
    if (v >= 1e3) return (v / 1e3).toFixed(2) + " us";
    if (v >= 100) return v.toFixed(0) + " ns";
    return v.toFixed(v >= 10 ? 1 : 2) + " ns";
}
const fmtPct = (x) => (x >= 0 ? "+" : "") + (x * 100).toFixed(1) + "%";
const fmtSec = (s) => (s >= 3600 ? (s / 3600).toFixed(1) + " h" : s >= 60 ? Math.round(s / 60) + " min" : Math.round(s) + " s");
const shortSha = (sha) => (sha || "").slice(0, 8);
const dateOf = (iso) => (iso || "").slice(0, 10);
function commitUrl(sha) {
    const base = state.data && state.data.repo_url;
    return base && sha ? base.replace(/\/$/, "") + "/commit/" + sha : null;
}

// ---- data loading ----------------------------------------------------------------------------

async function load() {
    const [dataRes, statusRes] = await Promise.allSettled([
        fetch("data.json", { cache: "no-store" }).then((r) => (r.ok ? r.json() : Promise.reject(new Error("data.json " + r.status)))),
        fetch("status.json", { cache: "no-store" }).then((r) => (r.ok ? r.json() : null)),
    ]);
    state.status = statusRes.status === "fulfilled" ? statusRes.value : null;
    if (dataRes.status === "fulfilled") {
        state.data = dataRes.value;
    } else {
        const main = $("main");
        main.replaceChildren(el("p", "notice notice--error", "No data.json yet: the stand has not published a report (" + dataRes.reason.message + ")."));
    }
    renderStatus();
    if (state.data) renderAll();
}

// ---- visible runs (the range filter) ---------------------------------------------------------

function visibleRunIndices() {
    const runs = state.data.runs;
    const n = runs.length;
    const from = state.runWindow > 0 ? Math.max(0, n - state.runWindow) : 0;
    const out = [];
    for (let i = from; i < n; i++) out.push(i);
    return out;
}

// ---- status header ---------------------------------------------------------------------------

function renderStatus() {
    const box = $("#status");
    box.replaceChildren();
    const st = state.status;
    if (st && st.state === "running") {
        const pill = el("span", "pill pill--running", "running " + (st.run_id || ""));
        box.append(pill, el("span", "muted", "started " + (st.started || "")));
    }
    if (!state.data || state.data.latest < 0) {
        if (!st) box.append(el("span", "pill", "no runs"));
        return;
    }
    const run = state.data.runs[state.data.latest];
    box.append(el("span", "pill pill--" + run.status, run.status.replace("_", " ")));
    const link = el("a", null, shortSha(run.sha) + " " + run.subject);
    link.href = commitUrl(run.sha) || "#";
    link.title = run.date;
    box.append(link);
    box.append(el("span", "muted", dateOf(run.started) + " on " + run.host + ", build " + fmtSec(run.build_seconds) + ", suite " + fmtSec(run.seconds)));
    const ageDays = (Date.now() - Date.parse(run.started)) / 86400000;
    if (ageDays > 2 && !(st && st.state === "running")) {
        box.append(el("span", "pill pill--stale", "last run " + Math.floor(ageDays) + " days ago"));
    }
    $("#generated").textContent = "report generated " + (state.data.generated || "");
}

// ---- filters ---------------------------------------------------------------------------------

function wireFilters() {
    document.querySelectorAll(".chip[data-range]").forEach((b) => {
        b.addEventListener("click", () => {
            document.querySelectorAll(".chip[data-range]").forEach((x) => x.classList.remove("is-on"));
            b.classList.add("is-on");
            state.runWindow = Number(b.dataset.range);
            renderAll();
        });
    });
    document.querySelectorAll("input[data-lane]").forEach((c) => {
        c.addEventListener("change", () => {
            if (c.checked) state.lanes.add(c.dataset.lane); else state.lanes.delete(c.dataset.lane);
            renderAll();
        });
    });
    $("#group").addEventListener("change", (e) => { state.group = e.target.value; renderSeries(); });
    let timer = 0;
    $("#search").addEventListener("input", (e) => {
        clearTimeout(timer);
        timer = setTimeout(() => { state.search = e.target.value.trim().toLowerCase(); renderSeries(); }, 120);
    });
    $("#only-changed").addEventListener("change", (e) => { state.onlyChanged = e.target.checked; renderSeries(); });
}

function fillGroups() {
    const sel = $("#group");
    const keep = sel.value;
    sel.replaceChildren(el("option", null, "all groups"));
    sel.firstChild.value = "";
    for (const g of state.data.groups) {
        const o = el("option", null, g);
        o.value = g;
        sel.append(o);
    }
    sel.value = state.data.groups.includes(keep) ? keep : "";
}

// ---- latest night ----------------------------------------------------------------------------

function renderNight() {
    const box = $("#night");
    box.replaceChildren();
    const d = state.data;
    if (d.latest < 0) return;
    const run = d.runs[d.latest];

    const fails = el("div", "card");
    if (run.build_status && run.build_status !== "ok") {
        fails.append(header("The build failed", "", "nothing was measured"));
        const why = el("div", "msg");
        why.append(el("div", null, "after " + fmtSec(run.build_seconds) + "; last lines of the build log:"));
        const pre = el("pre", "logtail");
        pre.textContent = run.build_log_tail || "the driver recorded no build log";
        why.append(pre);
        fails.append(why);
        const rec = el("a", "msg", "run record");
        rec.href = "runs/" + encodeURIComponent(run.id) + ".json";
        fails.append(rec);
        box.append(fails);
        return;
    }
    fails.append(header("Failures", run.failures.length));
    fails.append(list(run.failures, (f) => {
        const li = el("li");
        li.append(el("span", "tag tag--failure", f.status.replace("_", " ")), el("span", "tag tag--" + f.lane, f.lane), el("code", null, f.path));
        const m = el("div", "msg");
        m.append(el("code", null, f.message));
        li.append(m);
        return li;
    }, "every file ran"));
    const lanes = el("div", "msg");
    for (const lane of Object.keys(run.lanes)) {
        if (run.lanes[lane] !== "ok") lanes.append(el("div", null, lane + " lane did not run: " + run.lanes[lane]));
    }
    if (run.skipped.length) {
        const s = el("div", "msg");
        s.append(el("div", null, run.skipped.length + " skipped:"));
        for (const f of run.skipped) {
            const line = el("div");
            line.append(el("code", null, f.path), " (" + f.lane + "): " + f.message);
            s.append(line);
        }
        lanes.append(s);
    }
    fails.append(lanes);
    const link = el("a", "msg", "run record");
    link.href = "runs/" + encodeURIComponent(run.id) + ".json";
    fails.append(link);

    const regs = d.changes.filter((c) => c.kind === "regression");
    const imps = d.changes.filter((c) => c.kind === "improvement");
    const newOrMissing = d.changes.filter((c) => c.kind === "first_seen" || c.kind === "missing");
    const changeItem = (c) => {
        const li = el("li");
        const a = el("a", null, c.id);
        a.href = "#" + anchorId(c.id);
        li.append(el("span", "tag tag--" + c.lane, c.lane), a);
        if (c.kind === "regression" || c.kind === "improvement") {
            li.append(el("span", "delta " + (c.change > 0 ? "delta--up" : "delta--down"), fmtPct(c.change)));
            li.append(el("span", "muted", fmtNs(c.baseline) + " -> " + fmtNs(c.value) + ", noise " + (c.noise * 100).toFixed(1) + "%"));
        } else {
            li.append(el("span", "tag", c.kind));
        }
        return li;
    };
    const regCard = el("div", "card");
    regCard.append(header("Regressions", regs.length, "slower than " + fmtPct(d.regression_threshold) + " and " + d.noise_multiplier + "x noise over the median of " + d.baseline_runs + " runs"));
    regCard.append(list(regs, changeItem, "nothing slower"));
    const impCard = el("div", "card");
    impCard.append(header("Improvements", imps.length));
    impCard.append(list(imps, changeItem, "nothing faster"));
    if (newOrMissing.length) {
        impCard.append(header("First seen or missing", newOrMissing.length));
        impCard.append(list(newOrMissing, changeItem, ""));
    }
    box.append(fails, regCard, impCard);
}

function header(title, count, sub) {
    const h = el("h3");
    h.append(title);
    if (count !== "") h.append(el("span", "count", String(count)));
    if (sub) h.append(el("span", "muted", sub));
    return h;
}
function list(items, render, emptyText) {
    const ul = el("ul");
    if (!items.length) {
        if (emptyText) ul.append(el("li", "empty", emptyText));
        return ul;
    }
    for (const it of items) ul.append(render(it));
    return ul;
}

// ---- charts ----------------------------------------------------------------------------------

const W = 420, H = 150, PAD = { l: 44, r: 54, t: 10, b: 22 };

function niceTicks(lo, hi, count) {
    if (!(hi > lo)) { hi = lo + 1; }
    const span = hi - lo;
    const step0 = span / Math.max(1, count);
    const mag = Math.pow(10, Math.floor(Math.log10(step0)));
    const step = [1, 2, 2.5, 5, 10].map((m) => m * mag).find((s) => s >= step0) || 10 * mag;
    const start = Math.floor(lo / step) * step;
    const out = [];
    for (let v = start; v <= hi + step * 0.5; v += step) out.push(+v.toPrecision(12));
    return out;
}

// lines: [{lane, points: [{r, v, s}]}], runIdx: visible run indices (x positions)
function drawChart(lines, runIdx, opts) {
    const svg = document.createElementNS("http://www.w3.org/2000/svg", "svg");
    svg.setAttribute("viewBox", `0 0 ${W} ${H}`);
    svg.setAttribute("class", "chart");
    svg.setAttribute("role", "img");
    svg.setAttribute("aria-label", opts.label);
    const xOf = new Map();
    const n = runIdx.length;
    runIdx.forEach((ri, i) => xOf.set(ri, PAD.l + (n === 1 ? (W - PAD.l - PAD.r) / 2 : (i * (W - PAD.l - PAD.r)) / (n - 1))));
    let lo = Infinity, hi = -Infinity;
    for (const ln of lines) for (const p of ln.points) {
        if (!xOf.has(p.r)) continue;
        const top = p.v * (1 + (p.s || 0));
        if (p.v < lo) lo = p.v;
        if (top > hi) hi = top;
    }
    if (!isFinite(lo)) { lo = 0; hi = 1; }
    if (opts.zeroBased) lo = Math.min(lo, 0);
    const pad = (hi - lo) * 0.12 || hi * 0.1 || 1;
    lo = Math.max(opts.zeroBased ? 0 : -Infinity, lo - pad);
    hi = hi + pad;
    const ticks = niceTicks(lo, hi, 4);
    lo = Math.min(lo, ticks[0]);
    hi = Math.max(hi, ticks[ticks.length - 1]);
    const yOf = (v) => PAD.t + (H - PAD.t - PAD.b) * (1 - (v - lo) / (hi - lo));
    const svgEl = (tag, attrs) => {
        const e = document.createElementNS("http://www.w3.org/2000/svg", tag);
        for (const k in attrs) e.setAttribute(k, attrs[k]);
        return e;
    };
    for (const tv of ticks) {
        if (tv < lo || tv > hi) continue;
        const y = yOf(tv);
        svg.append(svgEl("line", { class: "grid-line", x1: PAD.l, x2: W - PAD.r, y1: y, y2: y }));
        const t = svgEl("text", { x: PAD.l - 6, y: y + 3.5, "text-anchor": "end" });
        t.textContent = opts.fmt(tv);
        svg.append(t);
    }
    svg.append(svgEl("line", { class: "axis-line", x1: PAD.l, x2: W - PAD.r, y1: H - PAD.b, y2: H - PAD.b }));
    const runs = state.data.runs;
    const labelEvery = Math.max(1, Math.ceil(n / 5));
    runIdx.forEach((ri, i) => {
        if (i % labelEvery !== 0 && i !== n - 1) return;
        const t = svgEl("text", { x: xOf.get(ri), y: H - 6, "text-anchor": i === n - 1 ? "end" : i === 0 ? "start" : "middle" });
        t.textContent = dateOf(runs[ri].date).slice(5);
        svg.append(t);
    });
    if (opts.failedRuns) {
        for (const ri of opts.failedRuns) {
            if (!xOf.has(ri)) continue;
            svg.append(svgEl("rect", { class: "fail-mark", x: xOf.get(ri) - 2, y: H - PAD.b - 5, width: 4, height: 5 }));
        }
    }
    for (const ln of lines) {
        const pts = ln.points.filter((p) => xOf.has(p.r));
        if (!pts.length) continue;
        if (opts.band) {
            let d = "";
            for (const p of pts) d += (d ? "L" : "M") + xOf.get(p.r).toFixed(1) + "," + yOf(p.v * (1 + p.s)).toFixed(1);
            for (let i = pts.length - 1; i >= 0; i--) d += "L" + xOf.get(pts[i].r).toFixed(1) + "," + yOf(pts[i].v).toFixed(1);
            svg.append(svgEl("path", { class: "band band--" + ln.lane, d: d + "Z" }));
        }
        let d = "";
        let prev = -2;
        for (const p of pts) {
            const gap = runIdx.indexOf(p.r) - prev > 1;
            d += (gap ? "M" : "L") + xOf.get(p.r).toFixed(1) + "," + yOf(p.v).toFixed(1);
            prev = runIdx.indexOf(p.r);
        }
        svg.append(svgEl("path", { class: "series series--" + ln.lane, d }));
        if (pts.length === 1) svg.append(svgEl("circle", { class: "marker marker--" + ln.lane, cx: xOf.get(pts[0].r), cy: yOf(pts[0].v), r: 4 }));
        const last = pts[pts.length - 1];
        const lbl = svgEl("text", { class: "end-label", x: W - PAD.r + 6, y: yOf(last.v) + 3.5 });
        lbl.textContent = opts.fmt(last.v);
        svg.append(lbl);
    }
    attachHover(svg, svgEl, lines, runIdx, opts, xOf, yOf);
    return svg;
}

// The hover layer: a crosshair that snaps to the nearest run, one tooltip listing every lane at
// that run, and a click that opens the commit. The hit rectangle spans the plot and overhangs it,
// so a reader aims at a date rather than at a 2px line.
function attachHover(svg, svgEl, lines, runIdx, opts, xOf, yOf) {
    const runs = state.data.runs;
    const cross = svgEl("line", { class: "crosshair", x1: 0, x2: 0, y1: PAD.t, y2: H - PAD.b, visibility: "hidden" });
    svg.append(cross);
    const markers = lines.map((ln) => {
        const m = svgEl("circle", { class: "marker marker--" + ln.lane, r: 4, visibility: "hidden" });
        svg.append(m);
        return m;
    });
    const hit = svgEl("rect", { class: "hit", x: PAD.l - 10, y: 0, width: W - PAD.l - PAD.r + 20, height: H });
    svg.append(hit);
    const tooltip = $("#tooltip");
    const nearestRun = (evt) => {
        const rect = svg.getBoundingClientRect();
        const px = ((evt.clientX - rect.left) / rect.width) * W;
        let best = -1, bestD = Infinity;
        for (const ri of runIdx) {
            const dx = Math.abs(xOf.get(ri) - px);
            if (dx < bestD) { bestD = dx; best = ri; }
        }
        return best;
    };
    const show = (evt) => {
        const best = nearestRun(evt);
        if (best < 0) return;
        const x = xOf.get(best);
        cross.setAttribute("x1", x); cross.setAttribute("x2", x); cross.setAttribute("visibility", "visible");
        tooltip.replaceChildren();
        const run = runs[best];
        tooltip.append(el("div", "tt-head", dateOf(run.date) + " " + shortSha(run.sha) + " " + run.subject));
        lines.forEach((ln, i) => {
            const p = ln.points.find((q) => q.r === best);
            if (!p) { markers[i].setAttribute("visibility", "hidden"); return; }
            markers[i].setAttribute("cx", x); markers[i].setAttribute("cy", yOf(p.v)); markers[i].setAttribute("visibility", "visible");
            const row = el("div", "tt-row");
            row.append(el("span", "tt-key tt-key--" + ln.lane), el("span", "tt-val", opts.fmt(p.v)), el("span", null, ln.lane));
            if (p.s !== undefined && opts.band) row.append(el("span", "tt-sub", "spread " + (p.s * 100).toFixed(1) + "%"));
            tooltip.append(row);
        });
        if (opts.failedRuns && opts.failedRuns.has(best)) tooltip.append(el("div", "tt-sub", "this file failed that night"));
        tooltip.hidden = false;
        const tw = tooltip.offsetWidth, th = tooltip.offsetHeight;
        let left = evt.clientX + 14, top = evt.clientY + 14;
        if (left + tw > window.innerWidth - 8) left = evt.clientX - tw - 14;
        if (top + th > window.innerHeight - 8) top = evt.clientY - th - 14;
        tooltip.style.left = left + "px"; tooltip.style.top = top + "px";
    };
    const hide = () => {
        cross.setAttribute("visibility", "hidden");
        markers.forEach((m) => m.setAttribute("visibility", "hidden"));
        tooltip.hidden = true;
    };
    hit.addEventListener("pointermove", show);
    hit.addEventListener("pointerleave", hide);
    hit.addEventListener("click", (evt) => {
        const best = nearestRun(evt);
        const url = best >= 0 ? commitUrl(runs[best].sha) : null;
        if (url) window.open(url, "_blank", "noopener");
    });
}

function legend(lanes) {
    const lg = el("div", "legend");
    for (const lane of lanes) lg.append(el("span", "legend--" + lane, lane));
    return lg;
}

function tableView(lines, runIdx, fmt) {
    const t = el("table", "mini");
    const thead = el("tr");
    thead.append(el("th", null, "run"));
    for (const ln of lines) thead.append(el("th", "num", ln.lane));
    t.append(thead);
    const shown = runIdx.slice(-12).reverse();
    for (const ri of shown) {
        const tr = el("tr");
        const run = state.data.runs[ri];
        tr.append(el("td", null, dateOf(run.date) + " " + shortSha(run.sha)));
        for (const ln of lines) {
            const p = ln.points.find((q) => q.r === ri);
            tr.append(el("td", "num", p ? fmt(p.v) : "-"));
        }
        t.append(tr);
    }
    return t;
}

function chartCard(title, fileLabel, lines, runIdx, opts) {
    const card = el("div", "card chart-card");
    if (opts.anchor) card.id = opts.anchor;
    const head = el("div", "chart-card__head");
    const ttl = el("div", "chart-card__title");
    if (fileLabel) ttl.append(el("span", "file", fileLabel + " "));
    ttl.append(title);
    head.append(ttl);
    const tools = el("div", "chart-card__tools");
    if (opts.badges) for (const b of opts.badges) tools.append(b);
    const toggle = el("button", null, "table");
    tools.append(toggle);
    head.append(tools);
    card.append(head);
    const svg = drawChart(lines, runIdx, opts);
    card.append(svg);
    if (lines.length > 1) card.append(legend(lines.map((l) => l.lane)));
    let table = null;
    toggle.addEventListener("click", () => {
        if (table) { table.remove(); table = null; toggle.textContent = "table"; return; }
        table = tableView(lines, runIdx, opts.fmt);
        card.append(table);
        toggle.textContent = "chart only";
    });
    return card;
}

const anchorId = (id) => "b-" + id.replace(/[^A-Za-z0-9_-]/g, "_");

function renderIndex() {
    const box = $("#index");
    box.replaceChildren();
    const runIdx = visibleRunIndices();
    const byGroup = new Map();
    for (const ix of state.data.index) {
        if (!state.lanes.has(ix.lane)) continue;
        if (!byGroup.has(ix.group)) byGroup.set(ix.group, []);
        byGroup.get(ix.group).push({ lane: ix.lane, points: ix.runs.map((r, i) => ({ r, v: ix.value[i], s: 0 })) });
    }
    if (!byGroup.size) { box.append(el("p", "notice", "The index needs at least three runs per series.")); return; }
    for (const [group, lines] of [...byGroup.entries()].sort((a, b) => a[0].localeCompare(b[0]))) {
        lines.sort((a, b) => LANES.indexOf(a.lane) - LANES.indexOf(b.lane));
        box.append(chartCard(group, "", lines, runIdx, { label: "index of " + group, fmt: (v) => v.toFixed(0), band: false, zeroBased: false }));
    }
}

function failedRunsByFile() {
    const out = new Map();
    state.data.runs.forEach((run, ri) => {
        for (const f of run.failures) {
            const key = f.lane + "\t" + f.path.replace(/\.das$/, "");
            if (!out.has(key)) out.set(key, new Set());
            out.get(key).add(ri);
        }
    });
    return out;
}

function renderSeries() {
    const box = $("#series");
    box.replaceChildren();
    const d = state.data;
    const runIdx = visibleRunIndices();
    const visible = new Set(runIdx);
    const changeOf = new Map();
    for (const c of d.changes) changeOf.set(c.lane + "\t" + c.id, c);
    const failed = failedRunsByFile();
    const arms = new Map();
    for (const s of d.series) {
        if (!state.lanes.has(s.lane)) continue;
        if (state.group && s.group !== state.group) continue;
        if (state.search && !s.id.toLowerCase().includes(state.search)) continue;
        const ch = changeOf.get(s.lane + "\t" + s.id);
        if (state.onlyChanged && !(ch && (ch.kind === "regression" || ch.kind === "improvement"))) continue;
        if (!s.runs.some((r) => visible.has(r))) continue;
        if (!arms.has(s.id)) arms.set(s.id, { group: s.group, file: s.file, lines: [], changes: [] });
        const arm = arms.get(s.id);
        arm.lines.push({ lane: s.lane, points: s.runs.map((r, i) => ({ r, v: s.ns[i], s: s.spread[i] })) });
        if (ch) arm.changes.push(ch);
    }
    $("#series-count").textContent = arms.size + " of " + new Set(d.series.map((s) => s.id)).size + " arms";
    if (!arms.size) { box.append(el("p", "notice", "Nothing matches the filters.")); return; }
    const byGroup = new Map();
    for (const [id, arm] of arms) {
        if (!byGroup.has(arm.group)) byGroup.set(arm.group, []);
        byGroup.get(arm.group).push([id, arm]);
    }
    for (const [group, entries] of [...byGroup.entries()].sort((a, b) => a[0].localeCompare(b[0]))) {
        const section = el("div", "group");
        const h = el("h3");
        h.append(group, el("span", "muted", entries.length + " arms"));
        section.append(h);
        const grid = el("div", "grid grid--series");
        entries.sort((a, b) => a[0].localeCompare(b[0]));
        for (const [id, arm] of entries) {
            arm.lines.sort((a, b) => LANES.indexOf(a.lane) - LANES.indexOf(b.lane));
            const badges = arm.changes.filter((c) => c.kind === "regression" || c.kind === "improvement")
                .map((c) => el("span", "tag tag--" + c.kind, c.lane + " " + fmtPct(c.change)));
            const failedRuns = new Set();
            for (const ln of arm.lines) for (const ri of failed.get(ln.lane + "\t" + arm.file) || []) failedRuns.add(ri);
            const title = id.slice(arm.file.length + 1);
            grid.append(chartCard(title, arm.file, arm.lines, runIdx, {
                label: id, fmt: fmtNs, band: true, zeroBased: true, anchor: anchorId(id), badges, failedRuns,
            }));
        }
        section.append(grid);
        box.append(section);
    }
}

function renderRuns() {
    const table = $("#runs");
    table.replaceChildren();
    const head = el("tr");
    for (const [txt, cls] of [["date", ""], ["commit", ""], ["status", ""], ["lanes", ""], ["build", "num"], ["suite", "num"], ["ok", "num"], ["failed", "num"], ["regressions", "num"], ["improvements", "num"], ["record", ""]]) {
        head.append(el("th", cls, txt));
    }
    table.append(head);
    const runIdx = visibleRunIndices().slice().reverse();
    for (const ri of runIdx) {
        const run = state.data.runs[ri];
        const tr = el("tr");
        tr.append(el("td", null, dateOf(run.started)));
        const c = el("td");
        const a = el("a", null, shortSha(run.sha));
        a.href = commitUrl(run.sha) || "#";
        a.title = run.subject;
        c.append(a, " ", el("span", "muted", run.subject.length > 60 ? run.subject.slice(0, 60) + "..." : run.subject));
        tr.append(c);
        const s = el("td");
        s.append(el("span", "pill pill--" + run.status, run.status.replace("_", " ")));
        tr.append(s);
        const lanes = el("td");
        for (const lane of Object.keys(run.lanes)) {
            const t = el("span", "tag tag--" + lane, lane);
            if (run.lanes[lane] !== "ok") { t.textContent = lane + " off"; t.title = run.lanes[lane]; t.className = "tag"; }
            lanes.append(t, " ");
        }
        tr.append(lanes);
        tr.append(el("td", "num", fmtSec(run.build_seconds)), el("td", "num", fmtSec(run.seconds)));
        tr.append(el("td", "num", String(run.files_ok)), el("td", "num", String(run.failures.length)));
        tr.append(el("td", "num", ri === state.data.latest ? String(run.regressions) : ""), el("td", "num", ri === state.data.latest ? String(run.improvements) : ""));
        const rec = el("td");
        const link = el("a", null, "json");
        link.href = "runs/" + encodeURIComponent(run.id) + ".json";
        rec.append(link);
        tr.append(rec);
        table.append(tr);
    }
}

function renderAll() {
    fillGroups();
    renderNight();
    renderIndex();
    renderSeries();
    renderRuns();
}

wireFilters();
load();

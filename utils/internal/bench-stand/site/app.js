"use strict";

const LANES = ["interp", "jit", "aot"];
const state = { data: null, status: null, runWindow: 90, lanes: new Set(LANES), group: "" };

const $ = (sel) => document.querySelector(sel);
const el = (tag, cls, text) => {
    const e = document.createElement(tag);
    if (cls) e.className = cls;
    if (text !== undefined) e.textContent = text;
    return e;
};
const svgEl = (tag, attrs) => {
    const e = document.createElementNS("http://www.w3.org/2000/svg", tag);
    for (const k in attrs) e.setAttribute(k, attrs[k]);
    return e;
};
function fmtNs(v) {
    if (v >= 1e9) return (v / 1e9).toFixed(2) + " s";
    if (v >= 1e6) return (v / 1e6).toFixed(2) + " ms";
    if (v >= 1e3) return (v / 1e3).toFixed(2) + " us";
    return v.toFixed(v >= 100 ? 0 : v >= 10 ? 1 : 2) + " ns";
}
const fmtRel = (v) => (v >= 10 ? v.toFixed(0) : v.toFixed(2)) + "x";
const fmtSec = (s) => (s >= 3600 ? (s / 3600).toFixed(1) + " h" : s >= 60 ? Math.round(s / 60) + " min" : Math.round(s) + " s");
const shortSha = (sha) => (sha || "").slice(0, 8);
const dateOf = (iso) => (iso || "").slice(0, 10);
const commitUrl = (sha) => (state.data.repo_url && sha ? state.data.repo_url.replace(/\/$/, "") + "/commit/" + sha : null);
const recordUrl = (run) => "runs/" + encodeURIComponent(run.id) + ".json";
const anchorId = (id) => "b-" + id.replace(/[^A-Za-z0-9_-]/g, "_");

async function load() {
    const [dataRes, statusRes] = await Promise.allSettled([
        fetch("data.json", { cache: "no-store" }).then((r) => (r.ok ? r.json() : Promise.reject(new Error("data.json " + r.status)))),
        fetch("status.json", { cache: "no-store" }).then((r) => (r.ok ? r.json() : null)),
    ]);
    state.status = statusRes.status === "fulfilled" ? statusRes.value : null;
    if (dataRes.status === "fulfilled") state.data = dataRes.value;
    else $("main").replaceChildren(el("p", "notice notice--error", "No data.json yet: the stand has not published a report (" + dataRes.reason.message + ")."));
    renderStatus();
    if (state.data) renderAll();
}

function visibleRunIndices() {
    const n = state.data.runs.length;
    const from = state.runWindow > 0 ? Math.max(0, n - state.runWindow) : 0;
    return Array.from({ length: n - from }, (_, i) => from + i);
}

function renderStatus() {
    const box = $("#status");
    box.replaceChildren();
    const st = state.status;
    if (st && st.state === "running") box.append(el("span", "pill pill--running", "running " + (st.run_id || "")));
    if (!state.data || state.data.latest < 0) { if (!st) box.append(el("span", "pill", "no runs")); return; }
    const run = state.data.runs[state.data.latest];
    box.append(el("span", "pill pill--" + run.status, run.status.replace("_", " ")));
    const link = el("a", null, shortSha(run.commit.sha) + " " + run.commit.subject);
    link.href = commitUrl(run.commit.sha) || "#";
    box.append(link, el("span", "muted", dateOf(run.started) + " on " + run.machine.host + ", build " + fmtSec(run.build.seconds) + ", suite " + fmtSec(run.seconds)));
    const ageDays = (Date.now() - Date.parse(run.started)) / 86400000;
    if (ageDays > 2 && !(st && st.state === "running")) box.append(el("span", "pill pill--stale", "last run " + Math.floor(ageDays) + " days ago"));
    $("#generated").textContent = "report generated " + (state.data.generated || "");
}

function wireFilters() {
    document.querySelectorAll(".chip[data-range]").forEach((b) => b.addEventListener("click", () => {
        document.querySelectorAll(".chip[data-range]").forEach((x) => x.classList.remove("is-on"));
        b.classList.add("is-on");
        state.runWindow = Number(b.dataset.range);
        renderAll();
    }));
    document.querySelectorAll("input[data-lane]").forEach((c) => c.addEventListener("change", () => {
        if (c.checked) state.lanes.add(c.dataset.lane); else state.lanes.delete(c.dataset.lane);
        renderAll();
    }));
    $("#group").addEventListener("change", (e) => { state.group = e.target.value; renderSeries(); });
}

function fillGroups() {
    const sel = $("#group");
    const keep = sel.value;
    sel.replaceChildren(el("option", null, "all groups"));
    sel.firstChild.value = "";
    for (const g of state.data.groups) { const o = el("option", null, g); o.value = g; sel.append(o); }
    sel.value = state.data.groups.includes(keep) ? keep : "";
}

function header(title, count, sub) {
    const h = el("h3");
    h.append(title);
    if (count !== undefined) h.append(el("span", "count", String(count)));
    if (sub) h.append(el("span", "muted", sub));
    return h;
}
function list(items, render, emptyText) {
    const ul = el("ul");
    if (!items.length) { if (emptyText) ul.append(el("li", "empty", emptyText)); return ul; }
    for (const it of items) ul.append(render(it));
    return ul;
}

function renderNight() {
    const box = $("#night");
    box.replaceChildren();
    const d = state.data;
    if (d.latest < 0) return;
    const run = d.runs[d.latest];
    const card = el("div", "card");
    const rec = el("a", "msg", "run record");
    rec.href = recordUrl(run);
    if (run.build.status && run.build.status !== "ok") {
        card.append(header("The build failed", undefined, "nothing was measured"));
        const pre = el("pre", "logtail");
        pre.textContent = run.build.log_tail || "the driver recorded no build log";
        card.append(el("div", "msg", "after " + fmtSec(run.build.seconds) + "; last lines of the build log:"), pre, rec);
        box.append(card);
        return;
    }
    card.append(header("Failures", run.failures.length));
    card.append(list(run.failures, (f) => {
        const li = el("li");
        li.append(el("span", "tag tag--failure", f.status.replace("_", " ")), el("span", "tag tag--" + f.lane, f.lane), el("code", null, f.path));
        const m = el("div", "msg");
        m.append(el("code", null, f.message));
        li.append(m);
        return li;
    }, "every file ran"));
    const notes = el("div", "msg");
    for (const lane of Object.keys(run.lanes)) if (run.lanes[lane] !== "ok") notes.append(el("div", null, lane + " lane did not run: " + run.lanes[lane]));
    for (const f of run.skipped) { const line = el("div"); line.append("skipped ", el("code", null, f.path), " (" + f.lane + "): " + f.message); notes.append(line); }
    card.append(notes, rec);

    box.append(card);
}

const W = 420, H = 150, PAD = { l: 44, r: 54, t: 10, b: 22 };

function niceTicks(lo, hi, count) {
    if (!(hi > lo)) hi = lo + 1;
    const step0 = (hi - lo) / Math.max(1, count);
    const mag = Math.pow(10, Math.floor(Math.log10(step0)));
    const step = [1, 2, 2.5, 5, 10].map((m) => m * mag).find((s) => s >= step0) || 10 * mag;
    const out = [];
    for (let v = Math.floor(lo / step) * step; v <= hi + step * 0.5; v += step) out.push(+v.toPrecision(12));
    return out;
}

function drawChart(lines, runIdx, failedRuns, label, fmt) {
    fmt = fmt || fmtNs;
    const svg = svgEl("svg", { viewBox: `0 0 ${W} ${H}`, class: "chart", role: "img", "aria-label": label });
    const n = runIdx.length;
    const xOf = new Map(runIdx.map((ri, i) => [ri, PAD.l + (n === 1 ? (W - PAD.l - PAD.r) / 2 : (i * (W - PAD.l - PAD.r)) / (n - 1))]));
    let lo = Infinity, hi = -Infinity;
    for (const ln of lines) for (const p of ln.points) if (xOf.has(p.r)) { lo = Math.min(lo, p.v); hi = Math.max(hi, p.v * (1 + p.s)); }
    if (!isFinite(lo)) { lo = 0; hi = 1; }
    const pad = (hi - lo) * 0.12 || hi * 0.1 || 1;
    lo = Math.max(0, Math.min(lo, 0) - pad); hi += pad;
    const ticks = niceTicks(lo, hi, 4);
    lo = Math.min(lo, ticks[0]); hi = Math.max(hi, ticks[ticks.length - 1]);
    const yOf = (v) => PAD.t + (H - PAD.t - PAD.b) * (1 - (v - lo) / (hi - lo));
    for (const tv of ticks) {
        if (tv < lo || tv > hi) continue;
        svg.append(svgEl("line", { class: "grid-line", x1: PAD.l, x2: W - PAD.r, y1: yOf(tv), y2: yOf(tv) }));
        const t = svgEl("text", { x: PAD.l - 6, y: yOf(tv) + 3.5, "text-anchor": "end" });
        t.textContent = fmt(tv);
        svg.append(t);
    }
    const runs = state.data.runs;
    const labelEvery = Math.max(1, Math.ceil(n / 5));
    runIdx.forEach((ri, i) => {
        if (i % labelEvery !== 0 && i !== n - 1) return;
        const t = svgEl("text", { x: xOf.get(ri), y: H - 6, "text-anchor": i === n - 1 ? "end" : i === 0 ? "start" : "middle" });
        t.textContent = dateOf(runs[ri].commit.date).slice(5);
        svg.append(t);
    });
    for (const ri of failedRuns) if (xOf.has(ri)) svg.append(svgEl("rect", { class: "fail-mark", x: xOf.get(ri) - 2, y: H - PAD.b - 5, width: 4, height: 5 }));
    for (const ln of lines) {
        const pts = ln.points.filter((p) => xOf.has(p.r));
        if (!pts.length) continue;
        let band = "";
        for (const p of pts) band += (band ? "L" : "M") + xOf.get(p.r).toFixed(1) + "," + yOf(p.v * (1 + p.s)).toFixed(1);
        for (let i = pts.length - 1; i >= 0; i--) band += "L" + xOf.get(pts[i].r).toFixed(1) + "," + yOf(pts[i].v).toFixed(1);
        svg.append(svgEl("path", { class: "band band--" + ln.lane, d: band + "Z" }));
        let d = "", prev = -2;
        for (const p of pts) {
            const at = runIdx.indexOf(p.r);
            d += (at - prev > 1 ? "M" : "L") + xOf.get(p.r).toFixed(1) + "," + yOf(p.v).toFixed(1);
            prev = at;
        }
        svg.append(svgEl("path", { class: "series series--" + ln.lane, d }));
        if (pts.length === 1) svg.append(svgEl("circle", { class: "marker marker--" + ln.lane, cx: xOf.get(pts[0].r), cy: yOf(pts[0].v), r: 4 }));
        const last = pts[pts.length - 1];
        const lbl = svgEl("text", { class: "end-label", x: W - PAD.r + 6, y: yOf(last.v) + 3.5 });
        lbl.textContent = fmt(last.v);
        svg.append(lbl);
    }
    attachHover(svg, lines, runIdx, failedRuns, xOf, yOf);
    return svg;
}

function attachHover(svg, lines, runIdx, failedRuns, xOf, yOf) {
    const runs = state.data.runs;
    const cross = svgEl("line", { class: "crosshair", x1: 0, x2: 0, y1: PAD.t, y2: H - PAD.b, visibility: "hidden" });
    const markers = lines.map((ln) => svgEl("circle", { class: "marker marker--" + ln.lane, r: 4, visibility: "hidden" }));
    const hit = svgEl("rect", { class: "hit", x: PAD.l - 10, y: 0, width: W - PAD.l - PAD.r + 20, height: H });
    svg.append(cross, ...markers, hit);
    const tooltip = $("#tooltip");
    const nearestRun = (evt) => {
        const rect = svg.getBoundingClientRect();
        const px = ((evt.clientX - rect.left) / rect.width) * W;
        let best = -1, bestD = Infinity;
        for (const ri of runIdx) { const dx = Math.abs(xOf.get(ri) - px); if (dx < bestD) { bestD = dx; best = ri; } }
        return best;
    };
    hit.addEventListener("pointermove", (evt) => {
        const best = nearestRun(evt);
        if (best < 0) return;
        const x = xOf.get(best);
        cross.setAttribute("x1", x); cross.setAttribute("x2", x); cross.setAttribute("visibility", "visible");
        const run = runs[best];
        tooltip.replaceChildren(el("div", "tt-head", dateOf(run.commit.date) + " " + shortSha(run.commit.sha) + " " + run.commit.subject));
        lines.forEach((ln, i) => {
            const p = ln.points.find((q) => q.r === best);
            if (!p) { markers[i].setAttribute("visibility", "hidden"); return; }
            markers[i].setAttribute("cx", x); markers[i].setAttribute("cy", yOf(p.v)); markers[i].setAttribute("visibility", "visible");
            const row = el("div", "tt-row");
            row.append(el("span", "tt-key tt-key--" + ln.lane), el("span", "tt-val", fmt(p.v)), el("span", null, ln.lane), el("span", "tt-sub", "spread " + (p.s * 100).toFixed(1) + "%"));
            tooltip.append(row);
        });
        if (failedRuns.has(best)) tooltip.append(el("div", "tt-sub", "this file failed that night"));
        tooltip.hidden = false;
        const tw = tooltip.offsetWidth, th = tooltip.offsetHeight;
        tooltip.style.left = (evt.clientX + 14 + tw > window.innerWidth - 8 ? evt.clientX - tw - 14 : evt.clientX + 14) + "px";
        tooltip.style.top = (evt.clientY + 14 + th > window.innerHeight - 8 ? evt.clientY - th - 14 : evt.clientY + 14) + "px";
    });
    hit.addEventListener("pointerleave", () => {
        cross.setAttribute("visibility", "hidden");
        markers.forEach((m) => m.setAttribute("visibility", "hidden"));
        tooltip.hidden = true;
    });
    hit.addEventListener("click", (evt) => {
        const best = nearestRun(evt);
        const url = best >= 0 ? commitUrl(runs[best].commit.sha) : null;
        if (url) window.open(url, "_blank", "noopener");
    });
}

function chartCard(id, file, lines, runIdx, failedRuns) {
    const card = el("div", "card chart-card");
    card.id = anchorId(id);
    const head = el("div", "chart-card__head");
    const ttl = el("div", "chart-card__title");
    ttl.append(el("span", "file", file + " "), id.slice(file.length + 1));
    head.append(ttl);
    card.append(head, drawChart(lines, runIdx, failedRuns, id));
    if (lines.length > 1) {
        const lg = el("div", "legend");
        for (const ln of lines) lg.append(el("span", "legend--" + ln.lane, ln.lane));
        card.append(lg);
    }
    return card;
}

function aggregateCard(group, entries, runIdx) {
    const lines = [];
    for (const lane of LANES) {
        if (!state.lanes.has(lane)) continue;
        const byRun = new Map();
        for (const [, arm] of entries) {
            const ln = arm.lines.find((l) => l.lane === lane);
            if (!ln || !ln.points.length) continue;
            const base = ln.points[0].v;
            if (!(base > 0)) continue;
            for (const p of ln.points) {
                if (!(p.v > 0)) continue;
                if (!byRun.has(p.r)) byRun.set(p.r, []);
                byRun.get(p.r).push(Math.log(p.v / base));
            }
        }
        const points = [...byRun.entries()].sort((a, b) => a[0] - b[0])
            .map(([r, ls]) => ({ r, v: Math.exp(ls.reduce((a, b) => a + b, 0) / ls.length), s: 0, n: ls.length }));
        if (points.length) lines.push({ lane, points });
    }
    if (!lines.length) return null;
    const card = el("div", "card chart-card chart-card--agg");
    const head = el("div", "chart-card__head");
    const ttl = el("div", "chart-card__title");
    ttl.append(el("span", "file", group + " "), "all arms");
    head.append(ttl);
    card.append(head, drawChart(lines, runIdx, new Set(), group + " aggregate", fmtRel));
    const note = el("div", "muted");
    note.textContent = "geometric mean, each arm against its own first run";
    card.append(note);
    if (lines.length > 1) {
        const lg = el("div", "legend");
        for (const ln of lines) lg.append(el("span", "legend--" + ln.lane, ln.lane));
        card.append(lg);
    }
    return card;
}

function renderSeries() {
    const box = $("#series");
    box.replaceChildren();
    const d = state.data;
    const runIdx = visibleRunIndices();
    const visible = new Set(runIdx);
    const failedBy = new Map();
    d.runs.forEach((run, ri) => {
        for (const f of run.failures) {
            const key = f.lane + "\t" + f.path.replace(/\.das$/, "");
            if (!failedBy.has(key)) failedBy.set(key, new Set());
            failedBy.get(key).add(ri);
        }
    });
    const arms = new Map();
    for (const s of d.series) {
        if (!state.lanes.has(s.lane) || (state.group && s.group !== state.group) || !s.runs.some((r) => visible.has(r))) continue;
        if (!arms.has(s.id)) arms.set(s.id, { group: s.group, file: s.file, lines: [] });
        const arm = arms.get(s.id);
        arm.lines.push({ lane: s.lane, points: s.runs.map((r, i) => ({ r, v: s.ns[i], s: s.spread[i] })) });
    }
    $("#series-count").textContent = arms.size + " of " + new Set(d.series.map((s) => s.id)).size + " arms";
    if (!arms.size) { box.append(el("p", "notice", "Nothing matches the filters.")); return; }
    const byGroup = new Map();
    for (const [id, arm] of arms) { if (!byGroup.has(arm.group)) byGroup.set(arm.group, []); byGroup.get(arm.group).push([id, arm]); }
    for (const [group, entries] of [...byGroup.entries()].sort((a, b) => a[0].localeCompare(b[0]))) {
        const section = el("div", "group");
        const h = el("h3");
        h.append(group, el("span", "muted", entries.length + " arms"));
        const grid = el("div", "grid");
        const agg = aggregateCard(group, entries, runIdx);
        if (agg) grid.append(agg);
        for (const [id, arm] of entries.sort((a, b) => a[0].localeCompare(b[0]))) {
            arm.lines.sort((a, b) => LANES.indexOf(a.lane) - LANES.indexOf(b.lane));
            const failedRuns = new Set();
            for (const ln of arm.lines) for (const ri of failedBy.get(ln.lane + "\t" + arm.file) || []) failedRuns.add(ri);
            grid.append(chartCard(id, arm.file, arm.lines, runIdx, failedRuns));
        }
        section.append(h, grid);
        box.append(section);
    }
}

function renderRuns() {
    const table = $("#runs");
    table.replaceChildren();
    const head = el("tr");
    for (const [txt, cls] of [["date", ""], ["commit", ""], ["status", ""], ["lanes", ""], ["build", "num"], ["suite", "num"], ["ok", "num"], ["failed", "num"], ["record", ""]]) head.append(el("th", cls, txt));
    table.append(head);
    for (const ri of visibleRunIndices().reverse()) {
        const run = state.data.runs[ri];
        const tr = el("tr");
        const c = el("td");
        const a = el("a", null, shortSha(run.commit.sha));
        a.href = commitUrl(run.commit.sha) || "#";
        c.append(a, " ", el("span", "muted", run.commit.subject.length > 60 ? run.commit.subject.slice(0, 60) + "..." : run.commit.subject));
        const s = el("td");
        s.append(el("span", "pill pill--" + run.status, run.status.replace("_", " ")));
        const lanes = el("td");
        for (const lane of Object.keys(run.lanes)) {
            const t = el("span", "tag tag--" + lane, lane);
            if (run.lanes[lane] !== "ok") { t.textContent = lane + " off"; t.title = run.lanes[lane]; t.className = "tag"; }
            lanes.append(t, " ");
        }
        const rec = el("td");
        const link = el("a", null, "json");
        link.href = recordUrl(run);
        rec.append(link);
        tr.append(el("td", null, dateOf(run.started)), c, s, lanes, el("td", "num", fmtSec(run.build.seconds)), el("td", "num", fmtSec(run.seconds)),
            el("td", "num", String(run.files_ok)), el("td", "num", String(run.failures.length)), rec);
        table.append(tr);
    }
}

function renderAll() {
    fillGroups();
    renderNight();
    renderSeries();
    renderRuns();
}

wireFilters();
load();

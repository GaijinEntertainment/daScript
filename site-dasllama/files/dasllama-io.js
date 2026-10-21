/* dasllama.io — all three pages render from the ladder service (/api/*, same origin).
   Nothing here is hand-placed data: every number and row derives from the /api/runs rows,
   every receipt from the verbatim submission document it names, and every command line
   shown is the cmd recorded inside that submission itself. A document loads only for a
   receipt a visitor opens. One script serves home / ladder / sidecars — the page is
   detected by which mount points exist.

   The table/bars builders are a copy of site/files/dasllama.js's, extended with the
   ladder's needs (das-only rows, source column, preselected filters). Extracting a
   shared builder file is a ledgered follow-up, same as the css was. */

(function () {
  'use strict';

  /* ── shared utils ─────────────────────────────────────────────── */
  function fmt(n, d) { return Number(n).toFixed(d == null ? 0 : d); }
  function tps(v) { return v > 0 ? fmt(v, 1) : '-'; }
  function ratio(das, ref) { return (ref > 0 && das > 0) ? das / ref : null; }

  // community rows are third-party text — escape everything that reaches innerHTML.
  // Quotes are escaped too: these values land inside quoted attributes (option value,
  // data-*), where a bare " closes the attribute and injects a new one (onfocus=…).
  function esc(s) {
    return String(s).replace(/&/g, '&amp;').replace(/</g, '&lt;').replace(/>/g, '&gt;')
      .replace(/"/g, '&quot;').replace(/'/g, '&#39;');
  }
  // a community-supplied string is safe as an href only if it is one of OUR api paths;
  // anything else (javascript:, //evil, data:) becomes '#' rather than a live sink
  function safeApiHref(u) {
    return /^\/api\/[A-Za-z0-9_./-]+$/.test(String(u)) ? String(u) : '#';
  }
  function boxLabel(cpu, fallback) {
    return cpu ? String(cpu).replace('64-Core Processor', '').trim() : fallback;
  }

  function api(url) {
    return fetch(url).then(function (r) {
      if (!r.ok) throw new Error('HTTP ' + r.status + ' on ' + url);
      return r.json();
    });
  }

  function unreachable(el) {
    if (el) el.innerHTML = '<div class="dl-empty">The ladder service is unreachable — live data appears here when it is back.</div>';
  }

  function ratioCell(r) {
    // an empty ratio is a das-only row: it MUST read "not raced", never a dash that a sorted
    // table lets sit next to real ratios and imply parity (site-dasllama/REVIEW.md: "an
    // empty ratio cell always reads 'not raced'")
    if (r === null) return '<td class="dl-num dl-dim" title="no llama.cpp reference was raced">not raced</td>';
    var cls = r > 1.005 ? 'dl-win' : (r < 0.995 ? 'dl-loss' : '');
    return '<td class="' + ('dl-num ' + cls).trim() + '">' + fmt(r, 2) + '×</td>';
  }

  /* ── data layer: the flat rows are the board ──
     /api/runs carries the filter columns, the pp512/tg128 cells and the cpu label of every
     run — das and reference alike — so the board paints and pairs from that one response.
     The verbatim submission document at (model_idx, run_idx) is the receipt: fetched once
     per submission, the first time one of its rows opens (the route is cacheable). */
  function loadBoard() {
    return api('/api/runs').then(function (board) { return board.runs || []; });
  }

  var docCache = {};
  function loadDoc(id) {
    var key = Number(id) || 0;   // numeric by construction; coerce so it can never carry a path
    if (!docCache[key]) {
      docCache[key] = api('/api/submission/' + key).catch(function (e) { delete docCache[key]; throw e; });
    }
    return docCache[key];
  }

  /* Pairing is methodology-faithful and within one submission only: a ratio appears
     when the reference flavor named by the lane rode along in the same document —
     never by matching some other upload. Anything unpaired stays a das-only row with
     the ratio columns empty. */
  var LANES = [
    { backend: 'cpu',    das: 'tuned', ref: 'clean-cpu', label: 'cpu' },
    { backend: 'cpu',    das: 'accel', ref: 'stock',     label: 'cpu + accel' },
    { backend: 'metal',  das: 'tuned', ref: 'stock',     label: 'gpu' },
    { backend: 'vulkan', das: 'tuned', ref: 'stock',     label: 'gpu' }
  ];

  // an mlx-lm row (Apple boxes, backend metal) is a row of its own on the LLM ladder, paired
  // with the llama.cpp stock run of its backend like a das gpu row
  function laneOf(fr) {
    if (fr.engine === 'mlx-lm') return { backend: fr.backend, ref: 'stock', label: fr.backend === 'cpu' ? 'cpu' : 'gpu' };
    for (var i = 0; i < LANES.length; i++) {
      if (LANES[i].backend === fr.backend && LANES[i].das === fr.flavor) return LANES[i];
    }
    return null;
  }

  function buildRows(runs) {
    var out = [];
    runs.forEach(function (fr) {
      if ((fr.engine !== 'das' && fr.engine !== 'mlx-lm') || fr.workload) return;   // LLM rows; audio has its own ladders
      var lane = laneOf(fr);
      var ref = null;
      if (lane) {
        runs.forEach(function (r) {
          if (r.submission_id !== fr.submission_id || r.model_idx !== fr.model_idx) return;
          if (r.engine !== 'llama.cpp' || r.box !== fr.box) return;
          if (r.backend !== lane.backend || r.flavor !== lane.ref) return;
          if ((r.workload || '') !== (fr.workload || '')) return;
          if (!ref || String(r.date || '').localeCompare(String(ref.date || '')) > 0) ref = r;
        });
      }
      var laneLabel = lane ? lane.label :
        (fr.backend === 'cpu' ? 'cpu' : (fr.backend === 'metal' || fr.backend === 'vulkan') ? 'gpu' : fr.backend);
      out.push({
        model: String(fr.gguf).replace(/\.gguf$/, ''), arch: fr.model_arch || '', quant: fr.quant || '',
        size: fr.size_bytes || 0, box: fr.box, engine: fr.engine,
        boxName: boxLabel(fr.cpu, fr.box),
        lane: laneLabel, threads: fr.threads || 0,
        source: fr.source || '', verified: !!fr.verified,
        version: riderVersion(runs, fr, ''), date: fr.date || '',
        submissionId: fr.submission_id,
        pp_das: fr.pp512 || 0, pp_ref: ref ? ref.pp512 || 0 : 0,
        tg_das: fr.tg128 || 0, tg_ref: ref ? ref.tg128 || 0 : 0,
        tgb_das: fr.tg128b4 || 0, tgb_ref: ref ? ref.tg128b4 || 0 : 0,
        pp_ratio: ref ? ratio(fr.pp512, ref.pp512) : null,
        tg_ratio: ref ? ratio(fr.tg128, ref.tg128) : null,
        tgb_ratio: ref ? ratio(fr.tg128b4, ref.tg128b4) : null,
        das: fr, ref: ref
      });
    });
    return out;
  }

  /* ── receipts: the full identity of both sides, straight from the document ── */
  function sideLines(tag, r, lines) {
    var hw = r.hardware || {};
    if (r.cmd) lines.push('<b>' + tag + '</b>  $ ' + esc(r.cmd));
    else lines.push('<b>' + tag + '</b>');
    lines.push('        ' + esc([r.engine, r.sha ? '@ ' + r.sha : '', r.version || '', r.date || '',
      r.threads ? r.threads + ' threads' : '',
      r.source && r.source !== 'official' ? r.source : ''].filter(Boolean).join(' · ')));
    if (r.versions) lines.push('        versions: ' + esc(r.versions));
    if (r.tune) lines.push('        tune: ' + esc(r.tune));
    if (r.exec_fmt) lines.push('        executes: ' + esc(r.exec_fmt));
    if (r.env) lines.push('        env: ' + esc(r.env));
    (r.files || []).forEach(function (f) {
      lines.push('        ' + esc(f.role + ': ' + f.name +
        (f.sha256 ? ' sha256:' + f.sha256.slice(0, 12) + '…' : ' (unhashed)') +
        (f.bytes ? ' · ' + f.bytes + ' B' : '')));
    });
    var hwLine = [hw.cpu, hw.model_id, hw.total_cores ? hw.total_cores + ' cores' : '',
      hw.remote_desktop ? (hw.remote_desktop === 'off' ? 'Parsec off' : 'REMOTE DESKTOP: ' + hw.remote_desktop) : '',
      hw.ram_gb ? hw.ram_gb + ' GB' : '', hw.ram_config, hw.gpu, hw.os, hw.power_plan,
      hw.smt ? 'SMT ' + hw.smt : ''].filter(Boolean).join(' · ');
    if (hwLine) lines.push('        ' + esc(hwLine));
    if (r.comment) lines.push('        <span class="dl-receipt-note"><b>note</b>  ' + esc(r.comment) + '</span>');
  }

  // a receipt side's tag: the engine name, padded to the 'reference' column, escaped
  function sideTag(engine) {
    var s = String(engine || '');
    return esc(s.length < 9 ? (s + '         ').slice(0, 9) : s);
  }

  // the BenchRun a flat row points at inside its submission document; the flat row itself
  // when the document does not hold that slot
  function docRun(doc, fr) {
    var model = doc && doc[fr.model_idx];
    var run = model && model.runs && model.runs[fr.run_idx];
    return run || fr;
  }

  function rowReceipt(r) {
    return loadDoc(r.submissionId).then(function (doc) {
      var lines = [];
      sideLines(sideTag(r.engine), docRun(doc, r.das), lines);
      lines.push('');
      if (r.ref) sideLines('reference', docRun(doc, r.ref), lines);
      else lines.push('<b>reference</b>  none rode along — the absolute numbers stand alone, no parity claimed.');
      lines.push('');
      var subId = Number(r.submissionId) || 0;   // numeric by construction; coerce so it can never carry markup
      lines.push('<a href="/api/submission/' + subId + '">verbatim submission #' + subId + ' ↗</a>');
      return lines.join('\n');
    });
  }

  // a receipt renders the first time its row opens; a failed document load says so and the
  // next open retries
  function fillReceipt(box, row, receiptFn) {
    if (!box || box.dataset.loaded) return;
    box.dataset.loaded = '1';
    box.textContent = 'loading the receipt…';
    receiptFn(row).then(function (html) { box.innerHTML = html; }, function () {
      box.dataset.loaded = '';
      box.textContent = 'the submission did not load — open the row again to retry.';
    });
  }

  function measuredLine(rows) {
    var dates = {};
    rows.forEach(function (r) {
      if (r.das.date) dates[r.das.date] = 1;
      if (r.ref && r.ref.date) dates[r.ref.date] = 1;
    });
    var ds = Object.keys(dates).sort();
    if (!ds.length) return '';
    return 'measured ' + (ds[0] === ds[ds.length - 1] ? ds[0] : ds[0] + ' → ' + ds[ds.length - 1]);
  }

  /* ── generic table builder (copy of dasllama.js's makeTable + two extensions:
        filterDefs may carry options(rows) for custom option lists and initial for a
        preselected value — the version filter needs both) ───────── */
  function makeTable(spec) {
    var table = document.querySelector(spec.table);
    var filterBox = document.querySelector(spec.filters);
    var caption = document.querySelector(spec.caption);
    if (!table) return null;

    var sort = { key: spec.sort.key, desc: !!spec.sort.desc };
    var filter = {};
    // a required filter has no "all" option: reset returns it to its initial value
    function resetValue(f) { return f.required ? String(f.initial || '') : ''; }
    spec.filterDefs.forEach(function (f) { filter[f.field] = f.initial || ''; });

    function colOf(key) {
      return spec.cols.filter(function (c) { return c.key === key; })[0] || spec.cols[0];
    }

    function visible() {
      return spec.rows.filter(function (r) {
        return spec.filterDefs.every(function (f) {
          return !filter[f.field] || String(f.get(r)) === filter[f.field];
        });
      });
    }

    function sorted(rows) {
      var col = colOf(sort.key), dir = sort.desc ? -1 : 1;
      return rows.slice().sort(function (a, b) {
        var x = col.get(a), y = col.get(b);
        if (x === null || x === undefined) return 1;
        if (y === null || y === undefined) return -1;
        var c = (typeof x === 'string' || typeof y === 'string')
          ? String(x).localeCompare(String(y)) : (x - y);
        if (c) return c * dir;
        return spec.tiebreak ? spec.tiebreak(a, b) : 0;
      });
    }

    function headHTML() {
      return '<thead><tr>' + spec.cols.map(function (c) {
        var cls = ['js-sort'];
        if (c.num) cls.push('dl-num');
        if (c.grp) cls.push('dl-grp');
        if (c.grpStart) cls.push('dl-grp-start');
        if (c.key === sort.key) { cls.push('is-sorted'); if (sort.desc) cls.push('is-desc'); }
        return '<th class="' + cls.join(' ') + '" data-sort="' + c.key + '">' + esc(c.label) + '</th>';
      }).join('') + '</tr></thead>';
    }

    function render() {
      var rows = sorted(visible());
      var ncol = spec.cols.length;
      var bodyHTML;

      if (!rows.length) {
        bodyHTML = '<tr><td class="dl-empty" colspan="' + ncol + '">' +
          (spec.rows.length ? 'No runs match these filters.' : spec.emptyText || 'Nothing here yet.') + '</td></tr>';
      } else {
        bodyHTML = rows.map(function (r, i) {
          var tds = spec.cols.map(function (c) {
            var out = c.cell(r, i);
            if (out.charAt(0) === '<') return out;
            var cls = [];
            if (c.num) cls.push('dl-num');
            if (c.dim) cls.push('dl-dim');
            if (c.grpStart) cls.push('dl-grp-start');
            if (c.cls) cls.push(c.cls);
            return '<td class="' + cls.join(' ') + '">' + out + '</td>';
          }).join('');
          return '<tr class="dl-row" data-row="' + i + '">' + tds + '</tr>' +
            (spec.receipt ? '<tr hidden data-receipt="' + i + '"><td colspan="' + ncol + '">' +
              '<div class="dl-bench-receipt"></div></td></tr>' : '');
        }).join('');
      }

      table.innerHTML = headHTML() + '<tbody>' + bodyHTML + '</tbody>';

      var body = table.querySelector('tbody');
      body.querySelectorAll('[data-row]').forEach(function (el) {
        el.addEventListener('click', function (ev) {
          if (ev.target.closest('a')) return;
          var n = body.querySelector('[data-receipt="' + el.dataset.row + '"]');
          if (!n) return;
          n.hidden = !n.hidden;
          fillReceipt(n.querySelector('.dl-bench-receipt'), rows[Number(el.dataset.row)], spec.receipt);
        });
      });
      table.querySelectorAll('.js-sort').forEach(function (th) {
        th.addEventListener('click', function () {
          var k = th.dataset.sort;
          if (sort.key === k) sort.desc = !sort.desc;
          else { sort.key = k; sort.desc = typeof colOf(k).get(spec.rows[0]) === 'number'; }
          render();
        });
      });

      if (caption) caption.innerHTML = spec.summary(rows, spec.rows.length);
    }

    function buildFilters() {
      if (!filterBox) return;
      var html = spec.filterDefs.map(function (f) {
        var opts;
        if (f.options) {
          opts = f.options(spec.rows);
        } else {
          var seen = {};
          opts = [];
          spec.rows.forEach(function (r) {
            var v = String(f.get(r));
            if (v && !seen[v]) { seen[v] = 1; opts.push({ v: v, label: f.label ? f.label(r) : v }); }
          });
          opts.sort(function (a, b) { return a.label.localeCompare(b.label); });
        }
        return '<label>' + esc(f.title || f.field) +
          ' <select class="js-filter" data-field="' + f.field + '">' +
          (f.required ? '' : '<option value="">' + esc(f.all) + '</option>') +
          opts.map(function (o) {
            var sel = String(o.v) === String(f.initial || '') ? ' selected' : '';
            return '<option value="' + esc(o.v) + '"' + sel + '>' + esc(o.label) + '</option>';
          }).join('') +
          '</select></label>';
      }).join('') + '<button class="dl-filters__reset js-reset" type="button">reset</button>';
      filterBox.innerHTML = html;

      filterBox.querySelectorAll('.js-filter').forEach(function (sel) {
        sel.addEventListener('change', function () { filter[sel.dataset.field] = sel.value; render(); });
      });
      filterBox.querySelector('.js-reset').addEventListener('click', function () {
        spec.filterDefs.forEach(function (f) { filter[f.field] = resetValue(f); });
        filterBox.querySelectorAll('.js-filter').forEach(function (s) { s.value = filter[s.dataset.field]; });
        sort = { key: spec.sort.key, desc: !!spec.sort.desc };
        render();
      });
    }

    buildFilters();
    render();
    return { render: render, visible: visible };
  }

  /* ── paired bars (copy of dasllama.js's renderPairBars, minus the note tooltips
        the ladder does not carry) ─────────────────────────────────── */
  function renderPairBars(box, items, receiptFn, footNote) {
    if (!items.length) {
      box.innerHTML = '<div class="dl-empty">No paired runs match these filters.</div>';
      return;
    }
    box.innerHTML = items.map(function (it, i) {
      var mx = Math.max(it.das, it.ref);
      var dasW = (it.das / mx) * 100, refW = (it.ref / mx) * 100;
      var rcls = it.ratio > 1.005 ? 'dl-win' : (it.ratio < 0.995 ? 'dl-loss' : '');
      var rec = !!(receiptFn && it.r);
      return '<div class="dl-bar-row' + (rec ? ' js-bar-expand" data-bar="' + i : '') + '">' +
        '<div class="dl-bar-label">' + esc(it.label) + '<span class="dl-dim2">' + esc(it.sub) + '</span></div>' +
        '<div class="dl-bar-pair">' +
        '<div class="dl-bar-line"><div class="dl-bar-das" style="width:' + dasW.toFixed(2) + '%"></div><span class="dl-bar-val dl-bar-val--das">' + tps(it.das) + '</span></div>' +
        '<div class="dl-bar-line"><div class="dl-bar-ref" style="width:' + refW.toFixed(2) + '%"></div><span class="dl-bar-val">' + tps(it.ref) + '</span></div>' +
        '</div>' +
        '<div class="dl-bar-ratio ' + rcls + '">' + fmt(it.ratio, 2) + '×</div>' +
        '</div>' +
        (rec ? '<div class="dl-bar-receipt" hidden data-bar-receipt="' + i + '">' +
          '<div class="dl-bench-receipt"></div></div>' : '');
    }).join('') + (footNote ? '<div class="dl-empty" style="border-top:1px solid var(--rule);margin-top:6px">' + footNote + '</div>' : '');
    box.querySelectorAll('.js-bar-expand').forEach(function (el) {
      el.addEventListener('click', function (ev) {
        if (ev.target.closest('a')) return;
        var n = box.querySelector('[data-bar-receipt="' + el.dataset.bar + '"]');
        if (!n) return;
        n.hidden = !n.hidden;
        fillReceipt(n.querySelector('.dl-bench-receipt'), items[Number(el.dataset.bar)].r, receiptFn);
      });
    });
  }

  /* ── ladder page ──────────────────────────────────────────────── */
  function versionOptions(rows) {
    var seen = {};
    rows.forEach(function (r) { seen[r.version] = true; });
    var vs = Object.keys(seen).map(Number).sort(function (a, b) { return b - a; });
    return vs.map(function (v, i) {
      return { v: v, label: (v === 0 ? 'pre-versioning' : 'v' + v) + (i === 0 && v > 0 ? ' — latest' : '') };
    });
  }
  function latestVersion(rows) {
    var best = 0;
    rows.forEach(function (r) { if (r.version > best) best = r.version; });
    return best;   // 0 when only unversioned history exists -> the filter starts at "all"
  }

  // an official row is its green dot alone (every official run is verified); a community row
  // carries its badge beside the dot the promotion sets
  function sourceCell(r) {
    var official = r.source === 'official';
    return '<td class="dl-grp-start" title="' + esc(r.source + (r.verified ? ' · verified' : ' · unverified')) + '"><span class="dl-src">' +
      '<span class="dl-src__dot ' + (r.verified ? 'dl-src__dot--ok' : 'dl-src__dot--un') + '"></span>' +
      (official ? '' : '<span class="dl-src__badge">' + esc(r.source) + '</span>') +
      '</span></td>';
  }

  function mountLadder(rows) {
    var init = latestVersion(rows);
    var t = makeTable({
      table: '#ladder-table', filters: '#ladder-filters', caption: '#ladder-caption',
      rows: rows, sort: { key: 'date', desc: true },
      emptyText: 'Nothing on the ladder yet — the first accepted submission lands here.',
      tiebreak: function (a, b) { return a.model.localeCompare(b.model) || a.boxName.localeCompare(b.boxName); },
      cols: [
        { key: 'model', label: 'model', get: function (r) { return r.model; },
          cell: function (r) {
            var sub = [r.arch, r.quant].filter(Boolean).join(' · ');
            return esc(r.model) + (sub ? ' <span class="dl-model-sub">' + esc(sub) + '</span>' : '');
          }, cls: 'dl-td-model' },
        { key: 'box', label: 'machine', get: function (r) { return r.boxName; },
          cell: function (r) { return esc(r.boxName); }, cls: 'dl-dim2 dl-td-machine' },
        { key: 'engine', label: 'engine', get: function (r) { return r.engine; },
          cell: function (r) { return esc(r.engine); }, cls: 'dl-dim2' },
        { key: 'lane', label: 'category', get: function (r) { return r.lane; },
          cell: function (r) { return esc(r.lane); }, cls: 'dl-dim2 dl-td-lane' },
        { key: 'threads', label: 'threads', num: true, dim: true,
          get: function (r) { return r.threads; },
          cell: function (r) { return r.threads ? String(r.threads) : '-'; } },
        { key: 'size', label: 'GB', num: true, dim: true, get: function (r) { return r.size; },
          cell: function (r) { return r.size ? fmt(r.size / 1073741824, 1) : '-'; } },
        // the das number heads each metric group under the metric's own name; the reference beside it is 'lcpp'
        { key: 'pp_das', label: 'pp512', num: true, grp: true, grpStart: true,
          get: function (r) { return r.pp_das; }, cell: function (r) { return tps(r.pp_das); } },
        { key: 'pp_ref', label: 'lcpp', num: true, dim: true,
          get: function (r) { return r.pp_ref; }, cell: function (r) { return tps(r.pp_ref); } },
        { key: 'pp_ratio', label: 'ratio', num: true, grp: true,
          get: function (r) { return r.pp_ratio; }, cell: function (r) { return ratioCell(r.pp_ratio); } },
        { key: 'tg_das', label: 'tg128', num: true, grp: true, grpStart: true,
          get: function (r) { return r.tg_das; }, cell: function (r) { return tps(r.tg_das); } },
        { key: 'tg_ref', label: 'lcpp', num: true, dim: true,
          get: function (r) { return r.tg_ref; }, cell: function (r) { return tps(r.tg_ref); } },
        { key: 'tg_ratio', label: 'ratio', num: true, grp: true,
          get: function (r) { return r.tg_ratio; }, cell: function (r) { return ratioCell(r.tg_ratio); } },
        { key: 'tgb_das', label: 'tg128@4', num: true, grp: true, grpStart: true,
          get: function (r) { return r.tgb_das; }, cell: function (r) { return tps(r.tgb_das); } },
        { key: 'tgb_ref', label: 'lcpp', num: true, dim: true,
          get: function (r) { return r.tgb_ref; }, cell: function (r) { return tps(r.tgb_ref); } },
        { key: 'tgb_ratio', label: 'ratio', num: true, grp: true,
          get: function (r) { return r.tgb_ratio; }, cell: function (r) { return ratioCell(r.tgb_ratio); } },
        { key: 'source', label: 'source', grpStart: true,
          get: function (r) { return r.source + (r.verified ? '' : ' unverified'); },
          cell: sourceCell },
        { key: 'date', label: 'date', num: true, dim: true,
          get: function (r) { return r.date; }, cell: function (r) { return esc(r.date); } }
      ],
      filterDefs: [
        { field: 'version', title: 'version', all: 'all versions',
          get: function (r) { return r.version; }, options: versionOptions,
          initial: init > 0 ? String(init) : '' },
        { field: 'model', title: 'model', all: 'all models', get: function (r) { return r.model; } },
        { field: 'box', title: 'machine', all: 'all machines', get: function (r) { return r.box; }, label: function (r) { return r.boxName; } },
        { field: 'engine', title: 'engine', all: 'all engines', get: function (r) { return r.engine; } },
        { field: 'lane', title: 'category', all: 'all categories', get: function (r) { return r.lane; } },
        { field: 'source', title: 'source', all: 'all sources',
          get: function (r) { return r.source; } }
      ],
      receipt: rowReceipt,
      summary: function (shown, total) {
        var solo = shown.filter(function (r) { return r.pp_ratio === null && r.tg_ratio === null; }).length;
        return esc(measuredLine(shown)) +
          ' &nbsp;·&nbsp; showing ' + shown.length + ' of ' + total + ' rows' +
          (solo ? ' (' + solo + ' das-only)' : '') +
          ' &nbsp;·&nbsp; click any row for its receipt' +
          ' &nbsp;·&nbsp; an empty ratio means <b style="color:var(--fg);font-weight:400">not raced</b>, never implied parity';
      }
    });

    // bars view: paired rows only, current filters applied, ranked by the active metric
    var viewSeg = document.getElementById('view'), metricSeg = document.getElementById('metric');
    var barsBox = document.getElementById('bars'), tableWrap = document.getElementById('table-wrap');
    function drawBars() {
      var m = 'pp';
      var mb = metricSeg && metricSeg.querySelector('.is-on');
      if (mb) m = mb.dataset.metric;
      var vis = t ? t.visible() : rows;
      var paired = vis.filter(function (r) { return r[m + '_ratio'] !== null; })
        .sort(function (a, b) { return b[m + '_ratio'] - a[m + '_ratio']; });
      // every visible row NOT plotted for this metric — a row that raced pp512 but not tg128 is
      // unplotted on the tg128 view and must still be counted (was: only both-null rows)
      var solo = vis.length - paired.length;
      renderPairBars(barsBox, paired.map(function (r) {
        return { label: r.model, sub: r.boxName + ' · ' + r.lane + (r.engine === 'das' ? '' : ' · ' + r.engine),
                 das: r[m + '_das'], ref: r[m + '_ref'], ratio: r[m + '_ratio'], r: r };
      }), rowReceipt,
      solo ? 'das-only rows are not plotted — bars are pairs. ' + solo + ' row' + (solo > 1 ? 's' : '') + ' in this filter ' + (solo > 1 ? 'have' : 'has') + ' no reference run; the table shows ' + (solo > 1 ? 'them' : 'it') + '.' : '');
    }
    function setSeg(seg, btn) {
      seg.querySelectorAll('button').forEach(function (b) { b.classList.toggle('is-on', b === btn); });
    }
    if (viewSeg) {
      viewSeg.querySelectorAll('button').forEach(function (b) {
        b.addEventListener('click', function () {
          setSeg(viewSeg, b);
          var bars = b.dataset.view === 'bars';
          barsBox.hidden = !bars;
          tableWrap.hidden = bars;
          if (metricSeg) metricSeg.style.visibility = bars ? 'visible' : 'hidden';
          if (bars) drawBars();
        });
      });
    }
    if (metricSeg) {
      metricSeg.querySelectorAll('button').forEach(function (b) {
        b.addEventListener('click', function () { setSeg(metricSeg, b); drawBars(); });
      });
    }
    var fbox = document.getElementById('ladder-filters');
    if (fbox) fbox.addEventListener('change', function () { if (!barsBox.hidden) drawBars(); });
    if (fbox) {
      var reset = fbox.querySelector('.js-reset');
      if (reset) reset.addEventListener('click', function () { if (!barsBox.hidden) drawBars(); });
    }
  }

  // stats aside: rows · boxes · last measurement, over every section of the ladder
  function fillLadderStats(rows) {
    var aside = document.getElementById('dio-ladder-stats');
    if (!aside) return;
    var boxes = {}, last = '';
    rows.forEach(function (r) {
      boxes[r.box] = true;
      if (r.date > last) last = r.date;
    });
    aside.innerHTML = '<b>' + rows.length + '</b> rows · <b>' + Object.keys(boxes).length + '</b> boxes<br/>' +
      'last measurement <b>' + esc(last || '—') + '</b><br/>' +
      '<a href="https://daslang.io/doc/reference/dasllama_methodology.html" style="color:var(--amber-dim);border-bottom:1px solid var(--rule)">methodology ↗</a>';
  }

  /* ── audio ladders (asr, tts): every engine is a row, ranked within one test ──
     A row is one (run, test) cell from the run's `audio` array: wall ms and the audio
     seconds it covers. The rank metric is × realtime = audio_s / wall s, higher is better.
     ASR clips are fixed files, so a run that did not record a clip's length borrows it from
     any run that did; TTS audio length is the engine's own output, so it is never borrowed
     and a row without one ranks last. */
  var AUDIO = {
    asr: { borrow: true, pooled: false, prefer: ['gb1.wav'], unit: 'clip',
           empty: 'No speech-to-text runs on the ladder yet.' },
    tts: { borrow: false, pooled: true, prefer: ['all'], unit: 'sentence',
           empty: 'No text-to-speech runs on the ladder yet — the first measured run lands here.' },
    'audio-chat': { borrow: true, pooled: false, prefer: ['jfk.wav'], unit: 'clip',
           empty: 'No audio-chat runs on the ladder yet.' }
  };
  var POOLED = 'all';

  // a reference engine carries no dasllama_version: its row takes the das version of the
  // submission it rode along in (same box, same model and workload first), so the version
  // filter keeps the race together
  function riderVersion(runs, fr, workload) {
    if (fr.dasllama_version || fr.engine === 'das') return fr.dasllama_version || 0;
    var best = 0, any = 0;
    runs.forEach(function (r) {
      if (r.submission_id !== fr.submission_id || r.engine !== 'das' || r.box !== fr.box) return;
      var v = r.dasllama_version || 0;
      if (v > any) any = v;
      if (r.workload === workload && r.model_idx === fr.model_idx && v > best) best = v;
    });
    return best || any;
  }

  function audioLane(fr) {
    var hw = fr.backend === 'cpu' ? 'cpu' : 'gpu';
    return fr.flavor && fr.flavor !== 'stock' ? hw + ' · ' + fr.flavor : hw;
  }

  function buildAudioRows(runs, workload) {
    var cfg = AUDIO[workload];
    var dur = {};
    if (cfg.borrow) {
      runs.forEach(function (fr) {
        if (fr.workload !== workload) return;
        (fr.audio || []).forEach(function (c) {
          // the smallest recorded length: a run that rounds (199 for 198.73) never wins
          var a = Number(c.audio_s) || 0;
          if (a > 0 && !(dur[c.test] <= a)) dur[c.test] = a;
        });
      });
    }
    var out = [];
    runs.forEach(function (fr) {
      if (fr.workload !== workload) return;
      var base = {
        model: String(fr.gguf).replace(/\.(gguf|bin)$/, ''), arch: fr.model_arch || '', quant: fr.quant || '',
        box: fr.box, boxName: boxLabel(fr.cpu, fr.box), engine: fr.engine || '', lane: audioLane(fr),
        threads: fr.threads || 0, source: fr.source || '', verified: !!fr.verified,
        version: riderVersion(runs, fr, workload), date: fr.date || '',
        submissionId: fr.submission_id, das: fr
      };
      var poolMs = 0, poolAudio = 0, poolN = 0;
      (fr.audio || []).forEach(function (c) {
        var ms = Number(c.ms) || 0;
        var audio = Number(c.audio_s) || (cfg.borrow ? dur[c.test] || 0 : 0);
        if (audio > 0) { poolMs += ms; poolAudio += audio; poolN++; }
        out.push(Object.assign({ bucket: String(c.test), test: String(c.test), ms: ms, audio: audio,
          n: 1, rtf: (ms > 0 && audio > 0) ? audio / (ms / 1000) : null }, base));
      });
      if (cfg.pooled && poolN > 0) {
        out.push(Object.assign({ bucket: POOLED, test: '', ms: poolMs, audio: poolAudio, n: poolN,
          rtf: poolAudio / (poolMs / 1000) }, base));
      }
    });
    return out;
  }

  function bucketOptions(cfg) {
    return function (rows) {
      var seen = {};
      rows.forEach(function (r) {
        var b = seen[r.bucket] || (seen[r.bucket] = { n: 0, audio: 0 });
        b.n++;
        if (r.n === 1 && r.audio > b.audio) b.audio = r.audio;
      });
      return Object.keys(seen).sort(function (a, b) {
        if (a === POOLED) return -1;
        if (b === POOLED) return 1;
        return (seen[a].audio - seen[b].audio) || a.localeCompare(b);
      }).map(function (k) {
        var s = seen[k];
        var label = k === POOLED ? 'all ' + cfg.unit + 's (Σ)' : k + (cfg.borrow && s.audio ? ' · ' + fmt(s.audio, 0) + ' s' : '');
        return { v: k, label: label + ' · ' + s.n + ' row' + (s.n > 1 ? 's' : '') };
      });
    };
  }

  function defaultBucket(rows, cfg) {
    var count = {};
    rows.forEach(function (r) { count[r.bucket] = (count[r.bucket] || 0) + 1; });
    for (var i = 0; i < cfg.prefer.length; i++) if (count[cfg.prefer[i]]) return cfg.prefer[i];
    var best = '';
    Object.keys(count).forEach(function (k) { if (!best || count[k] > count[best]) best = k; });
    return best;
  }

  function rtfCell(r, top) {
    if (r.rtf === null) return '<td class="dl-num dl-dim" title="the run recorded no audio length">—</td>';
    var w = top > 0 ? Math.max(2, Math.min(100, r.rtf / top * 100)) : 0;
    return '<td class="dl-num dio-rtf"><span class="dio-rtf__bar" style="width:' + w.toFixed(1) + '%"></span>' +
      '<span class="dio-rtf__val">' + fmt(r.rtf, r.rtf >= 100 ? 0 : 1) + '×</span></td>';
  }

  function audioReceipt(workload) {
    return function (r) {
      return loadDoc(r.submissionId).then(function (doc) {
        var lines = [];
        var run = docRun(doc, r.das);
        sideLines(sideTag(r.engine), run, lines);
        var t = r.test && run.tests && run.tests[workload + ':' + r.test];
        if (t && t.text) {
          var text = String(t.text);
          lines.push('        transcript: ' + esc(text.length > 240 ? text.slice(0, 240) + '…' : text));
        }
        if (!r.test) lines.push('        Σ over ' + Number(r.n) + ' ' + AUDIO[workload].unit + 's with a recorded audio length');
        lines.push('');
        var subId = Number(r.submissionId) || 0;
        lines.push('<a href="/api/submission/' + subId + '">verbatim submission #' + subId + ' ↗</a>');
        return lines.join('\n');
      });
    };
  }

  function mountAudioLadder(workload, rows) {
    var cfg = AUDIO[workload];
    var init = latestVersion(rows);
    // a bar's length is relative to the fastest row of its own test, whatever the filters show
    var topOf = {};
    rows.forEach(function (r) { if (r.rtf > (topOf[r.bucket] || 0)) topOf[r.bucket] = r.rtf; });
    makeTable({
      table: '#' + workload + '-table', filters: '#' + workload + '-filters', caption: '#' + workload + '-caption',
      rows: rows, sort: { key: 'rtf', desc: true }, emptyText: cfg.empty,
      tiebreak: function (a, b) { return (a.ms - b.ms) || a.model.localeCompare(b.model); },
      cols: [
        { key: 'rank', label: '#', num: true, dim: true, get: function (r) { return r.rtf; },
          cell: function (r, i) { return String(i + 1); } },
        { key: 'model', label: 'model', get: function (r) { return r.model; },
          cell: function (r) {
            var sub = [r.arch, r.quant].filter(Boolean).join(' · ');
            return esc(r.model) + (sub ? ' <span class="dl-model-sub">' + esc(sub) + '</span>' : '');
          }, cls: 'dl-td-model' },
        { key: 'engine', label: 'engine', get: function (r) { return r.engine; },
          cell: function (r) { return esc(r.engine); } },
        // the rank metric sits beside the identity, so a phone sees it without scrolling the table
        { key: 'rtf', label: '× realtime', num: true, grp: true, grpStart: true, get: function (r) { return r.rtf; },
          cell: function (r) { return rtfCell(r, topOf[r.bucket] || 0); } },
        { key: 'ms', label: 'wall ms', num: true, grp: true, get: function (r) { return r.ms; },
          cell: function (r) { return r.ms > 0 ? fmt(r.ms, r.ms >= 100 ? 0 : 1) : '-'; } },
        { key: 'audio', label: 'audio s', num: true, dim: true, get: function (r) { return r.audio; },
          cell: function (r) { return r.audio > 0 ? fmt(r.audio, 1) : '-'; } },
        { key: 'box', label: 'machine', grpStart: true, get: function (r) { return r.boxName; },
          cell: function (r) { return esc(r.boxName); }, cls: 'dl-dim2 dl-td-machine' },
        { key: 'lane', label: 'backend', get: function (r) { return r.lane; },
          cell: function (r) { return esc(r.lane); }, cls: 'dl-dim2 dl-td-lane' },
        { key: 'threads', label: 'threads', num: true, dim: true, get: function (r) { return r.threads; },
          cell: function (r) { return r.threads ? String(r.threads) : '-'; } },
        { key: 'source', label: 'source', grpStart: true,
          get: function (r) { return r.source + (r.verified ? '' : ' unverified'); }, cell: sourceCell },
        { key: 'date', label: 'date', num: true, dim: true,
          get: function (r) { return r.date; }, cell: function (r) { return esc(r.date); } }
      ],
      filterDefs: [
        { field: 'bucket', title: cfg.unit, required: true, get: function (r) { return r.bucket; },
          options: bucketOptions(cfg), initial: defaultBucket(rows, cfg) },
        { field: 'version', title: 'version', all: 'all versions',
          get: function (r) { return r.version; }, options: versionOptions,
          initial: init > 0 ? String(init) : '' },
        { field: 'model', title: 'model', all: 'all models', get: function (r) { return r.model; } },
        { field: 'box', title: 'machine', all: 'all machines', get: function (r) { return r.box; }, label: function (r) { return r.boxName; } },
        { field: 'engine', title: 'engine', all: 'all engines', get: function (r) { return r.engine; } },
        { field: 'source', title: 'source', all: 'all sources', get: function (r) { return r.source; } }
      ],
      receipt: audioReceipt(workload),
      summary: function (shown) {
        var dates = {};
        shown.forEach(function (r) { if (r.date) dates[r.date] = 1; });
        var ds = Object.keys(dates).sort();
        var when = ds.length ? 'measured ' + (ds[0] === ds[ds.length - 1] ? ds[0] : ds[0] + ' → ' + ds[ds.length - 1]) + ' &nbsp;·&nbsp; ' : '';
        return when + 'showing ' + shown.length + ' row' + (shown.length === 1 ? '' : 's') +
          ' &nbsp;·&nbsp; ranked by × realtime = audio seconds ÷ wall seconds, higher is faster' +
          ' &nbsp;·&nbsp; click any row for its receipt';
      }
    });
  }

  /* ── home page ────────────────────────────────────────────────── */
  function mountHome(rows) {
    function put(id, text) {
      var n = document.getElementById(id);
      if (n) n.textContent = text;
    }
    put('dio-fact-measurements', String(rows.length));

    var barsBox = document.getElementById('dio-teaser-bars');
    if (!barsBox) return;
    var paired = rows.filter(function (r) { return r.engine === 'das' && r.pp_ratio !== null; })
      .sort(function (a, b) { return b.date.localeCompare(a.date); }).slice(0, 3);
    if (!paired.length) {
      // the teaser plots PAIRED rows; say so, and don't contradict the "N measurements" tile
      // above when there are das-only rows but nothing raced against llama.cpp yet
      barsBox.innerHTML = rows.length
        ? '<div class="dl-empty">No paired measurements yet — the ' + rows.length + ' row' + (rows.length > 1 ? 's' : '') + ' on the ladder are das-only. See the full ladder →</div>'
        : '<div class="dl-empty">Nothing on the ladder yet — the first accepted submission lands here.</div>';
      return;
    }
    renderPairBars(barsBox, paired.map(function (r) {
      return { label: r.model, sub: r.boxName + ' · ' + r.lane,
               das: r.pp_das, ref: r.pp_ref, ratio: r.pp_ratio, r: r };
    }), rowReceipt, '');
    put('dio-teaser-scope', 'newest paired measurements · pp512');
  }

  /* ── sidecars page ────────────────────────────────────────────── */
  function scParse(row) {
    var p = String(row.box).split('|');
    return {
      sha: row.sha, box: row.box, verified: !!row.verified,
      noise: row.noise || '', written: row.written || '',
      date: String(row.written || '').slice(0, 10),
      kernels: row.kernel_count || 0, engineSha: row.engine_sha || '',
      version: row.dasllama_version || 0, url: row.url,
      platform: p[0] || '', arch: p[1] || '', modelId: p[2] || '', osBuild: p[3] || '', cpu: p[4] || ''
    };
  }

  function raceMargin(fam, doc) {
    var r = doc.race && doc.race[fam];
    if (!r || !r.rows) return '';
    var win = doc.kernels[fam];
    var wrow = r.rows[win];
    if (!wrow || !(wrow.med_us > 0)) return '';
    var best = null, bestName = '';
    Object.keys(r.rows).forEach(function (k) {
      if (k === win) return;
      var m = r.rows[k].med_us;
      if (m > 0 && (best === null || m < best)) { best = m; bestName = k; }
    });
    if (best === null) return '';
    var pct = (best - wrow.med_us) / wrow.med_us * 100;
    // a gap inside the family's own measured noise floor is noise, not a win margin. A missing
    // floor_pct is UNKNOWN, not zero — don't print a hard ±0.3% margin against an unproven floor.
    if (!(r.floor_pct > 0)) return 'winner ' + bestName + ' (noise floor not recorded)';
    if (Math.abs(pct) <= r.floor_pct) return 'within noise of ' + bestName;
    return (pct >= 0 ? '+' : '') + pct.toFixed(1) + '% over ' + bestName;
  }

  function kernelPanel(doc) {
    var fams = Object.keys(doc.kernels || {}).sort();
    var cells = fams.map(function (fam) {
      var win = doc.kernels[fam];
      if (!win) {
        // only claim "raced" when a race table for this family is actually in the document
        var raced = doc.race && doc.race[fam];
        return '<div class="dio-kernel dio-kernel--fallback"><div class="dio-kernel__fam">' + esc(fam) + '</div>' +
          '<div class="dio-kernel__win"><em>—</em> <span style="color:var(--fg-faint)">nothing applied</span></div>' +
          '<div class="dio-kernel__margin">' + (raced ? 'raced; no variant cleared the gate' : 'no variant applied') + '</div></div>';
      }
      var margin = raceMargin(fam, doc);
      return '<div class="dio-kernel"><div class="dio-kernel__fam">' + esc(fam) + '</div>' +
        '<div class="dio-kernel__win"><em>' + esc(win) + '</em></div>' +
        (margin ? '<div class="dio-kernel__margin">' + esc(margin) + '</div>' : '') + '</div>';
    });
    var scalars = Object.keys(doc.runtime || {}).sort().map(function (k) {
      return '<div><span>' + esc(k) + '</span><b>' + esc(String(doc.runtime[k])) + '</b></div>';
    });
    var prov = doc.provenance || {};
    var provLine = ['engine @ ' + (prov.engine_sha || '?'), prov.written || '',
      prov.noise ? 'noise ' + prov.noise : '',
      prov.noise_floor_cv_pct ? 'floor cv ' + prov.noise_floor_cv_pct + '%' : '',
      prov.noise_probes || ''].filter(Boolean).join(' · ');
    return '<div class="dio-subhead">kernel winners — ' + fams.length + ' families</div>' +
      '<div class="dio-kernels">' + cells.join('') + '</div>' +
      '<div class="dio-subhead" style="margin-top:22px">tuned runtime — ' + scalars.length + ' knobs</div>' +
      '<div class="dio-scalars">' + scalars.join('') + '</div>' +
      '<div class="dio-more">' + esc(provLine) + '</div>';
  }

  function mountSidecars(list) {
    var rows = list.map(scParse);
    var treeBox = document.getElementById('dio-tree');
    var listBox = document.getElementById('dio-sc-list');
    var fEngine = document.getElementById('f-engine');
    var fStatus = document.getElementById('f-status');
    var fSort = document.getElementById('f-sort');
    var reset = document.getElementById('f-reset');
    if (!treeBox || !listBox) return;

    // engine-version options, newest first, latest preselected
    var vs = {};
    rows.forEach(function (r) { vs[r.version] = true; });
    var versions = Object.keys(vs).map(Number).sort(function (a, b) { return b - a; });
    fEngine.innerHTML = versions.map(function (v, i) {
      return '<option value="' + v + '"' + (i === 0 ? ' selected' : '') + '>v' + v + (i === 0 ? ' — latest' : '') + '</option>';
    }).join('') + '<option value="">all versions</option>';

    var state = { group: '', cpu: '' };

    function filtered() {
      var v = fEngine.value, st = fStatus.value;
      var out = rows.filter(function (r) {
        if (v !== '' && String(r.version) !== v) return false;
        if (st === 'verified' && !r.verified) return false;
        if (st === 'unverified' && r.verified) return false;
        return true;
      });
      if (fSort.value === 'verified') {
        out.sort(function (a, b) { return (b.verified - a.verified) || b.written.localeCompare(a.written); });
      } else {
        out.sort(function (a, b) { return b.written.localeCompare(a.written); });
      }
      return out;
    }

    function groupKey(r) { return r.platform + ' · ' + r.arch; }

    function renderTree(pool) {
      var groups = {}, cpus = {};
      pool.forEach(function (r) {
        var g = groupKey(r);
        groups[g] = (groups[g] || 0) + 1;
        if (!state.group || g === state.group) cpus[r.cpu] = (cpus[r.cpu] || 0) + 1;
      });
      var gk = Object.keys(groups).sort();
      if (state.group && !groups[state.group]) state.group = '';
      var html = '<div class="dio-tree__label">platform · arch</div>' +
        '<button class="dio-tree__item' + (!state.group ? ' is-active' : '') + '" data-g="">all <i>' + pool.length + '</i></button>' +
        gk.map(function (g) {
          return '<button class="dio-tree__item' + (state.group === g ? ' is-active' : '') + '" data-g="' + esc(g) + '">' +
            esc(g) + ' <i>' + groups[g] + '</i></button>';
        }).join('');
      var ck = Object.keys(cpus).sort();
      if (state.cpu && !cpus[state.cpu]) state.cpu = '';
      html += '<div class="dio-tree__label" style="margin-top:8px">cpu model</div>' +
        '<button class="dio-tree__item' + (!state.cpu ? ' is-active' : '') + '" data-c="">all</button>' +
        ck.map(function (c) {
          return '<button class="dio-tree__item' + (state.cpu === c ? ' is-active' : '') + '" data-c="' + esc(c) + '">' +
            esc(boxLabel(c, c)) + ' <i>' + cpus[c] + '</i></button>';
        }).join('');
      treeBox.innerHTML = html;
      treeBox.querySelectorAll('[data-g]').forEach(function (b) {
        b.addEventListener('click', function () { state.group = b.dataset.g; state.cpu = ''; render(); });
      });
      treeBox.querySelectorAll('[data-c]').forEach(function (b) {
        b.addEventListener('click', function () { state.cpu = b.dataset.c; render(); });
      });
    }

    function renderList(pool) {
      var show = pool.filter(function (r) {
        if (state.group && groupKey(r) !== state.group) return false;
        if (state.cpu && r.cpu !== state.cpu) return false;
        return true;
      });
      var head = '<div class="dio-sc__head"><span>status</span><span>box</span><span>engine sha</span><span>date</span><span>noise</span><span></span><span></span></div>';
      if (!show.length) {
        listBox.innerHTML = head + '<div class="dl-empty">Nothing here yet — tuning your box takes ~12 min of quiet machine and publishes the sidecar that rides along.</div>';
        return;
      }
      listBox.innerHTML = head + show.map(function (r, i) {
        return '<div class="dio-sc__entry" data-sc="' + i + '">' +
          '<div class="dio-sc__row">' +
          '<span class="dio-status"><span class="dio-dot ' + (r.verified ? 'dio-dot--ok' : 'dio-dot--un') + '"></span>' + (r.verified ? 'verified' : 'unverified') + '</span>' +
          '<span class="dio-sc__box">' + esc(r.box) + '</span>' +
          '<span class="dio-cell" data-l="engine sha">' + esc(r.engineSha || '?') + ' <span style="color:var(--fg-faint)">· v' + Number(r.version || 0) + '</span></span>' +
          '<span class="dio-cell" data-l="date">' + esc(r.date) + '</span>' +
          '<span class="dio-cell" data-l="noise">' + esc(r.noise || '?') + ' <span style="color:var(--fg-faint)">· ' + Number(r.kernels || 0) + ' families</span></span>' +
          '<a class="dio-dl" href="' + safeApiHref(r.url) + '" download>download</a>' +
          '<span class="dio-chev">▾</span>' +
          '</div>' +
          '<div class="dio-panel"><div class="dio-more">loading…</div></div>' +
          '</div>';
      }).join('');
      listBox.querySelectorAll('.dio-sc__entry').forEach(function (entry) {
        var row = entry.querySelector('.dio-sc__row');
        row.addEventListener('click', function (ev) {
          if (ev.target.closest('a')) return;
          entry.classList.toggle('is-open');
          if (!entry.classList.contains('is-open') || entry.dataset.loaded) return;
          entry.dataset.loaded = '1';
          var r = show[Number(entry.dataset.sc)];
          var panel = entry.querySelector('.dio-panel');
          api(safeApiHref(r.url)).then(function (doc) {
            panel.innerHTML = kernelPanel(doc) +
              '<div class="dio-cmd" style="margin-top:14px"><a href="' + safeApiHref(r.url) + '" download style="color:var(--amber)">raw ' + esc(r.sha.slice(0, 12)) + '….tune.json ↓</a></div>';
          }).catch(function () {
            panel.innerHTML = '<div class="dio-more">could not load the sidecar document.</div>';
          });
        });
      });
    }

    function render() {
      var pool = filtered();
      renderTree(pool);
      renderList(pool);

      var aside = document.getElementById('dio-sc-stats');
      if (aside) {
        var groups = {};
        rows.forEach(function (r) { groups[groupKey(r)] = true; });
        aside.innerHTML = '<b>' + rows.length + '</b> sidecars · <b>' + Object.keys(groups).length + '</b> platform·arch<br/>' +
          (versions.length ? 'exchange version <b>v' + versions[0] + '</b><br/>' : '') +
          'a sidecar is valid for one <b>version × box</b>';
      }
    }

    [fEngine, fStatus, fSort].forEach(function (s) {
      if (s) s.addEventListener('change', render);
    });
    if (reset) {
      reset.addEventListener('click', function () {
        fEngine.selectedIndex = 0;
        fStatus.selectedIndex = 0;
        fSort.selectedIndex = 0;
        state.group = '';
        state.cpu = '';
        render();
      });
    }
    render();
  }

  /* ── get-it installer tabs (the daslang.io landing's forge-install block) ───── */
  function wireInstallTabs() {
    var tabs = Array.prototype.slice.call(document.querySelectorAll('.forge-install__tab'));
    var panes = Array.prototype.slice.call(document.querySelectorAll('.forge-install__pane'));
    if (!tabs.length) return;
    tabs.forEach(function (t) {
      t.id = t.id || 'install-tab-' + t.dataset.pane;
      t.setAttribute('aria-controls', 'install-pane-' + t.dataset.pane);
    });
    panes.forEach(function (p) {
      p.id = p.id || 'install-pane-' + p.dataset.pane;
      p.setAttribute('aria-labelledby', 'install-tab-' + p.dataset.pane);
    });
    function select(tab) {
      var key = tab.dataset.pane;
      tabs.forEach(function (t) {
        var on = t === tab;
        t.classList.toggle('is-active', on);
        t.setAttribute('aria-selected', on ? 'true' : 'false');
        t.tabIndex = on ? 0 : -1;
      });
      panes.forEach(function (p) { p.classList.toggle('is-active', p.dataset.pane === key); });
    }
    tabs.forEach(function (tab, i) {
      tab.addEventListener('click', function () { select(tab); });
      // ARIA tabs: arrows move the selection, Home/End jump to the ends
      tab.addEventListener('keydown', function (ev) {
        var step = ev.key === 'ArrowRight' ? 1 : ev.key === 'ArrowLeft' ? -1 : 0;
        var next = null;
        if (step) next = tabs[(i + step + tabs.length) % tabs.length];
        else if (ev.key === 'Home') next = tabs[0];
        else if (ev.key === 'End') next = tabs[tabs.length - 1];
        if (!next) return;
        ev.preventDefault();
        select(next);
        next.focus();
      });
    });
    var initial = tabs.filter(function (t) { return t.classList.contains('is-active'); })[0] || tabs[0];
    select(initial);
  }
  wireInstallTabs();

  /* ── dispatch by page ─────────────────────────────────────────── */
  var newsCount = document.getElementById('dio-feed-count');
  if (newsCount) {
    var n = document.querySelectorAll('#dio-feed-items .dio-item').length;
    newsCount.textContent = n + (n === 1 ? ' entry' : ' entries');
  }

  if (document.getElementById('ladder-table')) {
    loadBoard().then(function (runs) {
      var llm = buildRows(runs);
      mountLadder(llm);
      var all = llm.slice();
      Object.keys(AUDIO).forEach(function (w) {
        if (!document.getElementById(w + '-table')) return;
        var rows = buildAudioRows(runs, w);
        mountAudioLadder(w, rows);
        // the stats count runs, not (run, test) rows
        var seen = {};
        rows.forEach(function (r) {
          if (!seen[r.das.id]) { seen[r.das.id] = 1; all.push(r); }
        });
      });
      fillLadderStats(all);
    }).catch(function () {
      ['table-wrap', 'asr-wrap', 'tts-wrap', 'audio-chat-wrap'].forEach(function (id) { unreachable(document.getElementById(id)); });
      ['ladder-filters', 'asr-filters', 'tts-filters', 'audio-chat-filters'].forEach(function (id) {
        var f = document.getElementById(id);
        if (f) f.hidden = true;
      });
    });
  }

  if (document.getElementById('dio-teaser-bars')) {
    loadBoard()
      .then(function (runs) {
        mountHome(buildRows(runs));
      }).catch(function () {
        unreachable(document.getElementById('dio-teaser-bars'));
      });
  }

  if (document.getElementById('dio-sc-list')) {
    api('/api/sidecars').then(function (res) {
      mountSidecars(res.sidecars || []);
    }).catch(function () {
      unreachable(document.getElementById('dio-sc-list'));
    });
  }
})();

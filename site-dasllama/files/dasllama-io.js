/* dasllama.io — all three pages render from the ladder service (/api/*, same origin).
   Nothing here is hand-placed data: every number, row and receipt derives from the API
   documents, and every command line shown is the cmd recorded inside the submission
   itself. One script serves home / ladder / sidecars — the page is detected by which
   mount points exist.

   The table/bars builders are a copy of site/files/dasllama.js's, extended with the
   ladder's needs (das-only rows, source column, preselected filters). Extracting a
   shared builder file is a ledgered follow-up (plans/dasllama_io_site.md), same as
   the css was. */

(function () {
  'use strict';

  /* ── shared utils ─────────────────────────────────────────────── */
  function fmt(n, d) { return Number(n).toFixed(d == null ? 0 : d); }
  function tps(v) { return v > 0 ? fmt(v, 1) : '-'; }
  function ratio(das, ref) { return (ref > 0 && das > 0) ? das / ref : null; }

  // community rows are third-party text — escape everything that reaches innerHTML
  function esc(s) {
    return String(s).replace(/&/g, '&amp;').replace(/</g, '&lt;').replace(/>/g, '&gt;');
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
    if (r === null) return '<td class="dl-num dl-dim">-</td>';
    var cls = r > 1.005 ? 'dl-win' : (r < 0.995 ? 'dl-loss' : '');
    return '<td class="' + ('dl-num ' + cls).trim() + '">' + fmt(r, 2) + '×</td>';
  }

  /* ── data layer: flat rows joined with their verbatim submissions ──
     /api/runs carries the filter columns; the measurements and receipts live in the
     submission document at (model_idx, run_idx), fetched once per submission (the
     route is cacheable). A row whose document cannot be fetched is dropped rather
     than rendered half-empty. */
  function loadBoard() {
    return api('/api/runs').then(function (board) {
      var runs = board.runs || [];
      var ids = {};
      runs.forEach(function (r) { ids[r.submission_id] = true; });
      var docs = {};
      return Promise.all(Object.keys(ids).map(function (id) {
        return api('/api/submission/' + id)
          .then(function (doc) { docs[id] = doc; })
          .catch(function () { /* dropped rows are counted by the caller */ });
      })).then(function () { return { runs: runs, docs: docs }; });
    });
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

  function tok(r, k) { return (r.tests && r.tests[k]) ? r.tests[k].tok_s : 0; }

  function buildRows(runs, docs) {
    var out = [];
    runs.forEach(function (fr) {
      if (fr.engine !== 'das' || fr.workload) return;   // the ladder is LLM rows; audio is a follow-up
      var doc = docs[fr.submission_id];
      var model = doc && doc[fr.model_idx];
      var das = model && model.runs && model.runs[fr.run_idx];
      if (!das) return;
      var lane = null;
      for (var i = 0; i < LANES.length; i++) {
        if (LANES[i].backend === fr.backend && LANES[i].das === fr.flavor) { lane = LANES[i]; break; }
      }
      var ref = null;
      if (lane) {
        (model.runs || []).forEach(function (r) {
          if (r.engine === 'das' || r.box !== das.box) return;
          if (r.backend !== lane.backend || r.flavor !== lane.ref) return;
          if ((r.workload || '') !== (das.workload || '')) return;
          if (!ref || String(r.date || '').localeCompare(String(ref.date || '')) > 0) ref = r;
        });
      }
      var laneLabel = lane ? lane.label :
        (fr.backend === 'cpu' ? 'cpu' : (fr.backend === 'metal' || fr.backend === 'vulkan') ? 'gpu' : fr.backend);
      out.push({
        model: String(fr.gguf).replace(/\.gguf$/, ''), arch: fr.model_arch || '', quant: fr.quant || '',
        size: fr.size_bytes || 0, box: fr.box,
        boxName: boxLabel(das.hardware && das.hardware.cpu, fr.box),
        lane: laneLabel, threads: fr.threads || 0,
        source: fr.source || '', verified: !!fr.verified,
        version: fr.dasllama_version || 0, date: fr.date || '',
        submissionId: fr.submission_id,
        pp_das: tok(das, 'pp512'), pp_ref: ref ? tok(ref, 'pp512') : 0,
        tg_das: tok(das, 'tg128'), tg_ref: ref ? tok(ref, 'tg128') : 0,
        pp_ratio: ref ? ratio(tok(das, 'pp512'), tok(ref, 'pp512')) : null,
        tg_ratio: ref ? ratio(tok(das, 'tg128'), tok(ref, 'tg128')) : null,
        noted: !!((das && das.comment) || (ref && ref.comment)),
        das: das, ref: ref
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

  function rowReceipt(r) {
    var lines = [];
    sideLines('das      ', r.das, lines);
    lines.push('');
    if (r.ref) sideLines('reference', r.ref, lines);
    else lines.push('<b>reference</b>  none rode along — the absolute numbers stand alone, no parity claimed.');
    lines.push('');
    lines.push('<a href="/api/submission/' + r.submissionId + '">verbatim submission #' + r.submissionId + ' ↗</a>');
    return lines.join('\n');
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
            var out = c.cell(r);
            if (out.charAt(0) === '<') return out;
            var cls = [];
            if (c.num) cls.push('dl-num');
            if (c.dim) cls.push('dl-dim');
            if (c.grpStart) cls.push('dl-grp-start');
            if (c.cls) cls.push(c.cls);
            return '<td class="' + cls.join(' ') + '">' + out + '</td>';
          }).join('');
          var rec = spec.receipt ? spec.receipt(r) : null;
          return '<tr class="dl-row' + (r.noted ? ' dl-row--noted' : '') + '" data-row="' + i + '">' + tds + '</tr>' +
            (rec ? '<tr hidden data-receipt="' + i + '"><td colspan="' + ncol + '">' +
              '<div class="dl-bench-receipt">' + rec + '</div></td></tr>' : '');
        }).join('');
      }

      table.innerHTML = headHTML() + '<tbody>' + bodyHTML + '</tbody>';

      var body = table.querySelector('tbody');
      body.querySelectorAll('[data-row]').forEach(function (el) {
        el.addEventListener('click', function (ev) {
          if (ev.target.closest('a')) return;
          var n = body.querySelector('[data-receipt="' + el.dataset.row + '"]');
          if (n) n.hidden = !n.hidden;
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
          '<option value="">' + esc(f.all) + '</option>' +
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
        spec.filterDefs.forEach(function (f) { filter[f.field] = ''; });
        filterBox.querySelectorAll('.js-filter').forEach(function (s) { s.value = ''; });
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
      var rec = (receiptFn && it.r) ? receiptFn(it.r) : null;
      return '<div class="dl-bar-row' + (rec ? ' js-bar-expand" data-bar="' + i : '') + '">' +
        '<div class="dl-bar-label">' + esc(it.label) + '<span class="dl-dim2">' + esc(it.sub) + '</span></div>' +
        '<div class="dl-bar-pair">' +
        '<div class="dl-bar-line"><div class="dl-bar-das" style="width:' + dasW.toFixed(2) + '%"></div><span class="dl-bar-val dl-bar-val--das">' + tps(it.das) + '</span></div>' +
        '<div class="dl-bar-line"><div class="dl-bar-ref" style="width:' + refW.toFixed(2) + '%"></div><span class="dl-bar-val">' + tps(it.ref) + '</span></div>' +
        '</div>' +
        '<div class="dl-bar-ratio ' + rcls + '">' + fmt(it.ratio, 2) + '×</div>' +
        '</div>' +
        (rec ? '<div class="dl-bar-receipt" hidden data-bar-receipt="' + i + '">' +
          '<div class="dl-bench-receipt">' + rec + '</div></div>' : '');
    }).join('') + (footNote ? '<div class="dl-empty" style="border-top:1px solid var(--rule);margin-top:6px">' + footNote + '</div>' : '');
    box.querySelectorAll('.js-bar-expand').forEach(function (el) {
      el.addEventListener('click', function (ev) {
        if (ev.target.closest('a')) return;
        var n = box.querySelector('[data-bar-receipt="' + el.dataset.bar + '"]');
        if (n) n.hidden = !n.hidden;
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

  function sourceCell(r) {
    return '<td class="dl-grp-start"><span class="dl-src">' +
      '<span class="dl-src__dot ' + (r.verified ? 'dl-src__dot--ok' : 'dl-src__dot--un') + '"></span>' +
      '<span class="dl-src__badge' + (r.source === 'official' ? ' dl-src__badge--official' : '') + '">' + esc(r.source) + '</span>' +
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
          cell: function (r) { return esc(r.boxName); }, cls: 'dl-dim2' },
        { key: 'lane', label: 'category', get: function (r) { return r.lane; },
          cell: function (r) { return esc(r.lane); }, cls: 'dl-dim2' },
        { key: 'threads', label: 'threads', num: true, dim: true,
          get: function (r) { return r.threads; },
          cell: function (r) { return r.threads ? String(r.threads) : '-'; } },
        { key: 'size', label: 'size GB', num: true, dim: true, get: function (r) { return r.size; },
          cell: function (r) { return r.size ? fmt(r.size / 1073741824, 1) : '-'; } },
        { key: 'pp_das', label: 'pp512 das', num: true, grp: true, grpStart: true,
          get: function (r) { return r.pp_das; }, cell: function (r) { return tps(r.pp_das); } },
        { key: 'pp_ref', label: 'lcpp', num: true, dim: true,
          get: function (r) { return r.pp_ref; }, cell: function (r) { return tps(r.pp_ref); } },
        { key: 'pp_ratio', label: 'ratio', num: true, grp: true,
          get: function (r) { return r.pp_ratio; }, cell: function (r) { return ratioCell(r.pp_ratio); } },
        { key: 'tg_das', label: 'tg128 das', num: true, grp: true, grpStart: true,
          get: function (r) { return r.tg_das; }, cell: function (r) { return tps(r.tg_das); } },
        { key: 'tg_ref', label: 'lcpp', num: true, dim: true,
          get: function (r) { return r.tg_ref; }, cell: function (r) { return tps(r.tg_ref); } },
        { key: 'tg_ratio', label: 'ratio', num: true, grp: true,
          get: function (r) { return r.tg_ratio; }, cell: function (r) { return ratioCell(r.tg_ratio); } },
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

    // stats aside: rows · boxes · last submission date
    var aside = document.getElementById('dio-ladder-stats');
    if (aside) {
      var boxes = {}, last = '';
      rows.forEach(function (r) {
        boxes[r.box] = true;
        if (r.date > last) last = r.date;
      });
      aside.innerHTML = '<b>' + rows.length + '</b> rows · <b>' + Object.keys(boxes).length + '</b> boxes<br/>' +
        'last measurement <b>' + esc(last || '—') + '</b><br/>' +
        '<a href="https://daslang.io/doc/reference/dasllama_methodology.html" style="color:var(--amber-dim);border-bottom:1px solid var(--rule)">methodology ↗</a>';
    }

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
      var solo = vis.length - vis.filter(function (r) { return r.pp_ratio !== null || r.tg_ratio !== null; }).length;
      renderPairBars(barsBox, paired.map(function (r) {
        return { label: r.model, sub: r.boxName + ' · ' + r.lane,
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

  /* ── home page ────────────────────────────────────────────────── */
  function mountHome(rows, sidecarCount) {
    function put(id, text) {
      var n = document.getElementById(id);
      if (n) n.textContent = text;
    }
    put('dio-fact-measurements', String(rows.length));
    put('dio-fact-sidecars', String(sidecarCount));

    var barsBox = document.getElementById('dio-teaser-bars');
    if (!barsBox) return;
    var paired = rows.filter(function (r) { return r.pp_ratio !== null; })
      .sort(function (a, b) { return b.date.localeCompare(a.date); }).slice(0, 3);
    if (!paired.length) {
      barsBox.innerHTML = '<div class="dl-empty">Nothing on the ladder yet — the first accepted submission lands here.</div>';
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
    // a gap inside the family's own measured noise floor is noise, not a win margin
    if (Math.abs(pct) <= (r.floor_pct || 0)) return 'within noise of ' + bestName;
    return (pct >= 0 ? '+' : '') + pct.toFixed(1) + '% over ' + bestName;
  }

  function kernelPanel(doc) {
    var fams = Object.keys(doc.kernels || {}).sort();
    var cells = fams.map(function (fam) {
      var win = doc.kernels[fam];
      if (!win) {
        return '<div class="dio-kernel dio-kernel--fallback"><div class="dio-kernel__fam">' + esc(fam) + '</div>' +
          '<div class="dio-kernel__win"><em>—</em> <span style="color:var(--fg-faint)">nothing applied</span></div>' +
          '<div class="dio-kernel__margin">raced; no variant cleared the gate</div></div>';
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
        listBox.innerHTML = head + '<div class="dl-empty">Nothing here yet — tuning your box takes ~20 min of quiet machine and publishes the sidecar that rides along.</div>';
        return;
      }
      listBox.innerHTML = head + show.map(function (r, i) {
        return '<div class="dio-sc__entry" data-sc="' + i + '">' +
          '<div class="dio-sc__row">' +
          '<span class="dio-status"><span class="dio-dot ' + (r.verified ? 'dio-dot--ok' : 'dio-dot--un') + '"></span>' + (r.verified ? 'verified' : 'unverified') + '</span>' +
          '<span class="dio-sc__box">' + esc(r.box) + '</span>' +
          '<span class="dio-cell" data-l="engine sha">' + esc(r.engineSha || '?') + ' <span style="color:var(--fg-faint)">· v' + r.version + '</span></span>' +
          '<span class="dio-cell" data-l="date">' + esc(r.date) + '</span>' +
          '<span class="dio-cell" data-l="noise">' + esc(r.noise || '?') + ' <span style="color:var(--fg-faint)">· ' + r.kernels + ' families</span></span>' +
          '<a class="dio-dl" href="' + esc(r.url) + '" download>download</a>' +
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
          api(r.url).then(function (doc) {
            panel.innerHTML = kernelPanel(doc) +
              '<div class="dio-cmd" style="margin-top:14px"><a href="' + esc(r.url) + '" download style="color:var(--amber)">raw ' + esc(r.sha.slice(0, 12)) + '….tune.json ↓</a></div>';
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

  /* ── dispatch by page ─────────────────────────────────────────── */
  var newsCount = document.getElementById('dio-feed-count');
  if (newsCount) {
    var n = document.querySelectorAll('#dio-feed-items .dio-item').length;
    newsCount.textContent = n + (n === 1 ? ' entry' : ' entries');
  }

  if (document.getElementById('ladder-table')) {
    loadBoard().then(function (b) {
      mountLadder(buildRows(b.runs, b.docs));
    }).catch(function () {
      unreachable(document.getElementById('table-wrap'));
      var f = document.getElementById('ladder-filters');
      if (f) f.hidden = true;
    });
  }

  if (document.getElementById('dio-teaser-bars')) {
    Promise.all([loadBoard(), api('/api/sidecars').catch(function () { return { sidecars: [] }; })])
      .then(function (res) {
        mountHome(buildRows(res[0].runs, res[0].docs), (res[1].sidecars || []).length);
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

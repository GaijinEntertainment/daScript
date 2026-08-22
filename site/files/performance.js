/* performance page — renders straight from the committed records that
   benchmarks/sql/_update_results.das (files/performance_bench.json) and
   examples/benchmarks/sql/_update_results.das (files/performance_engines.json)
   write from the same sweeps that regenerate their results.md tables. Every cell
   is from a record, never hand-placed. */

(function () {
  'use strict';

  // the hero quotes the DISPLAYED boards: the array pair from the container matrix's record,
  // the sqlite pair from the engine board's record — never a cell no table on the page shows
  var HERO_FAMILY = 'groupby_average';
  var HERO_ARRAY_LANE = 'm3f';
  var HERO_SQL_LANE = 'sqlite';
  var HIDDEN_MATRIX_LANE = 'm1';

  function esc(s) {
    return String(s).replace(/&/g, '&amp;').replace(/</g, '&lt;').replace(/>/g, '&gt;');
  }

  function cellStr(v) { return v === null ? '—' : Number(v).toFixed(1); }

  /* one sortable INTERP/JIT matrix bound to a record; ids: the table element,
     the mode segment, and the caption line */
  function makeMatrix(tableId, segId, captionId) {
    var state = { data: null, mode: 'interp', sortKey: 'name', sortDesc: false };

    function sortedFamilies() {
      var fams = state.data.families.slice();
      var k = state.sortKey;
      fams.sort(function (a, b) {
        var r;
        if (k === 'name') {
          r = a.name < b.name ? -1 : (a.name > b.name ? 1 : 0);
        } else {
          // numeric lane column: absent (null) sorts last in either direction
          var av = a[state.mode][k], bv = b[state.mode][k];
          if (av === null && bv === null) return a.name < b.name ? -1 : (a.name > b.name ? 1 : 0);
          if (av === null) return 1;
          if (bv === null) return -1;
          r = av - bv;
        }
        return state.sortDesc ? -r : r;
      });
      return fams;
    }

    function render() {
      var d = state.data;
      var html = '<thead><tr><th data-key="name" class="' +
        (state.sortKey === 'name' ? 'is-sorted' + (state.sortDesc ? ' is-desc' : '') : '') +
        '">benchmark</th>';
      for (var i = 0; i < d.lanes.length; i++) {
        var cls = state.sortKey === i ? 'is-sorted' + (state.sortDesc ? ' is-desc' : '') : '';
        html += '<th data-key="' + i + '" class="' + cls + '">' + esc(d.lanes[i].name) + '</th>';
      }
      html += '</tr></thead><tbody>';

      var fams = sortedFamilies();
      for (var f = 0; f < fams.length; f++) {
        var row = fams[f][state.mode];
        var best = -1, bestV = Infinity, allZero = true;
        for (var mi = 0; mi < row.length; mi++) {
          if (row[mi] === null) continue;
          if (row[mi] !== 0) allZero = false;
          if (row[mi] < bestV) { bestV = row[mi]; best = mi; }
        }
        if (allZero) best = -1;   // all-zero rows (early-exit terminators) have no meaningful winner
        html += '<tr><td class="pf-td-name">' + esc(fams[f].name) + '</td>';
        for (var ci = 0; ci < row.length; ci++) {
          var cellCls = 'pf-num' + (row[ci] === null ? ' pf-dim' : '') + (ci === best ? ' pf-best' : '');
          html += '<td class="' + cellCls + '">' + cellStr(row[ci]) + '</td>';
        }
        html += '</tr>';
      }
      html += '</tbody>';

      var table = document.getElementById(tableId);
      table.innerHTML = html;
      table.querySelector('thead').addEventListener('click', function (e) {
        var th = e.target.closest('th');
        if (!th) return;
        var key = th.dataset.key === 'name' ? 'name' : Number(th.dataset.key);
        if (state.sortKey === key) { state.sortDesc = !state.sortDesc; }
        else { state.sortKey = key; state.sortDesc = false; }
        render();
      });

      document.getElementById(captionId).textContent =
        'ns/op, lower is better · ' + fams.length + ' query families · generated ' + d.generated +
        ' · ' + d.machine + ' · 0.0 = early-exit terminator below timer resolution · ' +
        '— = absent lane';
    }

    return function mount(d) {
      state.data = d;
      var seg = document.getElementById(segId);
      seg.addEventListener('click', function (e) {
        var b = e.target.closest('button');
        if (!b) return;
        state.mode = b.dataset.mode;
        var btns = seg.querySelectorAll('button');
        for (var i = 0; i < btns.length; i++) btns[i].classList.toggle('is-on', btns[i] === b);
        render();
      });
      render();
    };
  }

  function mountHero(d) {
    function heroPair(rec, laneId) {
      var fam = null;
      for (var i = 0; i < rec.families.length; i++) {
        if (rec.families[i].name === HERO_FAMILY) { fam = rec.families[i]; break; }
      }
      if (!fam) return null;
      for (var l = 0; l < rec.lanes.length; l++) {
        if (rec.lanes[l].id === laneId) {
          return fam.jit[l] === null ? null : { jit: fam.jit[l], interp: fam.interp[l] };
        }
      }
      return null;
    }
    function put(id, text) {
      var el = document.getElementById(id);
      if (el) el.textContent = text;
    }
    var arr = heroPair(d.bench, HERO_ARRAY_LANE);
    var sql = heroPair(d.engines, HERO_SQL_LANE);
    if (arr) {
      put('pf-hero-arr', cellStr(arr.jit) + ' ns/op');
      put('pf-hero-arr-i', '· interp ' + cellStr(arr.interp));
    }
    if (sql) {
      put('pf-hero-sql', cellStr(sql.jit) + ' ns/op');
      put('pf-hero-sql-i', '· interp ' + cellStr(sql.interp));
    }
    // the date advances only with the numbers — a fresh date must never label stale cells
    if (arr || sql) put('pf-hero-foot', '  ✓ same chain, engine or container · measured ' + d.bench.generated);
  }

  // the container board tells the das-containers story; the SQLite lane's cells stay in the
  // record (and on the engine board above), the page just does not column them here
  function dropLane(d, laneId) {
    var i = -1;
    for (var l = 0; l < d.lanes.length; l++) {
      if (d.lanes[l].id === laneId) { i = l; break; }
    }
    if (i < 0) return d;
    function without(arr) { return arr.filter(function (_, k) { return k !== i; }); }
    return {
      generated: d.generated, machine: d.machine, unit: d.unit,
      lanes: without(d.lanes),
      families: d.families.map(function (f) {
        return { name: f.name, interp: without(f.interp), jit: without(f.jit) };
      })
    };
  }

  function load(url, mount, captionId) {
    fetch(url)
      .then(function (r) { return r.json(); })
      .then(mount)
      .catch(function () {
        document.getElementById(captionId).textContent = 'benchmark record failed to load';
      });
  }

  var mountMatrix = makeMatrix('pf-table', 'pf-mode', 'pf-caption');
  var mountEngines = makeMatrix('pf-eng-table', 'pf-eng-mode', 'pf-eng-caption');
  var records = {};
  function tryHero() {
    if (records.bench && records.engines) mountHero(records);
  }
  load('files/performance_bench.json', function (d) {
    mountMatrix(dropLane(d, HIDDEN_MATRIX_LANE));
    records.bench = d;
    tryHero();
  }, 'pf-caption');
  load('files/performance_engines.json', function (d) {
    mountEngines(d);
    records.engines = d;
    tryHero();
  }, 'pf-eng-caption');
})();

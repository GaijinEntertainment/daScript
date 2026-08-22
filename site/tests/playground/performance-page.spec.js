// /performance.html — both boards render from their committed records
// (files/performance_bench.json, files/performance_engines.json): row counts,
// cell text, the INTERP/JIT toggle, column sorting with nulls last, and the
// hero pair re-derived from the record (the drift risk: a renamed hero family
// would silently leave the static commit-time numbers on screen).

const { test, expect } = require('./fixtures.js');

async function loadRecord(page, url) {
    return page.evaluate(u => fetch(u).then(r => r.json()), url);
}

// family names are identifiers today, but anchor-matching them as regexes must not
// break (or mismatch) if one ever grows a metacharacter
function nameRe(name) {
    return new RegExp('^' + name.replace(/[.*+?^${}()|[\]\\]/g, '\\$&') + '$');
}

// the container board drops the SQLite (m1) lane — its cells live on the engine board;
// mirror performance.js's dropLane so expectations line up with the rendered columns
function dropM1(rec) {
    const i = rec.lanes.findIndex(l => l.id === 'm1');
    if (i < 0) return rec;
    const without = arr => arr.filter((_, k) => k !== i);
    return {
        ...rec,
        lanes: without(rec.lanes),
        families: rec.families.map(f => ({ name: f.name, interp: without(f.interp), jit: without(f.jit) })),
    };
}

test('matrix renders every family from the record, dashes for null lanes', async ({ page }) => {
    await page.goto('/performance.html');
    const rec = dropM1(await loadRecord(page, '/files/performance_bench.json'));
    const rows = page.locator('#pf-table tbody tr');
    await expect(rows).toHaveCount(rec.families.length);

    // a family with at least one null lane renders an em-dash in that column
    const fi = rec.families.findIndex(f => f.interp.some(v => v === null));
    expect(fi).toBeGreaterThanOrEqual(0);
    const nullLane = rec.families[fi].interp.findIndex(v => v === null);
    const row = rows.filter({ has: page.locator('td.pf-td-name', { hasText: nameRe(rec.families[fi].name) }) });
    await expect(row.locator('td').nth(nullLane + 1)).toHaveText('—');
});

test('the INTERP/JIT toggle swaps a cell to the other mode', async ({ page }) => {
    await page.goto('/performance.html');
    const rec = dropM1(await loadRecord(page, '/files/performance_bench.json'));
    const fam = rec.families.find(f => f.interp[0] !== null && f.jit[0] !== null && f.interp[0] !== f.jit[0]);
    expect(fam).toBeTruthy();
    const row = page.locator('#pf-table tbody tr')
        .filter({ has: page.locator('td.pf-td-name', { hasText: nameRe(fam.name) }) });
    await expect(row.locator('td').nth(1)).toHaveText(fam.interp[0].toFixed(1));
    await page.locator('#pf-mode button[data-mode="jit"]').click();
    await expect(row.locator('td').nth(1)).toHaveText(fam.jit[0].toFixed(1));
});

test('clicking a lane header sorts ascending, again descending, nulls last', async ({ page }) => {
    await page.goto('/performance.html');
    await page.locator('#pf-table tbody tr').first().waitFor();
    const header = page.locator('#pf-table thead th[data-key="0"]');
    await header.click();
    const asc = await page.locator('#pf-table tbody td:nth-child(2)').allTextContents();
    const nums = asc.filter(t => t !== '—').map(Number);
    expect(nums).toEqual([...nums].sort((a, b) => a - b));
    expect(asc.indexOf('—') === -1 || asc.indexOf('—') >= nums.length).toBeTruthy();
    await header.click();
    const desc = await page.locator('#pf-table tbody td:nth-child(2)').allTextContents();
    const dnums = desc.filter(t => t !== '—').map(Number);
    expect(dnums).toEqual([...dnums].sort((a, b) => b - a));
    expect(desc.indexOf('—') === -1 || desc.indexOf('—') >= dnums.length).toBeTruthy();
});

test('the hero quotes the displayed boards, so the static snapshot cannot drift', async ({ page }) => {
    await page.goto('/performance.html');
    // array pair from the container matrix's record, sqlite pair from the engine board's —
    // the hero must never quote a cell no table on the page shows
    const bench = await loadRecord(page, '/files/performance_bench.json');
    const eng = await loadRecord(page, '/files/performance_engines.json');
    const bFam = bench.families.find(f => f.name === 'groupby_average');
    const eFam = eng.families.find(f => f.name === 'groupby_average');
    expect(bFam).toBeTruthy();
    expect(eFam).toBeTruthy();
    const arr = bench.lanes.findIndex(l => l.id === 'm3f');
    const sql = eng.lanes.findIndex(l => l.id === 'sqlite');
    await expect(page.locator('#pf-hero-arr')).toHaveText(bFam.jit[arr].toFixed(1) + ' ns/op');
    await expect(page.locator('#pf-hero-arr-i')).toHaveText('· interp ' + bFam.interp[arr].toFixed(1));
    await expect(page.locator('#pf-hero-sql')).toHaveText(eFam.jit[sql].toFixed(1) + ' ns/op');
    await expect(page.locator('#pf-hero-sql-i')).toHaveText('· interp ' + eFam.interp[sql].toFixed(1));
    await expect(page.locator('#pf-hero-foot')).toContainText('measured ' + bench.generated);
});

test('the engine board renders every family from its own record', async ({ page }) => {
    await page.goto('/performance.html');
    const rec = await loadRecord(page, '/files/performance_engines.json');
    await expect(page.locator('#pf-eng-table tbody tr')).toHaveCount(rec.families.length);
    await expect(page.locator('#pf-eng-table thead th')).toHaveCount(rec.lanes.length + 1);
    await expect(page.locator('#pf-eng-caption')).toContainText(rec.machine);
});

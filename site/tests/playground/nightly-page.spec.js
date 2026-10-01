// /nightly.html embeds the bench stand's viewer from /bench/, a path Caddy serves from the
// stand's output, not from site/. The spec routes /bench/app.js and /bench/style.css to the
// checked-in viewer sources and stubs the data, so it proves the two layouts the one script
// must serve: the site page at /, and the stand's own page at /bench/.

const fs = require('fs');
const path = require('path');
const { test, expect } = require('@playwright/test');

const viewerDir = path.resolve(__dirname, '../../../utils/internal/bench-stand/site');
const SAMPLE = {
    generated: '2026-09-23T00:00:00Z',
    repo_url: 'https://github.com/GaijinEntertainment/daScript',
    latest: 0,
    runs: [{
        id: '20260923T050000Z-deadbeef', started: '2026-09-23T05:00:00Z', status: 'ok', seconds: 51,
        commit: { sha: 'deadbeef00000000', date: '2026-09-23T04:00:00Z', subject: 'a night', author: 'x' },
        build: { status: 'ok', seconds: 120, log_tail: '' }, machine: { host: 'zen4', cores: 16 },
        lanes: { interp: 'ok' }, failures: [], skipped: [], files_ok: 1,
    }],
    groups: ['core/math'],
    series: [{ id: 'core/math/exp#exp_scalar/exp/65536', group: 'core/math', file: 'core/math/exp', lane: 'interp', runs: [0], ns: [2.3], spread: [0.01] }],
};

async function serveViewer(page, data) {
    const requests = [];
    page.on('request', (r) => requests.push(new URL(r.url()).pathname));
    await page.route('**/bench/app.js', (route) => route.fulfill({ path: path.join(viewerDir, 'app.js'), contentType: 'application/javascript' }));
    await page.route('**/bench/style.css', (route) => route.fulfill({ path: path.join(viewerDir, 'style.css'), contentType: 'text/css' }));
    await page.route('**/bench/index.html', (route) => route.fulfill({ path: path.join(viewerDir, 'index.html'), contentType: 'text/html' }));
    await page.route('**/bench/status.json', (route) => route.fulfill({ status: 404, body: '' }));
    await page.route('**/bench/data.json', (route) => data
        ? route.fulfill({ json: data })
        : route.fulfill({ status: 404, body: '' }));
    return requests;
}

test('nightly.html reads the stand data beside the script, not beside the page', async ({ page }) => {
    const errors = [];
    page.on('pageerror', (e) => errors.push(String(e)));
    const requests = await serveViewer(page, null);
    await page.goto('/nightly.html');
    await expect(page.locator('#stand .notice--error')).toContainText('No data.json yet');
    expect(requests).toContain('/bench/data.json');
    expect(requests).toContain('/bench/status.json');
    expect(requests).not.toContain('/data.json');
    expect(errors).toEqual([]);
});

test('nightly.html links every run record into /bench/runs/', async ({ page }) => {
    await serveViewer(page, SAMPLE);
    await page.goto('/nightly.html');
    await expect(page.locator('#stand .chart-card:not(.chart-card--agg)')).toHaveCount(1);
    const links = await page.locator('#stand a[href*="runs/"]').evaluateAll((as) => as.map((a) => new URL(a.href).pathname));
    expect(links.length).toBeGreaterThan(0);
    for (const href of links) expect(href).toMatch(/^\/bench\/runs\/.+\.json$/);
    await expect(page.locator('#status .pill--ok')).toBeVisible();
});

test('the stand page at /bench/ renders from the same script', async ({ page }) => {
    const errors = [];
    page.on('pageerror', (e) => errors.push(String(e)));
    const requests = await serveViewer(page, SAMPLE);
    await page.goto('/bench/index.html');
    await expect(page.locator('#stand .chart-card:not(.chart-card--agg)')).toHaveCount(1);
    expect(requests).toContain('/bench/data.json');
    expect(errors).toEqual([]);
});

test('hovering a chart shows the night and each lane value in the tooltip', async ({ page }) => {
    const errors = [];
    page.on('pageerror', (e) => errors.push(String(e)));
    await serveViewer(page, SAMPLE);
    await page.goto('/nightly.html');
    await page.locator('#stand .chart-card:not(.chart-card--agg) svg .hit').hover();
    await expect(page.locator('#tooltip')).toBeVisible();
    await expect(page.locator('#tooltip .tt-val')).toHaveText('2.30 ns');
    expect(errors).toEqual([]);
});

// 32 nights, the last one (31) under test; every history is 10 unless spelled out. Moved at the
// default 10%: slower small +15%, even +20%, slow 2x (2.2x in its jit lane), gap 2.5x, hslow 3x
// (core/hash), jit 4x (jit lane); faster dip -21%, fast -40%. tiny +7.5% moves only at 5%. Never
// moved: spreadnow, spreadpast
// and scatter (+30% inside a noise of 0.3 / 0.2 / 0.2, one noise source each), flat, young (three
// points), shift (moved against its whole history, flat against its last five), zero (a zero
// baseline). even's four-point baseline has an even-length median; gap's baseline starts on night 0,
// outside a 30-run range. core/hash/test02 failed last night. bulk adds that many slower arms.
const NIGHTS = 32, LAST = NIGHTS - 1;
function movesSample({ bulk = 0 } = {}) {
    const day = (i) => `2026-09-${String(1 + (i % 28)).padStart(2, '0')}`;
    const night = (i) => ({
        id: `n${i}`, started: `${day(i)}T01:00:00Z`, status: i === LAST ? 'bench_failed' : 'ok', seconds: 60,
        commit: { sha: `${String(i).padStart(8, '0')}00000000`, date: `${day(i)}T00:00:00Z`, subject: `night ${i}`, author: 'x' },
        build: { status: 'ok', seconds: 120, log_tail: '' }, machine: { host: 'zen4', cores: 16 }, lanes: { interp: 'ok', jit: 'ok' },
        failures: i === LAST ? [{ path: 'core/hash/test02.das', lane: 'interp', status: 'failed', message: 'benchmark function(s) failed: h' }] : [],
        skipped: [], files_ok: 1,
    });
    const series = (name, last, { history, runs, spread = 0.01, lane = 'interp', file = 'core/math/exp', group = 'core/math' } = {}) => {
        const ns = [...(history || Array(LAST).fill(10)), ...(last === null ? [] : [last])];
        const at = runs || ns.map((_, i) => i);
        return { id: `${file}#${name}/x/1`, group, file, lane, runs: at, ns, spread: Array.isArray(spread) ? spread : ns.map(() => spread) };
    };
    const tail = (values, head = 10) => [...Array(LAST - values.length).fill(head), ...values];
    return {
        generated: '2026-09-28T02:00:00Z', repo_url: 'https://github.com/GaijinEntertainment/daScript', latest: LAST,
        runs: Array.from({ length: NIGHTS }, (_, i) => night(i)), groups: ['bulk', 'core/hash', 'core/math'],
        series: [
            series('small', 11.5),
            series('dip', 7.9),
            series('tiny', 10.75),
            series('fast', 6),
            series('slow', 20),
            series('slow', 22, { lane: 'jit' }),
            series('gap', 25, { history: [10, 10, 10, 10, 10], runs: [0, 1, 2, 3, 4, LAST] }),
            series('even', 13.2, { history: [10, 10, 12, 12], runs: [27, 28, 29, 30, LAST] }),
            series('hslow', 30, { file: 'core/hash/test01', group: 'core/hash' }),
            series('jit', 40, { lane: 'jit' }),
            series('spreadnow', 13, { spread: [...Array(LAST).fill(0.01), 0.3] }),
            series('spreadpast', 13, { spread: [...Array(LAST).fill(0.2), 0.01] }),
            series('scatter', 13, { history: tail([8, 12, 8, 12, 10]) }),
            series('flat', 10.2),
            series('young', 20, { history: [10, 10], runs: [29, 30, LAST] }),
            series('shift', 10, { history: tail([10, 10, 10, 10, 10], 30) }),
            series('zero', 5, { history: Array(LAST).fill(0) }),
            series('h', null, { file: 'core/hash/test02', group: 'core/hash' }),
            ...Array.from({ length: bulk }, (_, k) => series(`b${k}`, 20 + k, { file: 'bulk/b', group: 'bulk' })),
        ],
    };
}
const armCards = (page) => page.locator('#stand .chart-card:not(.chart-card--agg)');
const cardIds = (page) => armCards(page).evaluateAll((cs) => cs.map((c) => c.querySelector('.chart-card__title').lastChild.textContent.split('/')[0]));
const sorted = async (page) => (await cardIds(page)).sort();
const chip = (page, key, value) => page.locator(`[data-filter="${key}"] .chip[data-value="${value}"]`);
const chipOn = (page, key) => page.locator(`[data-filter="${key}"] .chip.is-on`);
const movers = (page, which) => page.locator('#night .movers').nth(which === 'slower' ? 0 : 1);
const pageUrl = (page) => new URL(page.url());

test('the show chips keep the arms that moved past the threshold and the noise, or failed last night', async ({ page }) => {
    await serveViewer(page, movesSample());
    await page.goto('/nightly.html');
    await expect(armCards(page)).toHaveCount(17);
    await chip(page, 'show', 'slower').click();
    expect(await sorted(page)).toEqual(['even', 'gap', 'hslow', 'jit', 'slow', 'small']);
    await chip(page, 'show', 'faster').click();
    expect(await sorted(page)).toEqual(['dip', 'fast']);
    await chip(page, 'show', 'moved').click();
    expect(await sorted(page)).toEqual(['dip', 'even', 'fast', 'gap', 'hslow', 'jit', 'slow', 'small']);
    await chip(page, 'show', 'failing').click();
    expect(await cardIds(page)).toEqual(['h']);
    await chip(page, 'show', 'moved').click();
    await chip(page, 'threshold', '0.25').click();
    expect(await sorted(page)).toEqual(['fast', 'gap', 'hslow', 'jit', 'slow']);
    await chip(page, 'threshold', '0.05').click();
    expect(await sorted(page)).toEqual(['dip', 'even', 'fast', 'gap', 'hslow', 'jit', 'slow', 'small', 'tiny']);
    await expect(page).toHaveURL(/\?show=moved&thr=5$/);
});

test('a moved arm carries its delta and the baseline it was measured against', async ({ page }) => {
    await serveViewer(page, movesSample());
    await page.goto('/nightly.html?thr=5');
    const card = (name) => armCards(page).filter({ has: page.locator(`.chart-card__title:text-matches("^core/[a-z]+/[a-z0-9]+ ${name}/")`) });
    await expect(card('slow').locator('.delta--slower')).toHaveText(['2.0x', '2.2x']);
    await expect(card('fast').locator('.delta--faster')).toHaveText('−40%');
    await expect(card('small').locator('.delta--slower')).toHaveText('+15%');
    await expect(card('tiny').locator('.delta--slower')).toHaveText('+7.5%');
    await expect(card('even').locator('.delta--slower')).toHaveText('+20%');
    await expect(card('slow').locator('svg .baseline')).toHaveCount(2);
    await expect(card('gap').locator('svg .baseline')).toHaveCount(1);
    for (const still of ['spreadnow', 'spreadpast', 'scatter', 'flat', 'young', 'shift', 'zero']) {
        await expect(card(still).locator('.delta')).toHaveCount(0);
        await expect(card(still).locator('svg .baseline')).toHaveCount(0);
    }
    await chip(page, 'range', '30').click();
    await expect(card('gap').locator('.delta--slower')).toHaveText('2.5x');
    await expect(card('gap').locator('svg .baseline')).toHaveCount(0);
});

test('sort by change puts moved arms first, the largest first, and a narrowing filter drops the group aggregate', async ({ page }) => {
    await serveViewer(page, movesSample());
    await page.goto('/nightly.html');
    await expect(page.locator('#stand .chart-card--agg')).toHaveCount(2);
    await chip(page, 'sort', 'change').click();
    await expect(page).toHaveURL(/\?sort=change$/);
    await expect(page.locator('#series .group h3').first()).toContainText('by change');
    expect((await cardIds(page)).slice(0, 8)).toEqual(['jit', 'hslow', 'gap', 'slow', 'fast', 'dip', 'even', 'small']);
    await expect(page.locator('#stand .chart-card--agg')).toHaveCount(0);
    await chip(page, 'sort', 'name').click();
    await chip(page, 'show', 'slower').click();
    await expect(page.locator('#stand .chart-card--agg')).toHaveCount(0);
    await chip(page, 'show', 'all').click();
    await page.locator('#query').fill('exp fla');
    await expect(armCards(page)).toHaveCount(1);
    expect(await cardIds(page)).toEqual(['flat']);
    await expect(page.locator('#stand .chart-card--agg')).toHaveCount(0);
    await expect(page).toHaveURL(/\?q=exp\+fla$/);
});

test('the URL restores every filter and ignores a value no control offers', async ({ page }) => {
    await serveViewer(page, movesSample());
    await page.goto('/nightly.html?q=s&range=30&lanes=interp&group=core%2Fmath&show=bogus&thr=7&sort=zzz');
    await expect(page.locator('#query')).toHaveValue('s');
    await expect(chipOn(page, 'range')).toHaveText('30 runs');
    await expect(page.locator('input[data-lane="interp"]')).toBeChecked();
    await expect(page.locator('input[data-lane="jit"]')).not.toBeChecked();
    await expect(page.locator('#group')).toHaveValue('core/math');
    await expect(chipOn(page, 'show')).toHaveText('all');
    await expect(chipOn(page, 'threshold')).toHaveText('10%');
    await expect(chipOn(page, 'sort')).toHaveText('name');
    expect(await sorted(page)).toEqual(['fast', 'scatter', 'shift', 'slow', 'small', 'spreadnow', 'spreadpast']);
    await expect(page.locator('#runs tr')).toHaveCount(31);
    await page.goto('/nightly.html?group=nope');
    await expect(page.locator('#group')).toHaveValue('');
    await expect(armCards(page)).toHaveCount(17);
});

test('every control writes its value to the URL, and the defaults leave no query string', async ({ page }) => {
    await serveViewer(page, movesSample());
    await page.goto('/nightly.html');
    await expect(movers(page, 'slower').locator('h3 .count')).toHaveText('6');
    await page.locator('#group').selectOption('core/hash');
    expect(pageUrl(page).searchParams.get('group')).toBe('core/hash');
    await expect(movers(page, 'slower').locator('h3 .count')).toHaveText('1');
    await page.locator('#group').selectOption('');
    await page.locator('input[data-lane="jit"]').uncheck();
    expect(pageUrl(page).searchParams.get('lanes')).toBe('interp,aot');
    await expect(movers(page, 'slower').locator('h3 .count')).toHaveText('5');
    await page.locator('input[data-lane="jit"]').check();
    await chip(page, 'range', '30').click();
    expect(pageUrl(page).searchParams.get('range')).toBe('30');
    await expect(page.locator('#runs tr')).toHaveCount(31);
    await chip(page, 'range', '0').click();
    expect(pageUrl(page).searchParams.get('range')).toBe('0');
    await expect(page.locator('#runs tr')).toHaveCount(33);
    await chip(page, 'range', '90').click();
    await chip(page, 'threshold', '0.25').click();
    await expect(movers(page, 'slower').locator('h3 .count')).toHaveText('4');
    await chip(page, 'threshold', '0.1').click();
    expect(page.url()).toMatch(/\/nightly\.html$/);
});

test('the last-night cards list the largest moves of the visible lanes, and a mover reveals its arm', async ({ page }) => {
    await serveViewer(page, movesSample());
    await page.goto('/nightly.html?lanes=interp');
    await expect(movers(page, 'slower').locator('li .arm-link')).toHaveText([
        'core/hash/test01#hslow/x/1', 'core/math/exp#gap/x/1', 'core/math/exp#slow/x/1', 'core/math/exp#even/x/1', 'core/math/exp#small/x/1']);
    await expect(movers(page, 'faster').locator('li .arm-link')).toHaveText(['core/math/exp#fast/x/1', 'core/math/exp#dip/x/1']);
    await page.goto('/nightly.html');
    const slowRow = movers(page, 'slower').locator('li', { hasText: '#slow/' });
    await expect(slowRow).toHaveCount(1);
    await expect(slowRow.locator('.delta')).toHaveText(['2.0x', '2.2x']);
    await expect(slowRow.locator('.tag')).toHaveText(['interp', 'jit']);
    await page.goto('/nightly.html?show=slower&group=core%2Fmath');
    await expect(movers(page, 'slower').locator('h3 .count')).toHaveText('5');
    await movers(page, 'slower').locator('li .arm-link', { hasText: '#slow/' }).click();
    await expect(page.locator('#query')).toHaveValue('core/math/exp#slow/x/1');
    await expect(chipOn(page, 'show')).toHaveText('all');
    await expect(page.locator('#group')).toHaveValue('');
    expect(await cardIds(page)).toEqual(['slow']);
    expect([...pageUrl(page).searchParams.keys()]).toEqual(['q']);
    await expect(movers(page, 'slower').locator('h3 .count')).toHaveText('6');
});

test('a last-night list past eight entries offers the whole set, sorted by change', async ({ page }) => {
    await serveViewer(page, movesSample({ bulk: 9 }));
    await page.goto('/nightly.html');
    await expect(movers(page, 'slower').locator('li')).toHaveCount(8);
    await movers(page, 'slower').locator('button', { hasText: 'show all 15' }).click();
    await expect(chipOn(page, 'show')).toHaveText('slower');
    await expect(chipOn(page, 'sort')).toHaveText('change');
    await expect(page).toHaveURL(/\?show=slower&sort=change$/);
    await expect(armCards(page)).toHaveCount(15);
});

// the pages the e2e lane stages (playground-e2e.yml); the rest carry the same hand-written menu
test('every staged page carrying the performance menu links the nightly page', async ({ page }) => {
    for (const url of ['/', '/performance.html', '/downloads.html', '/examples.html', '/nightly.html', '/playground/']) {
        await page.goto(url);
        await expect(page.locator('.forge-nav__menu a[href$="nightly.html"]')).toHaveCount(1);
    }
});

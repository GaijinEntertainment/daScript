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

// the pages the e2e lane stages (playground-e2e.yml); the rest carry the same hand-written menu
test('every staged page carrying the performance menu links the nightly page', async ({ page }) => {
    for (const url of ['/', '/performance.html', '/downloads.html', '/examples.html', '/nightly.html', '/playground/']) {
        await page.goto(url);
        await expect(page.locator('.forge-nav__menu a[href$="nightly.html"]')).toHaveCount(1);
    }
});

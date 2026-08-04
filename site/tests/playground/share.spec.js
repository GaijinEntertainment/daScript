// ↗ share — the popover mints a stable daslang.io/s/<hash> link from the
// sample service (POST /api/samples), falling back to the legacy compressed
// #z= URL when the service is unreachable (e.g. the static mirror this suite
// serves from). The service is stubbed via Playwright's route() so the tests
// do not depend on a live backend.

const { test, expect } = require('./fixtures.js');

// The "fresh context" test spawns a second browser context. Under high
// parallelism the new-context bootstrap competes with the shared http.server
// and can race the polling pgInit. One retry on flake is plenty.
test.describe.configure({ retries: 1 });

const FAKE_HASH = 'a'.repeat(64);

async function waitTabsReady(page) {
    await page.waitForFunction(() => !!window.pgState, null, { timeout: 10_000 });
    await page.locator('#pg-tabs .pg-tab[data-file="main.das"]').waitFor();
}

async function stubMintOk(page) {
    const bodies = [];
    await page.route('**/api/samples', route => {
        if (route.request().method() !== 'POST') return route.continue();
        bodies.push(route.request().postData());
        return route.fulfill({
            status: 200,
            contentType: 'application/json',
            body: JSON.stringify({
                hash: FAKE_HASH,
                url: 'https://daslang.io/s/' + FAKE_HASH,
                created: true,
            }),
        });
    });
    return bodies;
}

test('share popover shows the minted /s/ link and posts the file bundle', async ({ playground }) => {
    const bodies = await stubMintOk(playground);
    await waitTabsReady(playground);
    await playground.evaluate(() => {
        window.pgSwitchFile('main.das');
        window.code.getDoc().setValue('// MAIN-PAYLOAD\n');
        window.pgAddFile('utils.das');
        window.code.getDoc().setValue('// UTILS-PAYLOAD\n');
    });

    await playground.locator('#share').click();
    const popover = playground.locator('.pg-share');
    await expect(popover).toBeVisible();
    await expect(popover.locator('.pg-share__url')).toHaveValue('https://daslang.io/s/' + FAKE_HASH);
    await expect(popover.locator('.pg-share__meta')).toHaveText(/2 files/);

    // Multi-file states post the same {files, active} bundle the loader reads.
    expect(bodies.length).toBe(1);
    const posted = JSON.parse(bodies[0]);
    expect(Object.keys(posted.files).sort()).toEqual(['main.das', 'utils.das']);
    expect(posted.files['main.das']).toContain('MAIN-PAYLOAD');
    expect(posted.files['utils.das']).toContain('UTILS-PAYLOAD');
});

test('single-file share posts the raw source, not a bundle', async ({ playground }) => {
    const bodies = await stubMintOk(playground);
    await waitTabsReady(playground);
    await playground.evaluate(() => {
        window.pgSwitchFile('main.das');
        window.code.getDoc().setValue('// SINGLE-PAYLOAD\n');
    });

    await playground.locator('#share').click();
    await expect(playground.locator('.pg-share__url')).toHaveValue('https://daslang.io/s/' + FAKE_HASH);

    expect(bodies.length).toBe(1);
    expect(bodies[0]).toContain('SINGLE-PAYLOAD');
    expect(() => {
        const o = JSON.parse(bodies[0]);
        if (o && o.files) throw new Error('bundle-shaped');
    }).not.toThrow('bundle-shaped');
});

test('service unavailable falls back to a legacy #z= URL that round-trips', async ({ playground }) => {
    await playground.route('**/api/samples', route => {
        if (route.request().method() !== 'POST') return route.continue();
        return route.fulfill({ status: 503, body: '' });
    });

    await waitTabsReady(playground);
    await playground.evaluate(() => {
        window.pgSwitchFile('main.das');
        window.code.getDoc().setValue('// MAIN-FALLBACK\n');
        window.pgAddFile('utils.das');
        window.code.getDoc().setValue('// UTILS-FALLBACK\n');
    });

    await playground.locator('#share').click();
    const popover = playground.locator('.pg-share');
    await expect(popover.locator('.pg-share__meta')).toHaveText(/share service unavailable/);
    const url = await popover.locator('.pg-share__url').inputValue();
    expect(url).toContain('#z=');

    // Decoding the hash via the same LZString library must round-trip.
    const decoded = await playground.evaluate((u) => {
        const z = u.split('#z=')[1];
        return JSON.parse(window.LZString.decompressFromEncodedURIComponent(z));
    }, url);
    expect(Object.keys(decoded.files).sort()).toEqual(['main.das', 'utils.das']);
    expect(decoded.files['main.das']).toContain('MAIN-FALLBACK');
    expect(decoded.files['utils.das']).toContain('UTILS-FALLBACK');
});

test('legacy #z= URL restores state in a fresh context', async ({ playground, browser }) => {
    await waitTabsReady(playground);
    await playground.evaluate(() => {
        window.pgSwitchFile('main.das');
        window.code.getDoc().setValue('// MAIN-SHARED\n');
        window.pgAddFile('utils.das');
        window.code.getDoc().setValue('// UTILS-SHARED\n');
    });
    const shareUrl = await playground.evaluate(() => window.pgBuildShareUrl());

    // Open the URL in a brand-new browser context so localStorage is empty
    // and only the hash can repopulate the state.
    const ctx = await browser.newContext();
    const page2 = await ctx.newPage();
    await page2.goto(shareUrl, { waitUntil: 'networkidle' });
    await page2.locator('.CodeMirror').waitFor();
    // pgInit can surface an empty main.das briefly before the hash payload
    // gets applied (both happen via polling). Wait until restoration finishes.
    await page2.waitForFunction(
        () => window.pgState && 'utils.das' in window.pgState.files,
        null,
        { timeout: 30_000 }
    );

    const restored = await page2.evaluate(() => ({
        files: Object.keys(window.pgState.files),
        main: window.pgState.files['main.das'].getValue(),
        utils: window.pgState.files['utils.das']?.getValue(),
    }));
    expect(restored.files.sort()).toEqual(['main.das', 'utils.das']);
    expect(restored.main).toContain('MAIN-SHARED');
    expect(restored.utils).toContain('UTILS-SHARED');
    await ctx.close();
});

test('?s= loader fetches the stored sample into the editor', async ({ playground, browser }) => {
    const ctx = await browser.newContext();
    const page2 = await ctx.newPage();
    await page2.route('**/api/samples/' + FAKE_HASH, route => {
        return route.fulfill({
            status: 200,
            contentType: 'text/plain; charset=utf-8',
            body: '// FROM-THE-STORE\n',
        });
    });
    const base = new URL(playground.url());
    await page2.goto(base.origin + base.pathname + '?s=' + FAKE_HASH, { waitUntil: 'networkidle' });
    await page2.locator('.CodeMirror').waitFor();
    await page2.waitForFunction(
        () => window.pgState && window.pgState.files['main.das'] &&
            window.pgState.files['main.das'].getValue().includes('FROM-THE-STORE'),
        null,
        { timeout: 30_000 }
    );
    await ctx.close();
});

// ↗ share — compressed multi-file state in a URL hash, with optional is.gd
// shortener. is.gd is stubbed via Playwright's route() so the test does not
// depend on the external service.

const { test, expect } = require('./fixtures.js');

// The "fresh context" test spawns a second browser context. Under high
// parallelism the new-context bootstrap competes with the main suite for
// the shared http.server and can race the polling pgInit. One retry on
// flake is plenty.
test.describe.configure({ retries: 1 });

async function waitTabsReady(page) {
    await page.waitForFunction(() => !!window.pgState, null, { timeout: 10_000 });
    await page.locator('#pg-tabs .pg-tab[data-file="main.das"]').waitFor();
}

test('share popover shows a #z= URL with all files', async ({ playground }) => {
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
    const urlInput = popover.locator('.pg-share__url');
    const url = await urlInput.inputValue();
    expect(url).toContain('#z=');
    await expect(popover.locator('.pg-share__meta')).toHaveText(/2 files/);

    // Decoding the hash via the same LZString library must round-trip.
    const decoded = await playground.evaluate((u) => {
        const z = u.split('#z=')[1];
        return JSON.parse(window.LZString.decompressFromEncodedURIComponent(z));
    }, url);
    expect(Object.keys(decoded.files).sort()).toEqual(['main.das', 'utils.das']);
    expect(decoded.files['main.das']).toContain('MAIN-PAYLOAD');
    expect(decoded.files['utils.das']).toContain('UTILS-PAYLOAD');
});

test('shared URL restores state in a fresh context', async ({ playground, browser }) => {
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

test('Shorten button replaces the URL with the is.gd response', async ({ playground }) => {
    // Stub is.gd so the test is self-contained.
    await playground.route('https://is.gd/create.php**', route => {
        return route.fulfill({
            status: 200,
            contentType: 'text/plain',
            body: 'https://is.gd/ABCxyz',
        });
    });

    await waitTabsReady(playground);
    await playground.evaluate(() => window.code.getDoc().setValue('// for shortening\n'));
    await playground.locator('#share').click();
    await playground.locator('.pg-share__shorten').click();

    await expect(playground.locator('.pg-share__url')).toHaveValue('https://is.gd/ABCxyz');
    await expect(playground.locator('.pg-share__shorten')).toHaveText(/shortened/);
});

test('Shorten button surfaces a failure', async ({ playground }) => {
    await playground.route('https://is.gd/create.php**', route => route.fulfill({ status: 502, body: '' }));

    await waitTabsReady(playground);
    await playground.locator('#share').click();
    const longUrl = await playground.locator('.pg-share__url').inputValue();
    await playground.locator('.pg-share__shorten').click();

    await expect(playground.locator('.pg-share__shorten')).toHaveText(/failed/);
    // Long URL untouched so the user still has a working share.
    await expect(playground.locator('.pg-share__url')).toHaveValue(longUrl);
});

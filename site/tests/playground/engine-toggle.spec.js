// The interpreter/wasm radio in the playground toolbar, and the build flow
// behind it (phase 3 of plans/dasweb_backend.md).
//
// The wasm engine compiles the editor's code on the build service, so
// availability is a property of the BROWSER (wasm64 artifacts need memory64)
// and the SERVICE (a live toolchain) — not of the loaded sample, the way the
// retired precompiled-artifact path worked. The service is stubbed via
// route() so these tests do not need a live backend.

const { test, expect } = require('./fixtures.js');

const wasmSel = 'input[name=engine][value=wasm]';
const interpSel = 'input[name=engine][value=interpreter]';
const FAKE_HASH = 'b'.repeat(64);
const ARTIFACT_URL = '/api/build/artifact/abc1234/' + FAKE_HASH + '/sample.wasm';

async function waitDropdownsPopulated(page) {
    await page.waitForFunction(
        () => document.getElementById('examples').options.length > 1,
        null,
        { timeout: 10_000 }
    );
}

// The page asks /api/build/info once at load, so a stub must be installed
// before navigation — hence routing on the context, then a reload.
async function stubBuildInfo(page, enabled) {
    await page.route('**/api/build/info', route => route.fulfill({
        status: 200,
        contentType: 'application/json',
        body: JSON.stringify({ enabled, toolchain: enabled ? 'abc1234' : '' }),
    }));
}

async function stubStore(page) {
    await page.route('**/api/samples', route => {
        if (route.request().method() !== 'POST') return route.continue();
        return route.fulfill({
            status: 200,
            contentType: 'application/json',
            body: JSON.stringify({ hash: FAKE_HASH, url: 'https://daslang.io/s/' + FAKE_HASH, created: true }),
        });
    });
}

// Serve a scripted sequence of status payloads, one per poll.
async function stubBuild(page, states) {
    let i = 0;
    await page.route('**/api/build/request/**', route => route.fulfill({
        status: 200, contentType: 'application/json', body: JSON.stringify(states[0]),
    }));
    await page.route('**/api/build/status/**', route => {
        const s = states[Math.min(++i, states.length - 1)];
        return route.fulfill({ status: 200, contentType: 'application/json', body: JSON.stringify(s) });
    });
}

async function reloadWithStubs(page) {
    await page.reload();
    await page.locator('.CodeMirror').waitFor({ state: 'visible' });
    await waitDropdownsPopulated(page);
}

test('wasm radio stays disabled when the build service is unavailable', async ({ playground }) => {
    await stubBuildInfo(playground, false);
    await reloadWithStubs(playground);

    await expect.poll(() => playground.locator(wasmSel).isDisabled(), { timeout: 5_000 }).toBe(true);
    expect(await playground.locator(interpSel).isChecked()).toBe(true);

    // Availability no longer depends on the sample — switching must not change it.
    await playground.locator('#examples').selectOption({ label: 'SHA-256 (benchmark)' });
    await expect.poll(() => playground.locator(wasmSel).isDisabled(), { timeout: 5_000 }).toBe(true);
});

test('wasm radio becomes selectable when the service can build', async ({ playground }) => {
    await stubBuildInfo(playground, true);
    await reloadWithStubs(playground);

    // Skipped where the browser cannot run wasm64 at all — the engine is
    // genuinely unavailable there and the radio is right to stay off.
    const memory64 = await playground.evaluate(() =>
        WebAssembly.validate(new Uint8Array([0, 0x61, 0x73, 0x6d, 1, 0, 0, 0, 5, 3, 1, 4, 1])));
    test.skip(!memory64, 'browser lacks wasm64/memory64');

    await expect.poll(() => playground.locator(wasmSel).isDisabled(), { timeout: 5_000 }).toBe(false);

    // Enabled for a multi-file sample too: content-addressing removed the
    // one-source-file restriction the old artifact naming imposed.
    await playground.locator('#examples').selectOption({ label: 'Macros (multi-file)' });
    await expect.poll(() => playground.locator(wasmSel).isDisabled(), { timeout: 5_000 }).toBe(false);
});

test('a disabled wasm radio never stays checked', async ({ playground }) => {
    // selectedEngine() reads the checked radio, so a disabled-but-checked wasm
    // option would send Run down a path the page cannot serve.
    await stubBuildInfo(playground, false);
    await reloadWithStubs(playground);
    await playground.evaluate((sel) => {
        const r = document.querySelector(sel);
        r.disabled = false;
        r.checked = true;
        updateEngineAvailability();
    }, wasmSel);

    await expect.poll(() => playground.locator(wasmSel).isChecked(), { timeout: 5_000 }).toBe(false);
    expect(await playground.locator(interpSel).isChecked()).toBe(true);
});

test('a queued build narrates its progress and then runs', async ({ playground }) => {
    await stubBuildInfo(playground, true);
    await stubStore(playground);
    await stubBuild(playground, [
        { state: 'queued', position: 2 },
        { state: 'building' },
        { state: 'done', toolchain: 'abc1234', files: [ARTIFACT_URL] },
    ]);
    // The artifact itself: an empty module is enough — the run path only has
    // to reach instantiation for the progress assertions to mean something.
    await playground.route('**' + ARTIFACT_URL, route => route.fulfill({
        status: 200,
        contentType: 'application/wasm',
        body: Buffer.from([0, 0x61, 0x73, 0x6d, 1, 0, 0, 0]),
    }));
    await reloadWithStubs(playground);

    const memory64 = await playground.evaluate(() =>
        WebAssembly.validate(new Uint8Array([0, 0x61, 0x73, 0x6d, 1, 0, 0, 0, 5, 3, 1, 4, 1])));
    test.skip(!memory64, 'browser lacks wasm64/memory64');

    await playground.locator(wasmSel).check();
    await playground.locator('#run').click();

    const output = playground.locator('#output');
    await expect(output).toContainText('queued for build (position 2)', { timeout: 15_000 });
    await expect(output).toContainText('building', { timeout: 15_000 });
    // Run is re-enabled once the build resolves — a stuck button would strand
    // the page after any build.
    await expect.poll(() => playground.locator('#run').isDisabled(), { timeout: 15_000 }).toBe(false);
});

test('a page-kind build runs as an embedded page frame', async ({ playground }) => {
    // Graphics/audio samples build as standalone html+js+wasm pages (kind:
    // 'page'); the playground embeds the html artifact in an iframe instead of
    // instantiating a bare module. Served absolute from the run origin in
    // production — here same-origin, which the embed path treats identically.
    const PAGE_URL = '/api/build/artifact/abc1234/' + FAKE_HASH + '/sample.html';
    await stubBuildInfo(playground, true);
    await stubStore(playground);
    await stubBuild(playground, [
        { state: 'queued', position: 1 },
        {
            state: 'done', kind: 'page', toolchain: 'abc1234',
            files: [PAGE_URL,
                '/api/build/artifact/abc1234/' + FAKE_HASH + '/sample.js',
                '/api/build/artifact/abc1234/' + FAKE_HASH + '/sample.wasm'],
        },
    ]);
    await playground.route('**' + PAGE_URL, route => route.fulfill({
        status: 200,
        contentType: 'text/html',
        body: '<html><body>page artifact stands</body></html>',
    }));
    await reloadWithStubs(playground);

    const memory64 = await playground.evaluate(() =>
        WebAssembly.validate(new Uint8Array([0, 0x61, 0x73, 0x6d, 1, 0, 0, 0, 5, 3, 1, 4, 1])));
    test.skip(!memory64, 'browser lacks wasm64/memory64');

    await playground.locator(wasmSel).check();
    await playground.locator('#run').click();

    const frame = playground.locator('iframe.pg-page-frame');
    await expect(frame).toHaveCount(1, { timeout: 15_000 });
    expect(await frame.getAttribute('src')).toContain('sample.html');
    // No sandbox attribute BY DESIGN: it would opaque the origin and break
    // emscripten's same-origin worker spawning — the separate run origin is
    // the isolation boundary.
    expect(await frame.getAttribute('sandbox')).toBeNull();
    expect(await frame.getAttribute('allow')).toContain('cross-origin-isolated');

    // Switching samples clears the page frame with the canvas.
    await playground.locator('#examples').selectOption({ label: 'SHA-256 (benchmark)' });
    await expect(playground.locator('iframe.pg-page-frame')).toHaveCount(0, { timeout: 5_000 });
});

test('a failed build shows the compiler error', async ({ playground }) => {
    await stubBuildInfo(playground, true);
    await stubStore(playground);
    await stubBuild(playground, [
        { state: 'queued', position: 1 },
        { state: 'failed', error: 'error[30101]: this is the user’s own mistake' },
    ]);
    await reloadWithStubs(playground);

    const memory64 = await playground.evaluate(() =>
        WebAssembly.validate(new Uint8Array([0, 0x61, 0x73, 0x6d, 1, 0, 0, 0, 5, 3, 1, 4, 1])));
    test.skip(!memory64, 'browser lacks wasm64/memory64');

    await playground.locator(wasmSel).check();
    await playground.locator('#run').click();

    await expect(playground.locator('#output')).toContainText('error[30101]', { timeout: 15_000 });
});

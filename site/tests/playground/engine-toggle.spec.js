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

// Artifacts link with -sALLOW_MEMORY_GROWTH, and growing a wasm memory DETACHES
// its previous ArrayBuffer. The wasi shim must therefore read the buffer live
// from the Memory object on every call — a snapshot taken at instantiation dies
// on the first fd_write after a grow ("Cannot perform DataView constructor on a
// detached ArrayBuffer"), which is how the f2s benchmark failed the day growth
// was enabled. This module prints, grows, prints again.
function growingModuleBytes() {
    const leb = (n) => { const out = []; do { let b = n & 0x7f; n >>>= 7; if (n) b |= 0x80; out.push(b); } while (n); return out; };
    const str = (s) => [s.length, ...[...s].map(c => c.charCodeAt(0))];
    const sec = (id, bytes) => [id, ...leb(bytes.length), ...bytes];
    const vec = (items) => [items.length, ...items.flat()];

    const type = sec(1, vec([
        [0x60, 4, 0x7f, 0x7f, 0x7f, 0x7f, 1, 0x7f],   // (i32 i32 i32 i32) -> i32 : fd_write
        [0x60, 0, 0],                                  // () -> ()               : _start
    ]));
    const imp = sec(2, vec([
        [...str('wasi_snapshot_preview1'), ...str('fd_write'), 0x00, 0],
    ]));
    const func = sec(3, vec([[1]]));
    const mem = sec(5, vec([[0x00, 1]]));              // min 1 page, NO max -> growable
    const exp = sec(7, vec([
        [...str('memory'), 0x02, 0],
        [...str('_start'), 0x00, 1],                   // func 0 is the import
    ]));
    // iovec1 @16 -> "one\n" (0,4); iovec2 @24 -> "two\n" (4,4); nwritten @32
    const wr = (iov) => [0x41, 1, 0x41, iov, 0x41, 1, 0x41, 32, 0x10, 0, 0x1a]; // fd_write(1,iov,1,32); drop
    const body = [0, ...wr(16), 0x41, 1, 0x40, 0x00, 0x1a, ...wr(24), 0x0b];    // print; grow(1); print
    const code = sec(10, vec([[...leb(body.length), ...body]]));
    const dataBytes = [
        ...'one\ntwo\n'.split('').map(c => c.charCodeAt(0)), 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 4, 0, 0, 0,                        // iovec1: ptr 0, len 4
        4, 0, 0, 0, 4, 0, 0, 0,                        // iovec2: ptr 4, len 4
    ];
    const data = sec(11, vec([[0, 0x41, 0, 0x0b, ...leb(dataBytes.length), ...dataBytes]]));
    return Buffer.from([0, 0x61, 0x73, 0x6d, 1, 0, 0, 0, ...type, ...imp, ...func, ...mem, ...exp, ...code, ...data]);
}

test('module-rail stdout survives a wasm memory grow', async ({ playground }) => {
    await stubBuildInfo(playground, true);
    await stubStore(playground);
    await stubBuild(playground, [{ state: 'done', toolchain: 'abc1234', files: [ARTIFACT_URL] }]);
    await playground.route('**' + ARTIFACT_URL, route => route.fulfill({
        status: 200,
        contentType: 'application/wasm',
        body: growingModuleBytes(),
    }));
    await reloadWithStubs(playground);

    const memory64 = await playground.evaluate(() =>
        WebAssembly.validate(new Uint8Array([0, 0x61, 0x73, 0x6d, 1, 0, 0, 0, 5, 3, 1, 4, 1])));
    test.skip(!memory64, 'browser lacks wasm64/memory64');

    await playground.locator(wasmSel).check();
    await playground.locator('#run').click();

    const output = playground.locator('#output');
    await expect(output).toContainText('one', { timeout: 15_000 });
    // "two" is written AFTER memory.grow — a shim holding the pre-grow buffer
    // throws here instead of printing.
    await expect(output).toContainText('two', { timeout: 15_000 });
    await expect(output).not.toContainText('wasm error');
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

    // Sized like the interpreter's run frame — off the COLUMN, not the host.
    // The host sizes to its CONTENT, so a percentage width is circular and
    // collapses the frame to the CSS default iframe width (300px + border).
    // It still renders, just far too small to use, which is why "a frame
    // exists" is not enough to assert. Measured 302px against a 906px column
    // on production before this was fixed — and note the collapsed width is a
    // constant, so a loose fraction-of-column bound passes at a small viewport
    // and misses it entirely.
    const box = await frame.boundingBox();
    const width = await playground.evaluate(() => {
        const host = document.querySelector('.pg-run-host');
        const column = host && host.parentElement ? host.parentElement.clientWidth : 0;
        // what PlaygroundRunner.fitElement should have produced, same rule
        const capped = Math.round(Math.round(window.innerHeight * 0.8) / (3 / 4));
        return { column, expected: Math.min(column, capped) };
    });
    expect(width.column).toBeGreaterThan(320);      // else the bound below proves nothing
    expect(box.width).toBeGreaterThan(width.expected * 0.95);
    // 4:3 landscape, matching the run frame's starting aspect.
    expect(box.height / box.width).toBeGreaterThan(0.6);
    expect(box.height / box.width).toBeLessThan(0.9);

    // The page reports its real drawing buffer the way run-frame.html does, and
    // the frame reshapes to it — that report is what lets the canvas fill
    // without letterbox bars, so a silent no-op here would look like the old
    // "renders in the middle" bug for any program that is not 4:3.
    await playground.evaluate(() => {
        const f = document.querySelector('iframe.pg-page-frame');
        window.dispatchEvent(new MessageEvent('message', {
            source: f.contentWindow,
            origin: new URL(f.src, location.href).origin,
            data: { type: 'canvas-visible', width: 800, height: 200 },   // 4:1, unmistakable
        }));
    });
    await expect.poll(async () => {
        const b = await frame.boundingBox();
        return b.height / b.width;
    }, { timeout: 5_000 }).toBeLessThan(0.35);

    // Switching samples clears the page frame with the canvas.
    await playground.locator('#examples').selectOption({ label: 'SHA-256 (benchmark)' });
    await expect(playground.locator('iframe.pg-page-frame')).toHaveCount(0, { timeout: 5_000 });
});

test('a module-kind run clears a previous page frame', async ({ playground }) => {
    // Edit a graphics sample into a compute-only one and the next build comes
    // back kind:'module'. Without an explicit teardown on that path the old page
    // keeps rendering while the module run prints — a dead program's last frame
    // on screen, which is what the run frame exists to prevent.
    const PAGE_URL = '/api/build/artifact/abc1234/' + FAKE_HASH + '/sample.html';
    await stubBuildInfo(playground, true);
    await stubStore(playground);
    await playground.route('**' + PAGE_URL, route => route.fulfill({
        status: 200, contentType: 'text/html', body: '<html><body>stale page</body></html>',
    }));
    await playground.route('**' + ARTIFACT_URL, route => route.fulfill({
        status: 200,
        contentType: 'application/wasm',
        body: Buffer.from([0, 0x61, 0x73, 0x6d, 1, 0, 0, 0]),
    }));

    // First build: page. Second: module. One status stub, two scripted rounds.
    let round = 0;
    await playground.route('**/api/build/request/**', route => {
        round++;
        return route.fulfill({
            status: 200, contentType: 'application/json',
            body: JSON.stringify(round === 1
                ? { state: 'done', kind: 'page', toolchain: 'abc1234', files: [PAGE_URL] }
                : { state: 'done', kind: 'module', toolchain: 'abc1234', files: [ARTIFACT_URL] }),
        });
    });
    await reloadWithStubs(playground);

    const memory64 = await playground.evaluate(() =>
        WebAssembly.validate(new Uint8Array([0, 0x61, 0x73, 0x6d, 1, 0, 0, 0, 5, 3, 1, 4, 1])));
    test.skip(!memory64, 'browser lacks wasm64/memory64');

    await playground.locator(wasmSel).check();
    await playground.locator('#run').click();
    await expect(playground.locator('iframe.pg-page-frame')).toHaveCount(1, { timeout: 15_000 });

    await playground.locator('#run').click();
    await expect(playground.locator('iframe.pg-page-frame')).toHaveCount(0, { timeout: 15_000 });
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

// The daslang runtime (daslang_static.wasm, ~40MB) is compiled ONCE per page:
// the parent compiles it and every run frame instantiates that shared module.
// Before this, each Run click built a fresh frame that fetched and compiled
// the whole runtime again, and a session of runs accumulated compiled-code
// memory in the browser process until WebAssembly compilation itself failed
// ("wasm streaming compile failed: InternalError: out of memory" — a state
// reloads don't clear, because the quota is per-process, not per-page).
//
// The observable is fetches of the runtime after the page is ready: a run must
// not trigger another daslang_static.wasm fetch. Counted twice — page-level
// request events, and each run frame's own Resource Timing — because a frame
// that fetches it went back to compiling for itself, and belt-and-braces here
// is what keeps the spec honest if either channel's visibility changes.

const { test, expect } = require('@playwright/test');

// Gate on the runner's own readiness, not the Run button: the button is
// re-gated on state *changes*, so mid-run it can briefly hold a stale enabled
// state while isReady() is false.
async function waitForRunReady(page) {
    await page.waitForFunction(
        () => window.PlaygroundRunner && window.PlaygroundRunner.isReady(),
        null, { timeout: 60_000 });
}

async function runAndWaitForOutput(page) {
    const before = await page.evaluate(() => document.querySelectorAll('#output .output_line').length);
    await page.click('#run');
    await page.waitForFunction(
        (n) => document.querySelectorAll('#output .output_line').length > n,
        before, { timeout: 60_000 });
}

test('a session of runs compiles the runtime once, not once per run @wasm', async ({ page }) => {
    test.slow();   // two full runs with respawned spares; parallel-suite load stretches each
    await page.goto('/playground/');
    await page.locator('.CodeMirror').waitFor({ state: 'visible' });
    await page.waitForFunction(() => window.pgSamplesReady === true, null, { timeout: 30_000 });
    await waitForRunReady(page);

    // The page is up and a frame is standing by — from here on, no run may
    // cost another fetch (= another compile) of the runtime.
    let lateWasmFetches = 0;
    page.on('request', (r) => {
        if (r.url().includes('daslang_static.wasm')) lateWasmFetches++;
    });

    await runAndWaitForOutput(page);
    await waitForRunReady(page);          // the respawned spare is what would re-fetch
    await runAndWaitForOutput(page);
    await waitForRunReady(page);

    // Both runs actually executed: the default sample prints "Hello World"
    // once per run. (A weaker any-output check would accept a status line.)
    const lines = await page.evaluate(() =>
        [...document.querySelectorAll('#output .output_line_text')].map((e) => e.textContent).join('\n'));
    expect((lines.match(/Hello World/g) || []).length).toBe(2);
    expect(lines).not.toContain('runtime aborted');

    expect(lateWasmFetches).toBe(0);

    // Ask each live run frame directly too, and prove the loop saw frames —
    // zero inspected frames would make this arm vacuously green.
    let inspected = 0;
    for (const f of page.frames()) {
        if (!f.url().includes('run-frame')) continue;
        inspected++;
        const frameWasmFetches = await f.evaluate(() =>
            performance.getEntriesByType('resource')
                .filter((e) => e.name.includes('daslang_static.wasm')).length);
        expect(frameWasmFetches).toBe(0);
    }
    expect(inspected).toBeGreaterThan(0);
});

// Version skew: an old cached parent never answers need-wasm-module, and the
// frame must fall back to compiling for itself (the pre-shared-module path)
// after the ack timeout. Loading run-frame.html as its own top document IS
// such a parent: its need-wasm-module post goes to itself and nothing answers.
// The /playground/ visit first is what registers the COI service worker, so
// the direct navigation is served cross-origin-isolated like production.
test('a frame whose parent never answers compiles for itself @wasm', async ({ page }) => {
    test.slow();   // a full self-compile after the ack timeout; CI load stretches it
    await page.goto('/playground/');
    await page.waitForFunction(() => window.pgSamplesReady === true, null, { timeout: 30_000 });

    await page.goto('/playground/run-frame.html');
    // The frame posts "ready" from onRuntimeInitialized; as its own top
    // document that post arrives on itself. (Module.callMain exists from
    // script-eval time, long before the runtime is up — don't wait on it.)
    await page.evaluate(() => {
        window.__frameReady = false;
        window.addEventListener('message', (e) => {
            if (e.data && e.data.type === 'ready') window.__frameReady = true;
        });
    });
    await page.waitForFunction(() => window.__frameReady === true, null, { timeout: 60_000 });
    const selfFetches = await page.evaluate(() =>
        performance.getEntriesByType('resource')
            .filter((e) => e.name.includes('daslang_static.wasm')).length);
    expect(selfFetches).toBe(1);
});

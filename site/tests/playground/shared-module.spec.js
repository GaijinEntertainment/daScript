// The daslang runtime (daslang_static.wasm, ~40MB) is compiled ONCE per page:
// the parent compiles it and every run frame instantiates that shared module.
// Before this, each Run click built a fresh frame that fetched and compiled
// the whole runtime again, and a session of runs accumulated compiled-code
// memory in the browser process until WebAssembly compilation itself failed
// ("wasm streaming compile failed: InternalError: out of memory" — a state
// reloads don't clear, because the quota is per-process, not per-page).
//
// The observable is network fetches of the runtime after the page is ready:
// a run must not trigger another daslang_static.wasm fetch — a frame that
// fetches it went back to compiling for itself.

const { test, expect } = require('@playwright/test');

async function waitForRunReady(page) {
    await page.waitForFunction(() => {
        const b = document.getElementById('run');
        return b && !b.disabled;
    }, null, { timeout: 60_000 });
}

async function runAndWaitForOutput(page) {
    const before = await page.evaluate(() => document.querySelectorAll('#output .output_line').length);
    await page.click('#run');
    await page.waitForFunction(
        (n) => document.querySelectorAll('#output .output_line').length > n,
        before, { timeout: 60_000 });
}

test('a session of runs compiles the runtime once, not once per run', async ({ page }) => {
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

    // Both runs actually executed (the count above must not pass vacuously).
    const lines = await page.evaluate(() =>
        [...document.querySelectorAll('#output .output_line_text')].map((e) => e.textContent).join('\n'));
    expect(lines.length).toBeGreaterThan(0);
    expect(lines).not.toContain('runtime aborted');

    expect(lateWasmFetches).toBe(0);
});

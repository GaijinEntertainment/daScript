// A page whose run frames kept aborting exhausts the runner's spare-rebuild
// budget and used to be dead for good: Run answered "daslang is still
// loading, please wait…" forever, and nothing short of closing the tab
// helped. Now a Run click on a dead page is the user's "try again" — the
// runner rebuilds the spare (PlaygroundRunner.revive) and says what it is
// doing.
//
// Aborts are induced by an init script that makes every run-frame document
// post its own "aborted" message the moment it is created — the same message a
// dead network or an out-of-memory runtime produces, landed before the runtime
// can ever come up, so a spare turning ready can never race the abort and
// reset the budget. The top window's __abortSpares flag gates it, so the test
// can stop the sabotage for the revive phase. (Blocking the runtime fetch with
// page.route can't induce this: the COI service worker fetches on the frames'
// behalf, and routes don't see service-worker fetches.)

const { test, expect } = require('@playwright/test');

test('a dead page revives on the next Run click @wasm', async ({ page }) => {
    test.slow();   // several full frame lifecycles; parallel-suite load stretches each
    await page.addInitScript(() => {
        if (window.top === window) { window.__abortSpares = true; return; }
        try {
            if (window.top.__abortSpares) {
                window.parent.postMessage({ type: 'aborted', message: 'test-induced abort' }, window.location.origin);
            }
        } catch (e) { /* cross-origin top — not our frame */ }
    });
    await page.goto('/playground/');
    await page.locator('.CodeMirror').waitFor({ state: 'visible' });
    await page.waitForFunction(() => window.pgSamplesReady === true, null, { timeout: 30_000 });

    // The runner reported the aborts on its way down.
    await expect(page.locator('.output_line_text', { hasText: 'runtime aborted' }).first())
        .toBeVisible({ timeout: 30_000 });

    // Dead page: the budget is spent, no frame standing by, none on the way —
    // and the Run BUTTON is enabled, because the click is the revive trigger.
    // (A disabled button here would make the dead state permanent from the UI.)
    await page.waitForFunction(
        () => document.querySelectorAll('iframe.pg-run-frame').length === 0
            && window.PlaygroundRunner && !window.PlaygroundRunner.isReady()
            && !document.getElementById('run').disabled,
        null, { timeout: 15_000 });

    // Stop the sabotage: the revived spare must be allowed to come up for real.
    await page.evaluate(() => { window.__abortSpares = false; });

    // Run on the dead page — the real button (a disabled button swallows even a
    // programmatic click, so this also proves UI reachability): revive, and say
    // so (not "still loading"). The second call runs in the same evaluate,
    // synchronously after the click, so the revived spare provably hasn't had a
    // frame's lifetime to become ready: it must report "still loading", not
    // claim another revive (the spare-exists arm). Scripted, because the button
    // is correctly disabled during the load — which is the point.
    await page.evaluate(() => {
        document.getElementById('run').click();
        runCode();
    });
    await expect(page.locator('.output_line_text', { hasText: 'failed to start — retrying' }))
        .toBeVisible({ timeout: 15_000 });
    await expect(page.locator('.output_line_text', { hasText: 'still loading, please wait' }))
        .toBeVisible({ timeout: 15_000 });

    // The revived spare comes up for real, the next Run actually executes the
    // program (the default sample prints "Hello World"), and no further abort
    // joined the output.
    await page.waitForFunction(() => {
        const b = document.getElementById('run');
        return b && !b.disabled && window.PlaygroundRunner && window.PlaygroundRunner.isReady();
    }, null, { timeout: 60_000 });
    const abortsBefore = await page.evaluate(() =>
        [...document.querySelectorAll('#output .output_line_text')]
            .filter((e) => e.textContent.includes('runtime aborted')).length);
    await page.click('#run');
    await expect(page.locator('.output_line_text', { hasText: 'Hello World' }))
        .toBeVisible({ timeout: 60_000 });
    const abortsAfter = await page.evaluate(() =>
        [...document.querySelectorAll('#output .output_line_text')]
            .filter((e) => e.textContent.includes('runtime aborted')).length);
    expect(abortsAfter).toBe(abortsBefore);
});

// The compile ladder: when streaming compilation is unavailable (canonically a
// server without the application/wasm MIME type), the ArrayBuffer arm carries
// the page — warn-and-work, never a dead page. Streaming is stubbed dead for
// the whole session; the page must come up and run without a single abort.
test('streaming-compile failure falls back to ArrayBuffer and the page works @wasm', async ({ page }) => {
    test.slow();   // a full ArrayBuffer compile + a run; parallel-suite load stretches it
    await page.addInitScript(() => {
        if (window.top !== window) return;   // the parent's compile is the one with the ladder
        WebAssembly.compileStreaming = () => Promise.reject(new TypeError('induced: streaming unavailable'));
    });
    await page.goto('/playground/');
    await page.locator('.CodeMirror').waitFor({ state: 'visible' });
    await page.waitForFunction(() => window.pgSamplesReady === true, null, { timeout: 30_000 });
    await page.waitForFunction(
        () => window.PlaygroundRunner && window.PlaygroundRunner.isReady(),
        null, { timeout: 60_000 });
    await page.click('#run');
    await expect(page.locator('.output_line_text', { hasText: 'Hello World' }))
        .toBeVisible({ timeout: 60_000 });
    const lines = await page.evaluate(() =>
        [...document.querySelectorAll('#output .output_line_text')].map((e) => e.textContent).join('\n'));
    expect(lines).not.toContain('runtime aborted');
});

// A failed runtime compile must surface its real reason (not hang as "still
// loading"), and the next request must retry the compile — compileRuntime
// drops its promise on rejection precisely so the failure is not final. The
// stub fails BOTH rungs of the compile ladder (streaming, then the ArrayBuffer
// retry) with a shared budget of two full ladders: the eager init call absorbs
// one ladder, the first spare's request eats the second and aborts, so the
// rebuilt spare's request is the retry that succeeds.
test('a failed runtime compile reports itself and the retry recovers @wasm', async ({ page }) => {
    test.slow();   // two compile attempts + a full run; parallel-suite load stretches each
    await page.addInitScript(() => {
        if (window.top !== window) return;   // stub the parent page only
        const origStreaming = WebAssembly.compileStreaming.bind(WebAssembly);
        const origCompile = WebAssembly.compile.bind(WebAssembly);
        let failures = 4;   // 2 ladders × 2 rungs
        const failing = (orig) => (...a) => {
            if (failures > 0) { failures--; return Promise.reject(new Error('induced compile failure')); }
            return orig(...a);
        };
        WebAssembly.compileStreaming = failing(origStreaming);
        WebAssembly.compile = failing(origCompile);
    });
    await page.goto('/playground/');
    await page.locator('.CodeMirror').waitFor({ state: 'visible' });
    await page.waitForFunction(() => window.pgSamplesReady === true, null, { timeout: 30_000 });

    // The first spare heard {module: null, error} and reported the real cause.
    await expect(page.locator('.output_line_text', { hasText: 'wasm compile failed: induced compile failure' }))
        .toBeVisible({ timeout: 30_000 });

    // The rebuilt spare's request re-compiles for real and the page recovers.
    await page.waitForFunction(() => {
        const b = document.getElementById('run');
        return b && !b.disabled && window.PlaygroundRunner && window.PlaygroundRunner.isReady();
    }, null, { timeout: 60_000 });
    const before = await page.evaluate(() => document.querySelectorAll('#output .output_line').length);
    await page.click('#run');
    await page.waitForFunction(
        (n) => document.querySelectorAll('#output .output_line').length > n,
        before, { timeout: 60_000 });
    const lines = await page.evaluate(() =>
        [...document.querySelectorAll('#output .output_line_text')].map((e) => e.textContent).join('\n'));
    expect(lines).not.toContain('runtime aborted: run-frame');
});

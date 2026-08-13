// A page whose run frames kept aborting exhausts the runner's spare-rebuild
// budget and used to be dead for good: Run answered "daslang is still
// loading, please wait…" forever, and nothing short of closing the tab
// helped. Now a Run click on a dead page is the user's "try again" — the
// runner rebuilds the spare (PlaygroundRunner.revive) and says what it is
// doing.
//
// Aborts are induced by posting each spare frame's own "aborted" message from
// inside that frame — the same message a dead network or an out-of-memory
// runtime produces. (Blocking the runtime fetch with page.route can't do it:
// the COI service worker fetches on the frames' behalf, and routes don't see
// service-worker fetches.)

const { test, expect } = require('@playwright/test');

function runFrame(page) {
    return page.frames().find((f) => f.url().includes('run-frame.html'));
}

test('a dead page revives on the next Run click', async ({ page }) => {
    await page.goto('/playground/');
    await page.locator('.CodeMirror').waitFor({ state: 'visible' });
    await page.waitForFunction(() => window.pgSamplesReady === true, null, { timeout: 30_000 });

    // Abort the spare; the runner rebuilds it (bounded), abort each rebuild.
    for (let round = 1; round <= 4; round++) {
        await page.waitForFunction(
            () => document.querySelectorAll('iframe.pg-run-frame').length === 1,
            null, { timeout: 15_000 });
        const frame = runFrame(page);
        expect(frame).toBeTruthy();
        await frame.evaluate(() =>
            parent.postMessage({ type: 'aborted', message: 'test-induced abort' }, location.origin));
        await expect(page.locator('.output_line_text', { hasText: 'runtime aborted' }))
            .toHaveCount(round, { timeout: 15_000 });
    }

    // Dead page: the budget is spent, no frame standing by, none on the way.
    await page.waitForFunction(
        () => document.querySelectorAll('iframe.pg-run-frame').length === 0
            && window.PlaygroundRunner && !window.PlaygroundRunner.isReady(),
        null, { timeout: 5_000 });

    // Run on the dead page: revive, and say so (not "still loading").
    await page.evaluate(() => runCode());
    await expect(page.locator('.output_line_text', { hasText: 'failed to start — retrying' }))
        .toBeVisible({ timeout: 5_000 });

    // The revived spare comes up for real and the next Run executes.
    await page.waitForFunction(() => {
        const b = document.getElementById('run');
        return b && !b.disabled;
    }, null, { timeout: 60_000 });
    const before = await page.evaluate(() => document.querySelectorAll('#output .output_line').length);
    await page.click('#run');
    await page.waitForFunction(
        (n) => document.querySelectorAll('#output .output_line').length > n,
        before, { timeout: 60_000 });
});

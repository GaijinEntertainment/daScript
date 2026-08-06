// Shared Playwright fixtures + helpers for /playground/ specs.
// `playground` fixture: navigates to /playground/ and waits for CodeMirror to
// fully initialize so subsequent assertions don't race the WASM script load.

const base = require('@playwright/test');

const test = base.test.extend({
    playground: async ({ page }, use) => {
        await page.goto('/playground/');
        await page.locator('.CodeMirror').waitFor({ state: 'visible' });
        await page.waitForFunction(
            () => typeof window.code === 'object' && window.code !== null && typeof window.code.swapDoc === 'function',
            null,
            { timeout: 10_000 }
        );
        // The sample list arrives over the network and its default selection
        // lands in the editor when it does. Handing the page over before that
        // settles lets it drop into the middle of a test's own edits, which
        // reads as an unrelated flake somewhere later in the spec.
        await page.waitForFunction(() => window.pgSamplesReady === true, null, { timeout: 15_000 });
        await use(page);
    },
});

const { expect } = base;

module.exports = { test, expect };

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
        await use(page);
    },
});

const { expect } = base;

module.exports = { test, expect };

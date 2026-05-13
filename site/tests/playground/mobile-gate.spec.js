// Mobile gate: at narrow viewports, /playground/ must NOT load the WASM
// bundle or initialize the CodeMirror IDE. Instead, render the "open on a
// laptop" notice. Guards against regressing the WASM short-circuit.

const { test, expect } = require('@playwright/test');

test.use({ viewport: { width: 390, height: 800 } });

test('mobile playground shows the notice and does not load WASM', async ({ page }) => {
    const requested = [];
    page.on('request', (r) => requested.push(r.url()));

    await page.goto('/playground/?nocache=mobile', { waitUntil: 'load' });

    // The is-pg-mobile root class is the gate signal.
    await expect.poll(() => page.evaluate(() => document.documentElement.classList.contains('is-pg-mobile')))
        .toBe(true);

    // Notice should be visible, .main hidden by CSS.
    await expect(page.locator('#pg-mobile-notice')).toBeVisible();
    await expect(page.locator('.main')).toBeHidden();

    // Neither the WASM nor the loader JS should ever have been requested.
    const heavy = requested.filter(u =>
        u.includes('daslang_static') || u.endsWith('.wasm'));
    expect(heavy).toEqual([]);
});

test('desktop playground does not show the mobile notice', async ({ page }) => {
    await page.setViewportSize({ width: 1400, height: 900 });
    await page.goto('/playground/?nocache=desktop', { waitUntil: 'domcontentloaded' });
    await page.locator('.CodeMirror').waitFor({ timeout: 10_000 });
    expect(await page.evaluate(() => document.documentElement.classList.contains('is-pg-mobile')))
        .toBe(false);
    await expect(page.locator('#pg-mobile-notice')).toBeHidden();
});

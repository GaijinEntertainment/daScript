// Hero ↗ playground handoff: the landing page's hero CodeMirror lets the
// user click ↗ playground to open /playground/ pre-populated with the
// current buffer via the legacy `#code=<percent>` hash format. This spec
// is the backstop that the legacy format still routes into main.das after
// the multi-file rework.

const { test, expect } = require('./fixtures.js');

test('hero ↗ playground hands off the current buffer to /playground/', async ({ page, context }) => {
    await page.goto('/');
    // The hero editor is a Forge-themed CodeMirror instance.
    await page.locator('.forge-editor .CodeMirror').waitFor();
    await page.waitForFunction(() => typeof window.CodeMirror === 'function');

    // Type a distinctive marker into the hero editor.
    const editor = page.locator('.forge-editor .CodeMirror').first();
    await editor.click();
    // Use the CM API directly to avoid contenteditable quirks.
    await page.evaluate(() => {
        const cmEl = document.querySelector('.forge-editor .CodeMirror');
        const cm = cmEl.CodeMirror;
        cm.setValue('// HERO-HANDOFF-MARKER\nprint("via hero")\n');
    });

    // Clicking ↗ playground opens a new tab.
    const popupPromise = context.waitForEvent('page');
    await page.locator('#hero-playground').click();
    const newPage = await popupPromise;
    await newPage.waitForLoadState('domcontentloaded');

    // Hash must be `#code=...` (legacy single-file format).
    expect(newPage.url()).toContain('/playground/');
    expect(newPage.url()).toContain('#code=');

    // Playground populates pgState with main.das holding the marker.
    await newPage.waitForFunction(
        () => !!window.pgState && 'main.das' in window.pgState.files,
        null,
        { timeout: 30_000 }
    );
    const files = await newPage.evaluate(() => Object.keys(window.pgState.files));
    expect(files).toEqual(['main.das']);
    const mainText = await newPage.evaluate(() => window.pgState.files['main.das'].getValue());
    expect(mainText).toContain('HERO-HANDOFF-MARKER');
    await newPage.close();
});

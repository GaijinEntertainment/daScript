// Phase-1 smoke spec: prove the test harness wires up against /playground/.
// Subsequent specs replace this with the real feature coverage; this one
// stays as a regression guard for "the page literally renders."

const { test, expect } = require('./fixtures.js');

test('playground renders with Forge-themed CodeMirror', async ({ playground }) => {
    await expect(playground).toHaveTitle(/Playground.*Daslang/i);

    const cm = playground.locator('.CodeMirror').first();
    await expect(cm).toBeVisible();
    await expect(cm).toHaveClass(/cm-s-forge/);
});

test('daslang mode tokenizes the default sample', async ({ playground }) => {
    // The default first example puts daslang source in the editor; once CM
    // is up the line-mode emits .cm-keyword spans for `def`, `let`, etc.
    const tokens = playground.locator('.CodeMirror-code .cm-keyword');
    await expect(tokens.first()).toBeVisible({ timeout: 10_000 });
});

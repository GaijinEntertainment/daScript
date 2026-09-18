// The examples page on a phone: the top-level nav link, the player filling the viewport, the
// touch controls line, and no fullscreen pill where element fullscreen does not exist. Nothing
// here needs the wasm artifacts: the player opens on a card whose iframe may 404 in the per-PR
// stage, and every assertion reads the chrome around the frame.

const { test, expect, devices } = require('@playwright/test');

test.use({ ...devices['iPhone 13'], browserName: 'chromium' });   // the per-PR lane installs chromium alone

test('examples is a top-level nav item and the phone player fills the viewport', async ({ page }) => {
    await page.goto('/examples.html');
    const links = page.locator('.forge-nav__links > a[href="/examples.html"]');
    await expect(links).toHaveCount(1);
    await expect(links).toHaveClass(/is-active/);

    await page.locator('.forge-ex-card[data-id="arcanoid"]').click();
    const player = page.locator('.forge-ex-player');
    await expect(player).toBeVisible();
    const box = await player.boundingBox();
    const size = page.viewportSize();
    expect(Math.round(box.width)).toBe(size.width);
    expect(Math.round(box.height)).toBe(size.height);

    // a coarse pointer gets the on-screen controls line, never the keyboard one
    await expect(page.locator('.forge-ex-player__controls')).toContainText('finger');
    await expect(page.locator('.forge-ex-player__controls')).not.toContainText('F3 F4');
});

test('the fullscreen pill is absent where element fullscreen does not exist', async ({ page }) => {
    await page.addInitScript(() => {
        Object.defineProperty(document, 'fullscreenEnabled', { get: () => false });
    });
    await page.goto('/examples.html');
    await page.locator('.forge-ex-card').first().click();
    await expect(page.locator('.forge-ex-player')).toBeVisible();
    await expect(page.locator('.forge-ex-pill[data-act="fullscreen"]')).toHaveCount(0);
    await expect(page.locator('.forge-ex-pill[data-act="restart"]')).toHaveCount(1);
});

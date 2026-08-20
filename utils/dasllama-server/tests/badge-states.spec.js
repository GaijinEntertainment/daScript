// The badge is the page's one-glance health surface: serving / draining /
// unreachable each have a distinct class + text, and a transient blip must
// NOT flip it (three consecutive failures required).

const { test, expect, fx, openControl } = require('./fixtures');

test('draining flips the badge to warn', async ({ page }) => {
    await openControl(page, { stats: { ...fx('stats'), draining: true } });
    await expect(page.locator('#badge')).toHaveText('draining');
    await expect(page.locator('#badge')).toHaveClass(/warn/);
});

test('three consecutive poll failures mark the server unreachable', async ({ page }) => {
    const good = fx('stats');
    await openControl(page, {
        stats: poll => (poll === 0 ? good : { status: 500, json: {} }),
    });
    await expect(page.locator('#badge')).toHaveText('serving');
    await expect(page.locator('#badge')).toHaveText('unreachable', { timeout: 10_000 });
    await expect(page.locator('#badge')).toHaveClass(/err/);
});

test('a single failed poll keeps the serving badge', async ({ page }) => {
    const good = fx('stats');
    await openControl(page, {
        stats: poll => (poll === 1 ? { status: 500, json: {} } : good),
    });
    // outlive the blip by two more good polls — the badge never left "serving"
    await page.waitForTimeout(3500);
    await expect(page.locator('#badge')).toHaveText('serving');
    await expect(page.locator('#badge')).toHaveClass(/ok/);
});

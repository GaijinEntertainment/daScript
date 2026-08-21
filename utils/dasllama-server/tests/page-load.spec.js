// First paint from one stats poll: header identity, badge, tiles, and the
// conditional tiles/buttons that must stay hidden on a plain CPU text server.

const { test, expect, fx, openControl } = require('./fixtures');

test('header, badge, and tiles render from /v1/stats', async ({ page }) => {
    const s = fx('stats');
    await openControl(page);

    await expect(page.locator('#badge')).toHaveText('serving');
    await expect(page.locator('#badge')).toHaveClass(/ok/);
    await expect(page.locator('#model')).toContainText(s.model);
    await expect(page.locator('#uptime')).toContainText('up');
    await expect(page.locator('#hw')).toHaveText(s.hardware);

    await expect(page.locator('#t-streams')).toHaveText(`${s.active} / ${s.max_streams}`);
    await expect(page.locator('#t-queued')).toHaveText(String(s.queued));
    await expect(page.locator('#t-batch')).toHaveText(s.avg_batch.toFixed(2));
    await expect(page.locator('#cfg')).toContainText(s.model);
    await expect(page.locator('#cfg')).toContainText(`streams ${s.max_streams}`);
});

test('without those arms: mtp/asr tiles and studio hide; image and mic dim to tower offers', async ({ page }) => {
    const s = fx('stats');
    const arms = [s.mtp_drafted > 0 && 'mtp', s.asr_workers > 0 && 'asr', s.vision && 'vision'].filter(Boolean);
    // hard failure, not a skip: a regenerated fixture that grew an arm must be re-captured
    // from a plain CPU text server (fixtures/README.md), not silently drop this coverage
    expect(arms, 'stats fixture carries an arm this test assumes absent').toEqual([]);
    await openControl(page);

    await expect(page.locator('#tile-mtp')).toBeHidden();
    await expect(page.locator('#tile-asr')).toBeHidden();
    await expect(page.locator('#asr-section')).toBeHidden();
    // the buttons stay visible-but-dimmed while serving: hidden read as "can't do that",
    // when the truth is "download the tower in § 03" (chat.spec covers the click paths)
    await expect(page.locator('#b-img')).toHaveClass(/off/);
    await expect(page.locator('#b-mic')).toHaveClass(/off/);
});

test('a vision arm reveals the image button; asr workers reveal the tile, mic, and studio', async ({ page }) => {
    const s = { ...fx('stats'), vision: true, asr_workers: 2, asr_ready: 2, asr_active: 0, asr_pending: 0 };
    await openControl(page, { stats: s });

    await expect(page.locator('#b-img')).toBeVisible();
    await expect(page.locator('#tile-asr')).toBeVisible();
    await expect(page.locator('#t-asr')).toHaveText('2 · 0 · 0');
    await expect(page.locator('#b-mic')).toBeVisible();
    await expect(page.locator('#asr-section')).toBeVisible();
});

test('mtp counters unhide their tile with the acceptance rate', async ({ page }) => {
    const s = { ...fx('stats'), mtp_drafted: 200, mtp_accepted: 150 };
    await openControl(page, { stats: s });

    await expect(page.locator('#tile-mtp')).toBeVisible();
    await expect(page.locator('#t-mtp')).toHaveText('150 / 200 (75%)');
});

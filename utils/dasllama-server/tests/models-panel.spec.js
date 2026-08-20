// §02 models: one card per served slot, activate wiring, and the VRAM bar.
// Fixture: stats_multi carries two slots (smol active + default, tiny idle cpu).

const { test, expect, fx, openControl, lastJson } = require('./fixtures');

test('two slots render as cards — active class, default tag, activate on the idle one', async ({ page }) => {
    const s = fx('stats_multi');
    const [act, idle] = [s.models.find(m => m.is_active), s.models.find(m => !m.is_active)];
    await openControl(page, { stats: s });

    const actCard = page.locator('#mc-' + act.name);
    const idleCard = page.locator('#mc-' + idle.name);
    await expect(actCard).toHaveClass(/active/);
    await expect(actCard).toContainText('active');
    await expect(actCard.locator('button[data-activate]')).toHaveCount(0);
    await expect(idleCard).not.toHaveClass(/active/);
    await expect(idleCard.locator('button[data-activate]')).toBeVisible();
    if (act.is_default) await expect(actCard.locator('.def')).toHaveText('default');
    await expect(idleCard).toContainText('backend ' + idle.backend);
});

test('a single slot offers no activate button', async ({ page }) => {
    const s = fx('stats');
    await openControl(page, { stats: s });
    await expect(page.locator('#mc-' + s.models[0].name)).toBeVisible();
    await expect(page.locator('button[data-activate]')).toHaveCount(0);
});

test('activate POSTs the slot name and reports the switch', async ({ page }) => {
    const s = fx('stats_multi');
    const idle = s.models.find(m => !m.is_active);
    const { posts } = await openControl(page, {
        stats: s,
        responses: { '/v1/models/activate': { json: fx('activate_response') } },
    });

    await page.locator('button[data-activate]').click();
    await expect(page.locator('#m-note')).toContainText('active · switch');
    expect(lastJson(posts.filter(p => p.path === '/v1/models/activate'))).toEqual({ model: idle.name });
});

test('a failed activate surfaces the server error message', async ({ page }) => {
    const s = fx('stats_multi');
    await openControl(page, {
        stats: s,
        responses: { '/v1/models/activate': { status: 409, json: { error: { message: 'switch refused: work is live' } } } },
    });
    await page.locator('button[data-activate]').click();
    await expect(page.locator('#m-note')).toHaveText('switch refused: work is live');
});

test('a GPU budget reveals the VRAM bar with the owner and fill', async ({ page }) => {
    const s = { ...fx('stats_multi'), gpu_budget_bytes: 8e9, gpu_vram_bytes: 2e9 };
    s.models = s.models.map((m, i) => (i === 0 ? { ...m, holds_gpu: true } : m));
    await openControl(page, { stats: s });

    await expect(page.locator('#vram-panel')).toBeVisible();
    await expect(page.locator('#vram-now')).toContainText(s.models[0].name);
    await expect(page.locator('#vram-now')).toContainText('2.00 / 8.00 GB');
    await expect(page.locator('#vram-fill')).toHaveCSS('width', /[1-9]/);
});

test('no GPU budget keeps the VRAM panel hidden', async ({ page }) => {
    await openControl(page, { stats: fx('stats') });
    await expect(page.locator('#vram-panel')).toBeHidden();
});

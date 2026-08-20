// §08 config editor: fields + per-key source chips from /config, the
// [[models]] roster, pre-save sanity, and the exchange-policy save guard
// (POST /config replaces the whole file — the policy must ride every save).

const { test, expect, fx, openControl, lastJson } = require('./fixtures');

test('form fields render with their source chips and values', async ({ page }) => {
    const c = fx('config');
    await openControl(page);

    await expect(page.locator('#f-streams')).toHaveValue(String(c.surface.config.streams));
    await expect(page.locator('#f-port')).toHaveValue(String(c.surface.config.port));
    for (const [key, src] of Object.entries(c.surface.sources)) {
        const chip = page.locator(`label[for="f-${key}"] .src`);
        if (await chip.count()) await expect(chip).toHaveText(src);
    }
    await expect(page.locator('#cfg-file')).toContainText('saves to');
});

test('the roster renders one row per model and add/remove works', async ({ page }) => {
    const c = fx('config');
    const nRows = Math.max(1, (c.surface.models || []).length);
    await openControl(page);

    await expect(page.locator('#roster-body input[data-k="path"]')).toHaveCount(nRows);
    await page.locator('#b-addmodel').click();
    await expect(page.locator('#roster-body input[data-k="path"]')).toHaveCount(nRows + 1);
    await page.locator('#roster-body button[data-act="del"]').last().click();
    await expect(page.locator('#roster-body input[data-k="path"]')).toHaveCount(nRows);
});

test('the last roster row cannot be removed', async ({ page }) => {
    const c = { ...fx('config') };
    c.surface = { ...c.surface, models: [] };   // flat single-model surface -> one synthesized row
    await openControl(page, { config: c });
    await expect(page.locator('#roster-body input[data-k="path"]')).toHaveCount(1);
    await expect(page.locator('#roster-body button[data-act="del"]')).toBeDisabled();
});

test('save POSTs the config with the exchange policy merged in', async ({ page }) => {
    const ex = fx('exchange');
    const { posts } = await openControl(page);

    await page.locator('#f-streams').fill('8');
    await page.locator('#b-save').click();
    await expect(page.locator('#cfg-note')).toHaveText('saved — restart to apply');

    const body = lastJson(posts.filter(p => p.path === '/config'));
    expect(body.streams).toBe(8);
    expect(body.exchange_accept).toBe(ex.policy.accept);
    expect(body.exchange_submit).toBe(ex.policy.submit);
    expect(body.exchange_url).toBe(ex.policy.url);
    expect(Array.isArray(body.models)).toBe(true);
    expect(body.models[0].path).toBeTruthy();
});

test('save is blocked while the exchange policy has not loaded', async ({ page }) => {
    const { posts } = await openControl(page, { exchange: { status: 500, json: {} } });
    await page.locator('#b-save').click();
    await expect(page.locator('#cfg-note')).toContainText('exchange policy still loading');
    expect(posts.filter(p => p.path === '/config')).toHaveLength(0);
});

test('a duplicate route name blocks the save before the round trip', async ({ page }) => {
    const { posts } = await openControl(page);
    await page.locator('#b-addmodel').click();
    const paths = page.locator('#roster-body input[data-k="path"]');
    const names = page.locator('#roster-body input[data-k="name"]');
    await paths.last().fill('D:/models/other.gguf');
    await names.first().fill('dup');
    await names.last().fill('dup');
    await page.locator('#b-save').click();
    await expect(page.locator('#cfg-note')).toContainText("duplicate model name 'dup'");
    expect(posts.filter(p => p.path === '/config')).toHaveLength(0);
});

test('a roster row without a path blocks the save', async ({ page }) => {
    const { posts } = await openControl(page);
    await page.locator('#b-addmodel').click();
    await page.locator('#b-save').click();
    await expect(page.locator('#cfg-note')).toContainText('every model row needs a path');
    expect(posts.filter(p => p.path === '/config')).toHaveLength(0);
});

// live model switch: a downloaded-but-unserved catalog row offers "serve live" (POST
// /v1/models/load — new slot, becomes default, no restart), and a non-default model card
// offers "unload" (POST /v1/models/unload — frees the slot). Both notes echo the server's answer.

const { test, expect, fx, openControl, lastJson } = require('./fixtures');

function statsServing(name, file) {
    const s = JSON.parse(JSON.stringify(fx('stats_multi')));
    if (name) s.models[0].name = name;
    if (file) s.models[0].file = file;
    return s;
}

// catalog_done rows, in fixture order
const E2B_ROW = 0, E4B_ROW = 1;

test('a present, unserved row offers serve live; a served row does not', async ({ page }) => {
    await openControl(page, {
        stats: statsServing('gemma-4-E2B-it-Q4_K_M.gguf'),
        catalog: fx('catalog_done'),
    });
    const rows = page.locator('#cat-body tr');
    const e2b = rows.nth(E2B_ROW), e4b = rows.nth(E4B_ROW);
    await expect(e2b.getByRole('button', { name: 'serve live' })).toHaveCount(0);   // served
    await expect(e4b.getByRole('button', { name: 'serve live' })).toBeVisible();    // present, unserved
});

test('a served file under a custom slot name still hides serve live', async ({ page }) => {
    await openControl(page, {
        stats: statsServing('my-gemma', 'gemma-4-E2B-it-Q4_K_M.gguf'),
        catalog: fx('catalog_done'),
    });
    const rows = page.locator('#cat-body tr');
    await expect(rows.nth(E2B_ROW).getByRole('button', { name: 'serve live' })).toHaveCount(0);
    await expect(rows.nth(E4B_ROW).getByRole('button', { name: 'serve live' })).toBeVisible();
});

test('serve live posts the load with the row path and echoes the switch', async ({ page }) => {
    const { posts } = await openControl(page, {
        stats: fx('stats_multi'),
        catalog: fx('catalog_done'),
        responses: { '/v1/models/load': { status: 200, json: {
            ok: true, model: 'gemma-4-E4B-it-Q4_K_M.gguf', backend_effective: 'gpu:rails',
            load_ms: 4200, ctx: 131072, tower_note: '' } } },
    });
    await page.locator('#cat-body tr').nth(E4B_ROW).getByRole('button', { name: 'serve live' }).click();
    await expect(page.locator('#cat-note')).toContainText('serving (gpu:rails, 4.2s load)');
    const body = lastJson(posts.filter(p => p.path === '/v1/models/load'));
    expect(body.path).toContain('gemma-4-E4B');
    expect(body.image_mmproj).toBe('');   // this fixture row has no vision tower on disk
});

test('a load refusal lands in the note and re-enables the button', async ({ page }) => {
    await openControl(page, {
        stats: fx('stats_multi'),
        catalog: fx('catalog_done'),
        responses: { '/v1/models/load': { status: 409, json: {
            error: { message: 'streams are live (active 2, queued 0) — retry when idle', type: 'busy' } } } },
    });
    const btn = page.locator('#cat-body tr').nth(E4B_ROW).getByRole('button', { name: 'serve live' });
    await btn.click();
    await expect(page.locator('#cat-note')).toContainText('streams are live');
    await expect(btn).toBeEnabled();
});

test('a non-default model card offers unload; the default does not', async ({ page }) => {
    const { posts } = await openControl(page, {
        stats: fx('stats_multi'),
        responses: { '/v1/models/unload': { status: 200, json: { ok: true, unloaded: 'tiny' } } },
    });
    const cards = page.locator('#mcards .mcard');
    await expect(cards.first().getByRole('button', { name: 'unload' })).toHaveCount(0);   // the default (smol)
    const un = cards.nth(1).getByRole('button', { name: 'unload' });
    await expect(un).toBeVisible();
    await un.click();
    await expect(page.locator('#m-note')).toHaveText('tiny unloaded');
    expect(lastJson(posts.filter(p => p.path === '/v1/models/unload')).model).toBe('tiny');
});

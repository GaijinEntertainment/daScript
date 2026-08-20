// §03 tower auto-offer: a downloaded model's vision tower and the parakeet ASR tower each
// walk download -> enable -> wired, and setup's serve-this-model wires present towers itself.

const { test, expect, fx, openControl, lastJson } = require('./fixtures');

const E2B_TOWER_PATH = 'C:\\Users\\user\\.dasllama\\models\\mmproj-gemma-4-E2B-it-bf16.gguf';
const ASR_PATH = 'C:\\Users\\user\\.dasllama\\models\\ggml-parakeet-tdt-0.6b-v3-f32.bin';

function withVisionPresent(doc) {
    const d = JSON.parse(JSON.stringify(doc));
    const e = d.entries.find(x => x.name === 'gemma-4-e2b');
    e.vision_present = true;
    e.vision_path = E2B_TOWER_PATH;
    return d;
}

function withAsrPresent(doc) {
    const d = JSON.parse(JSON.stringify(doc));
    d.asr.present = true;
    d.asr.path = ASR_PATH;
    return d;
}

// the config surface with the roster's default row serving the catalog's E2B file
function servingE2B() {
    const c = JSON.parse(JSON.stringify(fx('config')));
    c.surface.models[0].path = 'C:\\Users\\user\\.dasllama\\models\\gemma-4-E2B-it-Q4_K_M.gguf';
    return c;
}

test('a downloaded vision-capable model offers its tower with the pinned size', async ({ page }) => {
    const { posts } = await openControl(page, { catalog: fx('catalog_idle') });
    const row = page.locator('#cat-body tr', { hasText: 'Gemma 4 E2B' });
    const b = row.locator('button', { hasText: 'vision tower' });
    await expect(b).toHaveText('vision tower (1.0 GB)');
    await b.click();
    const body = lastJson(posts.filter(p => p.path === '/catalog/download'));
    expect(body).toEqual({ name: 'gemma-4-e2b', tower: 'vision' });
});

test('a running vision-tower download renders its progress on the row', async ({ page }) => {
    const doc = JSON.parse(JSON.stringify(fx('catalog_idle')));
    doc.download = { state: 'downloading', name: 'vision:gemma-4-e2b', got: 493416832, total: 986833664, error: '' };
    await openControl(page, { catalog: doc });
    const row = page.locator('#cat-body tr', { hasText: 'Gemma 4 E2B' });
    await expect(row).toContainText('downloading — 50% of 1.0 GB');
    // the model-download buttons wait their turn
    await expect(page.locator('#cat-body button', { hasText: 'download' }).first()).toBeDisabled();
});

test('a present tower on a served model offers enable-vision, which wires, saves, and offers the restart', async ({ page }) => {
    const { posts } = await openControl(page, {
        config: servingE2B(),
        catalog: withVisionPresent(fx('catalog_idle')),
    });
    const row = page.locator('#cat-body tr', { hasText: 'Gemma 4 E2B' });
    await row.locator('button', { hasText: 'enable vision' }).click();
    await expect(page.locator('#cat-note')).toContainText('vision tower wired');
    await expect(page.locator('#cat-note')).toContainText('dictation (below) can ride the same restart');
    const body = lastJson(posts.filter(p => p.path === '/config'));
    expect(body.models[0].image_mmproj).toBe(E2B_TOWER_PATH);
    // the note OFFERS the restart — clicking it drains + relaunches without a trip to § 09
    await page.locator('#cat-note button', { hasText: 'restart now' }).click();
    await expect(page.locator('#badge')).toHaveText(/restarting/);
    expect(posts.some(p => p.path === '/restart')).toBe(true);
});

test('a wired tower renders the vision check chip instead of a button', async ({ page }) => {
    const c = servingE2B();
    c.surface.models[0].image_mmproj = E2B_TOWER_PATH;
    await openControl(page, { config: c, catalog: withVisionPresent(fx('catalog_idle')) });
    const row = page.locator('#cat-body tr', { hasText: 'Gemma 4 E2B' });
    await expect(row.locator('.chip', { hasText: 'vision' })).toContainText('vision ✓');
    await expect(row.locator('button', { hasText: 'enable vision' })).toHaveCount(0);
});

test('the dictation offer downloads the pinned parakeet tower', async ({ page }) => {
    const { posts } = await openControl(page, { catalog: fx('catalog_idle') });
    const b = page.locator('#asr-offer button');
    await expect(b).toContainText('download parakeet asr (2.5 GB)');
    await b.click();
    const body = lastJson(posts.filter(p => p.path === '/catalog/download'));
    expect(body.tower).toBe('asr');
});

test('a downloaded ASR tower offers enable-dictation, which fills the asr override and saves', async ({ page }) => {
    const { posts } = await openControl(page, { catalog: withAsrPresent(fx('catalog_idle')) });
    await page.locator('#asr-offer button', { hasText: 'enable dictation' }).click();
    await expect(page.locator('#cat-note')).toContainText('dictation wired');
    const body = lastJson(posts.filter(p => p.path === '/config'));
    expect(body.asr).toBe(ASR_PATH);
    await expect(page.locator('#en-asr')).toBeChecked();
});

test('a wired ASR config renders the dictation check chip', async ({ page }) => {
    const c = JSON.parse(JSON.stringify(fx('config')));
    c.surface.config.asr = ASR_PATH;
    c.surface.sources.asr = 'toml';
    await openControl(page, { config: c, catalog: withAsrPresent(fx('catalog_idle')) });
    await expect(page.locator('#asr-offer .chip')).toHaveText('dictation ✓');
});

test('setup-mode serve wires already-present towers into the fresh config', async ({ page }) => {
    const doc = withAsrPresent(withVisionPresent(fx('catalog_idle')));
    const { posts } = await openControl(page, { stats: fx('stats_setup'), catalog: doc });
    const row = page.locator('#cat-body tr', { hasText: 'Gemma 4 E2B' });
    await row.locator('button', { hasText: 'serve this model' }).click();
    await expect(page.locator('#badge')).toHaveText(/restarting/);
    const cfg = lastJson(posts.filter(p => p.path === '/config'));
    expect(cfg.models[0].image_mmproj).toBe(E2B_TOWER_PATH);
    expect(cfg.asr).toBe(ASR_PATH);
    expect(posts.some(p => p.path === '/restart')).toBe(true);
});

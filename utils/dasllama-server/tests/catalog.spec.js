// §03 model catalog + setup mode: the "installed, ran, now what" surface — entry
// rendering, the one-at-a-time download rail, and setup's download→serve→restart exit.

const { test, expect, fx, openControl, lastJson } = require('./fixtures');

test('setup mode: badge, header, and chat gate flip; serving mode leaves them alone', async ({ page }) => {
    await openControl(page, { stats: fx('stats_setup') });
    await expect(page.locator('#badge')).toHaveText('setup — pick a model');
    await expect(page.locator('#badge')).toHaveClass(/warn/);
    await expect(page.locator('#model')).toHaveText('no model loaded');
    await expect(page.locator('#b-send')).toBeDisabled();
    await expect(page.locator('#chat-text')).toHaveAttribute('placeholder', /no model loaded/);
});

test('the catalog renders every entry: star on the default, present chip, size and needs', async ({ page }) => {
    const doc = fx('catalog_idle');
    await openControl(page, { stats: fx('stats_setup'), catalog: doc });

    await expect(page.locator('#cat-body tr')).toHaveCount(doc.entries.length);
    const def = doc.entries.find(e => e.is_default);
    const present = doc.entries.filter(e => e.present);
    expect(present.length).toBeGreaterThan(0);
    await expect(page.locator('#cat-body')).toContainText(def.display + ' ★');
    await expect(page.locator('#cat-body .chip', { hasText: 'downloaded' })).toHaveCount(present.length);
    await expect(page.locator('#cat-body')).toContainText((def.bytes / 1e9).toFixed(1) + ' GB');
    // setup mode: a present entry offers the serve button; absent entries offer download
    await expect(page.locator('#cat-body button', { hasText: 'serve this model' })).toHaveCount(present.length);
    await expect(page.locator('#cat-body button', { hasText: 'download' })).toHaveCount(doc.entries.length - present.length);
});

test('serving mode hides the serve button but still offers downloads', async ({ page }) => {
    await openControl(page, { catalog: fx('catalog_idle') });
    await expect(page.locator('#cat-body tr')).toHaveCount(fx('catalog_idle').entries.length);
    await expect(page.locator('#cat-body button', { hasText: 'serve this model' })).toHaveCount(0);
    await expect(page.locator('#cat-body button', { hasText: 'download' }).first()).toBeEnabled();
});

test('download click POSTs the entry name and the in-flight state renders progress', async ({ page }) => {
    const idle = fx('catalog_idle');
    const target = idle.entries.find(e => !e.present);
    // the in-flight doc: the served entry list with the byte progress of a real capture
    const wire = fx('catalog_downloading').download;
    const rolling = { ...idle, download: { state: 'downloading', name: target.name, got: wire.got, total: target.bytes, error: '' } };
    const { posts } = await openControl(page, {
        stats: fx('stats_setup'),
        // two pre-click fetches: the bootstrap load and the setup-flip re-render
        catalog: poll => (poll <= 1 ? idle : rolling),
    });

    await page.locator('#cat-body button', { hasText: 'download' }).first().click();
    expect(lastJson(posts.filter(p => p.path === '/catalog/download')).name).toBe(target.name);
    const pct = (100 * rolling.download.got / rolling.download.total).toFixed(0);
    await expect(page.locator('#cat-body')).toContainText(`downloading — ${pct}% of`);
    // every other row's download button waits its turn
    for (const b of await page.locator('#cat-body button', { hasText: 'download' }).all()) {
        await expect(b).toBeDisabled();
    }
});

test('the verifying state renders while the sha runs; done refreshes the model picker', async ({ page }) => {
    const rolling = fx('catalog_downloading');
    const verifying = { ...rolling, download: { ...rolling.download, state: 'verifying', got: rolling.download.total } };
    const done = fx('catalog_done');
    done.download = { state: 'done', name: rolling.download.name, got: rolling.download.total, total: rolling.download.total, error: '' };
    const baseCfg = fx('config');
    const grownCfg = JSON.parse(JSON.stringify(baseCfg));
    grownCfg.status.model_files = [...grownCfg.status.model_files,
        { name: 'freshly-downloaded.gguf', size: 3106738272, kind: 'gguf', path: 'D:/models/freshly-downloaded.gguf' }];
    await openControl(page, {
        // bootstrap + the setup-flip re-render both land before the 2 s poller advances
        catalog: poll => (poll <= 1 ? verifying : done),
        config: poll => (poll === 0 ? baseCfg : grownCfg),
    });

    await expect(page.locator('#cat-body')).toContainText('verifying sha256');
    // the poller advances to done, which re-pulls /config so the new file joins the picker
    await expect(page.locator('#model-files option[value="D:/models/freshly-downloaded.gguf"]')).toHaveCount(1, { timeout: 10_000 });
});

test('a refused download start surfaces the server message', async ({ page }) => {
    await openControl(page, {
        stats: fx('stats_setup'),
        catalog: fx('catalog_idle'),
        responses: { '/catalog/download': { status: 409, json: fx('catalog_refusal') } },
    });
    await page.locator('#cat-body button', { hasText: 'download' }).first().click();
    await expect(page.locator('#cat-note')).toContainText('a download is already running');
});

test('a failed download reports its error in the note', async ({ page }) => {
    const doc = fx('catalog_idle');
    const failed = { ...doc, download: { state: 'failed', name: 'gemma-4-e4b', got: 0, total: 0, error: 'sha256 MISMATCH - deleted' } };
    await openControl(page, { stats: fx('stats_setup'), catalog: failed });
    await expect(page.locator('#cat-note')).toContainText('download failed: sha256 MISMATCH');
});

test('serve-this-model writes the downloaded path as the roster default, then restarts', async ({ page }) => {
    const doc = fx('catalog_idle');
    const present = doc.entries.find(e => e.present);
    const { posts } = await openControl(page, { stats: fx('stats_setup'), catalog: doc });

    // several entries can be present - serve the one this test asserts on
    const row = page.locator('#cat-body tr', { hasText: present.display });
    await row.locator('button', { hasText: 'serve this model' }).click();
    await expect(page.locator('#badge')).toHaveText(/restarting/);

    const cfg = lastJson(posts.filter(p => p.path === '/config'));
    expect(cfg.models).toEqual([{ path: present.path, default: true }]);
    expect(posts.some(p => p.path === '/restart')).toBe(true);
});

test('an unreachable /catalog says so instead of a stuck spinner', async ({ page }) => {
    await openControl(page, { catalog: { status: 500, json: {} } });
    await expect(page.locator('#cat-note')).toHaveText('could not load /catalog');
});

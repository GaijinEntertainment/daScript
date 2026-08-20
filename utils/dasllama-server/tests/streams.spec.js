// §03 streams + §04 prefix cache: live cards per state, the history toggle,
// finished-stream migration, and cache-table lifecycle (rows -> drained note).

const { test, expect, fx, openControl } = require('./fixtures');

test('idle: the empty note shows and no cards render', async ({ page }) => {
    await openControl(page);
    await expect(page.locator('#cards-empty')).toBeVisible();
    await expect(page.locator('#cards .scard')).toHaveCount(0);
});

test('active streams render cards with per-state meta', async ({ page }) => {
    const streamsDoc = fx('streams_active');
    const dec = streamsDoc.streams.find(stream => stream.state === 'decoding');
    await openControl(page, { streams: streamsDoc });

    await expect(page.locator('#cards-empty')).toBeHidden();
    const card = page.locator('#sc-' + dec.id);
    await expect(card).toContainText('#' + dec.id);
    await expect(card).toContainText('decoding');
    await expect(card).toContainText(`${dec.n_gen}/${dec.max_new} tok`);
    await expect(card).toContainText(dec.prompt_head.slice(0, 24));
});

test('a finished stream moves to history and its card leaves the live view', async ({ page }) => {
    const active = fx('streams_active');
    const id = active.streams[0].id;
    const finished = {
        ...active,
        streams: active.streams.map(stream => (stream.id === id ? { ...stream, state: 'finished', finish: 'stop' } : stream)),
    };
    await openControl(page, { streams: poll => (poll === 0 ? active : finished) });

    await expect(page.locator('#sc-' + id)).toHaveCount(0, { timeout: 10_000 });
    await page.locator('#b-history').click();
    await expect(page.locator('#history')).toBeVisible();
    await expect(page.locator('#cards')).toBeHidden();
    await expect(page.locator('#history .scard').first()).toContainText('finished · stop');
    await page.locator('#b-live').click();
    await expect(page.locator('#cards')).toBeVisible();
});

test('prefix-cache chains render rows; a drained cache says so instead of freezing', async ({ page }) => {
    const withCache = fx('streams_finished');
    const entry = withCache.cache[0];
    const drained = { ...withCache, cache: [] };
    await openControl(page, { streams: poll => (poll < 3 ? withCache : drained) });

    const row = page.locator('#ctable-body tr').first();
    await expect(row).toContainText(entry.preview.slice(0, 16));
    await expect(row).toContainText(String(entry.pages));
    await expect(page.locator('#ctable-body')).toContainText('cache drained', { timeout: 10_000 });
});

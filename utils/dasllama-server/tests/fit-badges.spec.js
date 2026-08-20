// §03 fit badges: the catalog's per-entry box-fit verdict from /catalog's `box` facts —
// green fits (RAM with OS headroom, or the GPU tier's weight budget), amber tight, red too
// big; no facts (ram_gb 0) means no badge at all.

const { test, expect, fx, openControl } = require('./fixtures');

function withBox(ramGb, vramMb) {
    const d = JSON.parse(JSON.stringify(fx('catalog_idle')));
    d.box = { ram_gb: ramGb, vram_mb: vramMb || 0 };
    return d;
}

const needsCell = (page, display) => page.locator('#cat-body tr', { hasText: display }).locator('td').nth(4);

test('a roomy box fits everything green', async ({ page }) => {
    await openControl(page, { catalog: withBox(64) });
    await expect(needsCell(page, 'Gemma 4 E2B').locator('.chip')).toHaveText('fits');
    await expect(needsCell(page, 'Qwen 3.6 35B-A3B').locator('.chip')).toHaveText('fits');
});

test('a 16 GB box marks the big models red and the small ones green', async ({ page }) => {
    await openControl(page, { catalog: withBox(16) });
    await expect(needsCell(page, 'Gemma 4 E2B').locator('.chip')).toHaveText('fits');
    await expect(needsCell(page, 'Gemma 4 26B-A4B').locator('.chip')).toHaveText('too big');
    await expect(needsCell(page, 'Gemma 4 26B-A4B').locator('.chip')).toHaveClass(/bad/);
});

test('a working set inside RAM but past the OS headroom reads tight', async ({ page }) => {
    await openControl(page, { catalog: withBox(20) });
    const chip = needsCell(page, 'Gemma 4 26B-A4B').locator('.chip');
    await expect(chip).toHaveText('tight');
    await expect(chip).not.toHaveClass(/good|bad/);
});

test('a GPU weight budget that covers the working set reads fits-gpu', async ({ page }) => {
    await openControl(page, { catalog: withBox(16, 20480) });
    const chip = needsCell(page, 'Gemma 4 26B-A4B').locator('.chip');
    await expect(chip).toHaveText('fits gpu');
    await expect(chip).toHaveClass(/good/);
});

test('no box facts means no badge', async ({ page }) => {
    await openControl(page, { catalog: withBox(0) });
    await expect(needsCell(page, 'Gemma 4 E2B').locator('.chip')).toHaveCount(0);
});

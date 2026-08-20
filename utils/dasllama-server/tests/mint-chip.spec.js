// The header's sidecar-mint chip (scenario: "is this box tuned, and is its mint on the
// ladder?"): red = bad mint (absent / stale / version-lapsed / noisy race), amber = good
// mint not yet shared, green = tuned and shared or exchange-sourced. Click lands on §12.

const { test, expect, fx, openControl } = require('./fixtures');

function ex(mut) {
    const e = JSON.parse(JSON.stringify(fx('exchange')));
    Object.assign(e.current, mut || {});
    return e;
}

test('a good local mint that is not on the ladder reads amber not-shared', async ({ page }) => {
    await openControl(page);   // the stock fixture: local, noise ok, shared false
    const chip = page.locator('#mint-chip');
    await expect(chip).toHaveText('tuned - not shared');
    await expect(chip).not.toHaveClass(/good|bad/);
});

test('a shared local mint reads green tuned', async ({ page }) => {
    await openControl(page, { exchange: ex({ shared: true }) });
    const chip = page.locator('#mint-chip');
    await expect(chip).toHaveText('tuned');
    await expect(chip).toHaveClass(/good/);
});

test('an exchange-sourced sidecar reads green tuned', async ({ page }) => {
    await openControl(page, { exchange: ex({ origin: 'exchange', exchange_sha: 'abc123def456', local: false }) });
    const chip = page.locator('#mint-chip');
    await expect(chip).toHaveText('tuned');
    await expect(chip).toHaveClass(/good/);
});

test('a noisy race reads red noisy-mint', async ({ page }) => {
    await openControl(page, { exchange: ex({ noise: 'noisy' }) });
    const chip = page.locator('#mint-chip');
    await expect(chip).toHaveText('noisy mint');
    await expect(chip).toHaveClass(/bad/);
});

test('no sidecar reads red untuned', async ({ page }) => {
    await openControl(page, { exchange: ex({ present: false }) });
    const chip = page.locator('#mint-chip');
    await expect(chip).toHaveText('untuned');
    await expect(chip).toHaveClass(/bad/);
});

test('a stale sidecar reads red stale', async ({ page }) => {
    await openControl(page, { exchange: ex({ stale: true }) });
    await expect(page.locator('#mint-chip')).toHaveText('tune: stale');
    await expect(page.locator('#mint-chip')).toHaveClass(/bad/);
});

test('an engine-version lapse reads red re-tune due', async ({ page }) => {
    await openControl(page, { exchange: ex({ version_ok: false, dasllama_version: 4 }) });
    await expect(page.locator('#mint-chip')).toHaveText('tune: re-tune due');
    await expect(page.locator('#mint-chip')).toHaveClass(/bad/);
});

test('clicking the chip lands on the exchange section', async ({ page }) => {
    await openControl(page);
    await page.locator('#mint-chip').click();
    await expect(page.locator('#exchange')).toBeInViewport();
});

test('an unreachable /exchange hides the chip', async ({ page }) => {
    await openControl(page, { exchange: { status: 500, json: {} } });
    await expect(page.locator('#mint-chip')).toBeHidden();
});

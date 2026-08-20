// §09 controls + §10 benchmark + the restart guard: destructive actions are
// confirm-gated, a dismissed confirm fires nothing, and a restart with dirty
// edits saves first (a failed save must abort the restart).

const { test, expect, fx, openControl } = require('./fixtures');

test('gc posts and reports; the button re-arms', async ({ page }) => {
    const { posts } = await openControl(page);
    await page.locator('#b-gc').click();
    await expect(page.locator('#ctl-note')).toContainText('gc scheduled');
    expect(posts.some(p => p.path === '/gc')).toBe(true);
    await expect(page.locator('#b-gc')).toBeEnabled({ timeout: 5_000 });
});

test('drain asks first — dismissing the confirm fires nothing', async ({ page }) => {
    const { posts } = await openControl(page);
    page.once('dialog', async d => { await d.dismiss(); });
    await page.locator('#b-drain').click();
    await page.waitForTimeout(300);
    expect(posts.some(p => p.path === '/shutdown')).toBe(false);
    await expect(page.locator('#b-drain')).toBeEnabled();
});

test('confirmed drain posts /shutdown and disables the button', async ({ page }) => {
    const { posts } = await openControl(page);
    page.once('dialog', async d => { await d.accept(); });
    await page.locator('#b-drain').click();
    await expect(page.locator('#ctl-note')).toContainText('draining');
    expect(posts.some(p => p.path === '/shutdown')).toBe(true);
    await expect(page.locator('#b-drain')).toBeDisabled();
});

test('restart with dirty edits saves first; a failed save aborts the restart', async ({ page }) => {
    const { posts } = await openControl(page, {
        responses: { '/config': { status: 400, json: { error: { message: 'ctx must be positive' } } } },
    });
    await page.locator('#f-ctx').fill('-1');
    page.once('dialog', async d => {
        expect(d.message()).toContain('Save the edited config');
        await d.accept();
    });
    await page.locator('#b-restart').click();
    await expect(page.locator('#cfg-note')).toContainText('save failed: ctx must be positive');
    expect(posts.some(p => p.path === '/restart')).toBe(false);
    await expect(page.locator('#b-restart')).toBeEnabled();
});

test('confirmed clean restart posts /restart and flips the badge to restarting', async ({ page }) => {
    const { posts } = await openControl(page);
    page.once('dialog', async d => { await d.accept(); });
    await page.locator('#b-restart').click();
    await expect(page.locator('#badge')).toHaveText(/restarting/);
    await expect(page.locator('#badge')).toHaveClass(/warn/);
    expect(posts.some(p => p.path === '/restart')).toBe(true);
});

test('a failed benchmark shows its log and the failure note', async ({ page }) => {
    // an in-tree server can only ever fail here (lcpp_bench refuses -jit script
    // runs); a result-carrying bench_done fixture needs a deployed-bundle capture
    const b = fx('bench_failed');
    await openControl(page, { bench: b });
    await expect(page.locator('#bench-note')).toHaveText('failed — see the log above');
    await expect(page.locator('#bench-log')).toBeVisible();
    await expect(page.locator('#bench-log')).toContainText('bench failed');
    await expect(page.locator('#bench-table')).toBeHidden();
    await expect(page.locator('#bench-record-row')).toBeHidden();
    await expect(page.locator('#b-bench')).toBeEnabled();
});

test('a refused bench start surfaces the reason', async ({ page }) => {
    await openControl(page, {
        responses: { '/bench': { status: 409, json: { error: { message: 'streams are active — quiesce first' } } } },
    });
    await page.locator('#b-bench').click();
    await expect(page.locator('#bench-note')).toHaveText('streams are active — quiesce first');
});

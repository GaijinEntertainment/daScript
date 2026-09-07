// §11 controls + §12 benchmark + the restart guard: destructive actions are
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
    await page.locator('#en-ctx').check();   // ctx is default-gated — tick its override to edit it
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
    // the A/B mode's failure: an in-tree server with lcpp_bin set always fails its child run
    // (lcpp_bench refuses -jit script runs), which is what this fixture pins
    const b = fx('bench_failed');
    await openControl(page, { bench: b });
    await expect(page.locator('#bench-note')).toHaveText('failed — see the log above');
    await expect(page.locator('#bench-log')).toBeVisible();
    await expect(page.locator('#bench-log')).toContainText(b.log[b.log.length - 1]);
    await expect(page.locator('#bench-table')).toBeHidden();
    await expect(page.locator('#bench-record-row')).toBeHidden();
    await expect(page.locator('#b-bench')).toBeEnabled();
});

test('an A/B-mode server relabels the button for the llama.cpp race', async ({ page }) => {
    const b = fx('bench_failed');   // captured in the A/B mode
    expect(b.mode).toBe('ab');
    await openControl(page, { bench: b });
    await expect(page.locator('#b-bench')).toHaveText('run llama.cpp A/B');
});

test('a run in flight disables the button and clears the previous numbers', async ({ page }) => {
    const b = { ...fx('bench_done'), state: 'running' };   // the done capture, mid-run: no result yet
    delete b.result;
    await openControl(page, { bench: b });
    await expect(page.locator('#bench-note')).toContainText('running — one rep per tick');
    await expect(page.locator('#b-bench')).toBeDisabled();
    await expect(page.locator('#bench-table')).toBeHidden();
    await expect(page.locator('#bench-meta')).toHaveText('');
    await expect(page.locator('#bench-log')).toContainText(b.log[b.log.length - 1]);
});

test('an in-process result shows the served row, the comparison line, and no ratio', async ({ page }) => {
    // the bundle's mode: no daslang to spawn, no llama-bench - the server measured itself
    const b = fx('bench_done');
    await openControl(page, { bench: b });
    await expect(page.locator('#b-bench')).toHaveText('measure this box (pp512 / tg128)');
    await expect(page.locator('#bench-note')).toHaveText('done');
    await expect(page.locator('#bench-table')).toBeVisible();
    await expect(page.locator('#bench-body tr')).toHaveCount(1);
    await expect(page.locator('#bench-body')).toContainText(b.result.ours_pp.toFixed(1));
    await expect(page.locator('#bench-body')).toContainText(b.result.ours_tg.toFixed(1));
    await expect(page.locator('#bench-body')).not.toContainText('ratio');
    await expect(page.locator('#bench-meta')).toContainText(b.result.model);
    await expect(page.locator('#bench-meta')).toContainText('compare: ' + b.result.ref_cmd);
    await expect(page.locator('#bench-log')).toContainText(b.log[b.log.length - 1]);
    await expect(page.locator('#bench-record-row')).toBeHidden();   // no record: nothing to submit from a self-measure
});

test('the idle panel names the mode the server would run', async ({ page }) => {
    const b = fx('bench_idle');   // captured in the in-process mode
    await openControl(page, { bench: b });
    await expect(page.locator('#b-bench')).toHaveText(b.mode === 'inprocess' ? 'measure this box (pp512 / tg128)' : 'run llama.cpp A/B');
    await expect(page.locator('#bench-note')).toContainText('quiesced only');
});

test('a refused bench start surfaces the reason', async ({ page }) => {
    await openControl(page, {
        responses: { '/bench': { status: 409, json: { error: { message: 'streams are active — quiesce first' } } } },
    });
    await page.locator('#b-bench').click();
    await expect(page.locator('#bench-note')).toHaveText('streams are active — quiesce first');
});

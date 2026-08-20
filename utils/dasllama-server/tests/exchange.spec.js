// §11 sidecar exchange: the current-sidecar states the page must distinguish
// (none / local unshared / shared / exchange-sourced / version-stale), the
// matches table, and the always-share toggle's policy-only save.

const { test, expect, fx, openControl, lastJson } = require('./fixtures');

test('a local unshared sidecar shows its identity and offers the share button', async ({ page }) => {
    const ex = fx('exchange');
    // hard failure, not a skip: a regenerated exchange.json must stay the local-unshared
    // state (capture right after a local tune, before sharing) or this coverage vanishes
    expect(ex.current && ex.current.present && ex.current.local && !ex.current.shared,
        'exchange fixture must be the local-unshared state - recapture per fixtures/README.md').toBe(true);
    await openControl(page);

    const cur = page.locator('#ex-current');
    await expect(cur).toContainText('current: ' + ex.current.sha.slice(0, 12));
    await expect(cur).toContainText('tuned on this box');
    await expect(cur).toContainText('noise ' + ex.current.noise);
    await expect(cur).toContainText('not shared yet');
    await expect(page.locator('#b-ex-submit')).toBeVisible();
});

test('no sidecar yet: the first-start hint shows and share is hidden', async ({ page }) => {
    const ex = { ...fx('exchange'), current: { present: false } };
    await openControl(page, { exchange: ex });
    await expect(page.locator('#ex-current')).toContainText('no tune sidecar on this box yet');
    await expect(page.locator('#b-ex-submit')).toBeHidden();
});

test('a shared sidecar hides the share button', async ({ page }) => {
    const ex = fx('exchange');
    ex.current = { ...ex.current, shared: true };
    await openControl(page, { exchange: ex });
    await expect(page.locator('#ex-current')).toContainText('shared with the exchange');
    await expect(page.locator('#b-ex-submit')).toBeHidden();
});

test('an engine-version mismatch flags the re-tune', async ({ page }) => {
    const ex = fx('exchange');
    ex.current = { ...ex.current, version_ok: false, dasllama_version: 3 };
    ex.engine_version = 4;
    await openControl(page, { exchange: ex });
    await expect(page.locator('#ex-current')).toContainText('this build is v4, re-tune due');
});

test('check-the-exchange renders matches with apply buttons', async ({ page }) => {
    const doc = fx('exchange_matches');
    await openControl(page, { matches: doc });
    await page.locator('#b-ex-check').click();

    await expect(page.locator('#ex-note')).toContainText(doc.hits.length + ' match(es)');
    const row = page.locator('#ex-matches-body tr').first();
    await expect(row).toContainText(doc.hits[0].tier);
    await expect(row).toContainText(doc.hits[0].verified ? 'verified' : 'unverified');
    await expect(row.locator('button')).toContainText('apply ' + doc.hits[0].sha.slice(0, 12));
});

test('an unreachable exchange says so and shows no table', async ({ page }) => {
    await openControl(page, { matches: fx('exchange_matches_unreachable') });
    await page.locator('#b-ex-check').click();
    await expect(page.locator('#ex-note')).toHaveText('the exchange is unreachable');
    await expect(page.locator('#ex-matches')).toBeHidden();
});

test('the always-share toggle saves the policy on top of the LAST SAVED config', async ({ page }) => {
    const { posts } = await openControl(page);
    // dirty the form first — the toggle must NOT sweep this unsaved edit along
    await page.locator('#f-streams').fill('99');
    await page.locator('#ex-always').check();
    await expect(page.locator('#ex-note')).toContainText('shared automatically');

    const body = lastJson(posts.filter(p => p.path === '/config'));
    expect(body.exchange_submit).toBe('always');
    expect(body.streams).not.toBe(99);
});

test('the always-share toggle works even when /exchange loads slowly', async ({ page }) => {
    const { posts } = await openControl(page, { exchange: { __delay: 1200, json: fx('exchange') } });
    await page.locator('#ex-always').check();
    await expect(page.locator('#ex-note')).toContainText('shared automatically');
    expect(lastJson(posts.filter(p => p.path === '/config')).exchange_submit).toBe('always');
});

test('a failed policy save rolls the toggle back', async ({ page }) => {
    await openControl(page, {
        responses: { '/config': { status: 500, json: { error: { message: 'disk full' } } } },
    });
    // click, not check(): the rollback can land before check()'s post-click state verification
    await page.locator('#ex-always').click();
    await expect(page.locator('#ex-note')).toContainText('could not save');
    await expect(page.locator('#ex-always')).not.toBeChecked();
});

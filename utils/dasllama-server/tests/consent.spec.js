// §12 consent banner: the GDPR first-contact gate. It renders only on the zero-config path
// of a consent-capable server (consent_notice present in /exchange), records the choice via
// POST /exchange/consent, and never renders for an explicitly configured policy — an
// exchange_* key IS the expressed choice. The captured fixture is the zero-config never-asked
// state, so the banner cases are fixture-first; only consent/configured are varied.

const { test, expect, fx, openControl, lastJson } = require('./fixtures');

function consentEx(consent, configured) {
    const ex = fx('exchange');
    return { ...ex, consent, policy: { ...ex.policy, configured } };
}

test('no recorded choice on a zero-config policy shows the banner', async ({ page }) => {
    const ex = fx('exchange');
    // hard failure, not a skip: a regenerated exchange.json must stay the zero-config
    // never-asked state (capture with no .consent file and no exchange_* config)
    expect(ex.consent === '' && ex.policy.configured === false && !!ex.consent_notice,
        'exchange fixture must be the zero-config never-asked state - recapture per fixtures/README.md').toBe(true);
    await openControl(page);
    await expect(page.locator('#ex-consent')).toBeVisible();
    await expect(page.locator('#ex-consent-text')).toContainText(ex.consent_notice.slice(0, 40));
    await expect(page.locator('#b-consent-accept')).toBeVisible();
    await expect(page.locator('#b-consent-decline')).toBeVisible();
    await expect(page.locator('#ex-current')).toContainText('consent not asked yet');
});

test('accept POSTs the choice and reports the lookup restart', async ({ page }) => {
    const { posts } = await openControl(page, {
        responses: { '/exchange/consent': { status: 200, json: { ok: true, accepted: true, restarting: true } } },
    });
    await page.locator('#b-consent-accept').click();
    await expect(page.locator('#ex-note')).toContainText('restarting');
    const consentPosts = posts.filter(p => p.path === '/exchange/consent');
    expect(consentPosts.length).toBe(1);
    expect(lastJson(consentPosts)).toEqual({ accept: true });
});

test('accept on a tuned box reports enabled without a restart', async ({ page }) => {
    await openControl(page, {
        responses: { '/exchange/consent': { status: 200, json: { ok: true, accepted: true } } },
    });
    await page.locator('#b-consent-accept').click();
    await expect(page.locator('#ex-note')).toContainText('exchange enabled');
});

test('decline POSTs accept:false and says the box tunes locally', async ({ page }) => {
    const { posts } = await openControl(page, {
        responses: { '/exchange/consent': { status: 200, json: { ok: true, accepted: false } } },
    });
    await page.locator('#b-consent-decline').click();
    await expect(page.locator('#ex-note')).toContainText('tunes locally');
    expect(lastJson(posts.filter(p => p.path === '/exchange/consent'))).toEqual({ accept: false });
});

test('a declined record keeps the banner as the change-your-mind path', async ({ page }) => {
    await openControl(page, { exchange: consentEx('declined', false) });
    await expect(page.locator('#ex-consent')).toBeVisible();
    await expect(page.locator('#ex-consent-text')).toContainText('currently declined');
    await expect(page.locator('#ex-current')).toContainText('consent declined');
});

test('an accepted record hides the banner', async ({ page }) => {
    await openControl(page, { exchange: consentEx('accepted', false) });
    await expect(page.locator('#ex-consent')).toBeHidden();
    await expect(page.locator('#ex-current')).toContainText('consent accepted');
});

test('an explicitly configured policy never shows the banner', async ({ page }) => {
    await openControl(page, { exchange: consentEx('', true) });
    await expect(page.locator('#ex-consent')).toBeHidden();
    await expect(page.locator('#ex-current')).toContainText('consent configured');
});

test('a pre-consent server (no consent_notice) renders no banner', async ({ page }) => {
    const ex = consentEx('', false);
    delete ex.consent_notice;
    await openControl(page, { exchange: ex });
    await expect(page.locator('#ex-consent')).toBeHidden();
});

test('a failed consent POST surfaces the error and re-enables the buttons', async ({ page }) => {
    await openControl(page, {
        responses: { '/exchange/consent': { status: 500, json: { error: { message: 'disk full' } } } },
    });
    await page.locator('#b-consent-accept').click();
    await expect(page.locator('#ex-note')).toContainText('consent: disk full');
    await expect(page.locator('#b-consent-accept')).toBeEnabled();
    await expect(page.locator('#b-consent-decline')).toBeEnabled();
});

test('opting into always-share also records consent', async ({ page }) => {
    // the toggle saves an AUTHORITATIVE toml (not consent by itself) — the deliberate
    // exchange engagement it expresses must land in the consent file too
    const { posts } = await openControl(page, {
        responses: {
            '/config': { status: 200, json: { ok: true } },
            '/exchange/consent': { status: 200, json: { ok: true, accepted: true } },
        },
    });
    await page.locator('#ex-always').check();
    await expect
        .poll(() => posts.filter(p => p.path === '/exchange/consent').length, { timeout: 5000 })
        .toBe(1);
    expect(lastJson(posts.filter(p => p.path === '/exchange/consent'))).toEqual({ accept: true });
});

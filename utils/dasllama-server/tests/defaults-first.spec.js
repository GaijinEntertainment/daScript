// §09 defaults-first config editor: every flat knob is an override gated by its own
// checkbox — unchecked keys are OMITTED from POST /config so engine defaults stay live
// (ctx = model window, backend = auto-detected), and the advanced fold hides the long tail.

const { test, expect, fx, openControl, lastJson } = require('./fixtures');

// a surface whose only explicit key is the primary port — the fresh-install shape
function leanConfig() {
    const c = JSON.parse(JSON.stringify(fx('config')));
    for (const k of Object.keys(c.surface.sources)) {
        if (k !== 'port' && k !== 'models') c.surface.sources[k] = 'default';
    }
    return c;
}

test('unchecked knobs are omitted from the save; checked ones ride along', async ({ page }) => {
    const { posts } = await openControl(page);

    // quant is default-sourced -> unchecked -> disabled and NOT in the body
    await expect(page.locator('#en-quant')).not.toBeChecked();
    await expect(page.locator('#f-quant')).toBeDisabled();
    // streams is toml-sourced -> pre-checked and editable
    await expect(page.locator('#en-streams')).toBeChecked();
    await page.locator('#b-save').click();
    await expect(page.locator('#cfg-note')).toHaveText('saved — restart to apply');

    const body = lastJson(posts.filter(p => p.path === '/config'));
    expect(body.quant).toBeUndefined();
    expect(body.ctx).toBeUndefined();
    expect(body.gpu).toBeUndefined();
    expect(body.streams).toBe(4);
});

test('ticking an override enables the input and lands the key in the save', async ({ page }) => {
    const { posts } = await openControl(page);

    await page.locator('#en-quant').check();
    await expect(page.locator('#f-quant')).toBeEnabled();
    await page.locator('#f-quant').selectOption('fp32');
    await page.locator('#b-save').click();
    await expect(page.locator('#cfg-note')).toHaveText('saved — restart to apply');

    const body = lastJson(posts.filter(p => p.path === '/config'));
    expect(body.quant).toBe('fp32');
});

test('ctx reads as the model window; the defaults note names the detected backend', async ({ page }) => {
    await openControl(page);
    await expect(page.locator('#f-ctx')).toHaveAttribute('placeholder', "model's full window");
    await expect(page.locator('#f-ctx')).toHaveValue('');
    await expect(page.locator('#cfg-defaults-note')).toContainText('auto (vulkan detected)');
});

test('a lean config keeps the advanced fold closed; the toggle opens it', async ({ page }) => {
    await openControl(page, { config: leanConfig() });

    await expect(page.locator('#f-port')).toBeVisible();
    await expect(page.locator('#cfg-adv')).toBeHidden();
    await page.locator('#b-adv').click();
    await expect(page.locator('#cfg-adv')).toBeVisible();
    await expect(page.locator('#f-quant')).toBeVisible();
});

test('saved advanced overrides force the fold open and are counted on the toggle', async ({ page }) => {
    await openControl(page);   // the stock fixture carries toml-sourced advanced keys
    await expect(page.locator('#cfg-adv')).toBeVisible();
    await expect(page.locator('#b-adv')).toContainText('set)');
});

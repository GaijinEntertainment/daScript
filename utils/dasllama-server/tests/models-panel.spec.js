// §02 models: one card per served slot, activate wiring, and the VRAM bar.
// Fixture: stats_multi carries two slots (smol active + default, tiny idle cpu).

const { test, expect, fx, openControl, lastJson } = require('./fixtures');

test('two slots render as cards — active class, default tag, activate on the idle one', async ({ page }) => {
    const s = fx('stats_multi');
    const [act, idle] = [s.models.find(m => m.is_active), s.models.find(m => !m.is_active)];
    await openControl(page, { stats: s });

    const actCard = page.locator('#mc-' + act.name);
    const idleCard = page.locator('#mc-' + idle.name);
    await expect(actCard).toHaveClass(/active/);
    await expect(actCard).toContainText('active');
    await expect(actCard.locator('button[data-activate]')).toHaveCount(0);
    await expect(idleCard).not.toHaveClass(/active/);
    await expect(idleCard.locator('button[data-activate]')).toBeVisible();
    if (act.is_default) await expect(actCard.locator('.def')).toHaveText('default');
    await expect(idleCard).toContainText('backend ' + idle.backend);
});

test('a single slot offers no activate button', async ({ page }) => {
    const s = fx('stats');
    await openControl(page, { stats: s });
    await expect(page.locator('#mc-' + s.models[0].name)).toBeVisible();
    await expect(page.locator('button[data-activate]')).toHaveCount(0);
});

test('activate POSTs the slot name and reports the switch', async ({ page }) => {
    const s = fx('stats_multi');
    const idle = s.models.find(m => !m.is_active);
    const { posts } = await openControl(page, {
        stats: s,
        responses: { '/v1/models/activate': { json: fx('activate_response') } },
    });

    await page.locator('button[data-activate]').click();
    await expect(page.locator('#m-note')).toContainText('active · switch');
    expect(lastJson(posts.filter(p => p.path === '/v1/models/activate'))).toEqual({ model: idle.name });
});

test('a failed activate surfaces the server error message', async ({ page }) => {
    const s = fx('stats_multi');
    await openControl(page, {
        stats: s,
        responses: { '/v1/models/activate': { status: 409, json: { error: { message: 'switch refused: work is live' } } } },
    });
    await page.locator('button[data-activate]').click();
    await expect(page.locator('#m-note')).toHaveText('switch refused: work is live');
});

test('a slot names the towers it serves; a text-only slot wears no tower chip', async ({ page }) => {
    const s = fx('stats_multi');
    s.models = s.models.map((m, i) => (i === 0 ? { ...m, vision: true, audio: true } : { ...m, vision: false, audio: false }));
    await openControl(page, { stats: s });
    await expect(page.locator('#mc-' + s.models[0].name + ' .chip.tower')).toHaveText('vision + audio');
    await expect(page.locator('#mc-' + s.models[1].name + ' .chip.tower')).toHaveCount(0);
});

test('a GPU budget reveals the VRAM bar with the owner and fill', async ({ page }) => {
    const s = { ...fx('stats_multi'), gpu_budget_bytes: 8e9, gpu_vram_bytes: 2e9 };
    s.models = s.models.map((m, i) => (i === 0 ? { ...m, holds_gpu: true } : m));
    await openControl(page, { stats: s });

    await expect(page.locator('#vram-panel')).toBeVisible();
    await expect(page.locator('#vram-now')).toContainText(s.models[0].name);
    await expect(page.locator('#vram-now')).toContainText('2.00 / 8.00 GB');
    await expect(page.locator('#vram-fill')).toHaveCSS('width', /[1-9]/);
});

test('no GPU budget keeps the VRAM panel hidden', async ({ page }) => {
    await openControl(page, { stats: { ...fx('stats'), gpu_budget_bytes: 0 } });
    await expect(page.locator('#vram-panel')).toBeHidden();
});

test('every card says how its slot is served, in the words the server sent', async ({ page }) => {
    const s = fx('stats_multi');
    await openControl(page, { stats: s });
    for (const m of s.models) {
        await expect(page.locator('#mc-' + m.name + ' [data-served]')).toHaveText('served · ' + m.served);
        await expect(page.locator('#mc-' + m.name + ' [data-served-note]')).toHaveCount(0);
        await expect(page.locator('#mc-' + m.name + ' [data-cpu-passes]')).toHaveCount(0);
    }
});

test('a slot held back from the GPU prints the reason under its served line', async ({ page }) => {
    const s = fx('stats_multi');
    const note = 'not enough video memory for the whole model: lower ctx';
    s.models = s.models.map(m => (m.is_active ? { ...m, served_note: note } : m));
    await openControl(page, { stats: s });
    const act = s.models.find(m => m.is_active);
    await expect(page.locator('#mc-' + act.name + ' [data-served-note]')).toHaveText(note);
    await expect(page.locator('.mcard [data-served-note]')).toHaveCount(1);
});

test('calls the GPU handed back to the CPU are counted on the card of the slot that holds the GPU', async ({ page }) => {
    const s = fx('stats_cpu_pass');
    const first = s.gpu_cpu_passes[0];
    s.gpu_cpu_passes = [first, { ...first, reason: first.reason + '-again', count: first.count + 2 }];
    const owner = s.models.find(m => m.holds_gpu);
    await openControl(page, { stats: s });
    const line = page.locator('#mc-' + owner.name + ' [data-cpu-passes]');
    await expect(line).toContainText(s.gpu_cpu_passes.reduce((n, p) => n + p.count, 0) + ' calls ran on the CPU instead of the GPU');
    for (const p of s.gpu_cpu_passes) {
        await expect(line.locator('span[title="' + p.reason + '"]')).toHaveText(p.words + ' (' + p.count + ')');
    }
    await expect(page.locator('.mcard [data-cpu-passes]')).toHaveCount(1);
});

test('one call on the CPU reads in the singular, in the words the server sent', async ({ page }) => {
    const s = fx('stats_cpu_pass');
    const owner = s.models.find(m => m.holds_gpu);
    await openControl(page, { stats: s });
    const total = s.gpu_cpu_passes.reduce((n, p) => n + p.count, 0);
    await expect(page.locator('#mc-' + owner.name + ' [data-cpu-passes]'))
        .toContainText(total + (total > 1 ? ' calls' : ' call') + ' ran on the CPU instead of the GPU: ' + s.gpu_cpu_passes[0].words);
});

test('a stats document from before the served fields draws the card without them', async ({ page }) => {
    const s = fx('stats_multi');
    s.models = s.models.map(({ served, served_note, device_kv, ...m }) => m);
    delete s.gpu_cpu_passes;
    await openControl(page, { stats: s });
    await expect(page.locator('#mc-' + s.models[0].name)).toBeVisible();
    await expect(page.locator('.mcard [data-served]')).toHaveCount(0);
});

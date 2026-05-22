// Multi-file state survives a page reload via localStorage autosave.

const { test, expect } = require('./fixtures.js');

async function waitTabsReady(page) {
    await page.waitForFunction(() => !!window.pgState, null, { timeout: 10_000 });
    await page.locator('#pg-tabs .pg-tab[data-file="main.das"]').waitFor();
}

test('three-file state survives a reload', async ({ playground }) => {
    await waitTabsReady(playground);

    // Build state: main.das + utils.das + types.das with distinctive content.
    await playground.evaluate(() => {
        window.pgSwitchFile('main.das');
        window.code.getDoc().setValue('// main marker\nrequire utils\n');
        window.pgAddFile('utils.das');
        window.code.getDoc().setValue('// utils marker\n');
        window.pgAddFile('types.das');
        window.code.getDoc().setValue('// types marker\n');
        window.pgSwitchFile('main.das');
    });

    // Wait until autosave has actually persisted both the file set AND the
    // active-tab pointer. A fixed-150ms-margin `waitForTimeout(400)` over a
    // 250ms debounce is enough on localhost but flakes on CI under load —
    // poll the localStorage payload directly instead, so we proceed the
    // moment the writer fires (and don't proceed before it).
    await expect.poll(
        () => playground.evaluate(() => {
            const raw = localStorage.getItem('daslang.playground.state.v1');
            if (!raw) return null;
            try {
                const j = JSON.parse(raw);
                return {
                    files: Object.keys(j.files || {}).sort().join(','),
                    active: j.active,
                };
            } catch (e) { return null; }
        }),
        { timeout: 5_000 }
    ).toEqual({ files: 'main.das,types.das,utils.das', active: 'main.das' });

    await playground.reload();
    await waitTabsReady(playground);

    expect(await playground.evaluate(() => Object.keys(window.pgState.files))).toEqual([
        'main.das', 'utils.das', 'types.das',
    ]);
    expect(await playground.evaluate(() => window.pgState.active)).toBe('main.das');

    const mainText = await playground.evaluate(() => window.pgState.files['main.das'].getValue());
    expect(mainText).toContain('main marker');
    const utilsText = await playground.evaluate(() => window.pgState.files['utils.das'].getValue());
    expect(utilsText).toContain('utils marker');
    const typesText = await playground.evaluate(() => window.pgState.files['types.das'].getValue());
    expect(typesText).toContain('types marker');
});

test('autosave does not override URL #code hash on reload', async ({ playground }) => {
    await waitTabsReady(playground);

    // Plant autosave state.
    await playground.evaluate(() => {
        window.pgAddFile('utils.das');
        window.code.getDoc().setValue('autosaved');
    });
    await playground.waitForTimeout(400);

    // Force a fresh page load: same path + just-a-new-hash fires hashchange
    // without a reload (so pgInit wouldn't re-run). A cache-buster query
    // forces the load.
    await playground.goto(
        '/playground/?fresh=1#code=' + encodeURIComponent('// shared via hash\n')
    );
    await waitTabsReady(playground);

    const files = await playground.evaluate(() => Object.keys(window.pgState.files));
    expect(files).toEqual(['main.das']);
    const mainText = await playground.evaluate(() => window.pgState.files['main.das'].getValue());
    expect(mainText).toContain('shared via hash');
});

test('autosave does not override ?example= deep-link', async ({ playground }) => {
    // Same intent-signal rule as #code=: a visitor following the daslang.io
    // § 01 "try sha256 on the playground" deep-link should land on sha256,
    // even if they've used the playground before and have an autosaved buffer.
    await waitTabsReady(playground);

    await playground.evaluate(() => {
        window.code.getDoc().setValue('// stale autosave buffer\n');
    });
    await playground.waitForTimeout(400);

    await playground.goto('/playground/?example=sha256');
    await waitTabsReady(playground);

    // sha256.das ships a `// SHA-256` header comment — content match is
    // sturdier than asserting the dropdown label since selectSample resets
    // the <select> back to "Select example" after firing.
    //
    // Poll instead of a bare evaluate: waitTabsReady returns as soon as
    // pgState exists (initial empty doc), but main.js's `?example=` branch
    // still has to async-fetch data.json + sha256.das before pgLoadFiles
    // swaps the buffer content. Polling on the sha256 header marker is the
    // direct signal that the deep-link load actually landed.
    await expect.poll(
        () => playground.evaluate(() => window.pgState.files['main.das'].getValue()),
        { timeout: 5_000 }
    ).toMatch(/sha[- ]?256/i);
    const mainText = await playground.evaluate(() => window.pgState.files['main.das'].getValue());
    expect(mainText).not.toContain('stale autosave buffer');
});

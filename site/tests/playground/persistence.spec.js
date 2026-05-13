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

    // Wait for the debounced autosave (250ms) to actually fire.
    await playground.waitForTimeout(400);

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

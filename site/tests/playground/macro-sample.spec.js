// Multi-file macro sample: the "Macros (multi-file)" example ships two
// files. The loading half (two tabs appear, main.das is active) runs in
// any tier — even without WASM. The "click ▶ run and check the output"
// half is tagged @wasm so the no-WASM CI gate skips it.

const { test, expect } = require('./fixtures.js');

async function waitTabsReady(page) {
    await page.waitForFunction(() => !!window.pgState, null, { timeout: 10_000 });
    await page.locator('#pg-tabs .pg-tab[data-file="main.das"]').waitFor();
}

async function pickMacroSample(page) {
    // Wait for the dropdown to populate from data.json.
    await page.waitForFunction(
        () => Array.from(document.getElementById('examples').options)
            .some(o => o.text === 'Macros (multi-file)'),
        null,
        { timeout: 10_000 }
    );
    await page.locator('#examples').selectOption({ label: 'Macros (multi-file)' });
    // Wait until both files are present in pgState.
    await page.waitForFunction(
        () => window.pgState && 'main.das' in window.pgState.files
            && 'for_loop_macro_mod.das' in window.pgState.files,
        null,
        { timeout: 5_000 }
    );
}

test('Macros sample loads as two tabs', async ({ playground }) => {
    await waitTabsReady(playground);
    await pickMacroSample(playground);

    await expect(playground.locator('.pg-tab[data-file="main.das"]')).toBeVisible();
    await expect(playground.locator('.pg-tab[data-file="for_loop_macro_mod.das"]')).toBeVisible();
    // main.das should be the active buffer; the entry file flag.
    await expect(playground.locator('#pg-tabs .pg-tab.is-active')).toHaveAttribute(
        'data-file',
        'main.das',
    );
});

test('Macros sample main.das requires the macro module', async ({ playground }) => {
    await waitTabsReady(playground);
    await pickMacroSample(playground);

    const mainText = await playground.evaluate(() => window.pgState.files['main.das'].getValue());
    expect(mainText).toContain('require for_loop_macro_mod');
    // The macro syntax `for ((k, v) in tab)` is the headline feature.
    expect(mainText).toMatch(/for\s*\(\s*\(\s*k\s*,\s*v\s*\)\s+in\s+tab\s*\)/);
});

test('Macros sample runs successfully under WASM @wasm', async ({ playground }) => {
    await waitTabsReady(playground);
    await pickMacroSample(playground);
    // Wait for the WASM runtime to be ready.
    await playground.waitForFunction(
        () => typeof window.FS !== 'undefined' && typeof window.Module?.callMain === 'function',
        null,
        { timeout: 30_000 }
    );
    await playground.locator('#run').click();
    // Output panel should print the section-3 table-with-counter line.
    await expect(playground.locator('.output_line_text', { hasText: 'apple => 10' }))
        .toBeVisible({ timeout: 15_000 });
});

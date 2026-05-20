// Test button: write a file with [test] functions, hit ▶ test, check dastest
// output. Requires WASM (tagged @wasm so the no-WASM CI gate skips this file).

const { test, expect } = require('./fixtures.js');

async function waitTabsReady(page) {
    await page.waitForFunction(() => !!window.pgState, null, { timeout: 10_000 });
    await page.locator('#pg-tabs .pg-tab[data-file="main.das"]').waitFor();
}

async function waitWasmReady(page) {
    await page.waitForFunction(
        () => typeof window.FS !== 'undefined' && typeof window.Module?.callMain === 'function',
        null,
        { timeout: 30_000 }
    );
}

test('runs [test] functions and reports SUCCESS @wasm', async ({ playground }) => {
    await waitTabsReady(playground);
    await waitWasmReady(playground);

    await playground.evaluate(() => {
        window.pgSwitchFile('main.das');
        window.code.getDoc().setValue([
            'options gen2',
            'require dastest/testing_boost public',
            '',
            '[test]',
            'def test_math(t : T?) {',
            '    t |> equal(2 + 2, 4)',
            '}',
            '',
        ].join('\n'));
    });

    await playground.locator('#test').click();
    await expect(playground.locator('.output_line_text', { hasText: 'SUCCESS!' }))
        .toBeVisible({ timeout: 30_000 });
});

test('reports FAILED when a [test] assertion fails @wasm', async ({ playground }) => {
    await waitTabsReady(playground);
    await waitWasmReady(playground);

    await playground.evaluate(() => {
        window.pgSwitchFile('main.das');
        window.code.getDoc().setValue([
            'options gen2',
            'require dastest/testing_boost public',
            '',
            '[test]',
            'def test_fails(t : T?) {',
            '    t |> equal(1, 2)',
            '}',
            '',
        ].join('\n'));
    });

    await playground.locator('#test').click();
    await expect(playground.locator('.output_line_text', { hasText: 'FAILED!' }))
        .toBeVisible({ timeout: 30_000 });
});

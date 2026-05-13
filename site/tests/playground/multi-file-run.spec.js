// Multi-file run: write main.das + utils.das, hit ▶ run, check the
// daslang output. Requires WASM (tagged @wasm so the no-WASM CI gate
// skips this file entirely).

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

test('runs a user-authored two-file program @wasm', async ({ playground }) => {
    await waitTabsReady(playground);
    await waitWasmReady(playground);

    await playground.evaluate(() => {
        window.pgSwitchFile('main.das');
        window.code.getDoc().setValue(
            'options gen2\nrequire utils\n[export]\ndef main() {\n    hello("playground")\n}\n'
        );
        window.pgAddFile('utils.das');
        window.code.getDoc().setValue(
            'options gen2\ndef hello(name : string) {\n    print("hi, {name}\\n")\n}\n'
        );
        window.pgSwitchFile('main.das');
    });

    await playground.locator('#run').click();
    await expect(playground.locator('.output_line_text', { hasText: 'hi, playground' }))
        .toBeVisible({ timeout: 15_000 });
});

test('renaming utils.das to non-required name breaks the build @wasm', async ({ playground }) => {
    await waitTabsReady(playground);
    await waitWasmReady(playground);

    await playground.evaluate(() => {
        window.pgSwitchFile('main.das');
        window.code.getDoc().setValue(
            'options gen2\nrequire utils\n[export]\ndef main() { hello() }\n'
        );
        window.pgAddFile('utils.das');
        window.code.getDoc().setValue('options gen2\ndef hello() { print("ok\\n") }\n');
    });
    // Rename utils.das → helpers.das; require utils now fails to resolve.
    expect(await playground.evaluate(() => window.pgRenameFile('utils.das', 'helpers.das'))).toBe(true);
    await playground.evaluate(() => window.pgSwitchFile('main.das'));
    await playground.locator('#run').click();
    // daslang emits an error message containing "can't open" or "require".
    await expect(playground.locator('.output_line_text', { hasText: /can.t (?:open|locate)|require/i }))
        .toBeVisible({ timeout: 15_000 });
});

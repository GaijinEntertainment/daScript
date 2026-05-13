// Examples + Tests <select> dropdowns swap the editor buffer.
// Until phase 2 the change handlers were missing from index.html, so picking
// an item did nothing. The spec guards against that regression.

const { test, expect } = require('./fixtures.js');

async function waitDropdownsPopulated(page) {
    // selectSample("examples", 0) runs at startup; once samplesData is set the
    // dropdown has > 1 option (the "Select..." sentinel plus the entries).
    await page.waitForFunction(
        () => document.getElementById('examples').options.length > 1
            && document.getElementById('tests').options.length > 1,
        null,
        { timeout: 10_000 }
    );
}

async function editorText(page) {
    return await page.evaluate(() => window.code.getValue());
}

test('Examples dropdown loads chosen sample into the editor', async ({ playground }) => {
    await waitDropdownsPopulated(playground);

    // Loop sample lives at examples/loop.das — content contains range(0, 10).
    await playground.locator('#examples').selectOption({ label: 'Loops' });
    await expect.poll(() => editorText(playground), { timeout: 5_000 })
        .toContain('range(0, 10)');

    // Switch to Functions; signature line `def someFunction` ships in func.das.
    await playground.locator('#examples').selectOption({ label: 'Functions' });
    await expect.poll(() => editorText(playground), { timeout: 5_000 })
        .toMatch(/def\s+\w+/);
});

test('Tests dropdown swaps the editor too', async ({ playground }) => {
    await waitDropdownsPopulated(playground);

    // Pick the first available test entry (skipping the "Select test" sentinel).
    const firstTestLabel = await playground.evaluate(() => {
        const sel = document.getElementById('tests');
        return sel.options[1]?.text;
    });
    expect(firstTestLabel).toBeTruthy();

    const before = await editorText(playground);
    await playground.locator('#tests').selectOption({ label: firstTestLabel });
    await expect.poll(() => editorText(playground), { timeout: 5_000 })
        .not.toBe(before);
});

test('Dropdown resets to the "Select..." sentinel after a pick', async ({ playground }) => {
    await waitDropdownsPopulated(playground);

    await playground.locator('#examples').selectOption({ label: 'Loops' });
    // selectSample writes value="init" back after firing.
    await expect.poll(() => playground.locator('#examples').inputValue(), { timeout: 2_000 })
        .toBe('init');
});

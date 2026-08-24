// The "Linq: group by + average" sample — the code card on performance.html,
// deep-linked from its "try query.das on the playground" button as
// ?example=linq_cars. The loading half (slug resolves, the sample lands in
// the editor) runs in any tier; the run half is @wasm.

const { test, expect } = require('./fixtures.js');

async function waitReady(page) {
    await page.waitForFunction(() => !!window.pgState, null, { timeout: 10_000 });
    await page.waitForFunction(() => window.pgSamplesReady === true, null, { timeout: 15_000 });
}

test('linq_cars deep-link loads the sample into the editor', async ({ playground }) => {
    await playground.goto('/playground/?example=linq_cars');
    await waitReady(playground);
    await expect.poll(
        () => playground.evaluate(() => window.pgState.files['main.das'].getValue()),
        { timeout: 10_000 }
    ).toContain('require daslib/linq_boost');
    const text = await playground.evaluate(() => window.pgState.files['main.das'].getValue());
    expect(text).toContain('_group_by(_.brand)');
});

test('linq_cars sample runs successfully under WASM @wasm', async ({ playground }) => {
    await playground.goto('/playground/?example=linq_cars');
    await waitReady(playground);
    await playground.waitForFunction(
        () => !!(window.PlaygroundRunner && window.PlaygroundRunner.isReady()),
        null,
        { timeout: 30_000 }
    );
    await playground.locator('#run').click();
    await expect(playground.locator('.output_line_text', { hasText: 'Ka: avg 900' }))
        .toBeVisible({ timeout: 15_000 });
    await expect(playground.locator('.output_line_text', { hasText: 'Vaz: avg 1200' }))
        .toBeVisible({ timeout: 15_000 });
});

test('performance.html hero links the sample to the playground', async ({ page }) => {
    await page.goto('/performance.html');
    const link = page.locator('.forge-terminal .forge-bench__playground-link a');
    await expect(link).toHaveAttribute('href', '/playground/?example=linq_cars');
    await expect(link).toContainText('on the playground');
});

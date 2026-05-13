// Multi-file tab strip: add / switch / rename / delete.
// Drives both the UI path (clicks/keys) and the pg* JS API.

const { test, expect } = require('./fixtures.js');

async function waitTabsReady(page) {
    // Playwright contexts are clean per test, so localStorage starts empty.
    // pgInit polls until window.code is up, so we wait for pgState to surface.
    await page.waitForFunction(() => !!window.pgState, null, { timeout: 10_000 });
    await page.locator('#pg-tabs .pg-tab[data-file="main.das"]').waitFor();
}

test('initial state: only main.das, × is disabled', async ({ playground }) => {
    await waitTabsReady(playground);
    const tabs = playground.locator('#pg-tabs .pg-tab');
    await expect(tabs).toHaveCount(1);
    await expect(tabs.first()).toHaveClass(/is-active/);
    await expect(tabs.first()).toHaveClass(/is-entry/);
    await expect(tabs.first().locator('.pg-tab__close')).toBeDisabled();
});

test('+ button adds a new file with an untitled name', async ({ playground }) => {
    await waitTabsReady(playground);
    await playground.locator('#pg-tabs .pg-tab__add').click();
    await expect(playground.locator('#pg-tabs .pg-tab')).toHaveCount(2);
    await expect(playground.locator('#pg-tabs .pg-tab.is-active')).toHaveAttribute(
        'data-file',
        'untitled1.das',
    );
});

test('clicking a tab switches the editor buffer', async ({ playground }) => {
    await waitTabsReady(playground);
    // Add a second file with distinct content.
    await playground.evaluate(() => {
        window.pgAddFile('utils.das');
        window.code.getDoc().setValue('// utils contents');
    });
    // Switch to main via tab click.
    await playground.locator('.pg-tab[data-file="main.das"]').click();
    await expect(playground.locator('#pg-tabs .pg-tab.is-active')).toHaveAttribute(
        'data-file',
        'main.das',
    );
    const mainText = await playground.evaluate(() => window.code.getValue());
    expect(mainText).not.toContain('utils contents');

    // Back to utils.
    await playground.locator('.pg-tab[data-file="utils.das"]').click();
    const utilsText = await playground.evaluate(() => window.code.getValue());
    expect(utilsText).toBe('// utils contents');
});

test('double-click renames a file', async ({ playground }) => {
    await waitTabsReady(playground);
    await playground.evaluate(() => window.pgAddFile('utils.das'));
    await playground.locator('.pg-tab[data-file="utils.das"] .pg-tab__name').dblclick();
    const input = playground.locator('input.pg-tab__rename');
    await expect(input).toBeFocused();
    await input.fill('helpers.das');
    await input.press('Enter');

    await expect(playground.locator('.pg-tab[data-file="helpers.das"]')).toBeVisible();
    await expect(playground.locator('.pg-tab[data-file="utils.das"]')).toHaveCount(0);
    expect(await playground.evaluate(() => Object.keys(window.pgState.files))).toEqual([
        'main.das', 'helpers.das',
    ]);
});

test('rename rejects invalid filenames', async ({ playground }) => {
    await waitTabsReady(playground);
    await playground.evaluate(() => window.pgAddFile('utils.das'));
    // Suppress the alert dialog.
    playground.on('dialog', dialog => dialog.dismiss());

    const ok = await playground.evaluate(() => window.pgRenameFile('utils.das', 'no_extension'));
    expect(ok).toBe(false);
    expect(await playground.evaluate(() => Object.keys(window.pgState.files))).toContain('utils.das');
});

test('main.das cannot be renamed or deleted', async ({ playground }) => {
    await waitTabsReady(playground);
    expect(await playground.evaluate(() => window.pgDeleteFile('main.das'))).toBe(false);
    expect(await playground.evaluate(() => window.pgRenameFile('main.das', 'app.das'))).toBe(false);
    await expect(playground.locator('.pg-tab[data-file="main.das"]')).toHaveCount(1);
});

test('× on a non-entry file removes it and falls back to main.das', async ({ playground }) => {
    await waitTabsReady(playground);
    await playground.evaluate(() => window.pgAddFile('utils.das'));
    // utils.das is empty so confirm() should not be invoked.
    await playground.locator('.pg-tab[data-file="utils.das"] .pg-tab__close').click();
    await expect(playground.locator('.pg-tab[data-file="utils.das"]')).toHaveCount(0);
    await expect(playground.locator('#pg-tabs .pg-tab.is-active')).toHaveAttribute(
        'data-file',
        'main.das',
    );
});

test('× on a non-empty file asks to confirm', async ({ playground }) => {
    await waitTabsReady(playground);
    await playground.evaluate(() => {
        window.pgAddFile('utils.das');
        window.code.getDoc().setValue('not empty');
    });

    // Reject the confirm — file stays.
    playground.once('dialog', d => d.dismiss());
    await playground.locator('.pg-tab[data-file="utils.das"] .pg-tab__close').click();
    await expect(playground.locator('.pg-tab[data-file="utils.das"]')).toHaveCount(1);

    // Accept the confirm — file goes.
    playground.once('dialog', d => d.accept());
    await playground.locator('.pg-tab[data-file="utils.das"] .pg-tab__close').click();
    await expect(playground.locator('.pg-tab[data-file="utils.das"]')).toHaveCount(0);
});

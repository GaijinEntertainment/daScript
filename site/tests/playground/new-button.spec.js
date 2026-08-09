// The "new" button: one click discards the autosaved session and leaves a
// single empty main.das — a blank scratch buffer, whatever the page held
// before (edited session, deep-linked sample). Samples stay one dropdown
// pick away.

const { test, expect } = require('./fixtures.js');

async function waitReady(page) {
    await page.waitForFunction(() => !!window.pgState, null, { timeout: 10_000 });
    await page.waitForFunction(() => window.pgSamplesReady === true, null, { timeout: 15_000 });
}

test('new resets an edited session to an empty buffer', async ({ playground }) => {
    await waitReady(playground);

    // Dirty the buffer and add a second file; wait for autosave to persist it.
    await playground.evaluate(() => {
        window.pgAddFile('scratch.das');
        window.code.getDoc().setValue('// scratch content\n');
        window.pgSwitchFile('main.das');
        window.code.getDoc().setValue('// dirty marker\n');
    });
    await expect.poll(
        () => playground.evaluate(() => {
            const raw = localStorage.getItem('daslang.playground.state.v1');
            return raw ? raw.includes('dirty marker') : false;
        }),
        { timeout: 5_000 }
    ).toBe(true);

    await playground.locator('#new').click();

    // Back to a single empty tab, marker gone, autosave dropped.
    await expect.poll(
        () => playground.evaluate(() => Object.keys(window.pgState.files).length),
        { timeout: 10_000 }
    ).toBe(1);
    await expect.poll(
        () => playground.evaluate(() => window.pgState.files['main.das'].getValue()),
        { timeout: 10_000 }
    ).toBe('');
});

test('new after a deep-link clears that sample too', async ({ playground }) => {
    await playground.goto('/playground/?example=sha256');
    await waitReady(playground);
    await expect.poll(
        () => playground.evaluate(() => window.pgState.files['main.das'].getValue()),
        { timeout: 10_000 }
    ).toMatch(/sha[- ]?256/i);

    await playground.locator('#new').click();

    await expect.poll(
        () => playground.evaluate(() => window.pgState.files['main.das'].getValue()),
        { timeout: 10_000 }
    ).toBe('');
});

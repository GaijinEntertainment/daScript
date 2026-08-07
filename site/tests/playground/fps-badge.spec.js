// The fps checkbox and the badge it controls.
//
// The number itself is measured inside the run frame (interpreted) or the
// artifact shell (wasm) and posted to the parent, because only the frame can see
// the program's draw calls — the wasm artifact is cross-origin and cannot be
// inspected from this side at all. These specs drive that message directly, so
// they cover the parent half without needing a WASM runtime.

const { test, expect } = require('./fixtures.js');

const BOX = '#show-fps';
const BADGE = '.pg-fps';

// The runner only trusts fps from the frame currently running a program (the
// pre-warmed spare ticks too), so feed the badge through its own entry point —
// the same one main.js uses to forward the cross-origin page frame's reports.
async function feedFps(page, value) {
    await page.evaluate(v => window.PlaygroundRunner.noteFps(v), value);
}

test('the toggle sits in the header, left of clear, and is on by default', async ({ playground: page }) => {
    const box = page.locator(BOX);
    await expect(box).toBeChecked();

    const order = await page.evaluate(() => {
        const right = document.querySelector('.main_header_right');
        const kids = Array.from(right.children);
        return {
            fpsIndex: kids.findIndex(k => k.querySelector('#show-fps')),
            clearIndex: kids.findIndex(k => k.id === 'clear'),
        };
    });
    expect(order.fpsIndex).toBeGreaterThanOrEqual(0);
    expect(order.clearIndex).toBeGreaterThan(order.fpsIndex);
});

test('a reported frame rate shows over the canvas', async ({ playground: page }) => {
    await feedFps(page, 58.4);
    await expect(page.locator(BADGE)).toBeVisible();
    await expect(page.locator(BADGE)).toHaveText('58 fps');
});

// The badge exists to make the interpreter/wasm gap obvious, and an interpreted
// program can sit well under 1 fps — rounding those all to "0 fps" would erase
// the very comparison it is for.
test('sub-10 rates keep a decimal', async ({ playground: page }) => {
    await feedFps(page, 0.4);
    await expect(page.locator(BADGE)).toHaveText('0.4 fps');
});

test('unchecking hides it, and the choice survives a reload', async ({ playground: page }) => {
    await feedFps(page, 60);
    await expect(page.locator(BADGE)).toBeVisible();

    await page.locator(BOX).uncheck();
    await expect(page.locator(BADGE)).toBeHidden();

    await page.reload();
    await page.locator('.CodeMirror').waitFor({ state: 'visible' });
    await expect(page.locator(BOX)).not.toBeChecked();
    await feedFps(page, 60);
    await expect(page.locator(BADGE)).toBeHidden();

    await page.locator(BOX).check();
    await feedFps(page, 60);
    await expect(page.locator(BADGE)).toBeVisible();
});

// A program that stops presenting must not leave a stale number implying it is
// still running.
test('the badge clears when reports stop, and on reset', async ({ playground: page }) => {
    await feedFps(page, 60);
    await expect(page.locator(BADGE)).toBeVisible();
    await expect(page.locator(BADGE)).toBeHidden({ timeout: 5_000 });

    await feedFps(page, 60);
    await expect(page.locator(BADGE)).toBeVisible();
    await page.evaluate(() => window.PlaygroundRunner.reset());
    await expect(page.locator(BADGE)).toBeHidden();
});

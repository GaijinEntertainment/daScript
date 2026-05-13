// Vertical splitter between code and output. Drag sets `flex: 0 0 <pct>%`
// on the left column; the value persists via localStorage.

const { test, expect } = require('./fixtures.js');

async function waitWorkspaceReady(page) {
    await page.locator('.main_workspace > .main_col').first().waitFor();
    await page.locator('.pg-splitter').waitFor();
}

test('splitter sits between the two main_col panels', async ({ playground }) => {
    await waitWorkspaceReady(playground);
    const order = await playground.evaluate(() =>
        Array.from(document.querySelector('.main_workspace').children).map(e => e.className.trim().split(' ')[0])
    );
    expect(order).toEqual(['main_col', 'pg-splitter', 'main_col']);
});

test('real mouse drag resizes the left column and persists', async ({ playground }) => {
    await waitWorkspaceReady(playground);
    const handle = playground.locator('.pg-splitter');
    const ws = playground.locator('.main_workspace');
    const hbox = await handle.boundingBox();
    const wbox = await ws.boundingBox();
    const startX = hbox.x + hbox.width / 2;
    const y = hbox.y + hbox.height / 2;
    const targetX = wbox.x + wbox.width * 0.30;
    // Drive a real pointer drag through Playwright — this catches CSS
    // !important defeats that synthetic-event tests don't notice.
    await playground.mouse.move(startX, y);
    await playground.mouse.down();
    await playground.mouse.move(targetX, y, { steps: 10 });
    await playground.mouse.up();

    const result = await playground.evaluate(() => {
        const ws = document.querySelector('.main_workspace');
        const left = ws.querySelectorAll(':scope > .main_col')[0];
        const right = ws.querySelectorAll(':scope > .main_col')[1];
        return {
            inlineFlex: left.style.flex,
            leftPx: left.getBoundingClientRect().width,
            rightPx: right.getBoundingClientRect().width,
            wsPx: ws.getBoundingClientRect().width,
            ls: localStorage.getItem('daslang.playground.splitLeftPct'),
        };
    });
    expect(result.inlineFlex).toMatch(/^0 0 30(\.\d+)?%$/);
    // The visual layout actually changed (this is what the prior synthetic
    // test missed when CSS `!important` was beating the inline style).
    const leftFrac = result.leftPx / result.wsPx;
    expect(leftFrac).toBeGreaterThan(0.27);
    expect(leftFrac).toBeLessThan(0.33);
    expect(parseFloat(result.ls)).toBeCloseTo(30, 0);
});

test('double-click on splitter resets to 50/50', async ({ playground }) => {
    await waitWorkspaceReady(playground);
    await playground.evaluate(() => {
        const ws = document.querySelector('.main_workspace');
        const left = ws.querySelectorAll(':scope > .main_col')[0];
        left.style.flex = '0 0 25%';
        localStorage.setItem('daslang.playground.splitLeftPct', '25');
    });
    await playground.locator('.pg-splitter').dblclick();
    const after = await playground.evaluate(() => {
        const ws = document.querySelector('.main_workspace');
        const left = ws.querySelectorAll(':scope > .main_col')[0];
        return { flex: left.style.flex, ls: localStorage.getItem('daslang.playground.splitLeftPct') };
    });
    expect(after.flex).toMatch(/0 0 50%/);
    expect(after.ls).toBeNull();
});

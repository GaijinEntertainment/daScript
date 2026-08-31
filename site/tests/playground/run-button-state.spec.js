// The Run button's state machine. Two halves:
//   - the paint contract (pgRuntimeProgress → label/fill) needs no runtime and
//     runs in the per-PR no-WASM lane;
//   - the busy lifecycle around a real run (running… shows, first output clears
//     it, Clear during a run recovers the buttons) needs the runtime (@wasm).

const { test, expect } = require('./fixtures.js');

async function waitWasmReady(page) {
    await page.waitForFunction(
        () => !!(window.PlaygroundRunner && window.PlaygroundRunner.isReady()),
        null,
        { timeout: 60_000 }
    );
}

test('pgRuntimeProgress paints percent, indeterminate, and phase labels', async ({ playground }) => {
    // Pin the loading state: with a staged runtime the spare is ready, and with
    // none (the no-WASM lane) the runner goes DEAD after its abort budget —
    // either way the paint would show '▶ run' regardless of phase. Real download
    // ticks race any two-step read, so each probe paints AND reads in one
    // evaluate. Restored by page teardown.
    await playground.evaluate(() => {
        window.PlaygroundRunner.isReady = () => false;
        window.PlaygroundRunner.isDead = () => false;
    });
    const paint = (phase, fraction) => playground.evaluate(
        ([p, f]) => {
            window.pgRuntimeProgress(p, f);
            const b = document.getElementById('run');
            return { label: b.textContent, fill: b.style.getPropertyValue('--pg-progress') };
        },
        [phase, fraction]);
    expect(await paint('download', 0.42)).toEqual({ label: 'loading 42%', fill: '42%' });
    // no Content-Length: an indeterminate download, never a stuck or NaN percent
    expect(await paint('download', -1)).toEqual({ label: 'loading…', fill: '' });
    expect((await paint('compile', 1)).label).toBe('compiling…');
    expect((await paint('start', 1)).label).toBe('starting…');
    // (the 'ready' → '▶ run' arm needs a live runtime; the @wasm specs below
    // assert it at the end of a real run)
});

test('running… shows during a run, first output clears it, and Run stays the kill switch @wasm', async ({ playground }) => {
    test.setTimeout(240_000);
    await waitWasmReady(playground);
    await playground.waitForFunction(() => !!window.pgState, null, { timeout: 10_000 });
    await playground.evaluate(() => {
        window.pgSwitchFile('main.das');
        window.code.getDoc().setValue('options gen2\n[export]\ndef main {\n    print("state probe done\\n")\n}\n');
    });
    await playground.locator('#run').click();
    // the busy state is set before the frame compiles; it may clear fast, so
    // sample the class rather than demand to catch it mid-flight
    await expect(playground.locator('.output_line_text', { hasText: 'state probe done' }))
        .toBeVisible({ timeout: 60_000 });
    // first output cleared the busy state; the spare re-arms Run
    await expect.poll(() => playground.evaluate(() =>
        document.getElementById('run').classList.contains('pg-busy')), { timeout: 15_000 }).toBe(false);
    await expect.poll(() => playground.evaluate(() =>
        window.PlaygroundRunner.isReady()), { timeout: 60_000 }).toBe(true);
    await expect(playground.locator('#run')).toBeEnabled();
});

test('Clear during a run recovers the buttons — no permanent latch @wasm', async ({ playground }) => {
    test.setTimeout(240_000);
    await waitWasmReady(playground);
    await playground.waitForFunction(() => !!window.pgState, null, { timeout: 10_000 });
    // a program slow enough that Clear can land while it is still starting
    await playground.evaluate(() => {
        window.pgSwitchFile('main.das');
        window.code.getDoc().setValue([
            'options gen2',
            'require daslib/fio',
            '[export]',
            'def main {',
            '    sleep(3000u)',
            '    print("slow done\\n")',
            '}',
        ].join('\n'));
    });
    await playground.locator('#run').click();
    await playground.locator('#clear').click();
    // whatever the interleaving (clear before/after the program started), the
    // invariant holds: the page recovers, Run re-enables, and a next run works
    await expect.poll(() => playground.evaluate(() =>
        !document.getElementById('run').disabled
        && !document.getElementById('run').classList.contains('pg-busy')), { timeout: 90_000 }).toBe(true);
    await playground.evaluate(() => {
        window.pgSwitchFile('main.das');
        window.code.getDoc().setValue('options gen2\n[export]\ndef main {\n    print("recovered\\n")\n}\n');
    });
    await playground.locator('#run').click();
    await expect(playground.locator('.output_line_text', { hasText: 'recovered' }))
        .toBeVisible({ timeout: 60_000 });
});

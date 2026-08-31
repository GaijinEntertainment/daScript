// Jobque on the browser main thread: a join whose jobs can never complete used
// to park the interpreter's main thread in a condition-variable wait forever,
// freezing the whole page (the nightly "wedge" class — the path tracer sample
// hit it through the module-teardown-kills-the-persistent-que lifecycle bug).
// waitForJob now discriminates by refcount: a status nothing holds (appended,
// never dispatched) throws "join deadlock avoided"; a status live work holds
// waits as long as the work needs. These two specs are the two halves of that
// contract — lose the bound and the first wedges the tab; flatten the bound
// into a naive deadline and the second throws on a healthy long job.
//
// Requires WASM (tagged @wasm so the no-WASM CI gate skips this file). The
// nightly runs @wasm specs against the DEPLOYED site, so both stay red until a
// runtime carrying the fix ships.
//
// Budgets follow runtime-revive.spec.js: against the live site the runtime is
// a ~40MB download per fresh context, so test.slow()'s 90s is too tight.

const { test, expect } = require('./fixtures.js');

async function waitWasmReady(page) {
    await page.waitForFunction(
        () => !!(window.PlaygroundRunner && window.PlaygroundRunner.isReady()),
        null,
        { timeout: 60_000 }
    );
}

async function setMainDas(page, lines) {
    await page.waitForFunction(() => !!window.pgState, null, { timeout: 10_000 });
    await page.evaluate((src) => {
        window.pgSwitchFile('main.das');
        window.code.getDoc().setValue(src);
    }, lines.join('\n'));
}

test('a join that can never finish reports a deadlock instead of freezing the page @wasm', async ({ playground }) => {
    test.setTimeout(240_000);
    await waitWasmReady(playground);

    await setMainDas(playground, [
        'options gen2',
        'require daslib/jobque_boost',
        '',
        '[export]',
        'def main {',
        '    create_job_que()',
        '    var status = job_status_create()',
        '    status |> append(1)   // a job that is never dispatched',
        '    status |> join',
        '}',
    ]);

    await playground.locator('#run').click();
    await expect(playground.locator('.output_line_text', { hasText: 'join deadlock avoided' }))
        .toBeVisible({ timeout: 60_000 });
    // The page survived: a fresh frame stands by and Run is clickable again.
    await expect.poll(() => playground.evaluate(() => window.PlaygroundRunner.isReady()),
        { timeout: 60_000 }).toBe(true);
    await expect(playground.locator('#run')).toBeEnabled();
});

test('a join held by a live long job completes instead of tripping the bound @wasm', async ({ playground }) => {
    test.setTimeout(240_000);
    await waitWasmReady(playground);

    // The job sleeps past the 10s stall window on a worker thread; its captured
    // status ref is what tells the join "live work, keep waiting". with_wait_group
    // joins on the guarded stack shape — the arm a throw would terminate through.
    await setMainDas(playground, [
        'options gen2',
        'require daslib/jobque_boost',
        'require daslib/fio',
        '',
        '[export]',
        'def main {',
        '    create_job_que()',
        '    with_wait_group(1) $(wg) {',
        '        new_job() @() {',
        '            sleep(12000u)',
        '            wg |> done',
        '        }',
        '    }',
        '    destroy_job_que()',
        '    print("long join completed\\n")',
        '}',
    ]);

    await playground.locator('#run').click();
    await expect(playground.locator('.output_line_text', { hasText: 'long join completed' }))
        .toBeVisible({ timeout: 90_000 });
    const lines = await playground.evaluate(() =>
        [...document.querySelectorAll('.output_line_text')].map((e) => e.innerText).join('\n'));
    expect(lines).not.toContain('join deadlock avoided');
});

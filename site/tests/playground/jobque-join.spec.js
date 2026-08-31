// Jobque on the browser main thread: a join whose jobs can never complete used
// to park the interpreter's main thread in a condition-variable wait forever,
// freezing the whole page (the nightly "wedge" class — the path tracer sample
// hit it through the module-teardown-kills-the-persistent-que lifecycle bug).
// waitForJob now bounds a no-progress join and throws a das exception the
// output pane can show. This spec IS the wedge regression: if the bound is
// lost, the run never produces the exception line and the test times out
// instead of the visitor's tab.
//
// Requires WASM (tagged @wasm so the no-WASM CI gate skips this file).

const { test, expect } = require('./fixtures.js');

async function waitWasmReady(page) {
    await page.waitForFunction(
        () => !!(window.PlaygroundRunner && window.PlaygroundRunner.isReady()),
        null,
        { timeout: 30_000 }
    );
}

// The join gives up after 10s without progress; compile + slack on top.
test('a join that can never finish reports a deadlock instead of freezing the page @wasm', async ({ playground }) => {
    test.setTimeout(60_000);
    await waitWasmReady(playground);

    await playground.evaluate(() => {
        window.code.getDoc().setValue([
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
        ].join('\n'));
    });

    await playground.locator('#run').click();
    await expect(playground.locator('.output_line_text', { hasText: 'join deadlock avoided' }))
        .toBeVisible({ timeout: 40_000 });
    // The page survived: the editor still answers and Run comes back.
    await expect(playground.locator('#run')).toHaveText('▶ run', { timeout: 15_000 });
});

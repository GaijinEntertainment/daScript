// The browser gate (files/wasm-gate.js) decides before any runtime starts. Each test stubs what
// the gate reads - WebAssembly.validate, the user agent, the page's isolation - and asserts what
// the page does with the answer. None needs the daslang runtime: the refusals land before it
// loads, and the passing arm only has to append its script tag.

const { test, expect } = require('@playwright/test');

const ISOLATED = () => {
    Object.defineProperty(window, 'crossOriginIsolated', { get: () => true });
    if (typeof SharedArrayBuffer === 'undefined') window.SharedArrayBuffer = ArrayBuffer;
};
const NOT_ISOLATED = () => { Object.defineProperty(window, 'crossOriginIsolated', { get: () => false }); };
// the gate's two probes end in the feature's own byte: exnref last, v128 last but one
const NO_EXNREF = () => {
    const validate = WebAssembly.validate.bind(WebAssembly);
    WebAssembly.validate = (bytes) => (bytes[bytes.length - 1] === 0x69 ? false : validate(bytes));
};
const NO_SIMD = () => {
    const validate = WebAssembly.validate.bind(WebAssembly);
    WebAssembly.validate = (bytes) => (bytes[bytes.length - 2] === 0x7b ? false : validate(bytes));
};
const UA_32BIT = () => {
    Object.defineProperty(navigator, 'userAgent', { get: () => 'Mozilla/5.0 (Windows NT 10.0; WOW64; rv:159.0) Gecko/20100101 Firefox/159.0' });
};
const RECORD_POSTS = () => {
    window.__posted = [];
    window.addEventListener('message', (ev) => { window.__posted.push(ev.data); });
};

async function open(page, url, stubs) {
    for (const stub of stubs) await page.addInitScript(stub);
    await page.goto(url);
}

async function aborted(page) {
    await expect.poll(() => page.evaluate(() => window.__posted.filter((m) => m && m.type === 'aborted').length)).toBeGreaterThan(0);
    return page.evaluate(() => window.__posted.filter((m) => m && m.type === 'aborted')[0].message);
}

const runtimeTags = (page) => page.locator('script[src$="daslang_static.js"]');

test.describe('the gate answers from what it reads', () => {
    const FRAME = '/playground/run-frame.html';
    const reason = (page) => page.evaluate(() => window.dasWasmGate.reason());

    test('an isolated page on a current engine may run', async ({ page }) => {
        await open(page, FRAME, [ISOLATED]);
        expect(await reason(page)).toBe('');
    });

    test('a page without isolation is refused for it', async ({ page }) => {
        await open(page, FRAME, [NOT_ISOLATED]);
        expect(await reason(page)).toBe('isolation');
        expect(await page.evaluate(() => window.dasWasmGate.refusal('isolation').title)).toBe('SharedArrayBuffer is not available on this page.');
        expect(await page.evaluate(() => window.dasWasmGate.engineReason())).toBe('');
    });

    test('an engine without exnref or without v128 is refused, and told which', async ({ page, context }) => {
        await open(page, FRAME, [ISOLATED, NO_EXNREF]);
        expect(await reason(page)).toBe('wasm');
        expect(await page.evaluate(() => window.dasWasmGate.refusal('wasm').title)).toContain('exception references');
        const simdPage = await context.newPage();
        await open(simdPage, FRAME, [ISOLATED, NO_SIMD]);
        expect(await reason(simdPage)).toBe('wasm');
        expect(await simdPage.evaluate(() => window.dasWasmGate.refusal('wasm').title)).toContain('SIMD');
    });

    test('a 32-bit user agent is refused after the engine checks pass', async ({ page }) => {
        await open(page, FRAME, [ISOLATED, UA_32BIT]);
        expect(await reason(page)).toBe('32bit');
        expect(await page.evaluate(() => window.dasWasmGate.engineReason())).toBe('32bit');
    });

    test('?force=run skips every check and ?force=unsupported shows the isolation note', async ({ page, context }) => {
        await open(page, FRAME + '?force=run', [NOT_ISOLATED, NO_EXNREF, UA_32BIT]);
        expect(await reason(page)).toBe('');
        const forced = await context.newPage();
        await open(forced, FRAME + '?force=unsupported', [ISOLATED]);
        expect(await reason(forced)).toBe('isolation');
    });

    test('explain() words the two failures an engine reports after the gate', async ({ page }) => {
        await open(page, FRAME, [ISOLATED]);
        const explain = (what) => page.evaluate((w) => window.dasWasmGate.explain(w), what);
        expect(await explain('InternalError: out of memory')).toContain('ran out of memory compiling');
        expect(await explain("CompileError: WebAssembly.Module doesn't parse at byte 733")).toContain('does not accept the build');
        expect(await explain('exit code 1')).toBe('');
    });
});

test.describe('the run frame starts the runtime only past the gate', () => {
    const FRAME = '/playground/run-frame.html';

    test('a refused browser reads why and no runtime script is appended', async ({ page, context }) => {
        await open(page, FRAME, [RECORD_POSTS, ISOLATED, NO_EXNREF]);
        expect(await aborted(page)).toMatch(/^This browser's WebAssembly lacks exception references/);
        await expect(runtimeTags(page)).toHaveCount(0);

        const narrow = await context.newPage();
        await open(narrow, FRAME, [RECORD_POSTS, ISOLATED, UA_32BIT]);
        const refusal = await aborted(narrow);
        expect(refusal).toMatch(/^This browser is a 32-bit build\./);
        // the refusal quotes "out of memory"; it is not an abort to explain on top of itself
        expect(refusal).not.toContain('ran out of memory compiling');
        await expect(runtimeTags(narrow)).toHaveCount(0);
    });

    test('a frame whose gate script did not load does not start the runtime', async ({ page }) => {
        await page.route('**/files/wasm-gate.js', (route) => route.abort());
        await open(page, FRAME, [RECORD_POSTS, ISOLATED]);
        expect(await aborted(page)).toContain('the browser gate (files/wasm-gate.js) did not load');
        await expect(runtimeTags(page)).toHaveCount(0);
    });

    test('a passing browser gets the runtime script', async ({ page }) => {
        await open(page, FRAME, [RECORD_POSTS, ISOLATED]);
        await expect(runtimeTags(page)).toHaveCount(1);
    });
});

test.describe('the interpreted example page starts the runtime only past the gate', () => {
    const PAGE = '/examples/_interp.html?g=arcanoid';

    test('a refused browser reads why in the load line', async ({ page }) => {
        await open(page, PAGE, [ISOLATED, NO_EXNREF]);
        await expect(page.locator('#ex-load')).toContainText("This browser's WebAssembly lacks exception references");
        await expect(runtimeTags(page)).toHaveCount(0);
    });

    test('a page whose gate script did not load says so and starts nothing', async ({ page }) => {
        await page.route('**/files/wasm-gate.js', (route) => route.abort());
        await open(page, PAGE, [ISOLATED]);
        await expect(page.locator('#ex-load')).toContainText('the browser gate (files/wasm-gate.js) did not load');
        await expect(runtimeTags(page)).toHaveCount(0);
    });

    test('a passing browser gets the runtime script', async ({ page }) => {
        await open(page, PAGE, [ISOLATED]);
        await expect(runtimeTags(page)).toHaveCount(1);
    });
});

test('the playground refuses once, compiles nothing, and a Run click repeats the refusal', async ({ page }) => {
    await open(page, '/playground/', [ISOLATED, UA_32BIT]);
    const refusals = page.locator('.output_line_text', { hasText: 'This browser is a 32-bit build.' });
    await expect(refusals).toHaveCount(1);
    await expect(page.locator('iframe.pg-run-frame')).toHaveCount(0);
    expect(await page.evaluate(() => window.PlaygroundRunner.revive())).toBe(false);

    await page.locator('.CodeMirror').waitFor({ state: 'visible' });
    await page.waitForFunction(() => window.pgSamplesReady === true, null, { timeout: 15_000 });
    await page.locator('#run').click();
    await expect(refusals).toHaveCount(2);
    await expect(page.locator('.output_line_text', { hasText: 'still loading' })).toHaveCount(0);
    await expect(page.locator('iframe.pg-run-frame')).toHaveCount(0);
});

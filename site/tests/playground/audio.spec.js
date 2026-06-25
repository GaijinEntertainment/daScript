// dasAudio in the playground: the "Audio: strudel synth arpeggio" sample must
// actually PRODUCE SOUND in the browser (miniaudio's emscripten Web Audio /
// ScriptProcessor backend, single-threaded) and STOP cleanly. Tagged @wasm so the
// no-WASM CI gate skips it. No autoplay launch flag: the playground's
// installAudioUnlock glue (main.js) resumes the AudioContext under the trusted Run
// click, which is exactly the real-user path.
//
// "Sound" is proven by tapping the Web Audio graph: we wrap AudioNode.connect so
// any node feeding the context destination is teed into an AnalyserNode, and poll
// peak amplitude. "Stopped" is read from AudioContext.state (a closed context's
// AnalyserNode returns its stale last buffer, so amplitude can't prove a stop).

const { test, expect } = require('@playwright/test');

const installAudioTap = () => {
    window.__peak = 0;
    window.__states = () => window.__an.map(x => x.ctx.state);
    window.__an = [];
    const AN = window.AudioNode && window.AudioNode.prototype;
    if (AN && AN.connect) {
        const oc = AN.connect;
        AN.connect = function (dest, ...r) {
            const ret = oc.apply(this, [dest, ...r]);
            try {
                if (dest && dest.context && dest === dest.context.destination) {
                    const ctx = dest.context;
                    if (!ctx.__tap) {
                        const a = ctx.createAnalyser(); a.fftSize = 2048;
                        oc.call(a, ctx.destination); ctx.__tap = a;
                        window.__an.push({ a, b: new Float32Array(a.fftSize), ctx });
                    }
                    oc.call(this, ctx.__tap);
                }
            } catch (e) { /* ignore */ }
            return ret;
        };
    }
    setInterval(() => {
        for (const x of window.__an) {
            if (x.ctx.state !== 'running') continue;
            try {
                x.a.getFloatTimeDomainData(x.b);
                let p = 0;
                for (let i = 0; i < x.b.length; i++) { const v = Math.abs(x.b[i]); if (v > p) p = v; }
                if (p > window.__peak) window.__peak = p;
            } catch (e) { /* not ready */ }
        }
    }, 50);
};

async function loadPlaygroundWithTap(page) {
    await page.addInitScript(installAudioTap);
    await page.goto('/playground/');
    await page.waitForFunction(
        () => window.code && typeof window.code.setValue === 'function', null, { timeout: 20_000 });
    await page.waitForFunction(
        () => typeof window.FS !== 'undefined' && typeof window.Module?.callMain === 'function',
        null, { timeout: 30_000 });
    await page.waitForFunction(
        () => Array.from(document.getElementById('examples').options)
            .some(o => o.text === 'Audio: strudel synth arpeggio'),
        null, { timeout: 10_000 });
}

test('strudel audio sample plays then stops @wasm', async ({ page }) => {
    await loadPlaygroundWithTap(page);
    await page.selectOption('#examples', { label: 'Audio: strudel synth arpeggio' });
    await page.waitForFunction(() => window.code.getValue().includes('strudel_tick'), null, { timeout: 15_000 });

    await page.click('#run'); // trusted gesture -> installAudioUnlock resumes the context

    // Plays: live Web Audio output becomes non-zero within a few seconds.
    await expect.poll(() => page.evaluate(() => window.__peak), { timeout: 8_000 }).toBeGreaterThan(0.01);

    // Stops on its own (~7s, END_SECONDS): the AudioContext leaves 'running'.
    await expect
        .poll(() => page.evaluate(() => window.__states().every(s => s !== 'running') && window.__states().length > 0),
            { timeout: 12_000 })
        .toBe(true);
});

test('strudel audio sample reports its lifecycle @wasm', async ({ page }) => {
    await loadPlaygroundWithTap(page);
    await page.selectOption('#examples', { label: 'Audio: strudel synth arpeggio' });
    await page.waitForFunction(() => window.code.getValue().includes('strudel_tick'), null, { timeout: 15_000 });
    await page.click('#run');
    // init() and the eventual shutdown() both print to the output panel.
    await expect(page.locator('.output_line_text', { hasText: 'playing synth arpeggio' }))
        .toBeVisible({ timeout: 10_000 });
    await expect(page.locator('.output_line_text', { hasText: 'strudel: stopped' }))
        .toBeVisible({ timeout: 15_000 });
});

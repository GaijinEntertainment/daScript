// §06 native audio: with an audio arm and no ASR workers, the mic records a clip that rides the
// NEXT chat message as an input_audio part — no transcription pass, no second model copy.
// Chromium's fake media device supplies the "microphone" (a tone), so the record → stop → attach →
// send path runs for real.

const { test, expect, fx, raw, openControl, lastJson } = require('./fixtures');

test.use({
    launchOptions: { args: ['--use-fake-device-for-media-stream', '--use-fake-ui-for-media-stream'] },
    permissions: ['microphone'],
    // localhost, not the mock hostname: mediaDevices exists only in secure contexts, and
    // localhost is one — fixtures.js intercepts both hostnames identically
    baseURL: 'http://localhost',
});

const chatBody = posts => lastJson(posts.filter(p => p.path === '/v1/chat/completions'));

function audioStats() {
    const s = JSON.parse(JSON.stringify(fx('stats')));
    s.audio = true;
    if (s.models && s.models[0]) s.models[0].audio = true;
    return s;
}

async function recordClip(page, ms) {
    await page.locator('#b-mic').click();
    await expect(page.locator('#b-mic')).toHaveClass(/rec/);
    await page.waitForTimeout(ms);
    await page.locator('#b-mic').click();
    await expect(page.locator('#aud-strip')).toBeVisible();
    await expect(page.locator('#aud-strip')).toContainText('clip');
}

test('an audio arm without ASR workers flips the mic to attach mode', async ({ page }) => {
    await openControl(page, { stats: audioStats() });
    const mic = page.locator('#b-mic');
    await expect(mic).toBeVisible();
    await expect(mic).not.toHaveClass(/off/);
    await expect(mic).toHaveAttribute('title', /attaches to your next message/);
});

test('record, stop, send: the clip rides the message as an input_audio part, once', async ({ page }) => {
    const { posts } = await openControl(page, { stats: audioStats(), sse: raw('sse_chat') });
    await recordClip(page, 400);
    await page.locator('#chat-text').fill('what is being said?');
    await page.locator('#b-send').click();
    await expect(page.locator('#chat-log .msg.assistant').last()).toContainText(fx('sse_expected').content);
    const body = chatBody(posts);
    const last = body.messages[body.messages.length - 1];
    expect(Array.isArray(last.content)).toBe(true);
    const audio = last.content.find(p => p.type === 'input_audio');
    expect(audio.input_audio.format).toBe('wav');
    expect(audio.input_audio.data.length).toBeGreaterThan(100);
    expect(last.content.find(p => p.type === 'text').text).toBe('what is being said?');
    // the clip rode ONE message: the strip cleared, and a second send goes out as plain text
    await expect(page.locator('#aud-strip')).toBeHidden();
    await page.locator('#chat-text').fill('and now?');
    await page.locator('#b-send').click();
    await expect(page.locator('#chat-log .msg.assistant').last()).toContainText(fx('sse_expected').content);
    expect(typeof chatBody(posts).messages.at(-1).content).toBe('string');
});

test('a dimmed mic records inside the click and asks the server alongside', async ({ page }) => {
    // the first poll says no tower, so the button is dimmed; every later /v1/stats is held back.
    // getUserMedia must not wait behind that round trip — Safari drops the gesture across one.
    const stats = n => (n === 0 ? fx('stats') : { __delay: 6000, json: audioStats() });
    await openControl(page, { stats });
    const mic = page.locator('#b-mic');
    await expect(mic).toHaveClass(/off/);
    await mic.click();
    await expect(mic).toHaveClass(/rec/, { timeout: 3000 });
    await page.waitForTimeout(400);
    await mic.click();
    // the fresh answer decided the mode at stop: an audio arm attaches the clip, no transcription
    await expect(page.locator('#aud-strip')).toContainText('clip');
    await expect(mic).not.toHaveClass(/off/);
});

test('a dimmed mic whose fresh answer still has no tower drops the clip and walks to the catalog', async ({ page }) => {
    await openControl(page, { stats: fx('stats') });   // no ASR workers, no audio arm, ever
    const mic = page.locator('#b-mic');
    await expect(mic).toHaveClass(/off/);
    await mic.click();
    await expect(mic).toHaveClass(/rec/);
    await page.waitForTimeout(300);
    await mic.click();
    await expect(page.locator('#cat-note')).toContainText('ASR tower');
    await expect(page.locator('#aud-strip')).toBeHidden();
});

test('a recording that captured nothing says so instead of attaching an empty clip', async ({ page }) => {
    // a microphone that yields no audio: the processor node never delivers a buffer
    await page.addInitScript(() => {
        const make = AudioContext.prototype.createScriptProcessor;
        AudioContext.prototype.createScriptProcessor = function (...a) {
            const node = make.apply(this, a);
            Object.defineProperty(node, 'onaudioprocess', { set() { }, get() { return null; } });
            return node;
        };
    });
    await openControl(page, { stats: audioStats() });
    await page.locator('#b-mic').click();
    await expect(page.locator('#b-mic')).toHaveClass(/rec/);
    await page.waitForTimeout(400);
    await page.locator('#b-mic').click();
    await expect(page.locator('#aud-strip')).toContainText('no audio captured');
});

test('attaching an image drops the pending clip — the server takes one media per message', async ({ page }) => {
    await openControl(page, { stats: audioStats() });
    await recordClip(page, 300);
    // a 1x1 png through the image file input, the same rail a user's picker uses
    const png = Buffer.from(
        'iVBORw0KGgoAAAANSUhEUgAAAAEAAAABCAYAAAAfFcSJAAAADUlEQVR42mP8z8BQDwAEhQGAhKmMIQAAAABJRU5ErkJggg==',
        'base64');
    await page.locator('#img-file').setInputFiles({ name: 'dot.png', mimeType: 'image/png', buffer: png });
    await expect(page.locator('#img-strip')).toBeVisible();
    await expect(page.locator('#aud-strip')).toBeHidden();
});

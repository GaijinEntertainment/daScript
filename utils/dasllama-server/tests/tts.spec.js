// §08 speech studio: the section is gated on the `tts` block of /v1/stats, the voice select is
// that block's list, Speak posts the route's own body shape and reports the decoded audio, and
// the §03 offer card reads the three path-mode states the page can be in.

const { test, expect, fx, bin, openControl, lastJson } = require('./fixtures');

const SPEECH = '/v1/audio/speech';

function statsWithoutTts() {
    const s = JSON.parse(JSON.stringify(fx('stats_tts')));
    delete s.tts;
    return s;
}

// the fixture's own duration, off its RIFF header — the page decodes the same bytes
function wavSeconds(buf) {
    return buf.readUInt32LE(40) / (buf.readUInt32LE(24) * buf.readUInt16LE(32));
}

function wavAnswer() {
    return { responses: { [SPEECH]: { status: 200, body: bin('speech.wav'), contentType: 'audio/wav' } } };
}

test('a boot with no speech model keeps the studio hidden', async ({ page }) => {
    await openControl(page);   // the plain stats fixture carries no tts block
    await expect(page.locator('#tts-section')).toBeHidden();
});

test('a tts block that is not ready keeps the studio hidden', async ({ page }) => {
    const s = fx('stats_tts');
    s.tts.ready = false;
    await openControl(page, { stats: s });
    await expect(page.locator('#tts-section')).toBeHidden();
});

test('a ready tts block reveals the studio and fills the voice select from the block', async ({ page }) => {
    const s = fx('stats_tts');
    await openControl(page, { stats: s });
    await expect(page.locator('#tts-section')).toBeVisible();
    expect(await page.locator('#s-voice option').allTextContents()).toEqual(s.tts.voices);
    await expect(page.locator('#s-voice')).toHaveValue(s.tts.voices[0]);
    await expect(page.locator('#s-count')).toContainText('/ 4096');
});

test('the textarea counts against the route cap and gates the button', async ({ page }) => {
    await openControl(page, { stats: fx('stats_tts') });
    await expect(page.locator('#s-run')).toBeDisabled();
    await page.locator('#s-text').fill('hello');
    await expect(page.locator('#s-count')).toHaveText('5 / 4096');
    await expect(page.locator('#s-run')).toBeEnabled();
    await page.locator('#s-text').fill('x'.repeat(4097));
    await expect(page.locator('#s-run')).toBeDisabled();
});

test('speak posts the route body and the status line reports the decoded audio', async ({ page }) => {
    const s = fx('stats_tts');
    const { posts } = await openControl(page, Object.assign({ stats: s }, wavAnswer()));
    await page.locator('#s-text').fill('hello from the speech studio');
    await page.locator('#s-speed').fill('1.5');
    await page.locator('#s-run').click();
    await expect(page.locator('#s-note')).toContainText('of audio in');
    const body = lastJson(posts.filter(p => p.path === SPEECH));
    expect(body.input).toBe('hello from the speech studio');
    expect(body.voice).toBe(s.tts.voices[0]);
    expect(body.speed).toBe(1.5);
    expect(body.response_format).toBe('wav');
    const note = await page.locator('#s-note').textContent();
    const secs = Number(note.match(/^([\d.]+)s of audio/)[1]);
    expect(Math.abs(secs - wavSeconds(bin('speech.wav')))).toBeLessThan(0.05);
    expect(note).toMatch(/rtf \d/);
});

test('the picked voice survives a reload', async ({ page }) => {
    const s = fx('stats_tts');
    const second = s.tts.voices[1];
    await openControl(page, { stats: s });
    await page.locator('#s-voice').selectOption(second);
    await page.reload();
    await expect(page.locator('#s-voice')).toHaveValue(second);
});

test('a refusal renders in the status line', async ({ page }) => {
    const refusal = fx('speech_error');
    await openControl(page, {
        stats: fx('stats_tts'),
        responses: { [SPEECH]: { status: 400, json: refusal } },
    });
    await page.locator('#s-text').fill('speak this');
    await page.locator('#s-run').click();
    await expect(page.locator('#s-note')).toHaveText(refusal.error.message);
});

test('the offer card names the model, its voices and its lane when one is wired', async ({ page }) => {
    const s = fx('stats_tts');
    await openControl(page, { stats: s, config: fx('config_tts') });
    const card = page.locator('#tts-offer');
    await expect(card).toContainText('wired: ' + s.tts.id);
    await expect(card).toContainText(s.tts.voices.length + ' voices');
    await expect(card).toContainText(s.tts.lane);
});

test('the offer card says loading while a configured model has not answered ready', async ({ page }) => {
    await openControl(page, { stats: statsWithoutTts(), config: fx('config_tts') });
    await expect(page.locator('#tts-offer')).toContainText('speech model configured, loading');
});

test('the offer card points at the config field when nothing is configured', async ({ page }) => {
    await openControl(page);
    await expect(page.locator('#tts-offer')).toContainText('no speech model configured');
    await expect(page.locator('#tts-offer')).toContainText('tts field');
});

test('setup mode hides the studio the way it hides the mic', async ({ page }) => {
    const s = fx('stats_tts');
    s.setup = true;
    await openControl(page, { stats: s });
    await expect(page.locator('#tts-section')).toBeHidden();
    await expect(page.locator('#b-mic')).toBeHidden();
});

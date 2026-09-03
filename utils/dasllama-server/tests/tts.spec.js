// §08 speech studio: the section is gated on the `tts` block of /v1/stats, the voice select is
// that block's list, Speak posts the route's own body shape and reports the decoded audio, and
// the §03 offer card walks the speech set's download ladder before the three path-mode states.

const { test, expect, fx, bin, openControl, lastJson } = require('./fixtures');

const SPEECH = '/v1/audio/speech';

function statsWithoutTts() {
    const s = JSON.parse(JSON.stringify(fx('stats_tts')));
    delete s.tts;
    return s;
}

// the same names and sizes the card prints, read off whatever the capture recorded
const modelName = file => file.replace(/\.gguf$/i, '');
const sizeMB = bytes => Math.round(bytes / 1e6) + ' MB';
const speechModels = doc => doc.tts.filter(i => !i.pack);
const speechPacks = doc => doc.tts.filter(i => i.pack);
const bySize = list => [...list].sort((a, b) => a.bytes - b.bytes);

// the packs land before any model does - the rail runs one download at a time
function withPacksPresent(doc) {
    const d = JSON.parse(JSON.stringify(doc));
    for (const i of d.tts) {
        if (i.pack) i.present = true;
    }
    return d;
}

// a second model on disk beside the captured one, at the path the capture's own dir implies
function withModelPresent(doc, file) {
    const d = JSON.parse(JSON.stringify(doc));
    const m = d.tts.find(i => i.file === file);
    m.present = true;
    m.path = d.models_dir + '\\' + file;
    return d;
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

test('the offer card points at the config field when no catalog answers', async ({ page }) => {
    await openControl(page, { catalog: { status: 500, json: {} } });
    await expect(page.locator('#tts-offer')).toContainText('no speech model configured');
    await expect(page.locator('#tts-offer')).toContainText('tts field');
});

test('an empty models dir offers the front-end packs before any model', async ({ page }) => {
    const doc = fx('catalog_empty');
    const packs = speechPacks(doc);
    const { posts } = await openControl(page, { catalog: doc });
    const b = page.locator('#tts-offer button');
    // one button, not three: every speech model loads the packs, so they come first
    await expect(b).toHaveCount(1);
    await expect(b).toHaveText('download the front-end packs (' + sizeMB(packs.reduce((a, p) => a + p.bytes, 0)) + ')');
    await b.click();
    expect(lastJson(posts.filter(p => p.path === '/catalog/download')))
        .toEqual({ tower: 'tts', file: packs[0].file });
});

test('the packs on disk turn the card into one button per speech model', async ({ page }) => {
    const doc = withPacksPresent(fx('catalog_empty'));
    const models = speechModels(doc);
    const { posts } = await openControl(page, { catalog: doc });
    await expect(page.locator('#tts-offer button')).toHaveCount(models.length);
    for (const m of models) {
        await expect(page.locator('#tts-offer button', { hasText: modelName(m.file) }))
            .toHaveText('download ' + modelName(m.file) + ' (' + sizeMB(m.bytes) + ')');
    }
    await page.locator('#tts-offer button').first().click();
    expect(lastJson(posts.filter(p => p.path === '/catalog/download')))
        .toEqual({ tower: 'tts', file: models[0].file });
});

test('a speech download in flight renders its progress and holds the buttons back', async ({ page }) => {
    const doc = JSON.parse(JSON.stringify(fx('catalog_empty')));
    const pack = speechPacks(doc)[0];
    doc.download = { state: 'downloading', name: 'tts:' + pack.file, got: Math.round(pack.bytes / 2), total: pack.bytes, error: '' };
    await openControl(page, { catalog: doc });
    await expect(page.locator('#tts-offer'))
        .toContainText('speech (' + pack.file + '): downloading — 50% of ' + sizeMB(pack.bytes));
    await expect(page.locator('#tts-offer button')).toHaveCount(0);
});

test('a downloaded model beside the packs offers enable-speech, which wires and saves', async ({ page }) => {
    const doc = fx('catalog_idle');
    const pick = bySize(speechModels(doc).filter(m => m.present))[0];
    expect(pick).toBeTruthy();   // the capture stocks one speech model - the enable state is what it records
    const { posts } = await openControl(page, { catalog: doc });
    await page.locator('#tts-offer button', { hasText: 'enable speech' }).click();
    await expect(page.locator('#cat-note')).toContainText('speech wired (' + modelName(pick.file) + ')');
    expect(lastJson(posts.filter(p => p.path === '/config')).tts).toBe(pick.path);
    await expect(page.locator('#en-tts')).toBeChecked();
    // a saved wire is one restart from live, so the note offers it
    await page.locator('#cat-note button', { hasText: 'restart now' }).click();
    expect(posts.some(p => p.path === '/restart')).toBe(true);
});

test('several downloaded models become a picker that defaults to the smallest', async ({ page }) => {
    const idle = fx('catalog_idle');
    const absent = bySize(speechModels(idle).filter(m => !m.present))[0];
    const doc = withModelPresent(idle, absent.file);
    const present = bySize(speechModels(doc).filter(m => m.present));
    expect(present.length).toBeGreaterThan(1);
    const { posts } = await openControl(page, { catalog: doc });
    const sel = page.locator('#tts-pick');
    expect(await sel.locator('option').allTextContents()).toEqual(present.map(m => modelName(m.file)));
    await expect(sel).toHaveValue(present[0].file);
    // the picked row is the one the enable wires, not the default
    await sel.selectOption(present[1].file);
    await page.locator('#tts-offer button', { hasText: 'enable speech' }).click();
    expect(lastJson(posts.filter(p => p.path === '/config')).tts).toBe(present[1].path);
});

test('setup mode hides the studio the way it hides the mic', async ({ page }) => {
    const s = fx('stats_tts');
    s.setup = true;
    await openControl(page, { stats: s });
    await expect(page.locator('#tts-section')).toBeHidden();
    await expect(page.locator('#b-mic')).toBeHidden();
});

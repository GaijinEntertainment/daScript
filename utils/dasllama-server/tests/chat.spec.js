// §05 chat: the wire body (knob coercion, tri-state thinking, image parts),
// SSE rendering (reasoning span, tool-call chips, meta), and the error path.

const { test, expect, fx, raw, openControl, lastJson } = require('./fixtures');

const chatBody = posts => lastJson(posts.filter(p => p.path === '/v1/chat/completions'));

async function send(page, text) {
    await page.locator('#chat-text').fill(text);
    await page.locator('#b-send').click();
}

test('send posts the knobs coerced and renders the streamed reply with meta', async ({ page }) => {
    const { posts } = await openControl(page, { sse: raw('sse_chat') });
    await send(page, 'hello there');

    await expect(page.locator('.msg.user')).toHaveText(/hello there/);
    const reply = page.locator('.msg.assistant');
    await expect(reply).toContainText(fx('sse_expected').content);
    await expect(reply.locator('.meta')).toHaveText(/\d+\.\ds/);

    const body = chatBody(posts);
    expect(body.stream).toBe(true);
    expect(body.messages).toEqual([{ role: 'user', content: 'hello there' }]);
    expect(Number.isInteger(body.top_k)).toBe(true);
    expect(Number.isInteger(body.max_tokens)).toBe(true);
    expect(typeof body.temperature).toBe('number');
    // untouched thinking checkbox sends nothing — the family default stands
    expect('enable_thinking' in body).toBe(false);
});

test('touching the thinking checkbox puts enable_thinking on the wire', async ({ page }) => {
    const { posts } = await openControl(page, { sse: raw('sse_chat') });
    await page.locator('#kn-thinking').check();
    await send(page, 'think about it');
    expect(chatBody(posts).enable_thinking).toBe(true);
});

test('reasoning deltas render as the serif think span, separate from content', async ({ page }) => {
    await openControl(page, { sse: raw('sse_think') });
    await send(page, 'why?');
    const reply = page.locator('.msg.assistant');
    await expect(reply.locator('.think')).toContainText(fx('sse_expected').think);
    await expect(reply).toContainText(fx('sse_expected').think_content);
});

test('an error status renders the server message in the reply bubble', async ({ page }) => {
    await openControl(page, {
        responses: { '/v1/chat/completions': { status: 400, json: { error: { message: 'prompt exceeds the context window' } } } },
    });
    await send(page, 'way too long');
    await expect(page.locator('.msg.assistant')).toContainText('[error 400] prompt exceeds the context window');
});

test('an attached image rides only the newest message as content parts', async ({ page }) => {
    const png = Buffer.from(
        'iVBORw0KGgoAAAANSUhEUgAAAAEAAAABCAYAAAAfFcSJAAAADUlEQVR42mP8z8BQDwAEhQGAhKmMIQAAAABJRU5ErkJggg==', 'base64');
    const { posts } = await openControl(page, {
        stats: { ...fx('stats'), vision: true },
        sse: raw('sse_chat'),
    });

    await page.locator('#img-file').setInputFiles({ name: 'dot.png', mimeType: 'image/png', buffer: png });
    await expect(page.locator('#img-strip')).toBeVisible();
    await expect(page.locator('#img-strip')).toContainText('attached to every message');

    await send(page, 'what is this');
    await expect(page.locator('.msg.assistant')).toContainText(fx('sse_expected').content);
    let body = chatBody(posts);
    const last = body.messages[body.messages.length - 1];
    expect(Array.isArray(last.content)).toBe(true);
    expect(last.content[0].type).toBe('image_url');
    expect(last.content[0].image_url.url).toMatch(/^data:image\/png;base64,/);
    expect(last.content[1]).toEqual({ type: 'text', text: 'what is this' });

    // second turn: the sticky image re-attaches to the NEW last message; the
    // earlier turn goes back to plain text so old base64 is never resent
    await send(page, 'and now');
    body = chatBody(posts);
    expect(typeof body.messages[0].content).toBe('string');
    expect(Array.isArray(body.messages[body.messages.length - 1].content)).toBe(true);

    // remove -> next turn is plain text again
    await page.locator('#img-strip button').click();
    await send(page, 'no image now');
    body = chatBody(posts);
    expect(typeof body.messages[body.messages.length - 1].content).toBe('string');
});

test('the reply budget defaults long and rides the wire', async ({ page }) => {
    const { posts } = await openControl(page, { sse: raw('sse_chat') });
    await expect(page.locator('#kn-max_tokens')).toHaveValue('16384');
    await send(page, 'go long');
    expect(chatBody(posts).max_tokens).toBe(16384);
});

test('un-wired towers dim the image and mic buttons; clicking walks to the catalog', async ({ page }) => {
    await openControl(page);   // stock stats: serving, vision false, no asr workers
    const img = page.locator('#b-img'), mic = page.locator('#b-mic');
    await expect(img).toBeVisible();
    await expect(img).toHaveClass(/off/);
    await expect(mic).toBeVisible();
    await expect(mic).toHaveClass(/off/);
    await img.click();
    await expect(page.locator('#cat-note')).toContainText('vision tower');
    await expect(page.locator('#catalog-sec')).toBeInViewport();
    await mic.click();
    await expect(page.locator('#cat-note')).toContainText('ASR tower');
});

test('wired towers light the buttons up', async ({ page }) => {
    await openControl(page, { stats: { ...fx('stats'), vision: true, asr_workers: 1 } });
    await expect(page.locator('#b-img')).not.toHaveClass(/off/);
    await expect(page.locator('#b-mic')).not.toHaveClass(/off/);
});

test('clear resets the conversation', async ({ page }) => {
    await openControl(page, { sse: raw('sse_chat') });
    await send(page, 'first');
    await expect(page.locator('.msg')).toHaveCount(2);
    await page.locator('#b-chat-clear').click();
    await expect(page.locator('.msg')).toHaveCount(0);
    await expect(page.locator('#chat-log')).toContainText('cleared');
});

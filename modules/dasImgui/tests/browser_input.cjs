'use strict';
// Run against a daspkg WASM release of _fixtures/browser_input.
const assert = require('node:assert/strict');
const fs = require('node:fs');
const path = require('node:path');
const http = require('node:http');
const { chromium } = require('playwright');
(async () => {
  assert.ok(process.argv[2], 'Pass the browser_input release directory');
  const root = path.resolve(process.argv[2]);
  const server = http.createServer((req, res) => {
    if(req.url==='/favicon.ico'){res.writeHead(204);res.end();return;}
    const relative = decodeURIComponent(new URL(req.url, 'http://localhost').pathname);
    const file = path.resolve(root, '.' + relative);
    if (!file.startsWith(root + path.sep) || !fs.existsSync(file)) { res.writeHead(404); res.end(); return; }
    res.writeHead(200, {
      'Content-Type': path.extname(file) === '.wasm' ? 'application/wasm' : path.extname(file) === '.js' ? 'application/javascript' : 'text/html',
      'Cross-Origin-Opener-Policy': 'same-origin', 'Cross-Origin-Embedder-Policy': 'require-corp',
    });
    fs.createReadStream(file).pipe(res);
  });
  let browser, page, clipboard, savedClipboard = false;
  try {
    await new Promise(resolve => server.listen(0, '127.0.0.1', resolve));
    const origin = 'http://127.0.0.1:' + server.address().port;
    browser = await chromium.launch({channel:'chrome',headless:true});
    const context = await browser.newContext({viewport:{width:800,height:400}});
    await context.grantPermissions(['clipboard-read','clipboard-write'], {origin});
    page = await context.newPage();
    page.on('console',message=>{if(message.type()==='error')console.error('Browser console:',message.text());});
    const errors=[]; page.on('pageerror', error=>{errors.push(error.message);console.error('Browser exception:',error.stack || error.message);});
    await page.goto(origin+'/browser_input.html');
    await page.waitForFunction(()=>globalThis.Module?.FS?.analyzePath('/input-state.json').exists, {}, {timeout:60000});
    const state=()=>page.evaluate(()=>JSON.parse(Module.FS.readFile('/input-state.json',{encoding:'utf8'})));
    await page.mouse.click(120,100);
    await page.keyboard.type('before',{delay:60});
    await page.waitForFunction(()=>JSON.parse(Module.FS.readFile('/input-state.json',{encoding:'utf8'})).multi==='before');
    await page.keyboard.press('Shift+Enter',{delay:100});
    await page.waitForFunction(()=>JSON.parse(Module.FS.readFile('/input-state.json',{encoding:'utf8'})).multi==='before\n', {}, {timeout:3000});
    console.log('PASS browser Shift+Enter');
    clipboard=await page.evaluate(()=>navigator.clipboard.readText()); savedClipboard=true;
    await page.evaluate(()=>navigator.clipboard.writeText('Привет 世界 😀'));
    await page.evaluate(()=>{window.pasteObserved=false;document.addEventListener('paste',event=>{window.pasteObserved=event.isTrusted;},{once:true});});
    await page.keyboard.press(process.platform==='darwin'?'Meta+V':'Control+V',{delay:100});
    await page.waitForTimeout(250);
    assert.equal(await page.evaluate(()=>window.pasteObserved),true,'trusted browser paste event was delivered');
    assert.equal((await state()).multi,'before\nПривет 世界 😀','browser paste reaches the active UTF-8 editor');
    assert.deepEqual(errors,[]);
    console.log('PASS browser Unicode paste');
    const primary=process.platform==='darwin'?'Meta':'Control';
    await page.keyboard.press(primary+'+Z',{delay:100});
    await page.waitForFunction(()=>JSON.parse(Module.FS.readFile('/input-state.json',{encoding:'utf8'})).multi==='before\n');
    await page.keyboard.press(primary+'+A',{delay:100});
    await page.evaluate(()=>navigator.clipboard.writeText('é e\u0301 😀'));
    await page.keyboard.press(primary+'+V',{delay:100});
    await page.waitForFunction(()=>JSON.parse(Module.FS.readFile('/input-state.json',{encoding:'utf8'})).multi==='é e\u0301 😀');
    await page.evaluate(()=>{
      const input=document.createElement('input');input.id='outside-input';
      input.style.cssText='position:fixed;left:20px;top:350px;z-index:1000';document.body.appendChild(input);
    });
    await page.locator('#outside-input').focus();
    await page.evaluate(()=>navigator.clipboard.writeText('outside form'));
    await page.keyboard.press(primary+'+V',{delay:100});
    assert.equal(await page.locator('#outside-input').inputValue(),'outside form');
    assert.equal((await state()).multi,'é e\u0301 😀','HTML paste must not enter the canvas editor');
    console.log('PASS paste undo, selection replacement, combining text and HTML isolation');
    await page.mouse.click(120,100);
    await page.waitForFunction(()=>document.activeElement?.tagName==='TEXTAREA',{}, {timeout:3000});
    await page.keyboard.press(primary+'+A',{delay:80});
    await page.keyboard.insertText('Unicode 输入 😀');
    await page.waitForFunction(()=>JSON.parse(Module.FS.readFile('/input-state.json',{encoding:'utf8'})).multi==='Unicode 输入 😀',{}, {timeout:3000});
    console.log('PASS browser committed Unicode input');
    const cdp=await context.newCDPSession(page);
    await page.keyboard.press(primary+'+A',{delay:80});
    await cdp.send('Input.imeSetComposition',{text:'にほん',selectionStart:3,selectionEnd:3});
    await page.waitForTimeout(100);
    assert.equal((await state()).multi,'Unicode 输入 😀','preedit must not become committed text');
    await cdp.send('Input.insertText',{text:'日本'});
    await page.waitForFunction(()=>JSON.parse(Module.FS.readFile('/input-state.json',{encoding:'utf8'})).multi==='日本',{}, {timeout:3000});
    await page.keyboard.press(primary+'+A',{delay:80});
    await cdp.send('Input.imeSetComposition',{text:'未確定',selectionStart:3,selectionEnd:3});
    await cdp.send('Input.imeSetComposition',{text:'',selectionStart:0,selectionEnd:0});
    await page.waitForTimeout(100);
    assert.equal((await state()).multi,'日本','cancelled composition must not insert text');
    await page.keyboard.type('x',{delay:80});
    await page.waitForFunction(()=>JSON.parse(Module.FS.readFile('/input-state.json',{encoding:'utf8'})).multi==='x',{}, {timeout:3000});
    console.log('PASS Chromium composition preedit, commit once, cancellation and continued typing');
    await page.evaluate(()=>{
      window.rejectedPastePrevented=false;
      document.addEventListener('paste',event=>{window.rejectedPastePrevented=event.defaultPrevented;},{once:true});
    });
    await page.evaluate(()=>navigator.clipboard.writeText('x'.repeat(1048577)));
    await page.keyboard.press(primary+'+V',{delay:100});
    assert.equal(await page.evaluate(()=>window.rejectedPastePrevented),true,'oversized paste must be cancelled before default DOM insertion');
    assert.equal((await state()).multi,'x');
    await page.evaluate(()=>navigator.clipboard.writeText('valid'));
    await page.keyboard.press(primary+'+A',{delay:80});
    await page.keyboard.press(primary+'+V',{delay:100});
    await page.waitForFunction(()=>JSON.parse(Module.FS.readFile('/input-state.json',{encoding:'utf8'})).multi==='valid');

    await page.keyboard.press(primary+'+A',{delay:80});
    await page.evaluate(()=>navigator.clipboard.writeText('before-copy'));
    await page.keyboard.press(primary+'+C',{delay:80});
    await page.waitForTimeout(200);
    assert.equal(await page.evaluate(()=>navigator.clipboard.readText()),'valid','copy exports the selected text');
    await page.evaluate(()=>navigator.clipboard.writeText('before-cut'));
    await page.keyboard.press(primary+'+X',{delay:80});
    await page.waitForFunction(()=>JSON.parse(Module.FS.readFile('/input-state.json',{encoding:'utf8'})).multi==='');
    assert.equal(await page.evaluate(()=>navigator.clipboard.readText()),'valid','cut exports and removes selection');
    await page.keyboard.press(primary+'+Z',{delay:80});
    await page.waitForFunction(()=>JSON.parse(Module.FS.readFile('/input-state.json',{encoding:'utf8'})).multi==='valid');
    await page.evaluate(()=>Module.FS.writeFile('/input-disable','1'));
    await page.waitForFunction(()=>!JSON.parse(Module.FS.readFile('/input-state.json',{encoding:'utf8'})).enabled);
    await page.locator('textarea[aria-label="Text input"]').focus();
    await page.keyboard.insertText('ignored');
    await page.waitForTimeout(100);
    assert.equal((await state()).multi,'valid','disabled real input rejects DOM commits');
    await page.evaluate(()=>Module.FS.unlink('/input-disable'));
    await page.waitForFunction(()=>JSON.parse(Module.FS.readFile('/input-state.json',{encoding:'utf8'})).enabled);
    await page.mouse.click(120,100);
    await page.keyboard.press(primary+'+A',{delay:80});
    await page.keyboard.insertText('resumed');
    await page.waitForFunction(()=>JSON.parse(Module.FS.readFile('/input-state.json',{encoding:'utf8'})).multi==='resumed');
    await page.evaluate(()=>Module.FS.writeFile('/input-reinit','1'));
    await page.waitForFunction(()=>!Module.FS.analyzePath('/input-reinit').exists);
    assert.equal(await page.locator('textarea[aria-label="Text input"]').count(),1,'reinit removes the previous DOM host');
    await page.mouse.click(120,100);
    await page.keyboard.press(primary+'+A',{delay:80});
    await page.keyboard.insertText('after restart');
    await page.waitForFunction(()=>JSON.parse(Module.FS.readFile('/input-state.json',{encoding:'utf8'})).multi==='after restart');
    console.log('PASS copy/cut, real-input gating and backend reinitialization');
    await page.evaluate(()=>{window.previousEditingHost=document.activeElement;});
    await cdp.send('Input.imeSetComposition',{text:'pending',selectionStart:7,selectionEnd:7});
    await page.mouse.click(120,45);
    await page.waitForFunction(()=>document.activeElement?.tagName==='TEXTAREA'&&JSON.parse(Module.FS.readFile('/input-state.json',{encoding:'utf8'})).active_field==='single');
    await page.keyboard.insertText('probe');
    await page.waitForFunction(()=>JSON.parse(Module.FS.readFile('/input-state.json',{encoding:'utf8'})).single==='probe');
    await page.keyboard.press(primary+'+A',{delay:80});
    await page.keyboard.press('Backspace',{delay:80});
    await page.waitForFunction(()=>JSON.parse(Module.FS.readFile('/input-state.json',{encoding:'utf8'})).single==='');
    const oldField=(await state()).multi;
    await page.evaluate(()=>{
      const old=window.previousEditingHost;
      old.dispatchEvent(new CompositionEvent('compositionend',{data:'late'}));
      old.value='late';
      old.dispatchEvent(new InputEvent('input',{data:'late',inputType:'insertFromComposition'}));
    });
    await page.waitForTimeout(100);
    assert.equal((await state()).single,'','late composition must not enter another editor');
    assert.equal((await state()).multi,oldField,'late composition must not alter the departed editor');
    console.log('PASS focus-change composition isolation');
    assert.deepEqual(errors,[],'all input scenarios must finish without browser exceptions');
    console.log('PASS oversized paste rejection and recovery');
  } catch(error) {
    if(page && !page.isClosed())console.error('Input diagnostic:',await page.evaluate(()=>({
      active:document.activeElement?.tagName,
      bridge:Module['dasClipboardBridge']&&{active:Module['dasClipboardBridge'].active,visible:Module['dasClipboardBridge'].visible,enabled:Module['dasClipboardBridge'].enabled,value:Module['dasClipboardBridge'].host.value},
      receiver:typeof Module['_das_imgui_browser_input_text'],
      state:Module.FS.analyzePath('/input-state.json').exists?Module.FS.readFile('/input-state.json',{encoding:'utf8'}):null
    })).catch(()=>null));
    throw error;
  } finally {
    if (savedClipboard && page && !page.isClosed()) await page.evaluate(text=>navigator.clipboard.writeText(text),clipboard).catch(()=>{});
    if (browser) await browser.close();
    server.closeAllConnections(); await new Promise(resolve=>server.close(resolve));
  }
})().catch(error=>{console.error(error);process.exitCode=1;});

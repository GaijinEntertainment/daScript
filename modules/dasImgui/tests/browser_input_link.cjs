// EMCC=/path/to/emcc node browser_input_link.cjs; requires Playwright and Chrome.
const assert = require('node:assert/strict');
const fs = require('node:fs');
const os = require('node:os');
const path = require('node:path');
const http = require('node:http');
const {execFileSync} = require('node:child_process');
const {chromium} = require('playwright');
(async () => {
  const root = fs.mkdtempSync(path.join(os.tmpdir(), 'imgui-input-link-'));
  let server, browser;
  try {
    const header = fs.readFileSync(path.join(__dirname, '../src/browser_input.h'), 'utf8');
    const begin = header.indexOf('EM_JS_DEPS(');
    const end = header.indexOf('static ImGuiContext*');
    assert.ok(begin >= 0 && end > begin);
    const expected = 'sdk é 雪 😀';
    const literal = [...Buffer.from(expected)].map(b => `\\x${b.toString(16).padStart(2,'0')}`).join('');
    const source = `#include <emscripten.h>\n#include <string.h>\n${header.slice(begin,end)}
      static char received[128]; static int count;
      EMSCRIPTEN_KEEPALIVE void das_imgui_browser_input_text(const char *text) {
        strcpy(received, text); ++count;
      }
      EMSCRIPTEN_KEEPALIVE int probe_matches(void) { return strcmp(received,"${literal}")==0 && count==1; }
      int main(void) {
        das_browser_clipboard_install(sizeof(void*) == 8);
        das_browser_ime_position(1,8,8,16,640,480,7);
        das_browser_clipboard_state(1,1,7);
        return 0;
      }`;
    fs.writeFileSync(path.join(root,'probe.c'),source);
    for (const bits of [32,64]) {
      execFileSync(process.env.EMCC || 'emcc', [path.join(root,'probe.c'),'-O3','-flto=thin','-fwasm-exceptions','-sWASM_LEGACY_EXCEPTIONS=0',
        '-sEXPORTED_RUNTIME_METHODS=callMain,FS','-sEXPORTED_FUNCTIONS=_main,_malloc,_free','-sINVOKE_RUN=0','-sFORCE_FILESYSTEM=1',
        ...(bits===64 ? ['-sMEMORY64=2'] : []),'-o',path.join(root,`probe${bits}.js`)],{stdio:'inherit'});
      fs.writeFileSync(path.join(root,`probe${bits}.html`),`<canvas id="canvas" tabindex="0"></canvas><button id="start">Start</button>
        <script>var Module={canvas:document.getElementById('canvas'),onRuntimeInitialized:()=>{window.sdkReady=true}};document.getElementById('start').onclick=()=>{Module.canvas.focus();Module.callMain([])};</script><script src="probe${bits}.js"></script>`);
    }
    server=http.createServer((req,res)=>{
      const file=path.join(root,path.basename(new URL(req.url,'http://localhost').pathname));
      if(!fs.existsSync(file)||!fs.statSync(file).isFile()){res.writeHead(404);res.end();return;}
      res.setHeader('Content-Type',file.endsWith('.wasm')?'application/wasm':file.endsWith('.js')?'text/javascript':'text/html');
      fs.createReadStream(file).pipe(res);
    });
    await new Promise(r=>server.listen(0,'127.0.0.1',r));
    browser=await chromium.launch({channel:'chrome',headless:true});
    for(const bits of [32,64]){
      const page=await browser.newPage();const errors=[];page.on('pageerror',e=>errors.push(String(e)));
      await page.goto(`http://127.0.0.1:${server.address().port}/probe${bits}.html`);
      await page.waitForFunction(()=>window.sdkReady===true);
      await page.getByRole('button',{name:'Start'}).click();
      await page.locator('textarea').fill(expected);
      assert.deepEqual(errors,[],`wasm${bits} callback errors`);
      assert.equal(await page.evaluate(()=>Module._probe_matches()),1,`wasm${bits} receives the allocated UTF-8 pointer`);
      await page.close();
      console.log(`PASS wasm${bits} minimal runtime exports and UTF-8 callback`);
    }
  } finally {
    if(browser)await browser.close();
    if(server)await new Promise(r=>server.close(r));
    fs.rmSync(root,{recursive:true,force:true});
  }
})().catch(e=>{console.error(e);process.exitCode=1;});

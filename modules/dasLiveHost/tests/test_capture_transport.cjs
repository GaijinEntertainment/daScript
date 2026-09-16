const assert = require('node:assert/strict');
const fs = require('node:fs');
const vm = require('node:vm');
const path = require('node:path');
const test = require('node:test');
const files = new Map(), timers = new Map();
let timerId = 0;
const element = () => ({style:{},append(){},remove(){},setAttribute(){},addEventListener(){},focus(){},replaceChildren(){},getContext(){throw Error('No GPU in transport test');},width:640,height:480});
const canvas = element();
const context = {console:{warn(){},error(){}},window:{addEventListener(){}},document:{createElement:element,body:element(),getElementById(){return canvas},querySelector(){return canvas}},Module:{calledRun:true,FS:{mkdirTree(){},writeFile(path,value){files.set(path,value)},readFile(path){if(!files.has(path))throw Error('missing');return files.get(path)},analyzePath(path){return{exists:files.has(path)}},unlink(path){files.delete(path)}}},setTimeout(fn,delay){const id=++timerId;timers.set(id,{fn,delay});return id},clearTimeout(id){timers.delete(id)},navigator:{userAgent:'test',platform:'test'},location:{pathname:'/'},devicePixelRatio:1,performance:{now(){return 0}},Date,JSON};
vm.runInNewContext(fs.readFileSync(path.join(__dirname, '../web/live_capture.js'),'utf8'),context);
const api=context.window.dasLiveCapture;
function tick(delay){const pair=[...timers].find(([,x])=>x.delay===delay);assert.ok(pair);timers.delete(pair[0]);pair[1].fn();}
test('capture transport correlates replies and accepts native requests', async()=>{
 await assert.rejects(api.restore({}),/not a live report/);
 const first=api.capture();const timedOut=assert.rejects(first,/20 seconds/);
 await assert.rejects(api.capture(),/already running/);
 const oldId=JSON.parse(files.get('/live-capture/request.json')).id;
 tick(20000);await timedOut;
 const second=api.capture();const newId=JSON.parse(files.get('/live-capture/request.json')).id;
 files.set('/live-capture/response.json',JSON.stringify({id:oldId,operation:'capture',snapshot:{frame:1,build:'fixture'}}));
 tick(80);
 assert.equal(api.current,null,'a timed-out reply must not replace the current report');
 files.set('/live-capture/response.json',JSON.stringify({id:newId,operation:'capture',snapshot:{frame:2,build:'fixture'}}));
 tick(80);assert.equal((await second).snapshot.frame,2);assert.equal(files.has('/live-capture/response.json'),false);
 files.set('/live-capture/response.json',JSON.stringify({id:'button-3',operation:'capture',snapshot:{frame:3,build:'fixture'}}));
 tick(80);assert.equal(api.current.snapshot.frame,3,'native UI capture remains available');
 context.Module.calledRun=false;await assert.rejects(api.capture(),/still loading/);
});

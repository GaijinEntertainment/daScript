/* Transport/UI adapter only. Providers, validation and replay run through daslang live commands. */
(function () {
  for (const type of ['keydown','keypress']) window.addEventListener(type,e=>{
    if(e.target.closest?.('#live-report-dialog'))e.stopImmediatePropagation();
  },true);
  const root='/live-capture', pending=new Map(), logs=[];
  let serial=0,current=null,panel=null,message=null,list=null;
  const retain=(kind,args)=>{logs.push({time:performance.now(),kind,text:args.map(x=>String(x)).join(' ').slice(0,4096)});if(logs.length>100)logs.shift();};
  for(const kind of ['warn','error']){const original=console[kind];console[kind]=function(...args){retain(kind,args);return original.apply(console,args);};}
  window.addEventListener('error',e=>retain('error',[e.message,e.filename,e.lineno]));
  window.addEventListener('unhandledrejection',e=>retain('rejection',[e.reason]));
  function filesystem(){return typeof Module!=='undefined'&&Module.calledRun?Module.FS:null;}
  function send(operation,report){
    const fs=filesystem();if(!fs)return Promise.reject(Error('The preview is still loading.'));
    if(pending.size)return Promise.reject(Error('A capture or restore is already running.'));
    const id='web-'+(++serial);
    fs.mkdirTree(root);fs.writeFile(root+'/request.json',JSON.stringify({id,operation,report}));
    return new Promise((resolve,reject)=>{const timer=setTimeout(()=>{pending.delete(id);reject(Error('The preview did not answer within 20 seconds.'));},20000);pending.set(id,{resolve,reject,timer});});
  }
  function bytesURL(bytes){let binary='';for(let i=0;i<bytes.length;i+=32768)binary+=String.fromCharCode(...bytes.subarray(i,i+32768));return 'data:image/png;base64,'+btoa(binary);}
  function browserInfo(){
    const canvas=document.getElementById('canvas');let gpu={};
    try{const gl=canvas.getContext('webgl2')||canvas.getContext('webgl');const ext=gl.getExtension('WEBGL_debug_renderer_info');gpu={renderer:gl.getParameter(ext?ext.UNMASKED_RENDERER_WEBGL:gl.RENDERER),vendor:gl.getParameter(ext?ext.UNMASKED_VENDOR_WEBGL:gl.VENDOR),version:gl.getParameter(gl.VERSION)};}catch(e){gpu={error:String(e)};}
    return {user_agent:navigator.userAgent,platform:navigator.platform,device_pixel_ratio:devicePixelRatio,canvas:[canvas.width,canvas.height],page:location.pathname,gpu};
  }
  function makeReport(reply){
    const fs=filesystem();let image=null;
    if(reply.image){try{image={kind:'scene-framebuffer',data_url:bytesURL(fs.readFile(reply.image))};}catch(e){retain('error',[e]);}}
    return {schema:'daslang.live.report/1',created_at:new Date().toISOString(),description:'',snapshot:reply.snapshot,image,browser:browserInfo(),console:logs.slice(),input:(window.dasPreviewInputHistory||[]).slice()};
  }
  async function capture(){const reply=await send('capture');return current;}
  async function restore(report){
    if(!report||report.schema!=='daslang.live.report/1')throw Error('This is not a live report bundle.');
    const reply=await send('restore',{snapshot:report.snapshot});
    if(!reply.result?.ok)throw Error(reply.result?.error||'Restore failed.');
    close();return reply.result;
  }
  async function upload(report){
    const response=await fetch('/api/reports',{method:'POST',headers:{'Content-Type':'application/json'},body:JSON.stringify(report),signal:AbortSignal.timeout(15000)});
    const result=await response.json();if(!response.ok||!result.ok)throw Error(result.error||'Upload failed.');return result;
  }
  function download(report){
    const url=URL.createObjectURL(new Blob([JSON.stringify(report,null,2)],{type:'application/json'}));
    const a=document.createElement('a');a.href=url;a.download='live-report-'+(report.server_id||Date.now())+'.json';a.click();setTimeout(()=>URL.revokeObjectURL(url),1000);
  }
  function close(){if(panel)panel.remove();panel=null;document.querySelector('canvas').focus();}
  function button(label,fn){const b=document.createElement('button');b.textContent=label;b.type='button';b.onclick=async()=>{b.disabled=true;try{await fn();}catch(e){if(message)message.textContent=String(e.message||e)+' You can still download the bundle.';}finally{b.disabled=false;}};return b;}
  function open(report){
    if(panel)panel.remove();current=report||null;
    panel=document.createElement('section');panel.id='live-report-dialog';panel.setAttribute('role','dialog');panel.setAttribute('aria-label','Live bug report');panel.addEventListener('keydown',e=>e.stopPropagation());panel.addEventListener('keyup',e=>e.stopPropagation());
    Object.assign(panel.style,{position:'fixed',inset:'5vh 10vw',zIndex:10000,overflow:'auto',background:'#171c24',color:'#eee',padding:'24px',border:'1px solid #75808f',borderRadius:'10px',font:'14px system-ui',boxShadow:'0 15px 70px #000b'});
    const title=document.createElement('h2');title.textContent=report?'Captured live report':'Saved live reports';panel.append(title);
    message=document.createElement('p');message.id='live-report-status';message.setAttribute('role','status');panel.append(message);
    if(report){
      const info=document.createElement('p');info.textContent='Frame '+report.snapshot.frame+' · Build '+report.snapshot.build;panel.append(info);
      if(report.image){const image=document.createElement('img');image.src=report.image.data_url;image.alt='Captured scene';Object.assign(image.style,{display:'block',maxWidth:'100%',maxHeight:'38vh',objectFit:'contain'});panel.append(image);}
      const note=document.createElement('textarea');note.id='live-report-description';note.maxLength=4096;note.placeholder='What went wrong?';note.setAttribute('aria-label','Bug description');note.value=report.description||'';Object.assign(note.style,{display:'block',width:'95%',minHeight:'65px',margin:'12px 0'});panel.append(note);
      panel.append(button('Upload report',async()=>{report.description=note.value;const result=await upload(report);report.server_id=result.id;message.textContent='Saved '+result.id+' — give this ID to your assistant.';}));
      panel.append(button('Download bundle',()=>{report.description=note.value;download(report);}));
      panel.append(button('Restore this capture',()=>restore(report)));
      if(report.server_id)message.textContent='Report '+report.server_id;
    }
    panel.append(button('Capture current scene',()=>capture()));
    panel.append(button('Browse reports',()=>loadList()));
    const file=document.createElement('input');file.type='file';file.accept='.json,application/json';file.setAttribute('aria-label','Load report file');file.onchange=async()=>{try{const report=JSON.parse(await file.files[0].text());open(report);}catch(e){message.textContent='Could not read report: '+e.message;}};panel.append(file);
    panel.append(button('Close',()=>close()));
    list=document.createElement('div');list.id='live-report-list';panel.append(list);
    document.body.append(panel);
  }
  async function loadList(){
    if(!panel)open(null);
    const response=await fetch('/api/reports');if(!response.ok)throw Error('Report backend is unavailable.');
    const reports=await response.json();list.replaceChildren();
    for(const meta of reports.reverse())list.append(button(meta.id+' — '+(meta.description||'No description'),async()=>{const r=await fetch('/api/report?id='+encodeURIComponent(meta.id));if(!r.ok)throw Error('Report not found');const report=await r.json();report.server_id=meta.id;open(report);}));
    if(!reports.length)list.textContent='No reports yet.';
  }
  function poll(){
    const fs=filesystem();if(fs){try{
      const path=root+'/response.json';if(fs.analyzePath(path).exists){
        const reply=JSON.parse(fs.readFile(path,{encoding:'utf8'}));fs.unlink(path);
        const wait=pending.get(reply.id);
        const nativeCapture=reply.operation==='capture'&&typeof reply.id==='string'&&reply.id.startsWith('button-');
        if(wait||nativeCapture){
          if(reply.operation==='capture'){current=makeReport(reply);open(current);}
          if(wait){clearTimeout(wait.timer);pending.delete(reply.id);wait.resolve(reply);}
        }
      }
    }catch(e){retain('transport',[e]);}}
    setTimeout(poll,80);
  }
  const launcher=button('Reports',()=>loadList());launcher.id='live-report-launcher';Object.assign(launcher.style,{position:'fixed',right:'12px',bottom:'12px',zIndex:9000,padding:'8px 14px'});document.body.append(launcher);
  window.addEventListener('keydown',e=>{if(e.key==='F8'){e.preventDefault();capture().catch(err=>{open(null);message.textContent=err.message;});}});
  window.dasLiveCapture={capture,restore,upload,download,list:loadList,get current(){return current;}};
  poll();
})();

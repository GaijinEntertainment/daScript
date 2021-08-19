options indenting = 4
options no_global_variables = false
options no_unused_block_arguments = false
options no_unused_function_arguments = false
options persistent_heap = true
options persistent_string_heap = true
module debug shared

require strings
require rtti
require debugapi
require fio
require network

require daslib/json
require daslib/json_boost
require daslib/defer
require daslib/apply_in_context
require daslib/jobque_boost
require daslib/strings_boost
require daslib/dap

require uriparser

let
    LOCAL_VARS = 1
    ARGUMENT_VARS = 2

def ctx_id(var ctx: Context): uint64
    unsafe
        return intptr(addr(ctx))

// class DAWalker: DapiDataWalker

//     def override Float(var value:float&): void
//         print("[float] {value}")

//     def override Int(var value:int&): void
//         print("[int] {value}")


struct DAVariable
    name: string
    value: string


struct DAStackFrame
    name: string
    path: string
    line: uint = 1u
    variables: array<DAVariable>
    arguments: array<DAVariable>


class DAStackWalker: DapiStackWalker
    data_walker_adapter: smart_ptr<DataWalker>
    // data_walker: DAWalker?

    stack: array<DAStackFrame>

    def DAStackWalker()
        pass
        // data_walker = new DAWalker()
        // unsafe
        //     data_walker_adapter <- make_data_walker(data_walker)

    def finalize()
        unsafe
            delete data_walker_adapter
            // delete data_walker

    def override onCallAOT(pp:Prologue; fileName:string#): void
        // print("AOT {fileName}\n")
        stack |> emplace([[DAStackFrame() name="aot {pp.info.name}", path = "{fileName}" ]])
        pass

    def override onCallAt(pp:Prologue; info:FuncInfo; at:LineInfo): void
        // print("def {info.name} at {at}\n")
        stack |> emplace([[DAStackFrame name="def {info.name}", path="{at.fileInfo.name}", line=at.line ]])
        pass

    def override onCall(pp:Prologue; info:FuncInfo): void
        // print("def {info.name}\n")
        stack |> emplace([[DAStackFrame() name="def {info.name}" ]])
        pass

    def override onVariable(inf: FuncInfo; vinfo: LocalVariableInfo; arg: void?; inScope: bool): void
        if length(stack) > 0
            let value = !inScope ? "<out of scope>" : arg != null ? sprint_data(arg, type_info(vinfo), print_flags humanReadable) : "<optimized>"
            stack[length(stack)-1].variables |> emplace([[DAVariable name="{vinfo.name}", value=value]])

    def override onArgument (inf: FuncInfo; index: int; vinfo: VarInfo; arg: float4)
        if length(stack) > 0
            let value = sprint_data(arg, type_info(vinfo), print_flags humanReadable)
            stack[length(stack)-1].arguments |> emplace([[DAVariable name="{vinfo.name}", value=value]])


struct DABreakpoint
    id: uint64
    line: uint

typedef
    DABreakpoints = table<string; array<DABreakpoint>>


struct DAContext
    ctx: Context?


class DAgent: DapiDebugAgent

    walker_adapter: smart_ptr<StackWalker>
    walker: DAStackWalker?
    server: DAServer?

    breakpoints: DABreakpoints
    breakpointId: uint64 = 1ul

    contexts: table<uint64; DAContext>

    continueRequested: bool = false
    pauseRequested: bool = false

    def DAgent()
        walker = new DAStackWalker()
        unsafe
            walker_adapter <- make_stack_walker(walker)
        self->startServer()
        self->addContext(this_context())

    def startServer()
        let args <- get_command_line_arguments()
        debug(args)
        let fn = args[1]
        let main_time = stat(fn).mtime
        print("server at {fn} {main_time}\n")
        let port = 9000
        server = new DAServer()
        // var session : smart_ptr<NetworkServer> <- gc0_restore_smart_ptr("telnet-session")
        // if session != null
        //   print("restoring server after restart\n")
        //   server->restore(session)
        // elif server->init(port)
        unsafe
            server.agent = addr(self)
        if server->init(port)
            print("sever at port {port}\n")
        else
            print("server failed to initialize\n")
            unsafe
                delete server

    def finalize()
        unsafe
            delete walker_adapter
            delete walker
            delete server

    def override onBreakpoint(var ctx:Context; at:LineInfo): void
        server->log("{ctx} breakpoint at line {int(at.line)}\n")
        set_single_step(ctx,true)

    def override onSingleStep(var ctx:Context; at:LineInfo): void
        // unsafe
        //     self->log("{double(ctx_id(ctx))} step at {at.fileInfo.name}:{int(at.line)}\n")

        if server != null && !server.configurationDone
            // if string(at.fileInfo.name) |> find("debug_test.das") >= 0
            if (uint(ctx.category) & (1u << 31u)) != 0u
                // self->log("!!! wait debug_test.das in context {ctx} {uint(ctx.category)}")
                while !server.configurationDone
                    // self->log("wait server")
                    tick_debug_agent()
                    sleep(1u)

        if pauseRequested && !ctx.category.thread_clone
            self->sendStopped(ctx, at, "pause", -1ul)
            while pauseRequested
                tick_debug_agent()
                sleep(1u)

        let file = string(at.fileInfo.name)
        var prevBreak = -1ul
        var inBreak = true
        while inBreak
            inBreak = false
            if !key_exists(breakpoints, file)
                break
            let brs = breakpoints |> find(file)
            // self->log("search break {i} `{file}` {breakpoints[file]} in {at.fileInfo.name}\n")
            for brIdx in iter_range(*brs)
                let br & = (*brs)[brIdx]
                if br.line == uint(at.line)
                    if continueRequested && prevBreak == br.id
                        continueRequested = false
                        continue

                    inBreak = true

                    if prevBreak != br.id
                        self->sendStopped(ctx, at, "breakpoint", br.id)

                    prevBreak = br.id
                    tick_debug_agent()
                    sleep(1u)
                    break

        // walker.stack |> emplace([[DAStackFrame name="{at.fileInfo.name}", path="{at.fileInfo.name}", line=at.line ]], 0)

    def sendStopped(var ctx:Context; at:LineInfo; reason: string; brId: uint64)
        server->log("<< stopped `{reason}` at {at.fileInfo.name}:{int(at.line)}")
        self->addContext(ctx)

        delete walker.stack
        walk_stack(walker_adapter, ctx, at)
        if length(walker.stack) > 0
            for i in range(1, length(walker.stack))
                let j = length(walker.stack) - 1
                let q = j - 1
                walker.stack[j].path = walker.stack[q].path
                walker.stack[j].line = walker.stack[q].line
            walker.stack[0].path = "{at.fileInfo.name}"
            walker.stack[0].line = at.line
        let file = string(at.fileInfo.name)
        unsafe
            server->sendStopped(ctx, file, reason, brId)
            
    def override onTick()
        if server != null
            if !server.done
                server->tick()
            else
                unsafe
                    delete server
                self->startServer()
            // let new_time = stat(fn).mtime
            // if new_time != main_time
            //   print("restarting via source change {new_time}\n")
            //   server->restart()
            //   break
        // walker.stack |> clear()

    def override onCreateContext(var ctx: Context): void
        self->addContext(ctx)

    def addContext(var ctx: Context): void
        unsafe
            var ctxAddr = addr(ctx)
            contexts[ctx_id(ctx)] <- [[DAContext ctx=ctxAddr]]

    def override onDestroyContext(var ctx: Context): void
        unsafe
            var ctxAddr = addr(ctx)
            contexts |> erase(ctx_id(ctx))


class DAServer: Server
    [[do_not_delete]] agent: DAgent?

    done: bool = false

    // message data
    current_string: array<uint8>
    toRead: int = 0

    initialized: int = -1 // initialized event: ticks delay
    configurationDone: bool = false

    def DAServer
        Server`Server(cast<Server> self)

    def override onError(msg:string; code:int)
        print("server error:{code} - {msg}\n")

    def override onConnect()
        print("connected\n")

    def override onDisconnect()
        if length(current_string) > 0
            print(string(current_string))
            current_string |> clear()
        print("disconnected\n")

    def restart()
        if _server != null
            var session : smart_ptr<NetworkServer>
            self->save(session)
            gc0_save_smart_ptr("telnet-session", session)

    def override tick()
        Server`tick(self)
        if initialized > 0
            initialized -= 1
            if initialized == 0
                self->sendEvent("initialized", null)

    def override onData(message:uint8?; size:int)
        for i in range(size)
            var uch : uint8
            unsafe
                uch = message[i]
            let ch = int(uch)
            if toRead == 0
                push(current_string, uch)
                let prefix = "Content-Length: "
                if length(current_string) <= length(prefix)
                    continue
                if ch != '\n'
                    continue
                var str = string(current_string)
                if str |> starts_with(prefix)
                    unsafe
                        var lenStr = chop(str, 16, length(current_string) - length(prefix) - 2) // \r\n length
                        toRead = to_int(lenStr)
                        clear(current_string)
                continue
            if ch == '\n' || ch == '\r'
                continue
            push(current_string, uch)
            if length(current_string) < toRead
                continue
            toRead = 0
            // print(string(current_string))
            var error : string
            var msg = json::read_json(current_string, error)
            defer <| $
                unsafe
                    delete msg
            clear(current_string)
            self->log(">>>\n\n{write_json(msg)}\n\n")

            if msg.value is _object
                let seq = msg |> jon("seq", 0lf)
                let typ = msg |> jos("type", "n/a")
                self->log("type {typ}\n")
                if typ == "request"
                    let command = msg |> jos("command", "n/a")
                    self->log("command {command}\n")
                    if command == "initialize"
                        self->reqInitialize(seq, command, msg)
                        return
                    elif command == "configurationDone"
                        self->reqConfigurationDone(seq, command, msg)
                        return
                    elif command == "disconnect"
                        self->reqDisconnect(seq, command, msg)
                        return
                    elif command == "terminate"
                        self->reqTerminate(seq, command, msg)
                        return
                    elif command == "launch"
                        self->reqLaunch(seq, command, msg)
                        return
                    elif command == "setBreakpoints"
                        self->reqSetBreakpoints(seq, command, msg)
                        return
                    elif command == "threads"
                        self->reqThreads(seq, command, msg)
                        return
                    elif command == "stackTrace"
                        self->reqStackTrace(seq, command, msg)
                        return
                    elif command == "scopes"
                        self->reqScopes(seq, command, msg)
                        return
                    elif command == "variables"
                        self->reqVariables(seq, command, msg)
                        return
                    elif command == "continue"
                        self->reqContinue(seq, command, msg)
                        return
                    elif command == "pause"
                        self->reqPause(seq, command, msg)
                        return

            self->log("[Error] unknown message\n{write_json(msg)}\n")

    def reqInitialize(seq: double; command: string; data: JsonValue?)
        var ini = InitializeRequest(data)
        defer_delete(ini)
        var res = [[Capabilities
                                supportsConfigurationDoneRequest=true,
                                supportsRestartRequest=false,
                                supportTerminateDebuggee=true,
                                supportsTerminateRequest=true,
                                supportsExceptionOptions=false,
                                supportsExceptionFilterOptions=false
                                // supportsDelayedStackTraceLoading=true
                    ]]
        defer_delete(res)
        self->sendSuccessResponse(seq, command, JV(res))
        initialized = 100
        // self->sendEvent("initialized")

    def reqConfigurationDone(seq: double; command: string; data: JsonValue?)
        configurationDone = true
        self->sendSuccessResponse(seq, command, null)

    def reqLaunch(seq: double; command: string; data: JsonValue?)
        self->sendSuccessResponse(seq, command, null)

    def reqDisconnect(seq: double; command: string; data: JsonValue?)
        // var ini = DisconnectRequest(data)
        // defer_delete(ini)
        self->sendSuccessResponse(seq, command, null)
        // ini.arguments.restart
        done = true // force restart server

    def reqTerminate(seq: double; command: string; data: JsonValue?)
        self->sendSuccessResponse(seq, command, null)
        fio::exit(0)

    def reqSetBreakpoints(seq: double; command: string; data: JsonValue?)
        var ini <- SetBreakpointsRequest(data)
        defer_delete(ini)
        var res: SetBreakpointsResponse
        defer_delete(res)
        let path = ini.arguments.source.path
        if key_exists(agent.breakpoints, path)
            delete agent.breakpoints[path]
        agent.breakpoints |> erase(path)
        if length(ini.arguments.breakpoints) >= 0
            var breaks <- [[array<DABreakpoint>]]
            for b in ini.arguments.breakpoints
                breaks |> emplace([[DABreakpoint line=uint(b.line), id=agent.breakpointId ]])
                res.breakpoints |> emplace([[Breakpoint
                    verified=true,
                    id=double(agent.breakpointId),
                    source=ini.arguments.source,
                    line=b.line,
                    message="breakpoint #{int(agent.breakpointId)} {path}:{int(b.line)}"
                ]])
                agent.breakpointId += 1ul
            agent.breakpoints[path] <- breaks
        self->sendSuccessResponse(seq, command, JV(res))

    def reqThreads(seq: double; command: string; data: JsonValue?)
        var res: ThreadsResponseBody
        defer_delete(res)
        for ctxId, ctx in keys(agent.contexts), values(agent.contexts)
            let name = "{ctxId} `{ctx.ctx.name}` {ctx.ctx.category} ({int64(ctxId)})"
            res.threads |> emplace([[Thread id=double(ctxId), name=name ]])
        self->sendSuccessResponse(seq, command, JV(res))

    def reqStackTrace(seq: double; command: string; data: JsonValue?)
        var ini <- StackTraceRequest(data)
        defer_delete(ini)
        var res: StackTraceResponseBody
        defer_delete(res)
        for idx, line in range(ini.arguments.levels), agent.walker.stack
            res.stackFrames |> emplace([[StackFrame
                id=double(idx),
                name=line.name,
                line=double(line.line),
                column=1lf,
                source=[[Source name=line.path, path=line.path]]
            ]])
        res.totalFrames = double(length(agent.walker.stack))
        self->sendSuccessResponse(seq, command, JV(res))

    def reqScopes(seq: double; command: string; data: JsonValue?)
        var ini <- ScopesRequest(data)
        defer_delete(ini)
        var res: ScopesResponseBody
        defer_delete(res)
        let frameId = int(ini.arguments.frameId)
        if frameId >= 0 && frameId < length(agent.walker.stack)
            let line & = agent.walker.stack[frameId]
            if length(line.variables) > 0
                res.scopes |> emplace([[Scope
                    name="Locals",
                    variablesReference=double(frameId * 10 + LOCAL_VARS)
                ]])
            if length(line.arguments) > 0
                res.scopes |> emplace([[Scope
                    name="Arguments",
                    variablesReference=double(frameId * 10 + ARGUMENT_VARS)
                ]])
        self->sendSuccessResponse(seq, command, JV(res))

    def reqVariables(seq: double; command: string; data: JsonValue?)
        var ini <- VariablesRequest(data)
        defer_delete(ini)
        var res: VariablesResponseBody
        defer_delete(res)
        let frameId = int(ini.arguments.variablesReference / 10lf)
        let kind = ini.arguments.variablesReference % 10lf
        if frameId >= 0 && frameId < length(agent.walker.stack)
            let line & = agent.walker.stack[frameId]
            let vars & = kind == double(LOCAL_VARS) ? line.variables : line.arguments
            for v in vars
                res.variables |> emplace([[Variable
                    name=v.name,
                    value=v.value
                    // variablesReference=ini.arguments.variablesReference
                ]])
        self->sendSuccessResponse(seq, command, JV(res))

    def reqContinue(seq: double; command: string; data: JsonValue?)
        // var ini <- ContinueRequest(data)
        // defer_delete(ini)
        agent.continueRequested = true
        agent.pauseRequested = false
        self->sendSuccessResponse(seq, command, null)

    def reqPause(seq: double; command: string; data: JsonValue?)
        // var ini <- PauseArguments(data)
        // defer_delete(ini)
        agent.pauseRequested = true
        self->sendSuccessResponse(seq, command, null)

    def log(msg: string)
        print("{msg}\n")
        // self->sendOutput("console", msg)
        self->sendOutput("console", msg)

    def sendOutput(category: string; output: string)
        var res = [[OutputEventBody category=category, output=output ]]
        defer_delete(res)
        self->sendEvent("output", JV(res))

    def sendEvent(event: string; body: JsonValue?)
        self->sendRawData(JV({{"type"=>JV("event"); "event"=>JV(event); "body"=>body}}))

    def sendSuccessResponse(id: double; command: string; body: JsonValue?)
        self->sendRawData(JV({{"request_seq"=>JV(id); "type"=>JV("response");
                                "body"=>body; "command"=>JV(command); "success"=>JV(true)}}))

    // https://microsoft.github.io/language-server-protocol/specifications/specification-current/#responseMessage
    def sendRawData(var data : JsonValue?)
        var msg = write_json(data)
        var packStr = "Content-Length: {length(msg)}\r\n\r\n{msg}"
        print("<------\n{packStr}\n-------\n")
        print("<---- send data {length(packStr)}\n")
        unsafe
            self->send(reinterpret<uint8?> packStr, length(packStr))
            delete data

    def sendStopped(var ctx: Context; path: string; reason: string; brId: uint64)
        self->sendEvent("stopped", JV({{
            "reason"=>JV(reason);
            "threadId"=>JV(double(ctx_id(ctx)));
            "hitBreakpointIds"=>brId == -1ul ? null : JV([{auto[] JV(brId)}])
        }}))


[export,unused_argument(ctx)]
def private start_debug_agent(ctx: Context)
    install_new_debug_agent(new DAgent(), "debug")


[export]
def start_agent(force:bool = false): bool
    if force || !has_debug_agent_context("debug")
        fork_debug_agent_context(@@start_debug_agent)
        print("install debug agent\n")
    else
        print("debug agent already exists\n")
    set_single_step(this_context(), true)
    return true


[init,export]
def auto_start_debug()
    if !is_in_debug_agent_creation()
        this_context().category |= bitfield(1u << 31u) // TODO:
        if !has_debug_agent_context("debug")
            if start_agent()
                new_thread <| @
                    while true
                        tick_debug_agent()
                        sleep(1u)
        set_single_step(this_context(), true)

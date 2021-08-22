options indenting = 4
options no_global_variables = false
options no_unused_block_arguments = false
options no_unused_function_arguments = false
options persistent_heap = true
options persistent_string_heap = true
module debug shared

require math
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


let
    LOCAL_VARS = 1ul
    ARGUMENT_VARS = 2ul
    GLOBAL_VARS = 3ul

    print_flags_debug = (
            print_flags escapeString
        |   print_flags namesAndDimensions
        // |   print_flags humanReadable
        // |   print_flags typeQualifiers
        // |   print_flags refAddresses
    )

def ctx_at(var ctx: Context): DAContextAt
    unsafe
        return intptr(addr(ctx))

def get_int_arg(args: array<string>; name: string; def_val: int): int
    let idx = find_index(args, name)
    return idx >= 0 && idx + 1 < length(args) ? int(args[idx + 1]) : def_val

// from dascript-vscode plugin server\das\completion.das
def module_name(mod)
    return mod == null || length(mod.name) == 0 ? "" : "{mod.name}::"

def get_type_name(info)
    var res: string
    if info.basicType == Type tHandle
        res = info.annotation != null ? "{module_name(info.annotation._module)}{info.annotation.name}" : "tHandle"
    elif info.basicType == Type tStructure
        res = info.structType?.name ?? "structure"
    elif info.basicType == Type tEnumeration || info.basicType==Type tEnumeration8 || info.basicType==Type tEnumeration16
        res = info.enumType?.name ?? "enumeration"
    elif info.basicType == Type tPointer
        res = info.firstType != null ? "{get_type_name(*(info.firstType))}?" : "void?"
    elif info.basicType == Type tArray
        res = info.firstType != null ? "array<{get_type_name(*(info.firstType))}>" : "array"
    elif info.basicType == Type tTable
        if info.firstType != null && info.secondType!=null
            res = "table<{get_type_name(deref(info.firstType))};{get_type_name(deref(info.secondType))}>"
        else
            res = "table"
    elif info.basicType == Type tTuple
        if info.argTypes != null
            var args <- [{for argtype in arg_types(info); get_type_name(argtype)}]
            res = "tuple<" + join(args,";") + ">"
            delete args
        else
            res = "tuple"
    elif info.basicType == Type tVariant
        if info.argTypes != null && info.argNames != null
            res = "variant<"
            var variantDesc = "variant<"
            var f = true
            for argname, argtype in arg_names(info), arg_types(info)
                if !f
                    res += "; "
                f = false
                var desc = "{argname}: {get_type_name(argtype)}"
                res += desc
                variantDesc += "\n\t{desc}"
            res += ">"
            variantDesc += "\n>"
        else
            res = "variant"
    elif info.basicType == Type tFunction || info.basicType == Type tLambda || info.basicType == Type tBlock
        res = "{get_das_type_name(info.basicType)}<"
        if info.argTypes != null
            var args <- [{for argtype in arg_types(info); get_type_name(argtype)}]
            res += "(" + join(args,"; ") + ")"
            delete args
        if info.firstType != null
            res += (info.argTypes != null ? ":" : "") + "{get_type_name(*(info.firstType))}"
        res += ">"
    elif info.basicType == Type fakeContext
        res = "__context__"
    elif info.basicType == Type fakeLineInfo
        res = "__lineInfo__"
    else
        res = get_das_type_name(info.basicType)
    var dim <- [{for d in each_dim(info); d != -1 ? "[{d}]" : "[]"}]
    res += join(dim, "")
    delete dim
    if info.isConst
        res += " const"
    if info.isRef
        res += " &"
    if info.isTemp
        res += "#"
    if info.isImplicit
        res += " implicit"
    return res


// class DAWalker: DapiDataWalker

//     def override Float(var value:float&): void
//         print("[float] {value}")

//     def override Int(var value:int&): void
//         print("[int] {value}")


struct DAVariable
    name: string
    value: string
    _type: string


struct DAStackFrame
    name: string
    path: string
    line: uint = 1u
    variables: array<DAVariable>
    arguments: array<DAVariable>


def fix_var_value(val: string)
    return val |> replace("\n", " ") |> replace("     ", " ") |> replace("    ", " ")


class DAStackWalker: DapiStackWalker
    data_walker_adapter: smart_ptr<DataWalker>
    // data_walker: DAWalker?

    [[do_not_delete]] ctx: DAContext?

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
        ctx.stack |> emplace([[DAStackFrame() name="aot {pp.info.name}", path = "{fileName}" ]])
        pass

    def override onCallAt(pp:Prologue; info:FuncInfo; at:LineInfo): void
        // print("def {info.name} at {at}\n")
        ctx.stack |> emplace([[DAStackFrame name="def {info.name}", path="{at.fileInfo.name}", line=at.line ]])
        pass

    def override onCall(pp:Prologue; info:FuncInfo): void
        // print("def {info.name}\n")
        ctx.stack |> emplace([[DAStackFrame() name="def {info.name}" ]])
        pass

    def override onVariable(inf: FuncInfo; vinfo: LocalVariableInfo; arg: void?; inScope: bool): void
        let stackLen = length(ctx.stack)
        if stackLen > 0
            let ti = type_info(vinfo)
            let value = !inScope ? "<out of scope>" : arg != null ? sprint_data(arg, ti, print_flags_debug)  |> fix_var_value(): "<optimized>"
            ctx.stack[stackLen-1].variables |> emplace([[DAVariable name="{vinfo.name}", value=value, _type=ti != null ? get_type_name(*ti) : ""]])

    def override onArgument (inf: FuncInfo; index: int; vinfo: VarInfo; arg: float4)
        let stackLen = length(ctx.stack)
        if  stackLen > 0
            let ti = type_info(vinfo)
            let value = sprint_data(arg, ti, print_flags_debug) |> fix_var_value()
            ctx.stack[stackLen-1].arguments |> emplace([[DAVariable name="{vinfo.name}", value=value, _type=ti != null ? get_type_name(*ti) : ""]])


struct DABreakpoint
    id: uint64
    line: uint

typedef
    DABreakpoints = table<string; array<DABreakpoint>>
    DAContextAt = uint64

struct DAContext
    id: uint64
    at: DAContextAt
    ctx: Context?
    stack: array<DAStackFrame>
    continueRequested: bool = false
    pauseRequested: bool = false
    stepInRequested: bool = false
    stepRequestedStack: int = 0 // stack depth


def reset_debug_flags(var ctx: DAContext)
    ctx.continueRequested = false
    ctx.pauseRequested = false
    ctx.stepInRequested = false
    ctx.stepRequestedStack = 0


def to_string(ctx: DAContext)
    return build_string() <| $(str)
        let stoppable = (uint(ctx.ctx.category) & (1u << 31u)) != 0u
        if stoppable
            str |> write("*")
        str |> write(ctx.at)
        if length(ctx.ctx.name) > 0
            str |> write(" \"")
            str |> write(ctx.ctx.name)
            str |> write("\"")
        if int(ctx.ctx.category) > 0
            str |> write(" ")
            str |> write(ctx.ctx.category)
        str |> write(" (")
        str |> write(int64(ctx.id))
        str |> write(")")


class DAgent: DapiDebugAgent

    walker_adapter: smart_ptr<StackWalker>
    walker: DAStackWalker?
    server: DAServer?

    breakpoints: DABreakpoints
    breakpointId: uint64 = 1ul

    contexts: array<DAContext>
    contextId: uint64 = 1ul

    waitConnection: bool = true

    def DAgent()
        walker = new DAStackWalker()
        unsafe
            walker_adapter <- make_stack_walker(walker)
        self->startServer()
        self->addContext(this_context())

    def startServer()
        let args <- get_command_line_arguments()
        let port = args |> get_int_arg("--das-debug-port", 9000)
        server = new DAServer()
        server.port = port
        unsafe
            server.agent = addr(self)
        if !server->init(port)
            print("server failed to initialize\n")
            unsafe
                delete server

    def finalize()
        unsafe
            delete walker_adapter
            delete walker
            delete server

    def log(msg: string)
        if server != null
            server->log(msg)
        else
            print("{msg}\n")

    def override onBreakpoint(var ctx:Context; at:LineInfo): void
        if server == null
            self->log("[Error] onBreakpoint without server")
            return
        if !waitConnection && (!server.configurationDone || !server.threadsDone)
            return
        let path = "{at.fileInfo.name}"
        self->log("{ctx} breakpoint at {path}:{int(at.line)}\n")
        set_single_step(ctx,true)

        self->addContext(ctx)
        self->getContextDataFor(ctx) <| $(var ctxData: DAContext)
            self->sendStopped(ctx, ctxData, at, "pause", -1ul)
            ctxData.continueRequested = false
            while !ctxData.continueRequested && !ctxData.stepInRequested && ctxData.stepRequestedStack == 0
                tick_debug_agent()
                sleep(1u)
            ctxData.continueRequested = false

    def override onSingleStep(var ctx:Context; at:LineInfo): void
        if server == null
            self->log("[Error] onSingleStep without server")
            return

        // unsafe
        //     self->log("{double(ctx_at(ctx))} step at {at.fileInfo.name}:{int(at.line)}\n")

        if (uint(ctx.category) & (1u << 31u)) != 0u
            self->addContext(ctx)

        if waitConnection && (!server.configurationDone || !server.threadsDone)
            if (uint(ctx.category) & (1u << 31u)) != 0u
                while !server.configurationDone || !server.threadsDone
                    tick_debug_agent()
                    sleep(1u)
            else
                self->log("[Error] non debug context in single step {ctx.name}")

        self->getContextDataFor(ctx) <| $(var ctxData: DAContext)
            if ctxData.pauseRequested
                self->sendStopped(ctx, ctxData, at, "pause", -1ul)
                while ctxData.pauseRequested
                    tick_debug_agent()
                    sleep(1u)

            if ctxData.stepInRequested
                ctxData.stepInRequested = false
                self->sendStopped(ctx, ctxData, at, "step", -1ul)
                ctxData.continueRequested = false
                while !ctxData.continueRequested && !ctxData.stepInRequested && ctxData.stepRequestedStack == 0
                    tick_debug_agent()
                    sleep(1u)
                ctxData.continueRequested = false

            if ctxData.stepInRequested
                return

            if ctxData.stepRequestedStack > 0
                if stack_depth(*ctxData.ctx) > ctxData.stepRequestedStack
                    return
                ctxData.stepRequestedStack = 0
                self->sendStopped(ctx, ctxData, at, "step", -1ul)
                while !ctxData.continueRequested && !ctxData.stepInRequested && ctxData.stepRequestedStack == 0
                    tick_debug_agent()
                    sleep(1u)

            let file = string(at.fileInfo.name)
            var prevBreak = -1ul
            var inBreak = key_exists(breakpoints, file)
            // self->log("search break `{file}`:{at.line} - {inBreak}\n")
            while inBreak
                inBreak = false
                let brs = breakpoints |> find(file)
                var brIdx = 0
                while brs != null && brIdx < length(*brs)
                    let br & = (*brs)[brIdx++]
                    // self->log("search break `{file}` {brs} in {at.fileInfo.name}\n")
                    if br.line == uint(at.line)
                        if ctxData.continueRequested && prevBreak == br.id
                            ctxData.continueRequested = false
                            continue
                        if ctxData.stepInRequested || ctxData.stepRequestedStack > 0
                            continue

                        inBreak = true

                        if prevBreak != br.id
                            self->sendStopped(ctx, ctxData, at, "breakpoint", br.id)

                        prevBreak = br.id
                        tick_debug_agent()
                        sleep(1u)
                        break

    def nextStep(var ctx: Context; var ctxData:DAContext; at:LineInfo)
        delete ctxData.stack
        unsafe
            walker.ctx = addr(ctxData)
        walk_stack(walker_adapter, ctx, at)
        if length(ctxData.stack) > 0
            for i in range(1, length(ctxData.stack))
                let j = length(ctxData.stack) - 1
                let q = j - 1
                ctxData.stack[j].path = ctxData.stack[q].path
                ctxData.stack[j].line = ctxData.stack[q].line
            ctxData.stack[0].path = "{at.fileInfo.name}"
            ctxData.stack[0].line = at.line

    def sendStopped(var ctx: Context; var ctxData:DAContext; at:LineInfo; reason: string; brId: uint64)
        self->log("<< stopped `{reason}` at {at.fileInfo.name}:{int(at.line)}")

        self->nextStep(ctx, ctxData, at)
        server->sendStopped(ctxData, reason, brId)

    def override onTick()
        if server != null
            if !server.disconnected
                server->tick()
            else
                waitConnection = false
                unsafe
                    delete server
                self->startServer()

    def override onCreateContext(var ctx: Context): void
        self->addContext(ctx)

    def override onDestroyContext(var ctx: Context): void
        self->removeContext(ctx)

    def getContextDataFor(var ctx: Context; cb: block<(var ctx: DAContext): void>): bool
        let at = ctx_at(ctx)
        for i in iter_range(contexts)
            if contexts[i].at == at
                cb |> invoke(contexts[i])
                return true
        return false

    def getContextData(id: DAContextAt; cb: block<(var ctx: DAContext): void>): bool
        for i in iter_range(contexts)
            if contexts[i].id == id
                cb |> invoke(contexts[i])
                return true
        return false

    def getContextIdx(var ctx: Context): int
        var ctxAt = ctx_at(ctx)
        for idx in iter_range(contexts)
            if contexts[idx].at == ctxAt
                return idx
        return -1

    def addContext(var ctx: Context): bool
        let idx = self->getContextIdx(ctx)
        if idx >= 0
            return false
        unsafe
            contexts |> emplace([[DAContext at=ctx_at(ctx), id=contextId, ctx=addr(ctx)]], 0)
        contextId += 1ul
        contexts |> sort <| $(a, b: DAContext)
            let aid = uint(a.ctx.category) & ~(1u << 31u)
            let bid = uint(b.ctx.category) & ~(1u << 31u)
            return aid < bid
        // TODO: send `threads` event
        return true

    def removeContext(var ctx: Context): void
        let idx = self->getContextIdx(ctx)
        if idx >= 0
            self->removeContextIdx(idx)

    def removeContextIdx(idx: int)
        delete contexts[idx]
        contexts |> erase(idx)

class DAServer: Server
    port: int
    [[do_not_delete]] agent: DAgent?

    disconnected: bool = false

    // message data
    current_string: array<uint8>
    toRead: int = 0

    initialized: int = -1 // initialized event: ticks delay
    configurationDone: bool = false
    threadsDone: bool = false

    def DAServer
        Server`Server(cast<Server> self)

    def override onError(msg:string; code:int)
        print("dap: server error:{code} - {msg}\n")

    def override onConnect()
        print("dap: connected ::{port}\n")

    def override onDisconnect()
        if length(current_string) > 0
            print(string(current_string))
            current_string |> clear()
        // print("disconnected\n")

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
                if typ == "request"
                    let command = msg |> jos("command", "n/a")
                    let arguments = msg |> joj("arguments")
                    if command == "initialize"
                        self->reqInitialize(seq, command, arguments)
                    elif command == "configurationDone"
                        self->reqConfigurationDone(seq, command, arguments)
                    elif command == "disconnect"
                        self->reqDisconnect(seq, command, arguments)
                    elif command == "terminate"
                        self->reqTerminate(seq, command, arguments)
                    elif command == "launch"
                        self->reqLaunch(seq, command, arguments)
                    elif command == "attach"
                        self->reqAttach(seq, command, arguments)
                    elif command == "setBreakpoints"
                        self->reqSetBreakpoints(seq, command, arguments)
                    elif command == "threads"
                        self->reqThreads(seq, command, arguments)
                    elif command == "stackTrace"
                        self->reqStackTrace(seq, command, arguments)
                    elif command == "scopes"
                        self->reqScopes(seq, command, arguments)
                    elif command == "variables"
                        self->reqVariables(seq, command, arguments)
                    elif command == "continue"
                        self->reqContinue(seq, command, arguments)
                    elif command == "pause"
                        self->reqPause(seq, command, arguments)
                    elif command == "stepIn"
                        self->reqStepIn(seq, command, arguments)
                    elif command == "next"
                        self->reqNext(seq, command, arguments)
                    elif command == "stepOut"
                        self->reqStepOut(seq, command, arguments)
                    else
                        self->log("[Error] unknown request message\n{write_json(msg)}\n")
            else
                self->log("[Error] unknown message\n{write_json(msg)}\n")

    def reqInitialize(seq: double; command: string; arguments: JsonValue?)
        // var ini <- InitializeRequestArguments(arguments)
        // defer_delete(ini)
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

    def reqAttach(seq: double; command: string; data: JsonValue?)
        self->sendSuccessResponse(seq, command, null)

    def reqDisconnect(seq: double; command: string; data: JsonValue?)
        // var ini = DisconnectRequest(data)
        // defer_delete(ini)
        self->sendSuccessResponse(seq, command, null)
        for ctx in agent.contexts
            ctx |> reset_debug_flags()
            ctx.continueRequested = true
        for data in values(agent.breakpoints)
            delete data
        agent.breakpoints |> clear()
        // ini.arguments.restart
        disconnected = true

    def reqTerminate(seq: double; command: string; data: JsonValue?)
        self->sendSuccessResponse(seq, command, null)
        unsafe
            fio::exit(0)

    def reqSetBreakpoints(seq: double; command: string; data: JsonValue?)
        var ini <- SetBreakpointsArguments(data)
        defer_delete(ini)
        var res: SetBreakpointsResponse
        defer_delete(res)
        let path = ini.source.path
        if key_exists(agent.breakpoints, path)
            delete agent.breakpoints[path]
        agent.breakpoints |> erase(path)
        if length(ini.breakpoints) >= 0
            var breaks <- [[array<DABreakpoint>]]
            for b in ini.breakpoints
                breaks |> emplace([[DABreakpoint line=uint(b.line), id=agent.breakpointId ]])
                res.breakpoints |> emplace([[Breakpoint
                    verified=true,
                    id=double(agent.breakpointId),
                    source=ini.source,
                    line=b.line,
                    message="breakpoint #{int(agent.breakpointId)} {path}:{int(b.line)}"
                ]])
                agent.breakpointId += 1ul
            agent.breakpoints[path] <- breaks
        self->sendSuccessResponse(seq, command, JV(res))

    def reqThreads(seq: double; command: string; data: JsonValue?)
        var res: ThreadsResponseBody
        defer_delete(res)
        threadsDone = true
        for ctx in agent.contexts
            res.threads |> emplace([[Thread id=double(ctx.id), name=ctx |> to_string() ]])
        self->sendSuccessResponse(seq, command, JV(res))

    def reqStackTrace(seq: double; command: string; data: JsonValue?)
        var ini <- StackTraceArguments(data)
        defer_delete(ini)
        var res: StackTraceResponseBody
        defer_delete(res)
        agent->getContextData(uint64(ini.threadId)) <| $(var ctxData: DAContext)
            for idx, line in range(ini.levels), ctxData.stack
                res.stackFrames |> emplace([[StackFrame
                    id=double(ctxData.id * 100ul + uint64(idx)),
                    name=line.name,
                    line=double(line.line),
                    column=1lf,
                    source=[[Source name=line.path, path=line.path]]
                ]])
            res.totalFrames = double(length(ctxData.stack))
        self->sendSuccessResponse(seq, command, JV(res))

    def reqScopes(seq: double; command: string; data: JsonValue?)
        var ini <- ScopesArguments(data)
        defer_delete(ini)
        var res: ScopesResponseBody
        defer_delete(res)
        let ctxId = uint64(ini.frameId / 100lf)
        agent->getContextData(ctxId) <| $(var ctxData: DAContext)
            let frameId = int(ini.frameId % 100lf)
            if frameId >= 0 && frameId < length(ctxData.stack)
                let line & = ctxData.stack[frameId]
                if length(line.variables) > 0
                    res.scopes |> emplace([[Scope
                        name="Locals",
                        variablesReference=double(uint64(ini.frameId) * 10ul + LOCAL_VARS)
                    ]])
                if length(line.arguments) > 0
                    res.scopes |> emplace([[Scope
                        name="Arguments",
                        variablesReference=double(uint64(ini.frameId) * 10ul + ARGUMENT_VARS)
                    ]])
                if get_total_variables(*ctxData.ctx) > 0
                    res.scopes |> emplace([[Scope
                        name="Globals",
                        variablesReference=double(uint64(ini.frameId) * 10ul + GLOBAL_VARS)
                    ]])
        self->sendSuccessResponse(seq, command, JV(res))

    def reqVariables(seq: double; command: string; data: JsonValue?)
        var ini <- VariablesArguments(data)
        defer_delete(ini)
        var res: VariablesResponseBody
        defer_delete(res)
        let ctxId = uint64(ini.variablesReference / 1000lf)
        agent->getContextData(ctxId) <| $(var ctxData: DAContext)
            let frameId = int((ini.variablesReference / 10lf) % 100lf)
            if frameId >= 0 && frameId < length(ctxData.stack)
                let kind = uint64(ini.variablesReference % 10lf)
                let line & = ctxData.stack[frameId]
                if kind == LOCAL_VARS || kind == ARGUMENT_VARS
                    let vars & = kind == LOCAL_VARS ? line.variables : line.arguments
                    for v in vars
                        res.variables |> emplace([[Variable
                            name=v.name,
                            value=v.value,
                            _type=v._type
                            // variablesReference=ini.arguments.variablesReference
                        ]])
                elif kind == GLOBAL_VARS
                    for i in range(get_total_variables(*ctxData.ctx))
                        let vinfo & = get_variable_info(*ctxData.ctx, i)
                        var value: void?
                        unsafe
                            value = get_context_global_variable(*ctxData.ctx, vinfo.name)
                        let ti = type_info(vinfo)
                        res.variables |> emplace([[Variable
                            name="{vinfo.name}",
                            value=sprint_data(value, ti, print_flags_debug) |> fix_var_value(),
                            _type=ti != null ? get_type_name(*ti) : ""
                        ]])
        self->sendSuccessResponse(seq, command, JV(res))

    def reqContinue(seq: double; command: string; data: JsonValue?)
        var ini <- ContinueArguments(data)
        defer_delete(ini)
        agent->getContextData(uint64(ini.threadId)) <| $(var ctx: DAContext)
            ctx |> reset_debug_flags()
            ctx.continueRequested = true
        self->sendSuccessResponse(seq, command, null)

    def reqPause(seq: double; command: string; data: JsonValue?)
        var ini <- PauseArguments(data)
        defer_delete(ini)
        agent->getContextData(uint64(ini.threadId)) <| $(var ctx: DAContext)
            ctx |> reset_debug_flags()
            ctx.pauseRequested = true
        self->sendSuccessResponse(seq, command, null)

    def reqStepIn(seq: double; command: string; data: JsonValue?)
        var ini <- StepInArguments(data)
        defer_delete(ini)
        agent->getContextData(uint64(ini.threadId)) <| $(var ctx: DAContext)
            ctx |> reset_debug_flags()
            ctx.stepInRequested = true
        self->sendSuccessResponse(seq, command, null)

    def reqNext(seq: double; command: string; data: JsonValue?)
        var ini <- NextArguments(data)
        defer_delete(ini)
        agent->getContextData(uint64(ini.threadId)) <| $(var ctx: DAContext)
            ctx |> reset_debug_flags()
            ctx.stepRequestedStack = stack_depth(*ctx.ctx)
            ctx.stepInRequested = ctx.stepRequestedStack == 0
        self->sendSuccessResponse(seq, command, null)

    def reqStepOut(seq: double; command: string; data: JsonValue?)
        var ini <- NextArguments(data)
        defer_delete(ini)
        agent->getContextData(uint64(ini.threadId)) <| $(var ctx: DAContext)
            ctx |> reset_debug_flags()
            ctx.stepRequestedStack = max(stack_depth(*ctx.ctx) - 1, 0)
            ctx.stepInRequested = ctx.stepRequestedStack == 0
        self->sendSuccessResponse(seq, command, null)


    def log(msg: string)
        // print("{msg}\n")
        if self->is_connected()
            self->sendOutput("console", "{msg}\n")

    def sendOutput(category: string; output: string)
        var res = [[OutputEventBody category=category, output=output ]]
        defer_delete(res)
        self->sendRawData(JV({{"type"=>JV("event"); "event"=>JV("output"); "body"=>JV(res)}}), /*log*/false)

    def sendEvent(event: string; body: JsonValue?)
        self->sendRawData(JV({{"type"=>JV("event"); "event"=>JV(event); "body"=>body}}), /*log*/true)

    def sendSuccessResponse(id: double; command: string; body: JsonValue?)
        self->sendRawData(JV({{"request_seq"=>JV(id); "type"=>JV("response");
                                "body"=>body; "command"=>JV(command); "success"=>JV(true)}}), /*log*/true)

    // https://microsoft.github.io/language-server-protocol/specifications/specification-current/#responseMessage
    def sendRawData(var data : JsonValue?; logData: bool)
        var msg = write_json(data)
        var packStr = "Content-Length: {length(msg)}\r\n\r\n{msg}"
        if logData
            self->log("<------\n{packStr}")
        // else
        //     print("<------\n{packStr}\n")
        unsafe
            self->send(reinterpret<uint8?> packStr, length(packStr))
            delete data

    def sendStopped(ctx: DAContext; reason: string; brId: uint64)
        self->sendEvent("stopped", JV({{
            "reason"=>JV(reason);
            "threadId"=>JV(double(ctx.id));
            "allThreadsStopped"=>JV(false);
            "hitBreakpointIds"=>brId == -1ul ? null : JV([{auto[] JV(brId)}])
        }}))


[export,unused_argument(ctx)]
def private start_debug_agent(ctx: Context)
    install_new_debug_agent(new DAgent(), "debug")


[export]
def private start_agent(force:bool = false): bool
    if force || !has_debug_agent_context("debug")
        fork_debug_agent_context(@@start_debug_agent)
        print("dap: install debug agent\n")
    else
        print("dap: debug agent already exists\n")
    set_single_step(this_context(), true)
    return true


[init,export]
def auto_start_debug()
    if !is_in_debug_agent_creation()
        this_context().category |= bitfield(1u << 31u) // TODO:
        if !has_debug_agent_context("debug")
            if start_agent()
                new_thread <| @
                    this_context().name := "tick thread"
                    while true
                        tick_debug_agent()
                        sleep(1u)
        set_single_step(this_context(), true)

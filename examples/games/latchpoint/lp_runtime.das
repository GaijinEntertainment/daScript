options gen2
options persistent_heap

require lp_world public
require daslib/json_boost public
require daslib/strings_boost public
require daslib/fio public

struct ActionReceipt {
    ok : bool = false
    detail : string
}

struct ResidentMessage {
    role : string
    content : string
}

struct RuntimeTrace {
    time : float
    event : string
    data : string
}

typedef ResidentHistory = array<array<ResidentMessage>>
var @live histories : table<string; ResidentHistory>
var save_valid = true
var save_path = ""
var trace_path = ""
var @live runtime_logging = true

def runtime_path(file : string) : string {
    return path_join(get_das_root(), "examples/games/latchpoint/{file}")
}

def record_world_event(event : string) {
    world.events |> push(clone_string(event))
}

def task_owner(task : string) : int {
    return 0 if (task == "restore_power")
    return 1 if (task == "repair_patch")
    return 2 if (task == "inspect_archive")
    return -1
}

def action_allowed(resident : int; action : string) : bool {
    return false if (resident < 0 || resident > 2)
    return true if (action == "accept_task")
    if (resident == 0) { return action == "lend_actuator" || action == "open_workshop" }
    if (resident == 1) { return action == "follow" || action == "stop_follow" }
    return action == "open_archive" || action == "open_workshop"
}

def runtime_action(resident : int; action, task : string = "") : ActionReceipt {
    if (!action_allowed(resident, action)) { return ActionReceipt(detail = "That resident cannot perform this action.") }
    if (action == "accept_task") {
        if (task_owner(task) != resident) { return ActionReceipt(detail = "That task belongs to another resident.") }
        if (find_index(world.tasks, task) < 0) { world.tasks |> push(clone_string(task)) }
        return ActionReceipt(ok = true, detail = "Agreed to {task}; not yet completed.")
    }
    if (!empty(task)) { return ActionReceipt(detail = "Only task agreements take a task argument.") }
    if (action == "lend_actuator") {
        if (world.brack_actuator_lent) { return ActionReceipt(detail = "The one spare actuator was already given to you.") }
        world.brack_actuator_lent = true
        world.inventory |> push("actuator")
        record_world_event("Brack gave the avatar one spare actuator.")
        return ActionReceipt(ok = true, detail = "The actuator is yours. Bring it to Patch and press F to install it.")
    }
    if (action == "follow" || action == "stop_follow") {
        if (action == "follow" && !world.patch_repaired) { return ActionReceipt(detail = "My leg needs an actuator installed before I can follow.") }
        world.patch_following = action == "follow"
        return ActionReceipt(ok = true, detail = world.patch_following ? "I'll follow you. Lead the way." : "I'll wait here.")
    }
    if (action == "open_workshop") {
        if (!world.patch_repaired && find_index(world.tasks, "restore_power") < 0) {
            return ActionReceipt(detail = "Repair Patch, or agree with Brack to restore power, before workshop access.")
        }
        if (!world.workshop_open) { record_world_event("{resident_name(resident)} authorized workshop access.") }
        world.workshop_open = true
        return ActionReceipt(ok = true, detail = "Workshop access granted. The coil is on the metal workbench inside.")
    }
    if (!world.patch_repaired || !world.power) {
        return ActionReceipt(detail = "Archive access needs both repaired Patch and restored generator power.")
    }
    if (!world.archive_open) { record_world_event("Vesper opened the archive; an older identification plate is inside.") }
    world.archive_open = true
    return ActionReceipt(ok = true, detail = "Archive access granted. The old identification plate is inside.")
}

def runtime_physical(command : string) : ActionReceipt {
    if (command == "inspect archive record") {
        if (!world.archive_open) { return ActionReceipt(detail = "The archive is still locked.") }
        if (!world.archive_inspected) {
            world.archive_inspected = true
            record_world_event("The avatar inspected an old service plate: its serial matches the avatar, but its inspection predates this shift.")
        }
        return ActionReceipt(ok = true, detail = "OLD SERVICE PLATE: your avatar's serial number. Inspection completed before this shift began. Operator signature: already present. Vesper has filed it as a clerical error.")
    }
    if (command == "salvage actuator" || command == "salvage coil") {
        let item = command == "salvage actuator" ? "actuator" : "coil"
        if (item == "coil" && !world.workshop_open) { return ActionReceipt(detail = "The workshop is locked.") }
        if (find_index(world.salvaged, item) >= 0) { return ActionReceipt(detail = "That part was already collected.") }
        world.salvaged |> push(item)
        world.inventory |> push(item)
        return ActionReceipt(ok = true, detail = "Collected one {item}.")
    }
    if (command == "start generator") { return ActionReceipt(detail = "Hold F to crank the starter. Patch can hold the service clutch if repaired and following nearby.") }
    if (command != "install actuator" && command != "install coil") { return ActionReceipt(detail = "Unknown physical interaction.") }
    let repair = command == "install actuator"
    if (repair ? world.patch_repaired : (world.coil_installed || world.power)) { return ActionReceipt(detail = "Already installed; no part consumed.") }
    let index = find_index(world.inventory, repair ? "actuator" : "coil")
    if (index < 0) { return ActionReceipt(detail = "The required part is not in your cargo.") }
    world.inventory |> erase(index)
    let event = repair ? "The avatar repaired Patch's leg." : "Coil installed. Starter clutch jammed: hold F to crank manually, or bring repaired Patch to assist."
    if (repair) { world.patch_repaired = true } else { world.coil_installed = true }
    record_world_event(event)
    return ActionReceipt(ok = true, detail = event)
}

def advance_starter(dt : float; held : bool) : bool {
    starter_engaged = held && world.coil_installed && !world.power && length(avatar.xy - GENERATOR_POSITION.xy) < 3.1
    if (!starter_engaged) { starter_progress = 0.0; return false }
    starter_progress = min(1.0, starter_progress + clamp(dt, 0.0, 0.05) / (patch_can_assist() ? 3.0 : 9.0))
    if (starter_progress < 1.0) { return false }
    world.power = true
    record_world_event(patch_can_assist() ? "Patch held the service clutch while the avatar started the generator." : "The avatar manually cranked the generator into operation.")
    starter_engaged = false
    return true
}

def runtime_snapshot() : string {
    return write_json(JV((version = 1, world = JV(world), conversations = JV(histories))))
}

def runtime_load(text : string) : bool {
    var error : string
    let value = read_json(text, error)
    if (value == null || (value?.version ?? 0) != 1 || !(value?.world is _object) || !(value?.conversations is _object)) { return false }
    for (key in fixed_array("patch_repaired", "patch_following", "power", "workshop_open", "archive_open", "brack_actuator_lent")) {
        if (!(value?.world?[key] is _bool)) { return false }
    }
    for (key in fixed_array("inventory", "tasks", "salvaged")) {
        if (!(value?.world?[key] is _array)) { return false }
    }
    delete world
    world <- from_JV(value?.world, type<WorldState>)
    if (world.power) { world.coil_installed = true }
    delete histories
    histories <- from_JV(value?.conversations, type<table<string; ResidentHistory>>)
    return true
}

def private publish_runtime_save(text, backup_suffix : string) : bool {
    let staging = save_path + ".new"
    if (fexist(staging) && !remove(staging)) { return false }
    if (!fwrite(staging, text)) { remove(staging); return false }
    if (fexist(save_path) && !empty(backup_suffix)) {
        var error : string
        if (!copy_file(save_path, save_path + backup_suffix, true, error)) { remove(staging); return false }
    }
    if (get_platform_name() == "windows" && fexist(save_path) && !remove(save_path)) {
        remove(staging)
        return false
    }
    if (!rename(staging, save_path)) {
        remove(staging)
        return false
    }
    return true
}

def runtime_save(replace_invalid : bool = false) : bool {
    return false if (!save_valid && !replace_invalid)
    let suffix = !save_valid && fexist(save_path) ? ".corrupt" : ".previous"
    if (!publish_runtime_save(runtime_snapshot(), suffix)) { return false }
    save_valid = true
    return true
}

def runtime_trace(event, data : string) {
    return if (!runtime_logging)
    let line = sprint_json(RuntimeTrace(time = get_uptime(), event = event, data = data), false) + "\n"
    fopen(trace_path, "ab") $(file) {
        if (file != null) { fwrite(file, line) }
    }
}

def load_runtime_save(path : string) {
    save_path = path
    mkdir_rec(dir_name(save_path))
    save_valid = true
    let primary_exists = fexist(save_path)
    if (primary_exists) {
        if (runtime_load(fread(save_path))) { return }
        save_valid = false
    }
    let previous = save_path + ".previous"
    return if (!fexist(previous))
    let recovered = fread(previous)
    return if (!runtime_load(recovered))
    if (primary_exists) {
        var error : string
        return if (!copy_file(save_path, save_path + ".corrupt", true, error))
    }
    save_valid = publish_runtime_save(recovered, "")
}

def init_runtime() {
    load_runtime_save(runtime_path("saves/scene.json"))
    trace_path = path_join(get_das_root(), "logs/latchpoint-playtests/native-runtime-v1.jsonl")
    mkdir_rec(dir_name(trace_path))
    runtime_trace("runtime", "daslang-native-v1")
}

def remember_exchange(resident : int; question, answer : string) {
    var exchange <- [ResidentMessage(role = "user", content = clone_string(question)),
        ResidentMessage(role = "assistant", content = clone_string(answer))]
    histories[resident_id(resident)] |> emplace(exchange)
}

def history_messages(resident : int; count : int) : array<ResidentMessage> {
    var messages : array<ResidentMessage>
    let past = unsafe(histories?[resident_id(resident)])
    if (past != null) {
        for (i in range(max(length(*past) - count, 0), length(*past))) {
            for (message in (*past)[i]) {
                if ((message.role == "user" || message.role == "assistant") && !empty(message.content)) {
                    messages |> push(message)
                }
            }
        }
    }
    return <- messages
}

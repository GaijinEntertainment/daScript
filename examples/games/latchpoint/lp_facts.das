options gen2
require lp_runtime public

let INTENT_NAMES = "chat, clarify, open_workshop, open_archive, loan_actuator, follow, stop_follow, assist_generator, agree_power, agree_repair, agree_archive, where_coil, where_actuator, where_generator, where_workshop, where_archive, how_power, status_patch, status_power, status_workshop, status_archive, guide_coil"

def archive_fact() : string {
    return "The archive gate is open." if (world.archive_open)
    return "Both interlocks are satisfied. Ask Vesper to open the archive." if (world.power && world.patch_repaired)
    return "Patch is repaired; only generator power is missing." if (world.patch_repaired)
    return "Power is restored; Patch still needs an actuator installed." if (world.power)
    return "The archive needs both generator power and repaired Patch."
}

def workshop_fact() : string {
    return "WORKSHOP 01 is open. The coil is on the metal workbench." if (world.workshop_open)
    return "Workshop access is allowed. Ask Brack or Vesper to open the gate." if (world.patch_repaired || find_index(world.tasks, "restore_power") >= 0)
    return "Workshop access needs either repaired Patch or an agreement with Brack to restore power."
}

def gameplay_answer(intent : string; resident : int) : string {
    if (intent == "status_archive") { return archive_fact() }
    if (intent == "status_workshop") { return workshop_fact() }
    if (intent == "status_patch") {
        let subject = resident == 1 ? "My leg" : "Patch's leg"
        if (world.patch_repaired) { return "{subject} is repaired. That already satisfies archive safety inspection." }
        if (find_index(world.inventory, "actuator") >= 0) { return "You have the actuator. Approach Patch and press F to install it." }
        return "{subject} needs an actuator. Brack has a free spare, or collect one at SALVAGE 04."
    }
    if (intent == "how_power" || intent == "status_power") {
        if (world.power) { return "The generator is running. Power is restored." }
        if (world.coil_installed) { return "The coil is installed, but the starter clutch is jammed. Hold F at the generator to crank manually, or bring repaired Patch following you to hold the clutch." }
        if (find_index(world.inventory, "coil") >= 0) { return "Bring the coil to GENERATOR 02, behind the workshop, and press F to install it." }
        return "The generator needs a coil from the workbench inside WORKSHOP 01. Bring it to GENERATOR 02 and press F to install it."
    }
    if (intent == "where_coil") {
        if (world.coil_installed) { return world.power ? "The coil is installed and power is on." : "The coil is installed. Hold F at the generator to crank it, or bring Patch to assist." }
        if (find_index(world.salvaged, "coil") >= 0) { return "You collected the coil. Check your cargo." }
        return "The coil is in the lit cradle on the workbench inside WORKSHOP 01, east of the central yard."
    }
    if (intent == "where_actuator") { return "An actuator sits beneath the hanging lamp at SALVAGE 04, on the west side. Brack also has one spare." }
    if (intent == "where_generator") { return "GENERATOR 02 is behind the workshop beside the tall exhaust tower." }
    if (intent == "where_workshop") { return "WORKSHOP 01 is the enclosure on the east side. Its door faces the central yard." }
    if (intent == "where_archive") { return "ARCHIVE 03 is behind Vesper at the north end of the yard." }
    if (intent == "guide_coil") { return "Patch can follow you, but cannot lead a route. The coil is on the workshop bench." }
    return "Which object or task do you mean?"
}

def intent_action(intent : string; resident : int) : ActionReceipt {
    if (intent == "assist_generator") { return runtime_action(resident, "follow") }
    if (intent == "loan_actuator") { return runtime_action(resident, "lend_actuator") }
    if (intent == "agree_power") { return runtime_action(resident, "accept_task", "restore_power") }
    if (intent == "agree_repair") { return runtime_action(resident, "accept_task", "repair_patch") }
    if (intent == "agree_archive") { return runtime_action(resident, "accept_task", "inspect_archive") }
    return runtime_action(resident, intent)
}

def is_action_intent(intent : string) : bool {
    return (intent == "open_workshop" || intent == "open_archive" || intent == "loan_actuator" ||
        intent == "follow" || intent == "stop_follow" || intent == "assist_generator" || intent == "agree_power" || intent == "agree_repair" || intent == "agree_archive")
}

def valid_intent(intent : string) : bool {
    let names <- split(INTENT_NAMES, ", ")
    return find_index(names, intent) >= 0
}

options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options _dasllama_internal = true

require dasllama/dasllama_asr   // the ASR families and the facade's whisper lane
require dasllama/dasllama_audio   // the chat towers' lane trio
require dasllama/dasllama_tower   // announce_lane

// The fixture test_asr_verbs.das spawns under each DASLLAMA_AUDIO_LANE value: the knob loads once at context init, so each
// value is a process. Every lane answer prints as one `LANE <family> <state> <lane>` record, the policy each family's
// accessor reads un-pinned as `POLICY <family> <lane>`, and the lane lines `announce_lane` prints follow `CASE announce`.

def private lane_word(q8 : bool) : string => q8 ? "q8" : "exact"

def private say(family, state : string; q8 : bool) {
    to_log(LOG_INFO, "LANE {family} {state} {lane_word(q8)}\n")
}

//! a family's trio walked: un-pinned, pinned q8, pinned exact, reset - the answer after each
def private walk(family : string; pin : block<(on : bool) : void>; reset : block<() : void>; serves : block<() : bool>) {
    reset |> invoke()
    say(family, "unpinned", serves |> invoke())
    pin |> invoke(true)
    say(family, "pin_q8", serves |> invoke())
    pin |> invoke(false)
    say(family, "pin_exact", serves |> invoke())
    reset |> invoke()
    say(family, "reset", serves |> invoke())
}

[export]
def main() {
    set_asr_fp32(false)
    set_asr_tower_fp32(false)
    to_log(LOG_INFO, "POLICY qwen3a q8\nPOLICY gemma4a q8\nPOLICY canary q8\nPOLICY parakeet q8\nPOLICY whisper q8\n")
    to_log(LOG_INFO, "POLICY gigaam {lane_word(!gigaam_gpu_exact_serves())}\n")
    to_log(LOG_INFO, "POLICY tower {lane_word(audio_tower_dev_flavor(true) != TowerDev.none)}\n")
    walk("qwen3a", $(on) { set_qwen3a_q8(on); }, $ { reset_qwen3a_q8(); }, $ => qwen3a_serves_q8())
    walk("gemma4a", $(on) { set_gemma4a_q8(on); }, $ { reset_gemma4a_q8(); }, $ => gemma4a_serves_q8())
    walk("canary", $(on) { set_canary_enc_q8(on); }, $ { reset_canary_enc_q8(); }, $ => canary_enc_serves_q8())
    walk("parakeet", $(on) { set_parakeet_q8(on); }, $ { reset_parakeet_q8(); }, $ => parakeet_serves_q8())
    walk("gigaam", $(on) { set_gigaam_q8(on); }, $ { reset_gigaam_q8(); }, $ => gigaam_serves_q8())
    walk("tower", $(on) { set_audio_tower_q8(on); }, $ { reset_audio_tower_q8(); }, $ => audio_tower_serves_q8())
    say("whisper", "unpinned", whisper_tower_serves_q8())
    set_asr_tower_fp32(true)
    say("whisper", "rail_exact", whisper_tower_serves_q8())
    set_asr_tower_fp32(false)
    set_asr_tower_q8(true)
    say("parakeet", "facade_q8", parakeet_serves_q8())
    set_asr_tower_q8(false)
    say("parakeet", "facade_exact", parakeet_serves_q8())
    reset_parakeet_q8()
    reset_canary_enc_q8()
    reset_gigaam_q8()
    reset_gemma4a_q8()
    to_log(LOG_INFO, "CASE announce\n")
    announce_lane("probe", false, true, "set_probe_q8", false, false)
    announce_lane("probe", true, false, "set_probe_q8", false, false)
    announce_lane("probe", false, true, "set_probe_q8", true, false)
    to_log(LOG_INFO, "UNHEARD '{audio_lane_unheard("the probe's planes")}'\n")
    to_log(LOG_INFO, "ROOT_DONE\n")
}

options gen2
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

module strudel_sf2 shared public
//! SoundFont 2 (.sf2) file parser and preset lookup.
//! Reads SF2 files including samples, instruments, presets, generators, and modulators.

require daslib/fio
require strings
require math

// ─── SF2 Generator Operators ───

//! SF2 generator operator enumeration — identifies which parameter a generator controls.
enum SF2GenOper {
    startAddrsOffset = 0
    endAddrsOffset = 1
    startloopAddrsOffset = 2
    endloopAddrsOffset = 3
    startAddrsCoarseOffset = 4
    modLfoToPitch = 5
    vibLfoToPitch = 6
    modEnvToPitch = 7
    initialFilterFc = 8
    initialFilterQ = 9
    modLfoToFilterFc = 10
    modEnvToFilterFc = 11
    endAddrsCoarseOffset = 12
    modLfoToVolume = 13
    chorusEffectsSend = 15
    reverbEffectsSend = 16
    pan = 17
    delayModLFO = 21
    freqModLFO = 22
    delayVibLFO = 23
    freqVibLFO = 24
    delayModEnv = 25
    attackModEnv = 26
    holdModEnv = 27
    decayModEnv = 28
    sustainModEnv = 29
    releaseModEnv = 30
    keynumToModEnvHold = 31
    keynumToModEnvDecay = 32
    delayVolEnv = 33
    attackVolEnv = 34
    holdVolEnv = 35
    decayVolEnv = 36
    sustainVolEnv = 37
    releaseVolEnv = 38
    keynumToVolEnvHold = 39
    keynumToVolEnvDecay = 40
    instrument = 41
    keyRange = 43
    velRange = 44
    startloopAddrsCoarseOffset = 45
    keynum = 46
    velocity = 47
    initialAttenuation = 48
    endloopAddrsCoarseOffset = 50
    coarseTune = 51
    fineTune = 52
    sampleID = 53
    sampleModes = 54
    scaleTuning = 56
    exclusiveClass = 57
    overridingRootKey = 58
}

// ─── SF2 Sample Types ───

//! Mono sample — produces a single channel.
let public SF2_SAMPLE_MONO   = 1u
//! Right channel of a stereo pair (linked to a left sample).
let public SF2_SAMPLE_RIGHT  = 2u
//! Left channel of a stereo pair (linked to a right sample).
let public SF2_SAMPLE_LEFT   = 4u
//! Linked sample flag — indicates a multi-sample link set.
let public SF2_SAMPLE_LINKED = 8u

// ─── SF2 Data Structures ───

//! SF2 sample header — name, sample rate, pitch, loop points.
struct SF2SampleHeader {
    name : string           //! Sample name (up to 20 characters).
    start : uint            //! Start offset in sample data points.
    end : uint              //! End offset in sample data points.
    loop_start : uint       //! Loop start offset in sample data points.
    loop_end : uint         //! Loop end offset in sample data points.
    sample_rate : uint      //! Sample rate in Hz.
    original_pitch : int    //! MIDI key number of the recorded pitch (0-127).
    pitch_correction : int  //! Pitch correction in cents.
    sample_link : uint      //! Index of linked sample (for stereo pairs).
    sample_type : uint      //! Sample type flags (mono, left, right, linked).
}

//! SF2 generator — a parameter that modifies a voice (e.g. filter cutoff, envelope times).
struct SF2Generator {
    oper : int              //! Generator operator (SF2GenOper enum value).
    amount_i16 : int        //! Signed 16-bit amount.
    amount_lo : int         //! Low byte of the amount (for range-type generators).
    amount_hi : int         //! High byte of the amount (for range-type generators).
}

// SF2 modulator source flags (encoded in sfModSrcOper u16)
// Bits 0-6: source index (CC number or general controller enum)
// Bit 7: CC flag (0=general controller, 1=MIDI CC)
// Bit 8: direction (0=positive, 1=negative)
// Bit 9: polarity (0=unipolar, 1=bipolar)
// Bits 10-15: type (0=linear, 1=concave, 2=convex, 3=switch)
//
// General controller indices (when CC flag = 0):
//   0 = none (value = 1.0), 2 = noteOnVelocity, 3 = noteOnKey,
//   10 = keyPressure, 13 = channelPressure, 14 = pitchWheel, 16 = pitchWheelSensitivity

//! SF2 modulator — a routing from a source (e.g. velocity, CC) to a generator.
struct SF2Modulator {
    src_oper  : int     //! Primary modulator source (packed: index + flags).
    dest_oper : int     //! Destination generator operator index.
    amount    : int     //! Signed modulation amount.
    amt_src_oper : int  //! Secondary (amount) modulator source (packed: index + flags).
    transform : int     //! Transform type (0=linear, 2=absolute value).
}

// Unpack modulator source fields
def sf2_mod_src_index(src : int) : int {
    //! Extract the source index (bits 0-6) from a packed modulator source operand.
    return src & int(0x7F)
}
def sf2_mod_src_cc(src : int) : bool {
    //! Return true if the modulator source is a MIDI CC (bit 7 set).
    return (src & int(0x80)) != 0
}
def sf2_mod_src_negative(src : int) : bool {
    //! Return true if the modulator source direction is negative (bit 8 set).
    return (src & int(0x100)) != 0
}
def sf2_mod_src_bipolar(src : int) : bool {
    //! Return true if the modulator source polarity is bipolar (bit 9 set).
    return (src & int(0x200)) != 0
}
def sf2_mod_src_type(src : int) : int {
    //! Extract the modulator source curve type (bits 10-15): 0=linear, 1=concave, 2=convex, 3=switch.
    return (src >> 10) & int(0x3F)
}

//! SF2 zone -- a key/velocity region mapping to a sample with generators and modulators.
struct SF2Zone {
    key_lo : int = 0                    //! Lowest MIDI key this zone responds to.
    key_hi : int = 127                  //! Highest MIDI key this zone responds to.
    vel_lo : int = 0                    //! Lowest MIDI velocity this zone responds to.
    vel_hi : int = 127                  //! Highest MIDI velocity this zone responds to.
    generators : array<SF2Generator>    //! Generators that modify voice parameters for this zone.
    modulators : array<SF2Modulator>    //! Modulators that route dynamic sources to generators.
}

//! SF2 instrument definition -- a set of zones.
struct SF2InstrumentDef {
    name : string               //! Instrument name.
    global_zone : SF2Zone?      //! Optional global zone whose generators apply to all zones.
    zones : array<SF2Zone?>     //! Instrument zones, each mapping a key/velocity region to a sample.
}

//! SF2 preset definition -- bank/program number mapped to instruments.
struct SF2PresetDef {
    name : string               //! Preset name.
    preset : int                //! MIDI program number (0-127).
    bank : int                  //! MIDI bank number.
    global_zone : SF2Zone?      //! Optional global zone whose generators apply to all zones.
    zones : array<SF2Zone?>     //! Preset zones, each mapping a key/velocity region to an instrument.
}

//! Parsed SF2 soundfont file containing samples, instruments, and presets.
struct SF2File {
    name : string                       //! Soundfont name from the INFO chunk.
    sample_data : array<int16>          //! Raw 16-bit PCM sample data.
    samples : array<SF2SampleHeader>    //! Sample headers describing each sample region.
    instruments : array<SF2InstrumentDef?>  //! Instrument definitions.
    presets : array<SF2PresetDef?>      //! Preset definitions.
    preset_lookup : table<int; int>     //! Fast lookup: (bank << 8 | program) -> index into presets.
}

// ─── Binary Readers (Little-Endian) ───

def private read_u8(data : array<uint8>; var cursor : int&) : int {
    if (cursor >= length(data)) {
        return 0
    }
    let v = int(data[cursor])
    cursor ++
    return v
}

def private read_u16_le(data : array<uint8>; var cursor : int&) : int {
    if (cursor + 1 >= length(data)) {
        cursor = length(data)
        return 0
    }
    let v = int(data[cursor]) | (int(data[cursor + 1]) << 8)
    cursor += 2
    return v
}

def private read_i16_le(data : array<uint8>; var cursor : int&) : int {
    let v = read_u16_le(data, cursor)
    if (v >= int(0x8000)) {
        return v - int(0x10000)
    }
    return v
}

def private read_u32_le(data : array<uint8>; var cursor : int&) : uint {
    if (cursor + 3 >= length(data)) {
        cursor = length(data)
        return 0u
    }
    let v = uint(data[cursor]) | (uint(data[cursor + 1]) << 8u) | (uint(data[cursor + 2]) << 16u) | (uint(data[cursor + 3]) << 24u)
    cursor += 4
    return v
}

def private read_fourcc(data : array<uint8>; var cursor : int&) : string {
    if (cursor + 3 >= length(data)) {
        cursor = length(data)
        return ""
    }
    let cc = "{character_at(data[cursor])}{character_at(data[cursor+1])}{character_at(data[cursor+2])}{character_at(data[cursor+3])}"
    cursor += 4
    return cc
}

def private character_at(b : uint8) : string {
    return to_char(int(b))
}

def private read_fixed_string(data : array<uint8>; var cursor : int&; max_len : int) : string {
    let end_pos = min(cursor + max_len, length(data))
    let result = build_string() $(var writer) {
        for (i in range(cursor, end_pos)) {
            if (data[i] == 0u8) {
                break
            }
            write(writer, character_at(data[i]))
        }
    }
    cursor += max_len
    return result
}

// ─── Timecent Conversion ───

def public timecents_to_seconds(tc : int) : float {
    //! Convert SF2 timecents to seconds.
    if (tc <= -32768) {
        return 0.0
    }
    return pow(2.0, float(tc) / 1200.0)
}

def public cents_to_hz(cents : int) : float {
    //! Convert SF2 cents to Hz (relative to 8.176 Hz).
    return 8.176 * pow(2.0, float(cents) / 1200.0)
}

def public centibels_to_linear(cb : int) : float {
    //! Convert centibels to linear amplitude.
    return pow(10.0, float(-cb) / 200.0)
}

// ─── SF2 Parser ───

def private parse_sample_headers(data : array<uint8>; offset, size : int) : array<SF2SampleHeader> {
    var result : array<SF2SampleHeader>
    let record_size = 46
    let count = size / record_size
    if (count <= 1) {
        return <- result    // last record is terminal
    }
    result |> reserve(count - 1)
    var cursor = offset
    for (_ in range(count - 1)) {
        var sh = SF2SampleHeader()
        sh.name = read_fixed_string(data, cursor, 20)   // 20 bytes
        sh.start = read_u32_le(data, cursor)             // 4 bytes
        sh.end = read_u32_le(data, cursor)               // 4 bytes
        sh.loop_start = read_u32_le(data, cursor)        // 4 bytes
        sh.loop_end = read_u32_le(data, cursor)          // 4 bytes
        sh.sample_rate = read_u32_le(data, cursor)       // 4 bytes
        sh.original_pitch = read_u8(data, cursor)        // 1 byte
        let pc_byte = read_u8(data, cursor)              // 1 byte
        sh.pitch_correction = pc_byte >= 128 ? pc_byte - 256 : pc_byte
        sh.sample_link = uint(read_u16_le(data, cursor)) // 2 bytes
        sh.sample_type = uint(read_u16_le(data, cursor)) // 2 bytes = 46 total
        result |> push(sh)
    }
    return <- result
}

struct private RawPresetHeader {
    name : string
    preset : int
    bank : int
    pbag_index : int
}

struct private RawInstrumentHeader {
    name : string
    ibag_index : int
}

struct private RawBag {
    gen_index : int
    mod_index : int
}

def private parse_preset_headers(data : array<uint8>; offset, size : int) : array<RawPresetHeader> {
    var result : array<RawPresetHeader>
    let record_size = 38
    let count = size / record_size
    if (count <= 1) {
        return <- result
    }
    result |> reserve(count)
    var cursor = offset
    for (_ in range(count)) {
        var ph = RawPresetHeader()
        ph.name = read_fixed_string(data, cursor, 20)
        ph.preset = read_u16_le(data, cursor)
        ph.bank = read_u16_le(data, cursor)
        ph.pbag_index = read_u16_le(data, cursor)
        cursor += 4 + 4 + 4  // skip library, genre, morphology
        result |> push(ph)
    }
    return <- result
}

def private parse_instrument_headers(data : array<uint8>; offset, size : int) : array<RawInstrumentHeader> {
    var result : array<RawInstrumentHeader>
    let record_size = 22
    let count = size / record_size
    if (count <= 1) {
        return <- result
    }
    result |> reserve(count)
    var cursor = offset
    for (_ in range(count)) {
        var ih = RawInstrumentHeader()
        ih.name = read_fixed_string(data, cursor, 20)
        ih.ibag_index = read_u16_le(data, cursor)
        result |> push(ih)
    }
    return <- result
}

def private parse_bags(data : array<uint8>; offset, size : int) : array<RawBag> {
    var result : array<RawBag>
    let record_size = 4
    let count = size / record_size
    result |> reserve(count)
    var cursor = offset
    for (_ in range(count)) {
        var bag = RawBag()
        bag.gen_index = read_u16_le(data, cursor)
        bag.mod_index = read_u16_le(data, cursor)
        result |> push(bag)
    }
    return <- result
}

def private parse_generators(data : array<uint8>; offset, size : int) : array<SF2Generator> {
    var result : array<SF2Generator>
    let record_size = 4
    let count = size / record_size
    result |> reserve(count)
    var cursor = offset
    for (_ in range(count)) {
        var gen = SF2Generator()
        gen.oper = read_u16_le(data, cursor)
        let lo = read_u8(data, cursor)
        let hi = read_u8(data, cursor)
        gen.amount_lo = lo
        gen.amount_hi = hi
        gen.amount_i16 = lo | (hi << 8)
        if (gen.amount_i16 >= int(0x8000)) {
            gen.amount_i16 -= int(0x10000)
        }
        result |> push(gen)
    }
    return <- result
}

def private parse_modulators(data : array<uint8>; offset, size : int) : array<SF2Modulator> {
    var result : array<SF2Modulator>
    let record_size = 10
    let count = size / record_size
    if (count <= 1) {
        return <- result  // last entry is terminal
    }
    result |> reserve(count - 1)
    var cursor = offset
    for (_ in range(count - 1)) {
        var m = SF2Modulator()
        m.src_oper = read_u16_le(data, cursor)
        m.dest_oper = read_u16_le(data, cursor)
        let amount = read_u16_le(data, cursor)
        m.amount = amount >= int(0x8000) ? amount - int(0x10000) : amount
        m.amt_src_oper = read_u16_le(data, cursor)
        m.transform = read_u16_le(data, cursor)
        result |> push(m)
    }
    return <- result
}

// ─── Zone Resolution ───

def public has_generator(zone : SF2Zone; oper : int) : bool {
    //! Check whether a zone has a specific generator.
    for (gen in zone.generators) {
        if (gen.oper == oper) {
            return true
        }
    }
    return false
}

def public get_generator(zone : SF2Zone; oper : int; def_val : int = 0) : int {
    //! Get the value of a generator from a zone. Returns def_val if not found.
    for (gen in zone.generators) {
        if (gen.oper == oper) {
            return gen.amount_i16
        }
    }
    return def_val
}

def public get_generator_range(zone : SF2Zone; oper : int) : int2 {
    //! Get the range (low, high) of a range-type generator. Returns int2(0, 127) if not found.
    for (gen in zone.generators) {
        if (gen.oper == oper) {
            return int2(gen.amount_lo, gen.amount_hi)
        }
    }
    return int2(0, 127)
}

def private resolve_zones(bags : array<RawBag>; all_gens : array<SF2Generator>; all_mods : array<SF2Modulator>;
                          bag_start, bag_end : int; terminal_oper : int) : array<SF2Zone?> {
    var zones : array<SF2Zone?>
    zones |> reserve(bag_end - bag_start)
    for (bi in range(bag_start, bag_end)) {
        let gen_start = bags[bi].gen_index
        let gen_end = bi + 1 < length(bags) ? bags[bi + 1].gen_index : length(all_gens)
        let mod_start = bags[bi].mod_index
        let mod_end = bi + 1 < length(bags) ? bags[bi + 1].mod_index : length(all_mods)
        var zone = new SF2Zone()
        for (gi in range(gen_start, gen_end)) {
            if (gi < length(all_gens)) {
                var gen = all_gens[gi]
                // EMU8000 attenuation factor: initialAttenuation is multiplied by 0.4
                // at load time. This compensates for EMU8000 hardware gain that most SF2
                // editors calibrate against. Without this, attenuation values are too high.
                if (gen.oper == int(SF2GenOper.initialAttenuation)) {
                    gen.amount_i16 = int(float(gen.amount_i16) * 0.4)
                }
                zone.generators |> push(gen)
            }
        }
        for (mi in range(mod_start, mod_end)) {
            if (mi < length(all_mods)) {
                zone.modulators |> push(all_mods[mi])
            }
        }
        // extract key/vel ranges
        let kr = get_generator_range(*zone, int(SF2GenOper.keyRange))
        zone.key_lo = kr.x
        zone.key_hi = kr.y
        let vr = get_generator_range(*zone, int(SF2GenOper.velRange))
        zone.vel_lo = vr.x
        zone.vel_hi = vr.y
        zones |> push(zone)
    }
    return <- zones
}

def private split_global_zone(var zones : array<SF2Zone?>; terminal_oper : int) : SF2Zone? {
    if (length(zones) == 0) {
        return null
    }
    // first zone is global if it has NO terminal generator (instrument or sampleID)
    if (!has_generator(*zones[0], terminal_oper)) {
        var gz = zones[0]
        zones |> erase(0)
        return gz
    }
    return null
}

// ─── Top-Level Parse ───

def public sf2_load(path : string) : SF2File? {
    //! Load and parse an SF2 soundfont file from disk.
    var data : array<uint8>
    fopen(path, "rb") $(f) {
        if (f != null) {
            let st = fstat(f)
            if (st.is_valid) {
                data |> resize(int(st.size))
                fread(f, data)
            }
        }
    }
    if (length(data) < 12) {
        to_log(0, "SF2: file too small or not found: {path}\n")
        return null
    }
    return <- sf2_parse(data)
}

def public sf2_parse(data : array<uint8>) : SF2File? {
    //! Parse SF2 data from a byte array.
    var cursor = 0

    // RIFF header
    let riff = read_fourcc(data, cursor)
    if (riff != "RIFF") {
        to_log(0, "SF2: not a RIFF file\n")
        return null
    }
    let file_size = read_u32_le(data, cursor)
    let form = read_fourcc(data, cursor)
    if (form != "sfbk") {
        to_log(0, "SF2: not an SF2 file (form={form})\n")
        return null
    }

    var result = new SF2File()

    // raw pdta arrays
    var raw_phdr : array<RawPresetHeader>
    var raw_inst : array<RawInstrumentHeader>
    var raw_pbag : array<RawBag>
    var raw_ibag : array<RawBag>
    var raw_pgen : array<SF2Generator>
    var raw_igen : array<SF2Generator>
    var raw_pmod : array<SF2Modulator>
    var raw_imod : array<SF2Modulator>

    // parse LIST chunks
    while (cursor < length(data) - 8) {
        let chunk_id = read_fourcc(data, cursor)
        let chunk_size = int(read_u32_le(data, cursor))
        let chunk_end = cursor + chunk_size

        if (chunk_id == "LIST") {
            let list_type = read_fourcc(data, cursor)

            if (list_type == "INFO") {
                // parse INFO sub-chunks for name
                while (cursor < chunk_end) {
                    let sub_id = read_fourcc(data, cursor)
                    let sub_size = int(read_u32_le(data, cursor))
                    if (sub_id == "INAM") {
                        result.name = read_fixed_string(data, cursor, sub_size)
                    } else {
                        cursor += sub_size
                    }
                    // pad to even
                    if (sub_size % 2 != 0) {
                        cursor ++
                    }
                }

            } elif (list_type == "sdta") {
                // sample data
                while (cursor < chunk_end) {
                    let sub_id = read_fourcc(data, cursor)
                    let sub_size = int(read_u32_le(data, cursor))
                    if (sub_id == "smpl") {
                        let num_samples = sub_size / 2
                        result.sample_data |> resize(num_samples)
                        // bulk copy — data is little-endian int16, same as native on x86/x64
                        unsafe {
                            memcpy(addr(result.sample_data[0]), addr(data[cursor]), sub_size)
                        }
                        cursor += sub_size
                    } else {
                        cursor += sub_size
                    }
                    if (sub_size % 2 != 0) {
                        cursor ++
                    }
                }

            } elif (list_type == "pdta") {
                // preset data — 9 sub-chunks
                while (cursor < chunk_end) {
                    let sub_id = read_fourcc(data, cursor)
                    let sub_size = int(read_u32_le(data, cursor))
                    let sub_start = cursor

                    if (sub_id == "phdr") {
                        raw_phdr <- parse_preset_headers(data, sub_start, sub_size)
                    } elif (sub_id == "pbag") {
                        raw_pbag <- parse_bags(data, sub_start, sub_size)
                    } elif (sub_id == "pmod") {
                        raw_pmod <- parse_modulators(data, sub_start, sub_size)
                    } elif (sub_id == "pgen") {
                        raw_pgen <- parse_generators(data, sub_start, sub_size)
                    } elif (sub_id == "inst") {
                        raw_inst <- parse_instrument_headers(data, sub_start, sub_size)
                    } elif (sub_id == "ibag") {
                        raw_ibag <- parse_bags(data, sub_start, sub_size)
                    } elif (sub_id == "imod") {
                        raw_imod <- parse_modulators(data, sub_start, sub_size)
                    } elif (sub_id == "igen") {
                        raw_igen <- parse_generators(data, sub_start, sub_size)
                    } elif (sub_id == "shdr") {
                        result.samples <- parse_sample_headers(data, sub_start, sub_size)
                    }

                    cursor = sub_start + sub_size
                    if (sub_size % 2 != 0) {
                        cursor ++
                    }
                }
            } else {
                cursor = chunk_end
            }
        } else {
            cursor = chunk_end
        }
    }

    // ─── Resolve Instruments ───
    let num_inst = length(raw_inst) - 1  // last is terminal
    result.instruments |> reserve(num_inst)
    for (ii in range(num_inst)) {
        var inst = new SF2InstrumentDef()
        inst.name = raw_inst[ii].name
        let bag_start = raw_inst[ii].ibag_index
        let bag_end = raw_inst[ii + 1].ibag_index
        var zones <- resolve_zones(raw_ibag, raw_igen, raw_imod, bag_start, bag_end, int(SF2GenOper.sampleID))
        inst.global_zone = split_global_zone(zones, int(SF2GenOper.sampleID))
        inst.zones <- zones
        result.instruments |> push(inst)
    }

    // ─── Resolve Presets ───
    let num_presets = length(raw_phdr) - 1  // last is terminal
    result.presets |> reserve(num_presets)
    for (pi in range(num_presets)) {
        var preset = new SF2PresetDef()
        preset.name = raw_phdr[pi].name
        preset.preset = raw_phdr[pi].preset
        preset.bank = raw_phdr[pi].bank
        let bag_start = raw_phdr[pi].pbag_index
        let bag_end = raw_phdr[pi + 1].pbag_index
        var zones <- resolve_zones(raw_pbag, raw_pgen, raw_pmod, bag_start, bag_end, int(SF2GenOper.instrument))
        preset.global_zone = split_global_zone(zones, int(SF2GenOper.instrument))
        preset.zones <- zones
        result.presets |> push(preset)
        // build lookup
        let key = (preset.bank << 8) | preset.preset
        result.preset_lookup[key] = pi
    }

    to_log(0, "SF2: loaded \"{result.name}\" — {num_presets} presets, {num_inst} instruments, {length(result.samples)} samples, {length(result.sample_data)} sample frames\n")
    return <- result
}

// ─── Lookup ───

def public sf2_find_preset(sf2 : SF2File; bank, program : int) : int {
    //! Find a preset by bank and program number. Returns the preset index, or -1 if not found.
    let key = (bank << 8) | program
    return sf2.preset_lookup?[key] ?? -1
}

def public sf2_find_zones(sf2 : SF2File; preset_index : int; key, velocity : int) : array<int2> {
    //! Find instrument zones matching a key and velocity for a given preset. Returns array of (instrument_index, zone_index) pairs.
    var result : array<int2>
    if (preset_index < 0 || preset_index >= length(sf2.presets)) {
        return <- result
    }
    let preset = sf2.presets[preset_index]
    for (pzone in preset.zones) {
        // check key/vel range
        if (key < pzone.key_lo || key > pzone.key_hi) {
            continue
        }
        if (velocity < pzone.vel_lo || velocity > pzone.vel_hi) {
            continue
        }
        // get instrument index
        let inst_idx = get_generator(*pzone, int(SF2GenOper.instrument), -1)
        if (inst_idx < 0 || inst_idx >= length(sf2.instruments)) {
            continue
        }
        let inst = sf2.instruments[inst_idx]
        // SF2 spec: instrument zones inherit key/vel ranges from global zone
        // when they don't specify their own. Intersect ranges accordingly.
        let gi = inst.global_zone
        let gi_key_lo = gi != null ? gi.key_lo : 0
        let gi_key_hi = gi != null ? gi.key_hi : 127
        let gi_vel_lo = gi != null ? gi.vel_lo : 0
        let gi_vel_hi = gi != null ? gi.vel_hi : 127
        // find matching instrument zones
        for (iz_idx in range(length(inst.zones))) {
            let izone = inst.zones[iz_idx]
            // intersect izone range with global zone range
            let eff_key_lo = izone.key_lo > gi_key_lo ? izone.key_lo : gi_key_lo
            let eff_key_hi = izone.key_hi < gi_key_hi ? izone.key_hi : gi_key_hi
            let eff_vel_lo = izone.vel_lo > gi_vel_lo ? izone.vel_lo : gi_vel_lo
            let eff_vel_hi = izone.vel_hi < gi_vel_hi ? izone.vel_hi : gi_vel_hi
            if (key < eff_key_lo || key > eff_key_hi) {
                continue
            }
            if (velocity < eff_vel_lo || velocity > eff_vel_hi) {
                continue
            }
            result |> push(int2(inst_idx, iz_idx))
        }
    }
    return <- result
}

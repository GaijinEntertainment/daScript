options gen2
options indenting = 4
options stack = 524288
options persistent_heap   // a dastest root frees what it deletes only on this heap; the linear heap holds every carrier a cell deleted until the context ends

// model-free: the hardware-keyed box tag (box_ident) - the fleet table maps every recorded
// box's CPU brand to the store tag its records already carry, the override always wins, and
// unknown hardware gets its own slug, never another box's store (the m1-default landmine) -
// and the box probes beside it: the WMI answer's lines and their values, the CIM script that
// stands in for a missing wmic, the Linux RAM read under a cgroup limit, and this box's own
// RAM and CPU brand through the live probes (a Linux box with no "model name" line and no
// lscpu skips: the brand probe has nothing to read there).

require dastest/testing_boost public
require ../performance/box_ident.das

[test]
def test_wmi_answers(t : T?) {
    t |> run("a WMI answer's Key=value lines: wmic's \\r\\r\\n endings and blank lines dropped, CIM's plain lines kept") @(t : T?) {
        var inscope wmic <- wmi_lines_of("\r\r\nCapacity=34359738368\r\r\nSpeed=3200\r\r\n\r\r\nCapacity=34359738368\r\r\nSpeed=3200\r\r\n\r\r\n")
        t |> equal(4, length(wmic))
        t |> equal("Capacity=34359738368", wmic[0])
        t |> equal("Speed=3200", wmic[1])
        t |> equal("Speed=3200", wmic[3])
        var inscope cim <- wmi_lines_of("Name=NVIDIA GeForce RTX 5090\nName=Parsec Virtual Display Adapter\n")
        t |> equal(2, length(cim))
        t |> equal("Name=Parsec Virtual Display Adapter", cim[1])
        var inscope none <- wmi_lines_of("")
        t |> equal(0, length(none), "no answer, no lines")
        var inscope noise <- wmi_lines_of("'wmic' is not recognized as an internal or external command,\noperable program or batch file.\n")
        t |> equal(0, length(noise), "a line with no '=' is no value")
    }
    t |> run("a line's value by key: its own key only, the first '=' the split, both sides stripped") @(t : T?) {
        t |> equal("NVIDIA GeForce RTX 5090", kv_value("Name=NVIDIA GeForce RTX 5090", "Name"))
        t |> equal("", kv_value("Name=NVIDIA GeForce RTX 5090", "Speed"), "another key")
        t |> equal("3200", kv_value(" Speed = 3200 ", "Speed"))
        t |> equal("a=b", kv_value("Name=a=b", "Name"), "the first '=' splits")
        t |> equal("", kv_value("NumberOfCores", "NumberOfCores"), "no '=' at all")
        t |> equal("", kv_value("TotalPhysicalMemory=68632934400", "Memory"), "a key suffix is another key")
        t |> equal("", kv_value("Speeds", "Speed"), "no '=': the key alone is no match either")
    }
    t |> run("the values of one key across lines: empty ones and other keys skipped, order kept") @(t : T?) {
        var inscope vals <- kv_values(["Name=NVIDIA GeForce RTX 5090", "Name=", "Speed=3200", "Name=Parsec Virtual Display Adapter"], "Name")
        t |> equal(2, length(vals))
        t |> equal("NVIDIA GeForce RTX 5090", vals[0])
        t |> equal("Parsec Virtual Display Adapter", vals[1])
        var inscope none <- kv_values(["Speed=3200"], "Name")
        t |> equal(0, length(none))
    }
    t |> run("the RAM modules of a Capacity,Speed answer: the count, one module's GiB, its speed") @(t : T?) {
        let four = ram_modules_of(wmi_lines_of("\r\r\nCapacity=34359738368\r\r\nSpeed=3200\r\r\n\r\r\nCapacity=34359738368\r\r\nSpeed=3200\r\r\n\r\r\n"))
        t |> equal(2, four.count)
        t |> equal(32, four.cap_gb)
        t |> equal(3200, four.speed)
        let none = ram_modules_of(wmi_lines_of(""))
        t |> equal(0, none.count, "no answer, no modules")
    }
    t |> run("the CIM script answers the class's properties in wmic's own Key=value lines") @(t : T?) {
        t |> equal("Get-CimInstance Win32_PhysicalMemory | ForEach-Object \{ foreach ($p in 'Capacity','Speed') \{ $p + '=' + $_.$p \} \}",
            cim_script("Win32_PhysicalMemory", "Capacity,Speed"))
        t |> equal("Get-CimInstance Win32_ComputerSystem | ForEach-Object \{ foreach ($p in 'TotalPhysicalMemory') \{ $p + '=' + $_.$p \} \}",
            cim_script("Win32_ComputerSystem", "TotalPhysicalMemory"))
    }
}

[test]
def test_box_probes(t : T?) {
    t |> run("a cgroup limit file's byte count, no limit read as none") @(t : T?) {
        t |> equal(58, ram_gb_from_cgroup("61999996928\n"), "a 62 GB container limit (v2)")
        t |> equal(1, ram_gb_from_cgroup("268435456\n"), "256 MiB reads as one GiB, not no limit")
        t |> equal(0, ram_gb_from_cgroup("max\n"), "v2 with no limit")
        t |> equal(0, ram_gb_from_cgroup("9223372036854771712\n"), "v1 with no limit")
        t |> equal(0, ram_gb_from_cgroup(""), "no cgroup file")
        t |> equal(0, ram_gb_from_cgroup("+68719476736\n"), "a signed count")
        t |> equal(0, ram_gb_from_cgroup(".5e11\n"), "text opening on no digit")
        t |> equal(0, ram_gb_from_cgroup("nan\n"), "a float spelling")
    }
    t |> run("a Linux box's RAM: the v2 limit, else the v1 one, where under the total") @(t : T?) {
        let mem = "MemFree:  1024 kB\nMemTotal:       67108864 kB\n"
        let gb16 = "17179869184\n"
        let gb8 = "8589934592\n"
        t |> equal(64, linux_ram_gb(mem, "", ""), "no cgroup: the total")
        t |> equal(16, linux_ram_gb(mem, gb16, gb8), "the v2 limit, the v1 file unread")
        t |> equal(8, linux_ram_gb(mem, "max\n", gb8), "v2 with no limit: the v1 one")
        t |> equal(8, linux_ram_gb(mem, "", gb8), "no v2 file: the v1 one")
        t |> equal(64, linux_ram_gb(mem, "137438953472\n", ""), "a limit past the total")
        t |> equal(16, linux_ram_gb("", gb16, ""), "the total unread: the limit")
        t |> equal(0, linux_ram_gb("", "max\n", ""), "neither: unknown")
    }
    t |> run("the live probes: THIS box answers its RAM and its CPU brand; on Windows WMI answers too") @(t : T?) {
        let cpu = probe_cpu_brand()
        if (get_platform_name() == "linux" && empty(cpu) && empty(run_capture_argv(["lscpu", "--version"]))) {
            t |> skip("no \"model name\" line in /proc/cpuinfo and no lscpu on this box: the brand probe has nothing to read")
            return
        }
        t |> success(!empty(cpu), "the CPU brand reads non-empty ('{cpu}')")
        let gb = box_ram_gb()
        t |> success(gb > 0, "the box's RAM reads above 0 ({gb} GiB)")
        if (get_platform_name() == "windows") {
            var inscope ram <- wmi_lines("Win32_ComputerSystem", "TotalPhysicalMemory")
            t |> equal(1, length(ram), "one computer system instance")
            let first = empty(ram) ? "" : ram[0]
            t |> success(!empty(kv_value(first, "TotalPhysicalMemory")), "its TotalPhysicalMemory ('{first}')")
            var inscope cores <- wmi_prop("Win32_Processor", "NumberOfCores")
            t |> success(!empty(cores), "every processor's core count ({length(cores)} socket(s))")
        }
    }
}

[test]
def test_box_ident(t : T?) {
    t |> run("fleet table: recorded CPU brands map to their store tags") @(t : T?) {
        t |> equal("m1", box_tag_for_cpu("Apple M1 Max"))
        t |> equal("m3air", box_tag_for_cpu("Apple M3"))
        t |> equal("m4", box_tag_for_cpu("Apple M4 Pro"))
        t |> equal("m5", box_tag_for_cpu("Apple M5 Max"))
        t |> equal("zen2", box_tag_for_cpu("AMD Ryzen Threadripper 3990X 64-Core Processor"))
        t |> equal("zen4", box_tag_for_cpu("AMD Ryzen 7 PRO 8700GE w/ Radeon 780M Graphics"))
        t |> equal("gnr", box_tag_for_cpu("Intel(R) Xeon(R) 6975P-C"))
        t |> equal("epyc9r14", box_tag_for_cpu("AMD EPYC 9R14"))
        t |> equal("graviton4", box_tag_for_cpu("Neoverse-V2"))
    }
    t |> run("unknown hardware slugs from the brand - never a fleet tag") @(t : T?) {
        t |> equal("apple-m6-ultra", box_tag_for_cpu("Apple M6 Ultra"))
        t |> equal("intel-r-xeon-r-w9-3495x", box_tag_for_cpu("Intel(R) Xeon(R) w9-3495X"))
        t |> equal("unknown-box", box_tag_for_cpu(""))
        // the M3 row is EXACT (the Air's base-chip brand): every bigger M3 slugs, or it
        // would inherit the Air's crowns and poison the Air's store
        t |> equal("apple-m3-pro", box_tag_for_cpu("Apple M3 Pro"))
        t |> equal("apple-m3-max", box_tag_for_cpu("Apple M3 Max"))
        t |> equal("apple-m3-ultra", box_tag_for_cpu("Apple M3 Ultra"))
    }
    t |> run("the DASLLAMA_BOX override wins over any probe") @(t : T?) {
        t |> equal("zen4", derive_box_name("zen4"))
        t |> equal("custom-tag", derive_box_name("custom-tag"))
    }
    t |> run("an unset env derives the tag from THIS box's probed CPU brand") @(t : T?) {
        // the landmine this file exists for: an unset env must resolve through the probe +
        // table on every box (a reintroduced literal default breaks the equality anywhere
        // the hardware is not that literal's box)
        let cpu = probe_cpu_brand()
        let derived = derive_box_name("")
        t |> success(!empty(derived), "derived tag is non-empty ('{derived}' from '{cpu}')")
        t |> equal(box_tag_for_cpu(cpu), derived)
    }
}

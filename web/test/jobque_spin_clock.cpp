#include "../../src/misc/job_que_spin.h"

using Clock = das::detail::JobQueSpinClock;
using Deadline = das::detail::JobQueSpinDeadline<Clock>;
static Deadline window(0);

extern "C" double spin_clock_now() {
    return std::chrono::duration<double, std::milli>(Clock::now().time_since_epoch()).count();
}
extern "C" void spin_clock_reset(int micros) { window = Deadline(micros); }
extern "C" int spin_clock_expired(int worked) { return window.expired(worked != 0); }

#pragma once

// nano shadow of daScript/misc/performance_time.h - see simulate/bin_serializer.h
// for why the `require $` include set arrives here empty.
//
// The real header exposes ref_time_ticks / get_time_usec, which a freestanding
// target has no portable clock for. A script that actually calls them fails to
// link on nano rather than silently reading a stopped clock.

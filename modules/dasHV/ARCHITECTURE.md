# dasHV architecture

This document follows the repository's `ARCHITECTURE_COMMON.md` contract.

## WebSocket admission capacity {#websocket-admission-capacity}

Pending admission tickets and registered WebSocket channels share the channel
admission limit. A closed channel remains registered until its queued close
callback runs, so it continues to occupy admission capacity during that interval.
Close callbacks use reserved cleanup capacity instead of ordinary event admission;
the bounded channel registry limits the number of pending close callbacks.

# SIMH compatibility audit for the Fuzzball Pico branch

This branch is a Pico-native PDP-11/40-derived emulator. It cannot consume
SIMH's PDP-11 source as a merge: its CPU loop, bus, interrupts, disk devices,
and MMU are distinct implementations. Each SIMH change must be translated only
when the architectural behavior exists here.

Audit reference: `open-simh/simh` `origin/master`
`a1f57fa3738ed31148d31126ba1a7278ff845c6d`, refreshed 2026-09-16.

## Applied changes

| Topic | Pico implementation | SIMH relevance |
| --- | --- | --- |
| KT11-C register map | PAR/PDR blocks use the DCN-compatible supervisor, kernel, and user locations. | Required by the Mills DCN supervisor; recovered from the earlier `Pico_1140_fuzzball` snapshot. |
| PDR state | Successful translations set PDR A; successful writes also set PDR W. | Mirrors current SIMH relocation behavior. |

## Audited but not mechanically ported

| SIMH area | Decision |
| --- | --- |
| Model-specific 11/45, 11/60, 11/70, and J11 byte-lane fixes | Not applicable to this fixed Pico 11/40-derived CPU. |
| Q22 I/O-page boundary and 22-bit memory mapping | Not applicable until a viable external-memory topology is selected; the current Pico bus has a 16-bit virtual address path and 224 KiB guest array. |
| SIMH interrupt-table/vector acknowledgement fixes | The Pico uses its own fixed interrupt queue. Revisit while adding KWV and DMV/DMC devices. |
| FP11 MOD and undefined-variable fixes | The port's embedded SIMH-derived FP11 code already contains the equivalent control flow; no change made in this pass. |
| MMR1/MMR2 loading, trap order, and frozen-MMU semantics | Not safely mergeable. The Pico MMU exposes SR0/SR1/SR2 but does not yet model the full SIMH state machine. This is a dedicated correctness task before claiming a BOS boot. |

The most recent upstream PDP-11 CPU change is for the Terak 8510/a simulator
and is not applicable to this Pico 11/40-derived target. The current
SIMH-derived FP11 source already includes the relevant MODf and
undefined-variable behavior, so this audit intentionally does not re-import
it.

## Next device work

After the baseline builds and this MMU slice is tested on Pico 2 W, implement
the minimum KWV11 behavior and DMV/DMC boot register stub as Pico-native
devices. The ESP/SIMH implementations are behavioral references, not source
to copy.

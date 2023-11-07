## OPEMU for Linux

### Notice
```diff
- [warning this is experimentall fork]
```
 - TODO
   - fix compiler warnings
   - add tests for all instructions
      - maybe use single test by instructtions on assembler
      - maybe use test code contains all test instructions (some other project contains hard SIMD optimisations)

- This is an adaptation of the original Opcode Emulator from [xnu-amd's repo](https://github.com/sinetek/xnu-amd/tree/master/osfmk/OPEMU) for Linux.

- For the kernel module, and a few other things (fpins, SSE4.1...), I took some code from [Meowthra's AVX+ OPEMU](https://www.insanelymac.com/forum/topic/338919-opcode-emulator-opemu-for-linux-64-bit/), which looks pretty good, but does not fit my needs (SSSE3, SSE4.1, & SSE4.2).

- POPCNT and CRC32 are all merit of [Andy Vandijck](https://www.insanelymac.com/forum/topic/281450-mavericks-kernel-testing-on-amd-formerly-mountain-lion-kernel-testing-on-amd/?page=211#comment-1982883) instead.

## What's working

- SSE3, SSSE3, SSE4.2 (mostly). CRC32 doesn't pass [tests](https://github.com/htot/crc32c) though.
- most SSE4.1 instructions are missing, I tried to add a few but it's not working well yet.

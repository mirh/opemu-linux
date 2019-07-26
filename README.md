## OPEMU for Linux

- This is an adaptation of the original Opcode Emulator from [xnu-amd's repo](https://github.com/sinetek/xnu-amd/tree/master/osfmk/OPEMU) for Linux.

- For the kernel module, and a few other things (fpins, SSE4.1...), I took some code from [Meowthra's OPEMU](https://www.insanelymac.com/forum/topic/338919-opcode-emulator-opemu-for-linux-64-bit/), which looks pretty good, but does not fit my needs (SSSE3, SSE4.1, & SSE4.2).

## What's working

- SSE3, SSSE3, SSE4.2 (mostly).
- most SSE4.1 instructions are missing, I tried to add a few but it's not working well yet.

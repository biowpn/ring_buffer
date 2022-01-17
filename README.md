# Ring Buffer

Yet another multi-writer lock-free queue: [ring_buffer_mt.hpp](include\ring_buffer\ring_buffer_mt.hpp)

But this one assumes that the reader can always catch up: the writers don't wait for the reader.

This library is minimialist; there is only a single `struct ring_buffer_mt` and two methods: `read()` and `write()`. It is intended to serve as a building block.

All `ring_buffer_mt` needs is a block of memory, as well as two `atomic<size_t>` to play with.
 - Within a process, it can facilitate inter-thread communication
 - If you supply it with shared memory (the atomics need to be on shared memory as well), it can facilitate inter-process communication

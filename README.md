
# Ring Buffer

Simple header only ring buffer library. All algorithms are template functions. Works on any underlying container with bi-directional iterators.


## Usage

[One file](include/ring_buffer/ring_buffer.hpp) is all you need.

8 functions are provided:

- `next()` : increase the ring buffer iterator by one step
- `prev()` : decrease the ring buffer iterator by one step
- `empty()` : check if the buffer is empty
- `full()` : check if the buffer is full
- `push_back()` : add a new element at the end
- `pop_back()` : remove the last element
- `push_front()` : insert a new element at the beginning
- `pop_front()` : removes the first element

which are enough to perform most ring buffer operations.

## Example

```
// The algorithm "wastes" one slot,
// so if we want to have a buffer size of 4, the underlying container should be of size 5
std::vector<int> int_vec(4 + 1);

// auto two extra iterators to fully describe the ring buffer
auto head = int_vec.begin();
auto tail = int_vec.begin();

ring_buffer::push_back(int_vec.begin(), int_vec.end(), head, tail, 1);
// buffer: [1, ]

ring_buffer::push_back(int_vec.begin(), int_vec.end(), head, tail, 2);
ring_buffer::push_back(int_vec.begin(), int_vec.end(), head, tail, 3);
ring_buffer::push_back(int_vec.begin(), int_vec.end(), head, tail, 4);
// buffer: [1, 2, 3, 4, ]

ring_buffer::push_back(int_vec.begin(), int_vec.end(), head, tail, 5);
// buffer: [2, 3, 4, 5, ]

// Iterate over the ring buffer, from beginning to end
int sum = 0;
for (auto it = tail; it != head; next(int_vec.begin(), int_vec.end(), it))
{
    sum += *it;
}
```

Refer to [test_ring_buffer.cpp](test/test_ring_buffer.cpp) for more examples.

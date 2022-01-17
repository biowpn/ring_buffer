
#include <ring_buffer/ring_buffer_mt.hpp>

#include <thread>
#include <iostream>
#include <vector>

using namespace ring_buffer;

void writer(ring_buffer_mt ring /*by value is ok*/, const char *msg, std::size_t repeat)
{
    auto len = std::strlen(msg);
    for (; repeat > 0; --repeat)
    {
        write(msg, len, ring);
    }
}

void checker(const std::vector<char> &buf)
{
    for (std::size_t i = 0; i < buf.size();)
    {
        auto c = buf[i];
        std::cout << c;
        if ('1' <= c && c <= '4')
        {
            std::size_t n = c - '0';
            // expect the next (n - 1) bytes to be the same as this byte
            for (std::size_t j = 1; j < n; ++j)
            {
                auto c2 = buf[j];
                std::cout << c2;
                if (c2 != c)
                {
                    printf("\nFailed: position %zu got '%c' instead of '%c'\n", i + j, c2, c);
                    throw std::runtime_error("Bad byte");
                }
            }
            i += n;
        }
        else
        {
            throw std::runtime_error("Not digit");
        }
    }
}

// the idea is to have 4 writers write "1" "22" "333" "4444" respectively,
// then check if the content are correct
void test_ring_buffer_mt()
{
    // all writers write the same amount of bytes
    const std::size_t payload_per_writer = 12 * 1000;
    std::vector<char> buf(4 * payload_per_writer);
    std::atomic<std::size_t> head_committed{0};
    std::atomic<std::size_t> head_pending{0};
    ring_buffer_mt ring{(unsigned char *)buf.data(), buf.size(), &head_committed, &head_pending};

    const char *messages[]{"1", "22", "333", "4444"};
    std::thread workers[4]{};
    for (std::size_t i = 0; i < 4; ++i)
    {
        auto msg = messages[i];
        auto repeat = payload_per_writer / std::strlen(msg);
        workers[i] = std::thread(writer, ring, msg, repeat);
    }
    for (std::size_t i = 0; i < 4; ++i)
    {
        workers[i].join();
    }
    checker(buf);
}

int main()
{
    test_ring_buffer_mt();
    return 0;
}

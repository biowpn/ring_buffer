
#include <ring_buffer/ring_buffer_mt.hpp>

#include <thread>
#include <cctype>
#include <iostream>
#include <vector>

using namespace ring_buffer;

void check_content(const char *buf, std::size_t len)
{
    for (std::size_t i = 0; i < len;)
    {
        if (!std::isdigit(buf[i]))
        {
            printf("\nFailed: position %zu is not digit: '%c'\n", buf[i]);
            throw std::runtime_error("");
        }
        std::size_t n = buf[i] - '0';
        // expect the next (n - 1) bytes to be the same as this byte
        for (std::size_t j = i + 1; j < i + n; ++j)
        {
            if (buf[j] != buf[i])
            {
                printf("\nFailed: position %zu got '%c' instead of '%c'\n", j, buf[j], buf[i]);
                throw std::runtime_error("");
            }
        }
        i += n;
    }
}

void reader(ring_buffer_mt ring /*by value is ok*/, std::size_t total)
{
    std::vector<char> rbuf(total);
    std::size_t tail{0};
    std::size_t n_read = 0;
    auto dst = rbuf.data();
    for (std::size_t n; n_read < total; n_read += n)
    {
        while ((n = read(dst, total, ring, tail)) == 0)
            ;
        check_content(dst, n);
    }
    if (n_read != total)
    {
        printf("Failed: read more bytes than expected (%zu vs %zu)\n", n_read, total);
        throw std::runtime_error("");
    }
    printf("reader has verified all %zu bytes \n", total);
}

void writer(ring_buffer_mt ring /*by value is ok*/, const char *msg, std::size_t repeat)
{
    auto len = std::strlen(msg);
    for (; repeat > 0; --repeat)
    {
        write(msg, len, ring);
    }
}

// the idea is to have 4 writers write "1" "22" "333" "4444" respectively,
// and have a reader to read and check whether the content is correct
void test_ring_buffer_mt()
{
    // all writers write the same amount of bytes
    const std::size_t payload_per_writer = 12 * 1000;
    std::vector<char> buf(4 * payload_per_writer);
    std::atomic<std::size_t> head_committed{0};
    std::atomic<std::size_t> head_pending{0};
    ring_buffer_mt ring{(unsigned char *)buf.data(), buf.size(), &head_committed, &head_pending};

    const char *messages[]{"1", "22", "333", "4444"};

    std::thread treader(reader, ring, buf.size());
    std::thread tworkers[4]{};
    for (std::size_t i = 0; i < 4; ++i)
    {
        auto msg = messages[i];
        auto repeat = payload_per_writer / std::strlen(msg);
        tworkers[i] = std::thread(writer, ring, msg, repeat);
    }
    for (std::size_t i = 0; i < 4; ++i)
    {
        tworkers[i].join();
    }
    treader.join();

    printf("ring_buffer_mt test passed\n");
}

int main()
{
    test_ring_buffer_mt();
    return 0;
}

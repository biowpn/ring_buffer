
#pragma once

#include <algorithm>
#include <atomic>
#include <cstddef>
#include <cstring>

namespace ring_buffer
{
    struct ring_buffer_mt
    {
        unsigned char *buf;
        std::size_t capacity;
        std::atomic<std::size_t> *head_commited;
        std::atomic<std::size_t> *head_pending;
    };

    /// \brief Write \a len bytes from \a src to ring buffer \a ring.
    /// \return len
    std::size_t write(const void *src, std::size_t len, ring_buffer_mt &ring)
    {
        auto head_old = ring.head_pending->fetch_add(len);
        auto head_new = head_old + len;
        auto i_begin = head_old % ring.capacity;
        auto i_end = head_new % ring.capacity;
        if (i_begin < i_end)
        {
            std::memcpy(ring.buf + i_begin, src, len);
        }
        else
        {
            std::memcpy(ring.buf + i_begin, src, ring.capacity - i_begin);
            std::memcpy(ring.buf, (const unsigned char *)src + len - i_end, i_end);
        }
        for (auto ho = head_old;
             !std::atomic_compare_exchange_weak(ring.head_commited, &ho, head_new);
             ho = head_old)
            ;
        return len;
    }

    /// \brief Read at most \a len bytes into \a dst from ring buffer \a ring.
    /// \return The actual number of bytes read
    std::size_t read(void *dst, std::size_t len, ring_buffer_mt &ring, std::size_t &tail)
    {
        auto head = ring.head_commited->load();
        if (tail >= head)
        {
            return 0;
        }
        head = std::min(head, tail + len);
        len = head - tail;
        auto i_begin = tail % ring.capacity;
        auto i_end = head % ring.capacity;
        if (i_begin < i_end)
        {
            std::memcpy(dst, ring.buf + i_begin, len);
        }
        else
        {
            std::memcpy(dst, ring.buf + i_begin, ring.capacity - i_begin);
            std::memcpy((unsigned char *)dst + len - i_end, ring.buf, i_end);
        }
        return len;
    }
}

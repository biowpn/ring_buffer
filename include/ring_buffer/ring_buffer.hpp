
#pragma once

namespace ring_buffer
{

template <class It>
inline It &next(It first, It last, It &it)
{
    if (++it == last)
    {
        it = first;
    }
    return it;
}

template <class It>
inline It &prev(It first, It last, It &it)
{
    if (it-- == first)
    {
        it = --last;
    }
    return it;
}

template <class It>
inline bool full(It first, It last, It head, It tail)
{
    return next(first, last, head) == tail;
}

template <class It>
inline bool empty(It first, It last, It head, It tail)
{
    return head == tail;
}

template <class It, class T = decltype(*It())>
inline T &front(It first, It last, It tail)
{
    return *tail;
}

template <class It, class T = decltype(*It())>
inline T &back(It first, It last, It head)
{
    return *prev(first, last, head);
}

template <class It, class T>
inline void push_back(It first, It last, It &head, It &tail, const T &v)
{
    *head = v;
    if (next(first, last, head) == tail)
    {
        next(first, last, tail);
    }
}

template <class It>
inline void pop_front(It first, It last, It &head, It &tail)
{
    next(first, last, tail);
}

template <class It, class T>
inline void push_front(It first, It last, It &head, It &tail, const T &v)
{
    if (prev(first, last, tail) == head)
    {
        prev(first, last, head);
    }
    *tail = v;
}

template <class It>
inline void pop_back(It first, It last, It &head, It &tail)
{
    prev(first, last, head);
}

} // namespace ring_buffer


#include <ring_buffer.hpp>

#include <cassert>
#include <iostream>
#include <list>
#include <vector>

using namespace ring_buffer;

template <class It>
void print_ring_buffer(It first, It last, It head, It tail)
{
    std::cout << "[";
    for (; tail != head; next(first, last, tail))
    {
        std::cout << *tail << ",";
    }
    std::cout << "]" << std::endl;
}

template <class It>
void test_ring_buffer(It first, It last)
{
    It head = first, tail = first;

    assert(empty(first, last, head, tail));
    assert(!full(first, last, head, tail));

    push_back(first, last, head, tail, 1);
    assert(back(first, last, head) == 1);

    push_back(first, last, head, tail, 2);
    assert(back(first, last, head) == 2);
    assert(front(first, last, tail) == 1);

    push_back(first, last, head, tail, 3);
    assert(back(first, last, head) == 3);
    assert(front(first, last, tail) == 1);

    push_back(first, last, head, tail, 4);
    push_back(first, last, head, tail, 5);
    assert(front(first, last, tail) == 2);

    assert(full(first, last, head, tail));
    assert(!empty(first, last, head, tail));

    pop_back(first, last, head, tail);
    assert(back(first, last, head) == 4);
    assert(front(first, last, tail) == 2);

    push_front(first, last, head, tail, 1);
    assert(front(first, last, tail) == 1);
    assert(back(first, last, head) == 4);

    push_front(first, last, head, tail, 0);
    assert(front(first, last, tail) == 0);
    assert(back(first, last, head) == 3);

    pop_front(first, last, head, tail);
    assert(front(first, last, tail) == 1);
    assert(back(first, last, head) == 3);
}

int main()
{
    int int_arr[4 + 1];
    test_ring_buffer(int_arr, int_arr + 4 + 1);
    std::cout << "int array as ring buffer passed" << std::endl;

    std::vector<int> int_vec(4 + 1);
    test_ring_buffer(int_vec.begin(), int_vec.end());
    std::cout << "int vector as ring buffer passed" << std::endl;

    std::list<int> int_list(4 + 1);
    test_ring_buffer(int_list.begin(), int_list.end());
    std::cout << "int list as ring buffer passed" << std::endl;

    return 0;
}

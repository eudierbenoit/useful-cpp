#include <list>
#include <stack>
#include <forward_list>
#include <queue>
#include <functional>
#include <iostream>
#include <vector>

struct Foo {
    Foo(int num): num_(num) {}
    void print_add(int i) const { std::cout << num_ + i << std::endl;}
    int num_;
};

void print_num(int i)
{
    std::cout << i << "\n";
}

struct printNum {
    void operator()(int i) const
    {
        std::cout << i << "\n";
    }
};

/*
 * This works for vector, list, forward_list, deque
 * queue, stack, array have different constructor so they cannot be used with
 * this template function
 * */

template <class T, template<typename, typename = std::allocator<T>> class Container>
T reduce_container(std::function<T(T,T)> operation, Container<T> to_reduce)
{
    if (to_reduce.empty()) {
        throw("Cannot reduce an empty vector");
    }

    //Then, iterate.
    T accumulator;
    using iter = typename Container<T>::iterator;
    for (iter container_iter = to_reduce.begin();
         container_iter != to_reduce.end();
         ++container_iter) {
        if (container_iter == to_reduce.begin()) {
            accumulator = *container_iter;
        } else {
            accumulator = operation(accumulator, *container_iter);
        }
    }

    return accumulator;
}
/*
 * Using c++ varadic templates.
 * Vector, list, deque.. constructor is <typename T, typename =
 * std::allocator<T>>
 * set constructor is <typename T,
 *                     class Compare = less<T>,
 *                     class Alloc = allocator<T>>
 *  so we cannot use <class T, template<typename, typename = std::allocator<T>>
 *  class Container>. Instead, we use varadic templates.
 *  See here:
 *  http://stackoverflow.com/questions/20497315/template-template-parameters-with-container-and-default-allocator-can-i-make-my/20499809#20499809
 * */
template <class T, template<typename...> class Container>
T reduce(std::function<T(T,T)> operation, Container<T> to_reduce)
{
    if (to_reduce.empty()) {
        throw("Cannot reduce an empty vector");
    }

    //Then, iterate.
    T accumulator;
    using iter = typename Container<T>::iterator;
    for (iter container_iter = to_reduce.begin();
         container_iter != to_reduce.end();
         ++container_iter) {
        if (container_iter == to_reduce.begin()) {
            accumulator = *container_iter;
        } else {
            accumulator = operation(accumulator, *container_iter);
        }
    }

    return accumulator;
}

/*
 * Specialization for queues
 * No begin nor end iteration.
 * */
template <class T>
T reduce(std::function<T(T,T)> operation, std::queue<T> to_reduce)
{
    if (to_reduce.empty()) {
        throw("Cannot reduce an empty queue");
    }

    T accumulator = to_reduce.front();
    to_reduce.pop();

    while (!to_reduce.empty()) {
        accumulator = operation(accumulator, to_reduce.front());
        to_reduce.pop();
    }

    return accumulator;
}

/*
 * Specialization for stacks
 * no begin nor end iteration.
 * */
template <class T>
T reduce(std::function<T(T,T)> operation, std::stack<T> to_reduce)
{
    if (to_reduce.empty()) {
        throw("Cannot reduce an empty queue");
    }

    T accumulator = to_reduce.top();
    to_reduce.pop();

    while (!to_reduce.empty()) {
        accumulator = operation(accumulator, to_reduce.top());
        to_reduce.pop();
    }

    return accumulator;
}

int add(int a, int b)
{
    return a + b;
}

#include <deque>
#include <set>

int main()
{
    //store a free function
    std::function<void(int)> f_display = print_num;
    f_display(-9);

    //Store a lambda
    std::function<void()> f_display_42 = []() { print_num(42);};
    f_display_42();

    //Store the result of a call to std::bind
    std::function<void()> f_display_34 = std::bind(print_num, 37);
    f_display_34();

    //Store a call to a member function
    std::function<void(const Foo&, int)> f_add_display = &Foo::print_add;
    const Foo foo(21454);
    f_add_display(foo, 5);

    // store a call to a member function and object using std::placeholders::_1;
    std::function<void(int)> f_add_display2= std::bind(&Foo::print_add, foo, std::placeholders::_1 );
    f_add_display2(2);

    // store a call to a member function and object ptr
    std::function<void(int)> f_add_display3 = std::bind( &Foo::print_add, &foo, std::placeholders::_1 );
    f_add_display3(3);

    // store a call to a function object
    std::function<void(int)> f_display_obj = printNum();
    f_display_obj(18);

    // reduce test.
    std::vector<int> test = {1, 1, 1, 1};
    //int result2 = reduce(std::function<int(int,int)>(add), test);

    std::function<int(int,int)> add_l = [](int a, int b){return a+b;};
    std::stack<int> test_queue;
    test_queue.push(3);
    test_queue.push(6);
    test_queue.push(20);
    int result = reduce(add_l, test_queue);
    int myints[]= {10,20,30,40,50};
    std::set<int> second (myints,myints+5);
    int result2 = reduce(add_l, second);
    std::cout << result2 << std::endl;


}

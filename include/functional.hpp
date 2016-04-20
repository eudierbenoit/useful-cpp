#ifndef AELIB_FUNCTIONAL_HPP
#define AELIB_FUNCTIONAL_HPP

#include <vector>
#include <map>

/*
 * Will contain some functions to use in functional style (zip, reduce....)
 * */

namespace aelib {
    namespace functional {

        /*
         * Will zip vector A with vector b in a dictionary.
         * For example:
         * zip_dict({"key1", "key2", "key3"}, {1, 2, 3}) will return
         * map<string, int> = {
         *  {"key1", 1}, {"key2", 2}, {"key3", 3}};
         * Raise an exception if input vectors are not the same size.
         * */
        template<class A, class B>
        std::map<A, B> zip_dict(std::vector<A> keys, std::vector<B> values)
        {
            if (keys.size() != values.size()) {
                throw ("Vector should be the same size"); //TODO create custom exception
            }

            std::map<A,B> zipped_dict;
            for (int i = 0; i < keys.size(); i++) {
                zip_dict.insert(std::make_pair(keys[i], values[i]));
            }

            return zipped_dict;
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
    }
}

#endif

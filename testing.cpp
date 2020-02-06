//
// Created by giacomo on 03/02/2020.
//

#include <utility>
#include <iostream>
#include <chrono>

template <typename T> struct Y {
    T function;
    template <typename ...Args> decltype(auto) operator()(Args&&... x) {
        return function(*this, std::forward<Args>(x)...);
    }
};

template <typename F> struct Y<std::decay_t<F>> make_y_combinator(F&& f) {
    return {std::forward<F>(f)};
}

typedef std::chrono::high_resolution_clock  Clock;

int main(void) {

    auto fibonacci = make_y_combinator([](auto fibonacci, unsigned long x) {
        if (x <= 2)
            return 1;
        else return fibonacci(x-1)+fibonacci(x-2);
    });


    std::cout << fibonacci(3) << std::endl;
    std::cout << fibonacci(4) << std::endl;
    std::cout << fibonacci(50) << std::endl;

}
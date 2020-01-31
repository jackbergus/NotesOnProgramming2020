//
// Created by giacomo on 06/12/2019.
//

#ifndef TUTORIALS_Y_COMBINATOR_H
#define TUTORIALS_Y_COMBINATOR_H

#include <utility>

/**
 * Lambda functions, as the ones in Alonzo Church's Lambda Calculus, do not have native recursive definitions. 
 * Nevertheless, it is possible to define fix-point operators via other lambda functions. In particular, the Y-
 * operator, takes a function f and its arguments args, and returns the evaluation of f over such arguments. In
 * order to mimick the recursion, f must accept the operator itself as one of its arguments, via which the 
 * function will be called recursively.
 *
 * @tparam F The type associated to the lambda function
 */
template <class F> struct y_combinator {
    F f; // the lambda with a generic type
    //
    //
    // Structs have implicit constructors, that is the compiler will automatically assume that there is a constructor:
    // y_combinator(F f) : f{f} {}
    // This both improves code readibility and 
    // In fact, most of the industrial code does not use public/private, but just uses everything as in public.
    // to enhance the computations. So, it makes sense not to create classes but just structs in the first place,
    // forsooth!

/*
 * Why decltype(auto)?
 * 
 * Despite C++ allows programmers to use functional programming with static type check, the language has not been
 * desitned to do inference typing with unknown parameters, such as in Haskell or OCaml. Therefore, when the type
 * are defined via templates (generic code), you want to perfectly forward a return type without knowing whether 
 * you are dealing with a reference or a value. So, a decltype(auto) allows to correctly instruct the compiler
 * to take the returning type of the Fun, that is in here implicitely defined as a (lambda) function.
 *
 * template<class Fun, class... Args> decltype(auto) Identity_operator(Fun fun, Args&&... args) { 
 *   return fun(std::forward<Args>(args)...); 
 *  }
 *
 */

    // a forwarding operator(), returning the same type as the lambda function f
    template <class... Args> decltype(auto) operator()(Args&&... args) const {
        // we pass ourselves to f, then the arguments.
        // the lambda should take the first argument as `auto&& recurse` or similar.
        return f(*this, std::forward<Args>(args)...);
    }
};

/**
 * Why decay_t?
 * 
 * Surprisingly enough, C++ cv-qualifiers (const and static) are considered as part of the type information. 
 * This compile-time directive is often used to remove such information, as well as transform lambdas to 
 * type pointer information, as well as to transform arrays or references to just U*
 * 
 * Therefore, this operator ensures that no matter what is the exact type passed as a parameter, the "expected"
 * type will be always returned by the make_combinator.
 */

/**
 * Returning the lambda function after the decay, providing the lambda typecast
 * @tparam F    The lambda type of the associated function: this lambda function must accept itself as a first auto parameter
 * @param f     the actual lambda function
 * @return      The closure via the y_combinator typecasted as a lambda function
 */
template <class F> y_combinator<std::decay_t<F>> make_y_combinator(F&& f) {
    return {std::forward<F>(f)}; // Calling the implicit constructor of the y_combinator
    // Even forward is here used in order to ease the typecasting issues and in order to always pass the correct type, no matter what
    // value is passed to the parameter (lvalue or rvalue or whatever).
}

#endif //TUTORIALS_Y_COMBINATOR_H

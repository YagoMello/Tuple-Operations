/*
 * Author:  Yago Teodoro de Mello
 * Email:   yago.t.mello@gmail.com
 * Github:  yagomello
 * Created: 2020-05-24
 * Updated: 2020-05-25
 * License: MIT
 */


#ifndef TUPLE_OPERATIONS_H
#define TUPLE_OPERATIONS_H


//INCLUDE <>
#include <tuple>

/*
 * Disclaimer:
 * Everythong will appear to be defined 3 times.
 * The first is the real implementation.
 * The second is the case when the iterator is 0.
 * The third is a wrapper to simplify the call.
 * 
 * Example:
 * tuple_operations_internal<iterator>::add(arg1, arg2)
 * tuple_operations_internal<0>::add(arg1, arg2)
 * tuple_operatons::add(arg1, arg2)
 * 
 * The last one is also used to make the overloadings:
 * arg1 + arg2
 * 
 */

// The following classes are defined int this file:
class tuple_operations;

template <typename format>
class tuple_cast;


/*
 * This class only purpose is to allow partial specialization
 * Which is not allowed in function templates or namespaces
 * 
 * All functions are very similar, they have a "simple" function to call the internal functions
 * To prevent infinite template expansion, limited only by the template depth (default 1024), we use
 * a specialization when the iterator is 0, to stop the recursion.
 * 
 * Functions:
 * --------------------------------------------------------------------------------------
 * ADD        |  Add the correspondig elements of each tuple using the + operator       |
 * SUB        |  Subtract the correspondig elements of each tuple using the - operator  |
 * MUL        |  Multiply the correspondig elements of each tuple using the * operator  |
 * DIV        |  Divide the correspondig elements of each tuple using the / operator    |
 * ELEMENT_OP |  Apply the operator to the correspondig elements of each tuple          |
 * APPLY_TO   |  Apply the operator to each tuple element                               |
 * --------------------------------------------------------------------------------------
 * 
 * 
 */
template <size_t iterator>
class tuple_operations_internal{
private:

    // Making the functions visible to the wrapper class.
    friend class tuple_operations;

    /*
     * tuple_operator_internal<n> cannot access tuple_operator_internal<n - 1> private members by default.
     * Makes sense because are two different types, but still annoying.
     * So we just friend this class with every variation of itself.
     */
    template <size_t all>
    friend class tuple_operations_internal;

    // ADD
    /*
     * The internal variation of the add function.
     * concatenates another add with the result of the operation of 2 values, and return the resulting tuple.
     */
    template <typename ... args_lhs_t, typename ... args_rhs_t>
    static auto add(std::tuple<args_lhs_t...> lhs, std::tuple<args_rhs_t...> rhs){
        /*
         * This is where the recursion happens,
         * expands as
         * {add(...), a[n] + b[n]}
         * {add(...), a[n-1] + b[n-1], a[n] + b[n]}
         * {add(...), a[n-2] + b[n-2], a[n-1] + b[n-1], a[n] + b[n]}
         * when iterator - 1 == 0, calls the specialized version
         * which stops the recursion by returning only the std::make_tuple(std_get...) part
         * without calling add
         */
        return std::tuple_cat(
            tuple_operations_internal<iterator - 1>::add(lhs, rhs), 
            std::make_tuple(std::get<iterator>(lhs) + std::get<iterator>(rhs))
        );
    }
    
    // SUB
    template <typename ... args_lhs_t, typename ... args_rhs_t>
    static auto sub(std::tuple<args_lhs_t...> lhs, std::tuple<args_rhs_t...> rhs){
        return std::tuple_cat(
            tuple_operations_internal<iterator - 1>::sub(lhs, rhs), 
            std::make_tuple(std::get<iterator>(lhs) - std::get<iterator>(rhs))
        );
    }
    
    // MUL
    template <typename ... args_lhs_t, typename ... args_rhs_t>
    static auto mul(std::tuple<args_lhs_t...> lhs, std::tuple<args_rhs_t...> rhs){
        return std::tuple_cat(
            tuple_operations_internal<iterator - 1>::mul(lhs, rhs), 
            std::make_tuple(std::get<iterator>(lhs) * std::get<iterator>(rhs))
        );
    }
    
    // DIV
    template <typename ... args_lhs_t, typename ... args_rhs_t>
    static auto div(std::tuple<args_lhs_t...> lhs, std::tuple<args_rhs_t...> rhs){
        return std::tuple_cat(
            tuple_operations_internal<iterator - 1>::div(lhs, rhs), 
            std::make_tuple(std::get<iterator>(lhs) / std::get<iterator>(rhs))
        );
    }
    
    // ELEMENT_OP
    template <typename ... args_lhs_t, typename ... args_rhs_t, typename duck_typing_class>
    static auto element_op(duck_typing_class& duck, std::tuple<args_lhs_t...> lhs, std::tuple<args_rhs_t...> rhs){
        return std::tuple_cat(
            tuple_operations_internal<iterator - 1>::element_op(duck, lhs, rhs), 
            std::make_tuple(duck.operation(std::get<iterator>(lhs), std::get<iterator>(rhs)))
        );
    }

    // APPLY_TO
    template <typename ... args_t, typename duck_typing_class>
    static auto apply_to(duck_typing_class& duck, std::tuple<args_t...> args){
        return std::tuple_cat(
            tuple_operations_internal<iterator - 1>::apply_to(duck, args), 
            std::make_tuple(duck.operation(std::get<iterator>(args)))
        );
    }
    
};

/*
 * Special case when the iterator is 0.
 * Stops the recursion.
 */
template <>
class tuple_operations_internal<0>{
private:

    // Making the functions visible to the wrapper class.
    friend class tuple_operations;

    /*
     * tuple_operator_internal<1> cannot access tuple_operator_internal<0> private members by default.
     * So we just friend this class with every variation of itself.
     */
    template <size_t all>
    friend class tuple_operations_internal;

    // ADD
    template <typename ... args_lhs_t, typename ... args_rhs_t>
    static auto add(std::tuple<args_lhs_t...> lhs, std::tuple<args_rhs_t...> rhs){
        return std::make_tuple(std::get<0>(lhs) + std::get<0>(rhs));
    }
    
    // SUB
    template <typename ... args_lhs_t, typename ... args_rhs_t>
    static auto sub(std::tuple<args_lhs_t...> lhs, std::tuple<args_rhs_t...> rhs){
        return std::make_tuple(std::get<0>(lhs) - std::get<0>(rhs));
    }

    // MUL
    template <typename ... args_lhs_t, typename ... args_rhs_t>
    static auto mul(std::tuple<args_lhs_t...> lhs, std::tuple<args_rhs_t...> rhs){
        return std::make_tuple(std::get<0>(lhs) * std::get<0>(rhs));
    }
    
    // DIV
    template <typename ... args_lhs_t, typename ... args_rhs_t>
    static auto div(std::tuple<args_lhs_t...> lhs, std::tuple<args_rhs_t...> rhs){
        return std::make_tuple(std::get<0>(lhs) / std::get<0>(rhs));
    }
    
    // ELEMENT_OP
    template <typename ... args_lhs_t, typename ... args_rhs_t, typename duck_typing_class>
    static auto element_op(duck_typing_class& duck, std::tuple<args_lhs_t...> lhs, std::tuple<args_rhs_t...> rhs){
        return std::make_tuple(duck.operation(std::get<0>(lhs), std::get<0>(rhs)));
    }

    // APPLY_TO
    template <typename ... args_t, typename duck_typing_class>
    static auto apply_to(duck_typing_class& duck, std::tuple<args_t...> args){
        return std::make_tuple(duck.operation(std::get<0>(args)));
    }
    
};


class tuple_operations{
public:
    // ADD
    /*
     * This function returns a tuple.
     * When using 2 variadic templates is only possible to use automatic template deduction,
     * the types are deducted from the tuple.
     */
    template <typename ... args_lhs_t, typename ... args_rhs_t>
    static auto add(std::tuple<args_lhs_t...> lhs, std::tuple<args_rhs_t...> rhs){
        /*
         * We call a internal function to operate on the last element of the tuple,
         * which is "sizeof...(args_lhs_t) - 1".
         * This value is specified on the class because of the partial specialization problem.
         */
        return tuple_operations_internal<sizeof...(args_lhs_t) - 1>::add(lhs, rhs);
    }
    
    // SUB
    template <typename ... args_lhs_t, typename ... args_rhs_t>
    static auto sub(std::tuple<args_lhs_t...> lhs, std::tuple<args_rhs_t...> rhs){
        return tuple_operations_internal<sizeof...(args_lhs_t) - 1>::sub(lhs, rhs);
    }

    // MUL
    template <typename ... args_lhs_t, typename ... args_rhs_t>
    static auto mul(std::tuple<args_lhs_t...> lhs, std::tuple<args_rhs_t...> rhs){
        return tuple_operations_internal<sizeof...(args_lhs_t) - 1>::mul(lhs, rhs);
    }

    // DIV
    template <typename ... args_lhs_t, typename ... args_rhs_t>
    static auto div(std::tuple<args_lhs_t...> lhs, std::tuple<args_rhs_t...> rhs){
        return tuple_operations_internal<sizeof...(args_lhs_t) - 1>::div(lhs, rhs);
    }

    // ELEMENT_OP
    /*
     * In this function we use duck typing. Polymorphism may be a better solution, but needs a vtable access.
     * A 10 million value tuple would need 10 million vtable accesses. Imagine the ammount of cache misses. Oof.
     * I hacked this thing in 5 minutes just to have the option of using custom functions.
     * May change in the future. Or not.
     */
    template <typename ... args_lhs_t, typename ... args_rhs_t, typename duck_typing_class>
    static auto element_op(duck_typing_class& duck ,std::tuple<args_lhs_t...> lhs, std::tuple<args_rhs_t...> rhs){
        return tuple_operations_internal<sizeof...(args_lhs_t) - 1>::element_op(duck, lhs, rhs);
    }

    // APPLY_TO
    /*
     * element_op but adapted to operate on a single tuple.
     */
    template <typename ... args_t, typename duck_typing_class>
    static auto apply_to(duck_typing_class& duck ,std::tuple<args_t...> args){
        return tuple_operations_internal<sizeof...(args_t) - 1>::apply_to(duck, args);
    }

};





/*
 * Adaptation of apply_to using static_cast.
 * 
 * Functions:
 * --------------------------------------------------------------------------
 * ALL        |  Cast every element of the tuple to the specified format    |
 * --------------------------------------------------------------------------
 * 
 */
template <typename format, size_t iterator>
class tuple_cast_internal{
private:

    // Making the functions visible to the wrapper class.
    template <typename all>
    friend class tuple_cast;

    // Making members accessible to variations of this class.
    template <typename all_types, size_t all_values>
    friend class tuple_cast_internal;

    // (STATIC) CAST
    /*
     * static cast go brrrrrrrrrrrr
     */
    template <typename ... args_t>
    static auto all(std::tuple<args_t...> args){
        return std::tuple_cat(
            tuple_cast_internal<format, iterator - 1>::all(args), 
            std::make_tuple(static_cast<format>(std::get<iterator>(args)))
        );
    }
    
};

template <typename format>
class tuple_cast_internal<format, 0>{
private:

    // Making the functions visible to the wrapper class.
    template <typename all>
    friend class tuple_cast;

    // Making members accessible to variations of this class.
    template <typename all_types, size_t all_values>
    friend class tuple_cast_internal;

    // (STATIC) CAST
    template <typename ... args_t>
    static auto all(std::tuple<args_t...> args){
        return std::make_tuple(static_cast<format>(std::get<0>(args)));
    }
};

template <typename format>
class tuple_cast{
public:
    template <typename ... args_t>
    static auto all(std::tuple<args_t...> args){
        return tuple_cast_internal<format, sizeof...(args_t) - 1>::all(args);
    }
};


// OPERATOR OVERLOADING 
/* 
 * Just some basic overloading, simplifies the code a lot
 * define NO_TUPLE_OPERATOR_OVERLOADING to prevent the overloads
 */

#ifndef NO_TUPLE_OPERATOR_OVERLOADING

template <typename ... args_lhs_t, typename ... args_rhs_t>
auto operator +(std::tuple<args_lhs_t...> lhs, std::tuple<args_rhs_t...> rhs){
    return tuple_operations::add(lhs, rhs);
}

template <typename ... args_lhs_t, typename ... args_rhs_t>
auto operator -(std::tuple<args_lhs_t...> lhs, std::tuple<args_rhs_t...> rhs){
    return tuple_operations::sub(lhs, rhs);
}

template <typename ... args_lhs_t, typename ... args_rhs_t>
auto operator *(std::tuple<args_lhs_t...> lhs, std::tuple<args_rhs_t...> rhs){
    return tuple_operations::mul(lhs, rhs);
}

template <typename ... args_lhs_t, typename ... args_rhs_t>
auto operator /(std::tuple<args_lhs_t...> lhs, std::tuple<args_rhs_t...> rhs){
    return tuple_operations::div(lhs, rhs);
}

#endif //NO_TUPLE_OPERATOR_OVERLOADING

#endif //TUPLE_OPERATIONS_H

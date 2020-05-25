/*
 * Author:  Yago Teodoro de Mello
 * Email:   yago.t.mello@gmail.com
 * Github:  yagomello
 * Created: 2020-05-24
 * Updated: 2020-05-24
 * License: MIT
 */


#ifndef TUPLE_OPERATIONS_H
#define TUPLE_OPERATIONS_H


//INCLUDE <>
#include <tuple>

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
        return tuple_operations<sizeof...(args_lhs_t) - 1>::add_internal(lhs, rhs);
    }
    /*
     * The internal variation of the add function.
     * concatenates another add_internal with the result of the operation of 2 values, and return the resulting tuple.
     */
    template <typename ... args_lhs_t, typename ... args_rhs_t>
    static auto add_internal(std::tuple<args_lhs_t...> lhs, std::tuple<args_rhs_t...> rhs){
        /*
         * This is where the recursion happens,
         * expands as
         * {add_internal(...), a[n] + b[n]}
         * {add_internal(...), a[n-1] + b[n-1], a[n] + b[n]}
         * {add_internal(...), a[n-2] + b[n-2], a[n-1] + b[n-1], a[n] + b[n]}
         * when iterator - 1 == 0, calls the specialized version
         * which stops the recursion by returning only the std::make_tuple(std_get...) part
         * without calling add_internal
         */
        return std::tuple_cat(
            tuple_operations<iterator - 1>::add_internal(lhs, rhs), 
            std::make_tuple(std::get<iterator>(lhs) + std::get<iterator>(rhs))
        );
    }
    
    // SUB
    template <typename ... args_lhs_t, typename ... args_rhs_t>
    static auto sub(std::tuple<args_lhs_t...> lhs, std::tuple<args_rhs_t...> rhs){
        return tuple_operations<sizeof...(args_lhs_t) - 1>::sub_internal(lhs, rhs);
    }
    template <typename ... args_lhs_t, typename ... args_rhs_t>
    static auto sub_internal(std::tuple<args_lhs_t...> lhs, std::tuple<args_rhs_t...> rhs){
        return std::tuple_cat(
            tuple_operations<iterator - 1>::sub_internal(lhs, rhs), 
            std::make_tuple(std::get<iterator>(lhs) - std::get<iterator>(rhs))
        );
    }
    
    // MUL
    template <typename ... args_lhs_t, typename ... args_rhs_t>
    static auto mul(std::tuple<args_lhs_t...> lhs, std::tuple<args_rhs_t...> rhs){
        return tuple_operations<sizeof...(args_lhs_t) - 1>::mul_internal(lhs, rhs);
    }
    template <typename ... args_lhs_t, typename ... args_rhs_t>
    static auto mul_internal(std::tuple<args_lhs_t...> lhs, std::tuple<args_rhs_t...> rhs){
        return std::tuple_cat(
            tuple_operations<iterator - 1>::mul_internal(lhs, rhs), 
            std::make_tuple(std::get<iterator>(lhs) * std::get<iterator>(rhs))
        );
    }
    
    // DIV
    template <typename ... args_lhs_t, typename ... args_rhs_t>
    static auto div(std::tuple<args_lhs_t...> lhs, std::tuple<args_rhs_t...> rhs){
        return tuple_operations<sizeof...(args_lhs_t) - 1>::div_internal(lhs, rhs);
    }
    template <typename ... args_lhs_t, typename ... args_rhs_t>
    static auto div_internal(std::tuple<args_lhs_t...> lhs, std::tuple<args_rhs_t...> rhs){
        return std::tuple_cat(
            tuple_operations<iterator - 1>::div_internal(lhs, rhs), 
            std::make_tuple(std::get<iterator>(lhs) / std::get<iterator>(rhs))
        );
    }
    
    // ELEMENT_OP
    /*
     * In this function we use duck typing. Polymorphism may be a better solution, but needs a vtable access.
     * A 10 million value tuple would need 10 million vtable accesses. Oof.
     * I hacked this thing in 5 minutes just to have the option of using custom functions.
     * May change in the future. Or not.
     */
    template <typename ... args_lhs_t, typename ... args_rhs_t, typename duck_typing_class>
    static auto element_op(duck_typing_class& duck ,std::tuple<args_lhs_t...> lhs, std::tuple<args_rhs_t...> rhs){
        return tuple_operations<sizeof...(args_lhs_t) - 1>::element_op_internal(duck, lhs, rhs);
    }
    template <typename ... args_lhs_t, typename ... args_rhs_t, typename duck_typing_class>
    static auto element_op_internal(duck_typing_class& duck, std::tuple<args_lhs_t...> lhs, std::tuple<args_rhs_t...> rhs){
        return std::tuple_cat(
            tuple_operations<iterator - 1>::element_op_internal(duck, lhs, rhs), 
            std::make_tuple(duck.operation(std::get<iterator>(lhs), std::get<iterator>(rhs)))
        );
    }

    // APPLY_TO
    /*
     * element_op but adapted to operate on a single tuple.
     */
    template <typename ... args_t, typename duck_typing_class>
    static auto apply_to(duck_typing_class& duck ,std::tuple<args_t...> args){
        return tuple_operations<sizeof...(args_t) - 1>::apply_to_internal(duck, args);
    }
    template <typename ... args_t, typename duck_typing_class>
    static auto apply_to_internal(duck_typing_class& duck, std::tuple<args_t...> args){
        return std::tuple_cat(
            tuple_operations<iterator - 1>::apply_to_internal(duck, args), 
            std::make_tuple(duck.operation(std::get<iterator>(args)))
        );
    }
    
};

/*
 * Special case when the iterator is 0.
 * Stops the recursion.
 */
template <>
class tuple_operations<0>{
public:

    // ADD
    template <typename ... args_lhs_t, typename ... args_rhs_t>
    static auto add_internal(std::tuple<args_lhs_t...> lhs, std::tuple<args_rhs_t...> rhs){
        return std::make_tuple(std::get<0>(lhs) + std::get<0>(rhs));
    }
    
    // SUB
    template <typename ... args_lhs_t, typename ... args_rhs_t>
    static auto sub_internal(std::tuple<args_lhs_t...> lhs, std::tuple<args_rhs_t...> rhs){
        return std::make_tuple(std::get<0>(lhs) - std::get<0>(rhs));
    }

    // MUL
    template <typename ... args_lhs_t, typename ... args_rhs_t>
    static auto mul_internal(std::tuple<args_lhs_t...> lhs, std::tuple<args_rhs_t...> rhs){
        return std::make_tuple(std::get<0>(lhs) * std::get<0>(rhs));
    }
    
    // DIV
    template <typename ... args_lhs_t, typename ... args_rhs_t>
    static auto div_internal(std::tuple<args_lhs_t...> lhs, std::tuple<args_rhs_t...> rhs){
        return std::make_tuple(std::get<0>(lhs) / std::get<0>(rhs));
    }
    
    // ELEMENT_OP
    template <typename ... args_lhs_t, typename ... args_rhs_t, typename duck_typing_class>
    static auto element_op_internal(duck_typing_class& duck, std::tuple<args_lhs_t...> lhs, std::tuple<args_rhs_t...> rhs){
        return std::make_tuple(duck.operation(std::get<0>(lhs), std::get<0>(rhs)));
    }

    // APPLY_TO
    template <typename ... args_t, typename duck_typing_class>
    static auto apply_to_internal(duck_typing_class& duck, std::tuple<args_t...> args){
        return std::make_tuple(duck.operation(std::get<0>(args)));
    }
    
};

#endif //TUPLE_OPERATIONS_H

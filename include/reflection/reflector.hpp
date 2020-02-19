#ifndef REFLECTOR_HPP
#define REFLECTOR_HPP

#define NAME(x) #x
#define NAME_STR(x) "#x"

template <typename T>
struct member_trait {
    // get function ptr
    static auto get_func_ptr() {
        // return function ptr
        return nullptr;
    }
    
    static const char *name() {
        // return member name
        return nullptr;
    }
};

template <typename T>
struct reflect {

    static const char *name() { return typeid(T).name(); }

    // ReturnType (T::*)(T *const, Args)
    template <typename ReturnType, typename ...Args>
    static ReturnType call(ReturnType (T::*func)(Args...), T &obj, Args... args) {
        return (obj.*func)(args...);
    }

    // MemberType T::*
    template <typename M>
    static M &get_attr(M T::*attr, T &obj) {
        return obj.*attr;
    }
    
};


template <typename T>
struct object_trait {
    
};

#endif // REFLECT_HPP
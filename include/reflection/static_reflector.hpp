#ifndef REFLECTOR_HPP
#define REFLECTOR_HPP

#define NAME(x) #x
#define NAME_STR(x) "#x"

template <typename T>
struct static_reflect {

    static const char *name() { return typeid(T).name(); }

    // ReturnType (T::*)(T *const, Args)
    template <typename ReturnType, typename ...Args>
    static ReturnType invoke(ReturnType (T::*func)(Args...), T &obj, Args... args) {
        return (obj.*func)(args...);
    }

    // MemberType T::*
    template <typename M>
    static M &attr(M T::*attr, T &obj) {
        return obj.*attr;
    }
    
};

#endif // REFLECT_HPP
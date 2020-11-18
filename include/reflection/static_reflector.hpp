#ifndef REFLECTOR_HPP
#define REFLECTOR_HPP

#define NAME(x) #x
#define NAME_STR(x) "#x"

namespace anarion {
template <typename T>
class StaticReflector {
public:

    static const char *name() { return typeid(T).name(); }

    // ReturnType (T::*)(T *const, Args)
    template <typename ReturnType, typename ...Args>
    static ReturnType invoke(ReturnType (T::*func)(Args...), T &obj, Args... args) {
        return (obj.*func)(args...);
    }

    template <typename ...Args>
    static void invoke(void (T::*func)(Args...), T &obj, Args ...args) {
        (obj.*func)(args...);
    }

    // MemberType T::*
    template <typename M>
    static M &attr(M T::*attr, T &obj) {
        return obj.*attr;
    }

    template <typename M>
    static int memberOffset(M T::*member) {
        char space[sizeof(T)];
        T &objRef = *reinterpret_cast<T*>(space);
        return reinterpret_cast<char *>(&(objRef.*member)) - space;
    }

    template <typename M>
    static M *getAttrPointer(T &object, int offset) {
        return reinterpret_cast<M *>(reinterpret_cast<char *>(&object) + offset);
    }
};
}
#endif // REFLECT_HPP
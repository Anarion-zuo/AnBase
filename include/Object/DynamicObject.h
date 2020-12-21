////
//// Created by anarion on 2020/3/5.
////
//
//#ifndef MYCPPBASE_DYNAMICOBJECT_H
//#define MYCPPBASE_DYNAMICOBJECT_H
//
//#include <container/Map/HashMap.hpp>
//#include <container/SString.h>
//#include "RootObject.h"
//
////#define def_attr(name, )
//
//namespace anarion {
//
//    class ClassMember {
//    protected:
//        void *pointer = nullptr;
//        SString name;
//    public:
//        ClassMember() = default;
//        ClassMember(const ClassMember &) = default;
//        ClassMember(ClassMember &&rhs) noexcept : pointer(rhs.pointer), name(move(rhs.name)) {
//            rhs.pointer = nullptr;
//        }
//        virtual ~ClassMember() { delete pointer; }
//        template <typename T> constexpr T *get() { return static_cast<T*>(pointer); }
//        template <typename T> constexpr void set(T *p) { pointer = p; }
//
//    };
//
//    /*
//     * Whenever constructing a dynamic object, the attributes of the object must be initialized as an instance of class Attribute.
//     *
//     */
//    class Attribute : virtual public ClassMember {
//        friend class DynamicObject;
//    private:
//    public:
//        Attribute() = default;
//        Attribute(const Attribute &) = default;
//        Attribute(Attribute &&rhs) noexcept : ClassMember(forward<Attribute>(rhs)) {}
//        template <typename ClassType, typename AttrType>
//        AttrType &getReference(ClassType &obj) { return obj.*get<AttrType>(); }
//    };
//
//    /*
//     * All methods of a dynamic object must be registered here
//     */
//    class Method : virtual public ClassMember {
//        friend class DynamicObject;
//    private:
//    public:
//        Method() = default;
//        Method(const Method &) = default;
//        Method(Method &&rhs) noexcept : ClassMember(forward<Method>(rhs)) {}
//        template <typename ClassType, typename ReturnType, typename ...Args>
//        ReturnType invoke(ClassType &obj, Args... args) {
//            typedef ReturnType (ClassType::*func_type)(Args...);
//            return (obj.*((func_type)pointer))(args...);
//        }
//    };
//
//    /*
//     * The Dynamic Object class is a tool class, consisting of a set of static methods.
//     * For dynamic reflection to work, the derived classed must fulfill the mappings between names in strings and attributes or methods.
//     * One may implement this via a init static method.
//     */
//
//    class DynamicObject : virtual public RootObject {
//    private:
//
//        static HashSet<Attribute> attrs;
//        static HashSet<Method> methods;
//
//    protected:
//
//        static HashSet<Attribute> &getAttrs() { return attrs; }
//        static HashSet<Method> &getMethods() { return methods; }
//
//    public:
//
//        DynamicObject() = default;
//        DynamicObject(const DynamicObject &) = default;
//        DynamicObject(DynamicObject &&) = default;
//        DynamicObject &operator=(const DynamicObject &) = default;
//        DynamicObject &operator=(DynamicObject &&) = default;
//        ~DynamicObject() = default;
//
//        template <typename ClassType, typename T>
//        static void registerAttr(SString &&name, T ClassType::*p) {
//            Attribute attr;
//            attr.pointer = p;
//            attr.name = forward<SString>(name);
//            attrs.insert(move(attr));
//        }
//
//        template <typename ClassType, typename ReturnType, typename ...Args>
//        static void registerMethod(SString &&name, ReturnType (ClassType::*func)(Args...)) {
//            Method method;
//            method.name = forward<SString>(name);
//            method.pointer = func;
//        }
//
//        static Attribute &getAttr(SString &&key);
//        static Method &getMethod(SString &&key);
//    };
//
//    struct MemberNotFound : public std::exception {
//        const char *what() const noexcept override;
//    };
//}
//
//#endif //MYCPPBASE_DYNAMICOBJECT_H

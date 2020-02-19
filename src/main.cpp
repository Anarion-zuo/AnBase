#include <iostream>
#include <cstring>
#include "allocator/PoolAllocator.h"
#include "container/SString.h"
#include "reflection/reflector.hpp"

using namespace std;
using namespace anarion;

class A {
    friend struct reflect<A>;
    friend int main();
private:
    int x;

public:

    A(int x) : x(x) { cout << "A(): " << x << " " << this << endl; }
    A(const A &) = default;
    A(A &&) = default;
    ~A() { cout << "~A(): " << this << endl; }

    void hello(int y) {
        cout << "hello function call... " << x << ", " << y << endl;
        return;
    }
};

template <typename T>
void get_type(A* _this, T o) {
    // o(_this);
    // (_this->*o)(777);
    cout << typeid(T).name() << endl;
    cout << "x:  " << (_this->*o) <<endl;
    return;
}

int main() {
    A a(666);
    // reflect<A>::call(&A::hello, a, 777);
    reflect<A>::get_attr(&A::x, a) = 333;
    return 0;
}

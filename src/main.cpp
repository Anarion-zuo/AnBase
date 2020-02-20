#include <iostream>
#include <cstring>
#include <functional>
#include "allocator/PoolAllocator.h"
#include "container/SString.h"
#include "reflection/static_reflector.hpp"
#include "container/Bind/binded.hpp"

using namespace std;
using namespace anarion;

class A {
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
void get_type(T o) {
    // o(_this);
    // (_this->*o)(777);
    cout << o << endl;
    return;
}

int main() {
    A a(666);
    auto f = anarion::bind(&get_type<int>, 777);
    f();
    return 0;
}

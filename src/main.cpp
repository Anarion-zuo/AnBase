#include <iostream>
#include <cstring>
#include "allocator/PoolAllocator.h"
#include "container/SString.h"

using namespace std;
using namespace anarion;

class A {
private:
    int x;

public:

    A(int x) : x(x) { cout << "A(): " << x << " " << this << endl; }
    A(const A &) = default;
    A(A &&) = default;
    ~A() { cout << "~A(): " << this << endl; }
};

int main() {
    Vector<int> vec;
    vec.push_back(1);
    vec.push_back(2);
    vec.push_back(3);
    vec.push_back(4);
    vec.push_back(5);
    vec.print();
    vec.insert(vec.end_iterator(), 666);
    vec.print();
    return 0;
}

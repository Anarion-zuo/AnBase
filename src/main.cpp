#include <iostream>
#include <cstring>
#include "allocator/PoolAllocator.h"
#include "container/SString.h"
#include "io/channel/file/FileChannel.h"

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
    FileChannel channel = FileChannel::open(SString("ha"));
    return 0;
}

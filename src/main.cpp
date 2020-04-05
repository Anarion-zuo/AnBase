#include <iostream>
#include <cstring>
#include <functional>
#include <concurrent/pool/ThreadPool.h>
#include <io/channel/file/FileChannel.h>
#include <concurrent/concurrent.h>
#include <io/channel/network/TcpServerSocketChannel.h>
#include "container/SString.h"
#include "reflection/static_reflector.hpp"
#include "container/Bind/binded.hpp"
#include "concurrent/base/Barrier.h"
#include "io/buffer/Buffer.h"
#include <container/Pointer.h>
#include <allocator/ListConcurrentAllocator.h>
#include <container/Map/HashMap.hpp>
#include <parser/MapParser.h>

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

void printx(int x) {
    cout << "thread: " << x << endl;
}

int main() {
    MapParser parser('=', ';');
    parser.parse(SString("max=100;timeout=99"));
    return 0;
}

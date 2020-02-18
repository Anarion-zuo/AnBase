#include <iostream>
#include <cstring>
#include "allocator/PoolAllocator.h"
#include "container/SString.h"

using namespace std;
using namespace anarion;

int main() {
    Vector<int> vec;
    vec.push_back(1);
    vec.push_back(2);
    vec.push_back(3);
    vec.push_back(4);
    vec.push_back(5);
    vec.insert(vec.begin_iterator() + 1, 666);
    return 0;
}

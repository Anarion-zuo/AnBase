#include <iostream>
#include <cstring>
#include <functional>
#include <concurrent/pool/ThreadPool.h>
#include <io/channel/network/TcpServerSocketChannel.h>
#include "container/SString.h"
#include "reflection/static_reflector.hpp"
#include "container/Bind/binded.hpp"
#include "concurrent/base/Barrier.h"
#include "io/buffer/Buffer.h"
#include <allocator/ListConcurrentAllocator.h>
#include <container/Map/HashMap.hpp>
#include <parser/MapParser.h>
#include <parser/ListParser.h>
#include <io/channel/file/Directory.h>
#include <parser/json/JsonMap.h>
#include <concurrent/pool/AsyncCaller.h>
#include <parser/xml/XmlElement.h>
#include <time/Time.h>
#include <logger/Logger.h>
#include <pointers/UniquePointer.h>
#include <pointers/SharedPointer.hpp>
#include <container/Trie.h>
#include <io/channel/terminal/TerminalPrintChannel.h>
#include <time/Date.h>
#include <container/Set/ExtHashSet.hpp>
#include <container/base/BplusTree.hpp>
#include <container/base/BTree.hpp>
#include <container/List/PagedVector.hpp>
#include <io/channel/file/MemoryMappedFile.h>

using namespace std;
using namespace anarion;

int main() {
    MemoryMappedFile mappedFile(SString("/Users/anarion/Downloads/test.txt"));
    void *p = mappedFile.getPointer();
    mappedFile.getChannel().in("abcdefgh", 8);
    memcpy(p, "123456", 6);

    return 0;
}

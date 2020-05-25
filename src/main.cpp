#include <iostream>
#include <cstring>
#include <functional>
#include <concurrent/pool/ThreadPool.h>
#include <io/channel/file/FileChannel.h>
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

using namespace std;
using namespace anarion;

struct AA {
    void *pthis;
    AA() : pthis(this) {}
};

class MyInfo : public LoggerInfo {
public:
    void toChannel(InChannel &inChannel) override {

    }
};

int main() {
//    MapParser parser('=', ';');
//    while (true) {
//        HashMap<SString, SString> map = parser.parse(SString("timeout=99"));
//        auto it = map.find(SString("timeout"));
//    }
//    TcpServerSocketChannel server(9898);
//    server.listen(128);
//    while (true) {
//        TcpSocketChannel client{server.accept()};
//        Buffer buffer = client.out();
//        buffer.print();
//        char *p = "HTTP/1.1 200 OK\r\n\r\nhahaha";
//        Buffer outbuf;
//        outbuf.append_arr(p, strlen(p));
//        client.in(outbuf);
//        client.close();
//    }
    BTree<int> tree;

    tree.insert(1);
    tree.insert(2);
    tree.insert(3);
    tree.insert(4);
    tree.insert(5);
    tree.insert(6);
    tree.insert(7);
    tree.insert(8);
    tree.insert(9);
    tree.insert(10);
    tree.insert(11);
    tree.insert(12);
    tree.insert(12);
    tree.insert(12);
    tree.insert(12);
    tree.insert(12);
    tree.insert(12);
    tree.insert(12);
    tree.insert(12);
    tree.insert(12);
    tree.insert(12);
    tree.insert(12);
    tree.insert(12);
    tree.insert(12);
    tree.insert(12);

    bool flag;
    flag = tree.hasMember(1);
    flag = tree.hasMember(3);
    flag = tree.hasMember(5);
    flag = tree.hasMember(4);
    flag = tree.hasMember(6);
    flag = tree.hasMember(10);
    flag = tree.hasMember(11);
    flag = tree.hasMember(12);
    flag = tree.hasMember(8);

    return 0;
}

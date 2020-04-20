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
#include <parser/ListParser.h>
#include <io/channel/file/Directory.h>

using namespace std;
using namespace anarion;


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

    Directory directory(SString("/home/anarion/Documents/CppProjects/MyMVC/static/"));
    directory.open();
    FileChannel * file = dynamic_cast<FileChannel *>(directory.getChild(SString("html"))->getChild(
            SString("hello.html")));
    file->rewind();
    Buffer buffer = file->out();
    buffer.print();
    return 0;
}

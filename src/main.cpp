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

    Time time;
    time.setCurrent();
    Date date(time);
    SString str = date.print();
    return 0;
}

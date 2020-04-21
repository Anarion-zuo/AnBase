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
#include <parser/json/JsonMap.h>
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
    const char *json_str =
            "{\"array\": [{"
            "\"first_name\": \"anarion\", "
            "\"family\": \"zuo\", "
            "\"isMale\": true, \"age\": 99"
            "}]}";
    JsonObject *json = JsonObject::parse(json_str, strlen(json_str));
    SString str = json->toString();
    return 0;
}

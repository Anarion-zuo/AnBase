#include "io/channel/file/FileChannel.h"

using namespace anarion;

FileChannel FileChannel::open(const SString &dir) {
    int fd = ::open(dir.cstr(), );
}
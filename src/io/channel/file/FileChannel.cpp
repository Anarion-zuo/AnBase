#include "io/channel/file/FileChannel.h"
#include <sys/file.h>

using namespace anarion;

FileChannel FileChannel::open(const SString &dir) {
    int fd = ::open(dir.cstr(), O_RDWR);
}
//
// Created by anarion on 8/20/20.
//

#ifndef ANBASE_FILESYSTEMNOTIFIER_H
#define ANBASE_FILESYSTEMNOTIFIER_H

#include <SystemException.h>
#include <container/List/Vector.hpp>
#include <sys/epoll.h>
#include <concurrent/base/Thread.h>
#include "FileChannel.h"

namespace anarion {
class FileSystemNotifier {
protected:
    int ifd, epfd;

    void addEpoll(int ops, int fd, struct epoll_event &event);
    void addInotifier(const char *path, int ops);
public:
    FileSystemNotifier();
    ~FileSystemNotifier();

    void addFileChannel(const FileChannel &fileChannel);

    [[noreturn]] void run() ;
};
struct NotifierException : public SystemException {};
struct NotifierInitFailed : public NotifierException {};
struct NotifierPollFailed : public NotifierException {};
struct NotifierAppendListenFailed : public NotifierException {};
}

#endif //ANBASE_FILESYSTEMNOTIFIER_H

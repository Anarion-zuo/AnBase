//
// Created by anarion on 8/20/20.
//

#include "io/channel/file/FileSystemNotifier.h"
#include <unistd.h>

#ifdef __linux__
#include <sys/inotify.h>

anarion::FileSystemNotifier::FileSystemNotifier() {
    // init inotify
    ifd = inotify_init();
    if (ifd == -1) {
        throw NotifierInitFailed();
    }
    // init epoll
    epfd = epoll_create(1);
    if (epfd < 0) {
        throw NotifierInitFailed();
    }
    struct epoll_event event;
    event.events = EPOLLIN;
    event.data.fd = ifd;
    addEpoll(0, ifd, event);
}

[[noreturn]] void anarion::FileSystemNotifier::run() {
    const size_type bufferSize = 81;
    char buf[bufferSize];
    while (true) {
        struct epoll_event event;
        int num = epoll_wait(epfd, &event, 1, -1);
        if (num > 0) {
            // read off the message
            int msgLen = read(ifd, buf, bufferSize);
            printf("%.*s\n", msgLen, buf);
        }
    }
}

void anarion::FileSystemNotifier::addEpoll(int ops, int fd, epoll_event &event) {
    epoll_ctl(epfd, ops | EPOLL_CTL_ADD, fd, &event);
}

void anarion::FileSystemNotifier::addInotifier(const char *path, int ops) {
    int wd = inotify_add_watch(ifd, path, ops);
    if (wd < 0) {
        throw NotifierAppendListenFailed();
    }
}

anarion::FileSystemNotifier::~FileSystemNotifier() {

}

void anarion::FileSystemNotifier::addFileChannel(const anarion::FileChannel &fileChannel) {
    addInotifier(fileChannel.getAbsolutePath().cstr(), IN_ACCESS);
}
#endif

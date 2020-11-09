//
// Created by 左钰 on 2020/1/15.
//

#include "exceptions/Context/ContextCreateException.h"
#include <unistd.h>
#include <context/LocalContext.h>
#ifdef __APPLE__
#include <sys/wait.h>
#elif __linux__
#include <wait.h>
#endif

using namespace anarion;

//hash_map<int, LocalContext> *LocalContext::pid2context = nullptr;

void LocalContext::wait() {
    // nothing happens if it is under this process
    if (pid != ::getpid()) {   // context under another process
        int status;
        ::waitpid(pid, &status, 0);
    }
}

void LocalContext::fork() {
    pid_t newid = ::fork();
    if (newid == -1) {
        throw ContextCreateException();
    }
    if (newid == 0) {
        // child process
        pid = ::getpid();   // update process id
        /*
         * process id changed for both the parent and the child
         * the parent may fetch the process id of the child from its object
         */
        // run...
        execute();
    }
}

void LocalContext::stop() {
    if (pid != ::getpid()) {
        ::exit(0);   // exit process normally
    }
    // kill the target process
}

int LocalContext::getPid() const {
    return pid;
}

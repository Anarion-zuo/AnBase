//
// Created by anarion on 4/11/20.
//

#ifndef MYCPPBASE_LOGGER_H
#define MYCPPBASE_LOGGER_H

#include <io/channel/InChannel.h>
#include <container/SString.h>
#include <concurrent/container/Queue/BlockQueue.hpp>
#include <time/Time.h>
#include <concurrent/base/Thread.h>

/*
 *
 */

namespace anarion {
    class LoggerInfo {
    public:
        virtual void toChannel(InChannel &inChannel) = 0;

        virtual ~LoggerInfo() = default;
    };

    class Logger : public Thread {
    protected:
        LinkedList<InChannel*> outputs;
        Mutex infoLock;
        ListQueue<LoggerInfo*> infoQueue;
        Time refreshTime{100};

        void processOne(LoggerInfo *info);
        void roll();

    public:

        Logger() { start(); }

        virtual ~Logger() {
            for (auto it = outputs.begin_iterator(); it != outputs.end_iterator(); ++it) {
                delete *it;
            }
        }

        /*
         * Must setup channels in constructor of the derived class
         */

        void setRefreshTime(size_type msec);
        void addInfo(LoggerInfo *info) {
            infoLock.lock();
            infoQueue.push(info);
            infoLock.unlock();
        }

        void run() override ;
    };

}

#endif //MYCPPBASE_LOGGER_H

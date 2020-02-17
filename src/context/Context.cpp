//
// Created by 左钰 on 2020/1/15.
//

#include "Context.h"
#include "exceptions/Context/ContextCreateException.h"
#include <unistd.h>

using namespace anarion;

Context::Context() {

}

void Context::execute() {
    try {
        preRun();
        run();
    } catch(std::exception &e) {
        onException(e);
    }
    try {
        postRun();
    } catch(std::exception &e) {
        onException(e);
    }
}

void Context::onException(std::exception &e) {

}

void Context::preRun() {

}

void Context::postRun() {

}

void Context::wait() {

}

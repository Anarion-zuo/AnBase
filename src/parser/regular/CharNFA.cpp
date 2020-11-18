//
// Created by anarion on 11/18/20.
//

#include <container/List/LinkedList.hpp>
#include "parser/regular/CharNFA.h"

anarion::CharNFA::Node *anarion::CharNFA::Node::destination = nullptr;
anarion::CharNFA::Node *anarion::CharNFA::Node::error = nullptr;

anarion::Vector<anarion::CharNFA::Node *> &anarion::CharNFA::Node::getNext(char c) {
    return char2nexts.find(c)->get_val();
}

anarion::CharNFA::Node *anarion::CharNFA::Node::getDestination() {
    mutex.lock();
    if (destination == nullptr) {
        destination = new Node();
    }
    mutex.unlock();
    return destination;
}

bool anarion::CharNFA::Node::isDestination() const {
    return getDestination() == this;
}

anarion::size_type anarion::CharNFA::Node::nextsCount() const {
    return char2nexts.size();
}

anarion::CharNFA::Node *anarion::CharNFA::Node::parseRegular(const char *regular, anarion::size_type length) {
    return nullptr;
}

anarion::CharNFA::CharNFA(const char *regular, anarion::size_type length)
    : source(Node::parseRegular(regular, length)) {

}

bool anarion::CharNFA::match(const char *expr, anarion::size_type length) {
    if (expr == nullptr || length == 0) {
        // empty string
        return true;
    }
    LinkedList<Node*> nodes;
    if (source) {
        nodes.push_front(source);
    } else {
        // empty regular
        return false;
    }
    for (size_type index = 0; index < length; ++index) {
        if (nodes.empty()) {
            // all states end up failed
            return false;
        }
        LinkedList<Node *> newnodes;
        // look at all trying states
        while (!nodes.empty()) {
            Node *node = nodes.pop_front();
            auto it = node->char2nexts.find(expr[index]);
            if (it == node->char2nexts.end_iterator()) {
                // cannot find a next move for this character
                // this state must end here
            } else {
                // has next move(s)
                for (auto newit = it->get_val().begin_iterator(); newit != it->get_val().end_iterator(); ++newit) {
                    if ((**newit).isDestination()) {
                        // a destination is found
                        return true;
                    }
                    newnodes.push_back(*newit);
                }
            }
        }
        nodes = move(newnodes);
    }
    // no destination is acheived after draining the whole string
    return false;
}

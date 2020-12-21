//
// Created by anarion on 4/14/20.
//

#ifndef MYCPPBASE_BUFFER_H
#define MYCPPBASE_BUFFER_H

#include <container/List/LinkedList.hpp>
#include <allocator/FixedLengthAllocator.h>
#include <container/Map/HashMap.hpp>

namespace anarion {

    size_type writen(int fd, void *buf, size_type nbytes);
    size_type readn(int fd, void *buf, size_type nbytes);
    size_type sendn(int cfd, void *buf, size_type nbytes, int flags);
    size_type recvn(int cfd, void *buf, size_type nbytes, int flags);

    class SString;
    class Buffer {

        /*
         * unit of buffer memory
         * Directly uses the FixedLengthAllocator for fast memory allocation.
         *
         * Initialization:
         *      Must provide an empty parameter constructor, therefore no direct constructor is available.
         *      1) set allocator pointer
         *      2) allocate memory and set begin and cur
         *      3) reset pos to begin for readings from the beginning
         */
//        struct frame {
//            char *begin = nullptr, *cur = nullptr, *pos = nullptr;
//            FixedLengthAllocator *allocator = nullptr;
//
//            frame() = default;
//
//            frame(const frame &rhs) :
//                begin(static_cast<char *>(rhs.allocator->allocate())),
//                cur(begin + rhs.writtenSize()),
//                pos(begin + rhs.readSize()),
//                allocator(rhs.allocator)
//            {
//                memcpy(begin, rhs.begin, rhs.writtenSize());
//            }
//
//            frame(frame &&rhs) noexcept :
//                begin(rhs.begin), cur(rhs.cur), pos(rhs.pos), allocator(rhs.allocator)
//            {
//                rhs.begin = nullptr;
//                rhs.cur = nullptr;
//                rhs.pos = nullptr;
//            }
//
//            ~frame() {
//                allocator->deallocate(begin);
//            }
//
//            constexpr size_type capacity() const { return allocator->getLength(); }
//            constexpr size_type writtenSize() const { return cur - begin; }
//            constexpr size_type readSize() const { return pos - begin; }
//        };

        LinkedList<void *> frames;
        const size_type frameLength;
        FixedLengthAllocator *allocator;

        // allocate a new frame
        void newFrame();
        void newFrames(size_type n);

        // select allocator
        static HashMap<size_type, FixedLengthAllocator*> allocatorMap;
        static FixedLengthAllocator *getAllocator(size_type frameLength);
        static Mutex allocatorMapLock;

        struct iterator {
            LinkedList<void*>::iterator listIt;
            size_type offset, listIndex = 0;

            iterator() : offset(0), listIt(nullptr) {}
            iterator(const iterator &rhs) = default;
            iterator(iterator &&rhs) noexcept :
                listIt(rhs.listIt), offset(rhs.offset)
            {
                rhs.offset = 0;
                rhs.listIt = LinkedList<void*>::iterator(nullptr);
            }
            iterator &operator=(const iterator &rhs) = default;

            void *offsettedPtr() { return (char*)(*listIt) + offset; }
            constexpr void increaseOffset(size_type off) { offset += off; }
        };
        iterator readIterator, writeIterator;
        constexpr size_type iteratorUnTouched(const iterator &it) const { return frameLength - it.offset; }
        static void iteratorNext(iterator &it) {
            it.offset = 0;
            it.listIt++;
            it.listIndex++;
        }
        bool iteratorIsEnd(const iterator &it) {
            return frames.end_iterator() == it.listIt;
        }

        void allocateNew(size_type more_size);

    public:
        explicit Buffer(size_type frameLength = 1024);
        Buffer(const Buffer &);
        Buffer(Buffer &&) noexcept ;
        ~Buffer() { clear(); }

        void clear();

        constexpr size_type unread() const { return size() - (readIterator.listIndex * frameLength + readIterator.offset); }
        constexpr size_type size() const { return writeIterator.listIndex * frameLength + writeIterator.offset; }
        constexpr size_type unwritten() const { return capacity() - size(); }
        constexpr size_type capacity() const { return frames.size() * frameLength; }
        constexpr bool empty() const { return size() == 0; }

        void rewind();
        void refresh();
        void setWriteIndex(size_type index);

        // arr
        void append_c(char c);
        void append_arr(char *p, size_type len);
        void append_arr(const char *str);
        void append_string(const SString &str);
        void write_arr(char *p, size_type len);
        void append_arr(Buffer &buffer, size_type len);
        void write_arr(Buffer &buffer, size_type len);

        // file descriptor
        size_type append_fd(int fd, size_type nbytes);
        size_type append_fd(int fd);
        size_type write_fd(int fd, size_type nbytes);
        size_type send_fd(int cfd, size_type nbytes, int flags);
        size_type recv_fd(int cfd, int flags);
        size_type recv_fd(int cfd, size_type nbytes, int flags);

        void print();
    };

}

#endif //MYCPPBASE_BUFFER_H

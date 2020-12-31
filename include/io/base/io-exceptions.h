//
// Created by anarion on 5/9/20.
//

#ifndef ANBASE_IO_EXCEPTIONS_H
#define ANBASE_IO_EXCEPTIONS_H

#include <exception>
#include <SystemException.h>

struct InvalidIoOperations : std::exception {
    const char *callingFunctionName;

    explicit InvalidIoOperations(const char *callingFunctionName) : callingFunctionName(callingFunctionName) {}

    const char *what() const noexcept override {
        return callingFunctionName;
    }
};

struct InvalidIOParameters : std::exception {};

class FdWriteFailed : public SystemException {};
class FdReadFailed : public SystemException {};
class OpenFdFailed : public SystemException {};
class CreatFailed : public SystemException {};
class FsyncFailed : public SystemException {};
class LinkFailed : public SystemException {};
class StatFailed : public SystemException {};
class RenameFailed : public SystemException {};
class ChmodFailed : public SystemException {};
class ChownFailed : public SystemException {};
struct RemoveFileFailed : public SystemException {};
struct FileOperationOnNotOpened : public SystemException {};

struct DirectoryException : public SystemException {};
struct DirectoryOpenFailed : public DirectoryException {};
struct DirectoryCreateFailed : public DirectoryException {};
struct DirectoryIterateFailed : public DirectoryException {};
struct TellDirFailed : public DirectoryException {};
struct DirEntryToFileFailed : public DirectoryException {};
struct RemoveDirectoryFailed : public DirectoryException {};

class SocketException : public SystemException {};
class SocketSendException : public SocketException {};
class SocketRecvException : public SocketException {};
class SocketCreateException : public SocketException {};
class ListenException : public SocketException {};
class KqueueException : public SocketException {};
class FcntlException : public SocketException {};
class EpollException : public SocketException {};
class ConnectException : public SocketException {};
class BindException : public SocketException {};
class AcceptException : public SocketException {};


#endif //ANBASE_IO_EXCEPTIONS_H

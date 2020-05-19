//
// Created by anarion on 5/9/20.
//

#ifndef ANBASE_IO_EXCEPTIONS_H
#define ANBASE_IO_EXCEPTIONS_H

struct FdWriteException : public SystemException {};
struct FdReadException : public SystemException {};
struct OpenFdException : public SystemException {};

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

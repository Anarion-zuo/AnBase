//
// Created by anarion on 12/2/20.
//

#include <gtest/gtest.h>
#include <serialize/logger/LogLevel.h>
#include <serialize/Serializer.h>
#include <serialize/logger/LogEntry.h>
#include <io/channel/terminal/TerminalPrintChannel.h>
#include <io/channel/file/FileChannel.h>

using namespace anarion;

TEST(TestLog, TestTerminalPrint) {
    printf("\033[1;31m");
    printf("Hello world\n");
    printf("\033[0m");
}

TEST(TestLog, TestTerminalOutput) {
    Serializer serializer;
    StringMessageLogEntry *entry = new StringMessageLogEntry(SString("hello logger"));
    serializer.add(entry);
    serializer.add(entry);
    serializer.add(entry);
    TerminalPrintChannel termnial;
    serializer.commit(termnial);
}

TEST(TestLog, TestFileOutput) {
    Serializer serializer;
    StringMessageLogEntry *entry = new StringMessageLogEntry(SString("hello logger"), new WarningLogLevel);
    serializer.add(entry);
    serializer.add(entry);
    serializer.add(entry);

    FileOpenFlag oflag;
    oflag.setAppend();
    oflag.setReadWrite();
    oflag.setCreateIfNotExists();
    FileChannel file(Path(SString("./test.loginfo")), oflag);
    file.open(S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);
    serializer.commit(file);
    file.close();
}

TEST(TestLog, Test2Output) {
    Serializer serializer;
    StringMessageLogEntry *entry = new StringMessageLogEntry(SString("hello logger"), new WarningLogLevel);
    serializer.add(entry);
    serializer.add(entry);
    serializer.add(entry);

    FileOpenFlag oflag;
    oflag.setAppend();
    oflag.setReadWrite();
    oflag.setCreateIfNotExists();
    FileChannel file(Path(SString("./test.loginfo")), oflag);
    file.open(S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);

    TerminalPrintChannel termnial;

    Vector<InChannel *> channels {&termnial, &file};
    serializer.commit(channels.beginIterator(), channels.endIterator());

    file.close();
}

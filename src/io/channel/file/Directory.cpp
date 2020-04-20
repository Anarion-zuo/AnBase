//
// Created by anarion on 4/6/20.
//

#include <unistd.h>
#include <io/channel/file/FileChannel.h>
#include "io/channel/file/Directory.h"

static bool isDots(const char *str) {
    int len = strlen(str);
    if (len == 1) {
        return *str == '.';
    }
    if (len == 2) {
        return *str == '.' && str[1] == '.';
    }
    return false;
}

void anarion::Directory::open() {
    childs.clear();
    char *name_cstr = name.copy_cstr();
    handle = opendir(name_cstr);
    if (handle == nullptr) {
        name.release_copied(name_cstr);
        return;
    }

    // current working directory
    char *cwd = getcwd(nullptr, 0);
    // change for convenience
    chdir(name_cstr);

    dirent *info = nullptr;

    while (true) {
        info = readdir(handle);
        if (info == nullptr) {
            break;
        }
        if (info->d_type == DT_DIR) {
            // is directory
            // check ../.
            if (isDots(info->d_name)) {
                continue;
            }
            Directory *directory = new Directory(SString(const_cast<const char*>(info->d_name)));
            directory->open();
            childs.push_back(directory);
        } else if (info->d_type == DT_REG) {
            // is regular file
            FileChannel ob = FileChannel::open(SString(const_cast<const char*>(info->d_name)));
            FileChannel *file = new FileChannel(move(ob));
            childs.push_back(file);
        }
    }
    // change back
    chdir(cwd);
    free(cwd);

    name.release_copied(name_cstr);
}

anarion::Directory::Directory(SString &&name) : FileEntry(forward<SString>(name)) {}

void anarion::Directory::release() {
    for (auto it = childs.begin_iterator(); it != childs.end_iterator(); ++it) {
        (**it).release();
        delete *it;
    }
    childs.clear();
}

anarion::Directory::~Directory() {
//    release();
}

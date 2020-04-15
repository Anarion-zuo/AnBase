//
// Created by anarion on 4/6/20.
//

#include <unistd.h>
#include <io/channel/file/FileChannel.h>
#include "io/channel/file/Directory.h"

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
            Directory *directory = new Directory(SString(const_cast<const char*>(info->d_name)));
            directory->open();
            childs.push_back(directory);
        } else if (info->d_type == DT_REG) {
            // is regular file
            FileChannel *file = new FileChannel(FileChannel::open(SString(const_cast<const char*>(info->d_name))));
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
    }
    childs.clear();
}

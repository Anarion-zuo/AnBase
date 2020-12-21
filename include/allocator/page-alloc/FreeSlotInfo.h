//
// Created by anarion on 8/11/20.
//

#ifndef ANBASE_FREESLOTINFO_H
#define ANBASE_FREESLOTINFO_H


namespace anarion {
namespace page_alloc {
    struct FreeSlotInfo {
        FreeSlotInfo *next;

        static void makeList(char *begin, unsigned long counts, unsigned long step);
    };

}
}

#endif //ANBASE_FREESLOTINFO_H

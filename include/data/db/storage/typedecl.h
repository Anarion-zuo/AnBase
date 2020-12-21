//
// Created by anarion on 11/25/20.
//

#ifndef ANBASE_TYPEDECL_H
#define ANBASE_TYPEDECL_H

#include <container/List/Vector.hpp>

namespace anarion {
    namespace db {
        using pageno_t = uint32_t;
        using pageoff_t = uint32_t;

        using bufferno_t = uint32_t;
        using bufferoff_t = uint32_t;


        using blockno_t = uint16_t;
        using blockoff_t = uint16_t;
    }
}

#endif //ANBASE_TYPEDECL_H

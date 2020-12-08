//
// Created by anarion on 12/8/20.
//

#include <gtest/gtest.h>
#include <data/db/IdNamed.h>

using namespace anarion;
using namespace anarion::db;

struct NamedStruct {
    using IdType = uint16_t;
    IdType id;
    IdType getId() const {
        return id;
    }
    void setId(IdType nid) {
        this->id = nid;
    }
};

TEST(TestNamedAllocator, TestInit) {
    NamedAllocator<NamedStruct> allocator;
    NamedStruct *p = allocator.allocate();
    ASSERT_EQ(p->getId(), 0);
}

TEST(TestNamedAllocator, TestNameReuseSingle) {
    NamedAllocator<NamedStruct> allocator;
    size_type allocateLength = 100, noteIndex = 44;
    NamedStruct *note;
    for (size_type index = 0; index < allocateLength; ++index) {
        NamedStruct *p = allocator.allocate();
        if (index == noteIndex) {
            note = p;
        }
    }
    allocator.deallocate(noteIndex);
    ASSERT_EQ(noteIndex, allocator.allocate()->getId());
}

TEST(TestNamedAllocator, TestNameReuseMultiple) {
    NamedAllocator<NamedStruct> allocator;
    size_type allocateLength = 100;
    for (size_type index = 0; index < allocateLength; ++index) {
        NamedStruct *p = allocator.allocate();
    }
    size_type noteLength = 10, step = allocateLength / noteLength;
    for (size_type index = 0; index < noteLength; ++index) {
        allocator.deallocate(index * step);
    }
    for (size_type index = 0; index < noteLength; ++index) {
        ASSERT_EQ(allocator.allocate()->getId(), index * step);
    }
}

TEST(TestNamedAllocator, TestDeallocateMoreThanOnce) {
    NamedAllocator<NamedStruct> allocator;
    size_type allocateLength = 100;
    for (size_type index = 0; index < allocateLength; ++index) {
        NamedStruct *p = allocator.allocate();
    }
    size_type targetIndex = rand() % allocateLength;
    allocator.deallocate(targetIndex);
    allocator.deallocate(targetIndex);
    allocator.deallocate(targetIndex);
    allocator.deallocate(targetIndex);
    ASSERT_EQ(allocator.allocate()->getId(), targetIndex);
}

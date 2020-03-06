// slist_test.cpp
// Unit Test: wdl::interlocked::slist
//
// Build
//  cl /EHsc /nologo /std:c++17 /W4 /I C:\Dev\WDL slist_test.cpp

#include <cstdio>
#include <malloc.h>

#include "wdl/interlocked/slist.hpp"

constexpr auto N_ITEMS = 10;

struct PROGRAM_ITEM
{
    SLIST_ENTRY   slist_entry;
    unsigned long id;
};

int main()
{
    wdl::interlocked::slist list{};

    for (unsigned long i = 0; i < N_ITEMS; ++i)
    {
        auto ptr = static_cast<PROGRAM_ITEM*>(
            _aligned_malloc(sizeof(PROGRAM_ITEM), MEMORY_ALLOCATION_ALIGNMENT)
            );

        ptr->id = i;
        list.push_entry(&(ptr->slist_entry));
    }

    printf("Size = %u\n", list.size());

    for (auto i = 0; i < N_ITEMS; ++i)
    {
        auto p_entry = list.pop_entry();
        auto p_program_item = reinterpret_cast<PROGRAM_ITEM*>(p_entry);

        printf("Popped Id: %u\n", p_program_item->id);

        // assumes that SLIST_ENTRY is the first entry in struct
        _aligned_free(static_cast<void*>(p_entry));
    }

    list.flush();

    printf("Size = %u\n", list.size());
}
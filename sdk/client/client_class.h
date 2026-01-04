#pragma once

#include "recv_table.h"
#include "../const.h"
#include "../../utils/memory.h"

class CClientClass
{
public:
        void* createFn;
        void* createEventFn;
        char* networkName;
        CRecvTable* table;
        CClientClass* next;
        int classID;
};

class CHLClient
{
public:
        CClientClass* GetAllClasses()
        {
                return memory::CallVFunc<CClientClass*>(this, 8);
        }
};

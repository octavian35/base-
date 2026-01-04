#pragma once
#include <cstdint>

class CRecvProxyData;
class CRecvProp;
class CRecvTable;

using ArrayLengthRecvProxy_t = void(__cdecl*)(void* ptr, int id, int len);
using RecvVarProxy_t = void(__cdecl*)(CRecvProxyData* data, void* struct_ptr, void* out);
using DataTableRecvVarProxy_t = void(__cdecl*)(CRecvProp* prop, void** out, void* data, int id);

using CreateClientClass_t = void* (__cdecl*)(int index, int serial);
using CreateEvent_t = void* (__cdecl*)();

enum class ESendPropType : std::int32_t
{
    DPT_Int = 0,
    DPT_Float,
    DPT_Vector,
    DPT_String,
    DPT_Array,
    DPT_DataTable
};

class CDataVariant
{
public:
    union {
        float	     m_Float;
        long	     m_Int;
        char* m_pString;
        void* m_pData;
        float	     m_Vector[3];
        long long	 m_Int64;
    };

    ESendPropType     m_Type;
};

class CRecvProp
{
public:
    char* m_pVarName;
    int						 m_RecvType;
    int						 m_Flags;
    int						 m_StringBufferSize;
    bool					 m_bInsideArray;
    const void* m_pExtraData;
    CRecvProp* m_pArrayProp;
    ArrayLengthRecvProxy_t	 m_ArrayLengthProxy;
    RecvVarProxy_t			 m_ProxyFn;
    DataTableRecvVarProxy_t	 m_DataTableProxyFn;
    CRecvTable* m_pDataTable;
    int						 m_Offset;
    int						 m_ElementStride;
    int						 m_nElements;
    const char* m_pParentArrayPropName;
};

class CRecvProxyData
{
public:
    CRecvProp* prop;       // 0x00
    CDataVariant    value;      // 0x08
    std::int32_t    element;    // 0x20
    std::int32_t    objectID;   // 0x24
    // total: 0x28 bytes
};

class CRecvTable
{
public:
    CRecvProp* m_pProps;
    int				 m_nProps;
    void* m_pDecoder;
    char* m_pNetTableName;
    bool			 m_bInitialized;
    bool			 m_bInMainList;
};

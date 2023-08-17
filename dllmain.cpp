#include "pch.h"
class DataSlot
{
public:
    char pad_0000[72]; //0x0000
    char* Slot; //0x0048
    char pad_0050[48]; //0x0050
}; //Size: 0x0080

class Entry
{
public:
    char pad_0000[88]; //0x0000
    wchar_t* Name; //0x0058
    char pad_0060[72]; //0x0060
    class N000002DE* Funnyptr; //0x00A8
    char pad_00B0[48]; //0x00B0
    int32_t Cost; //0x00E0
    int32_t Quantity; //0x00E4
    char pad_00E8[160]; //0x00E8
}; //Size: 0x0188

class N000002DE
{
public:
    char pad_0000[120]; //0x0000
    class N000002F2* SetIDPtr; //0x0078
    char pad_0080[120]; //0x0080
    char Rank; //0x00F8
    char pad_0100[140]; //0x0100
}; //Size: 0x0188

class N000002F2
{
public:
    char pad_0000[472]; //0x0000
    int32_t SetID; //0x01D8
    char pad_01DC[172]; //0x01DC
}; //Size: 0x0288
struct Node
{
    wchar_t* Name;
    int location;
    int32_t Quantity;
    int Rank;
    int32_t SetID;
    int32_t Cost;
    Node* next;
};

typedef void** (__fastcall* _RefreshCatagory)(uintptr_t** ptr, void* ShopcatagoryStruct);

DWORD WINAPI hackthread(LPVOID param)
{
    AllocConsole();
    FILE* f;
    //Utility::HookManager* Hookmanager = new Utility::HookManager();
    freopen_s(&f, "CONOUT$", "w", stdout);
    std::cout << "Injected immediately! LOL\n";
    uintptr_t base = (uintptr_t)GetModuleHandle(NULL);
    _RefreshCatagory RefreshCat = (_RefreshCatagory)(base + 0xA8A550); //0x1E2480 = ShopCatagory
    while (true)
    {
        if (GetAsyncKeyState(VK_NUMPAD2) & 1)
        {
            break;
        }
        if (GetAsyncKeyState(VK_NUMPAD1) & 1)
        {
            int isLoad = *(int *)FindDMAAddy((base + 0x02FBD6C0), { 0x400,0xA80,0x668,0x9F0});
            std::cout << "Starting...\n";
            void* FireBase = (void *)0x6F85D4D5C0;
            uintptr_t bazaarentries = FindDMAAddy((base + 0x030D3BC0), { 0x88,0x60,0x400,0x778,0x10,0x40,0x0 });
            if (!bazaarentries || bazaarentries == 0 || !isLoad)
            {
                std::cout << "Help! Null PTR! Is bazaar open?\n";
            }
            else
            {
                std::cout << "We are Hooked! Bazaar didnt know what hit them!\n";
                uintptr_t** thisptr = (uintptr_t**)FindDMAAddy((base + 0x02FBD6C0), {0x258,0x208,0x0});
                DataSlot* SLUT = (DataSlot*)FindDMAAddy((base + 0x0304AFE0), { 0x8,0x260 });
                char* CatBase = (char*)FindDMAAddy((base + 0x02FBD6C0), { 0x400,0x70,0x0 });
                while (SLUT)
                {
                    for (int i = 0; i < 8; i++)
                    {
                        char* CatInject = *(char**)(CatBase + 0x180 + 0x10 * i);
                        SLUT->Slot = CatInject;
                        RefreshCat(thisptr, SLUT);
                        Sleep(200);
                        isLoad = *(int*)FindDMAAddy((base + 0x02FBD6C0), { 0x400,0xA80,0x668,0x9F0 });
                        while (!isLoad)
                        {
                            isLoad = *(int*)FindDMAAddy((base + 0x02FBD6C0), { 0x400,0xA80,0x668,0x9F0 });
                            Sleep(50);
                        }
                        UINT16 PageQuantity = *(uintptr_t*)(FindDMAAddy((base + 0x02FBD6C0), { 0x400,0x260,0x88,0xA80,0x1B0 }));
                        Node* EntryLinkedList = GenerateBazaar(base, PageQuantity);
                    }
                }
            }
        }
        Sleep(20);
    }
    // Free the DLL
    fclose(f);
    FreeConsole();
    FreeLibraryAndExitThread((HMODULE)param, NULL);
    return NULL;
}

BOOL WINAPI DllMain(HINSTANCE hModule, DWORD dwReason, LPVOID lpReserved)
{
    switch (dwReason)
    {
    case DLL_PROCESS_ATTACH:
        CreateThread(0, 0, hackthread, hModule, 0, 0);
        break;

    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}
Node* GenerateBazaar(uintptr_t base,UINT16 PageQuantity)
{
    Entry* bazaarentry;
    int loc = 0;
    uintptr_t bazaarentries = FindDMAAddy((base + 0x030D3BC0), { 0x88,0x60,0x400,0x778,0x10,0x40,0x0 });
    while (loc == 0)
    {
        if (bazaarentries)
        {
            bazaarentry = *(Entry**)bazaarentries;
            loc = 1;
        }
        else
        {
            Sleep(50);
            bazaarentries = FindDMAAddy((base + 0x030D3BC0), { 0x88,0x60,0x400,0x778,0x10,0x40,0x0 });
        }
    }
    struct Node* Head = NULL;
    for (int i = 0; i < PageQuantity; i++)
    {
        bazaarentry = *(Entry**)(bazaarentries + (i * 0x10));

        char* help = (char*)(&(bazaarentry->Name));
        help = help + 5;
        bool Arrcheck = *(bool*)help;
        struct Node* Entry = new Node;
        if (Arrcheck)
        {
            Entry->Name = bazaarentry->Name;
        }
        else
        {
            Entry->Name = (wchar_t*)&(bazaarentry->Name);
        }
        Entry->Cost = bazaarentry->Cost;
        Entry->Quantity = bazaarentry->Quantity;
        Entry->Rank = (int)bazaarentry->Funnyptr->Rank;
        Entry->SetID = bazaarentry->Funnyptr->SetIDPtr->SetID;
        Entry->next = Head;
        Entry->location = loc;
        Head = Entry;
        std::wcout << Entry->Name;
        std::cout << " | ";
        std::cout << Entry->Quantity;
        std::cout << " | ";
        std::cout << Entry->Rank;
        std::cout << " | ";
        std::cout << Entry->SetID;
        std::cout << " | ";
        std::cout << Entry->Cost;
        std::cout << "\n";
        loc++;
    }
    return Head;
}
uintptr_t  FindDMAAddy(uintptr_t  BaseAddress, std::vector<unsigned int> Offsets)
{
    //DWORD exception = EXCEPTION_ACCESS_VIOLATION;
    uintptr_t Ptr = *(uintptr_t*)(BaseAddress);
    if (Ptr == 0) return NULL;

    for (int i = 0; i < Offsets.size(); i++)
    {
        if (i == Offsets.size() - 1)
        {
            Ptr = (uintptr_t)(Ptr + Offsets[i]);
            if (Ptr == 0) return NULL;
            return Ptr;
        }
        else
        {
                Ptr = *(uintptr_t*)(Ptr + Offsets[i]);
                if (Ptr == 0) return NULL;
        }
    }
    return Ptr;
}
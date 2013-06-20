#define HOOK_API __declspec(dllexport)

HOOK_API int initHook(HWND hWnd,TCHAR* username);

typedef __success(return == 0) LONG    DNS_STATUS;

#define WSAAPI                  FAR PASCAL

typedef struct addrinfoW
{
    int                 ai_flags;       // AI_PASSIVE, AI_CANONNAME, AI_NUMERICHOST
    int                 ai_family;      // PF_xxx
    int                 ai_socktype;    // SOCK_xxx
    int                 ai_protocol;    // 0 or IPPROTO_xxx for IPv4 and IPv6
    size_t              ai_addrlen;     // Length of ai_addr
    PWSTR               ai_canonname;   // Canonical name for nodename
    __field_bcount(ai_addrlen) struct sockaddr *   ai_addr;        // Binary address
    struct addrinfoW *  ai_next;        // Next structure in linked list
}ADDRINFOW, *PADDRINFOW;
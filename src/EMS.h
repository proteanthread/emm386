/*
 * ems.inc - --- EMS status codes (C17 standard)
 *
 * Architectural Role:
 *   Serves as the C counterpart header representing EMS.INC.
 *
 * Changeability & Constraints:
 *   - CAN BE CHANGED: Local helper functions, logging wrappers, and diagnostic outputs.
 *   - CANNOT BE CHANGED: Standard API calling conventions, hardware entry vectors, and binary structure alignments.
 *
 * Expected Behavior:
 *   - Mapped counterpart declarations and logic flow from the original assembly source.
 *
 * Diagnostics & Recovery:
 *   - Verify compiler alignment flags and register preservation states if system lockups occur.
 */

#define EMSS_OK 00h
#define EMSS_SOFTWARE_ERR 80h // unexpected error
#define EMSS_HARDWARE_ERR 81h // should never occur with EMM emulators
#define EMSS_EMM_BUSY 82h // should never occur with EMS 3.2+
#define EMSS_INVALID_HANDLE 83h
#define EMSS_INVALID_FUNCTION 84h // function code in AH not defined
#define EMSS_NO_MORE_HANDLES 85h // all handles in use
#define EMSS_CONTEXT_EXISTS 86h // 45h: try to free a handle with saved context
#define EMSS_OUT_OF_PAGES 87h // 43h, 51h, 5A00h, 5A01h
#define EMSS_OUT_OF_FREE_PAGES 88h // 43h, 51h, 5A00h, 5A01h
#define EMSS_ZERO_PAGES 89h // 43h
#define EMSS_LOG_PAGE_INVALID 8Ah // 44h, 50h, 55h, 56h, 57h
#define EMSS_PHYS_PAGE_INVALID 8Bh // 44h, 4Fh, 50h, 55h, 56h
#define EMSS_CONTEXT_STACK_FULL 8Ch // 46h
#define EMSS_STATE_ALREADY_SAVED 8Dh // 47h
#define EMSS_NO_STATE_IS_SAVED 8Eh // 48h
#define EMSS_INVALID_SUBFUNC 8Fh // 4Eh, 4Fh, 50h, 52h, 53h, 54h, 57h, 58h, 59h, 5Bh
#define EMSS_UNDEF_ATTR_TYPE 90h
#define EMSS_FEATURE_UNSUPP 91h // 52h
#define EMSS_OVERLAP_OCCURED 92h // 5700h
#define EMSS_LENGTH_EXCEEDED 93h // 57h
#define EMSS_CONVEMS_OVERLAPP 94h // 57h (jemm never returns this code)
#define EMSS_OFFS_EXCEEDS_PGSIZ 95h // 57h
#define EMSS_REGLEN_EXCEEDS_1MB 96h // 57h
#define EMSS_REGIONS_OVERLAPP 97h // 5701h
#define EMSS_TYPE_UNDEFINED 98h // 57h
#define EMSS_ALT_MAPS_UNSUPP 9Ch // 5B01h, 5B04h, 5B06h, 5B07h, 5B08h
#define EMSS_INVALID_ALT_MAP 9Dh // 5Bh (jemm never returns this code)
#define EMSS_MBBOUNDARY_CROSSED 0A2h // 57h
#define EMSS_SRC_ARRAY_INVALID 0A3h // 4E01h, 5B01h
#define EMSS_ACCESS_DENIED 0A4h // 5Bh


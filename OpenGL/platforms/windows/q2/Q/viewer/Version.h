//#include "svn_version.h"

#define STRINGIZE2(s) #s
#define STRINGIZE(s) STRINGIZE2(s)

#define VERSION_MAJOR               1
#define VERSION_MINOR               0
#define VERSION_REVISION            0
#define VERSION_BUILD               1

#if SVN_LOCAL_MODIFICATIONS
  #define VERSION_MODIFIER "M"
  #define VER_VER_PATCHED  VS_FF_PATCHED
#else
  #define VERSION_MODIFIER
  #define VER_VER_PATCHED  0
#endif

#define VER_COMPANY_NAME_STR        "Epson Edge"
#define VER_FILE_VERSION            VERSION_MAJOR, VERSION_MINOR, VERSION_REVISION, VERSION_BUILD
#define VER_FILE_VERSION_STR        STRINGIZE(VERSION_MAJOR)        \
    "." STRINGIZE(VERSION_MINOR)    \
    "." STRINGIZE(VERSION_REVISION) \
    "." STRINGIZE(VERSION_BUILD)    \

#define VER_PRODUCTNAME_STR         "Temp"
#define VER_PRODUCT_VERSION         VER_FILE_VERSION
#define VER_PRODUCT_VERSION_STR     VER_FILE_VERSION_STR
#define VER_ORIGINAL_FILENAME_STR   VER_PRODUCTNAME_STR ".exe"
#define VER_INTERNAL_NAME_STR       VER_ORIGINAL_FILENAME_STR
#define VER_COPYRIGHT_STR           "(c) Seiko Epson Corporation.  All rights reserved."

#ifdef _DEBUG
#define VER_VER_DEBUG             VS_FF_DEBUG
#else
#define VER_VER_DEBUG             0
#endif

#define VER_FILEFLAGS               VER_VER_DEBUG | VER_VER_PATCHED

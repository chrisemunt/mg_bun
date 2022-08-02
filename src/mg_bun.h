/*
   ----------------------------------------------------------------------------
   | mg_bun.so|dll                                                            |
   | Description: An abstraction of the InterSystems Cache/IRIS API           |
   |              and YottaDB API                                             |
   | Author:      Chris Munt cmunt@mgateway.com                               |
   |                         chris.e.munt@gmail.com                           |
   | Copyright (c) 2021-2022 M/Gateway Developments Ltd,                      |
   | Surrey UK.                                                               |
   | All rights reserved.                                                     |
   |                                                                          |
   | http://www.mgateway.com                                                  |
   |                                                                          |
   | Licensed under the Apache License, Version 2.0 (the "License"); you may  |
   | not use this file except in compliance with the License.                 |
   | You may obtain a copy of the License at                                  |
   |                                                                          |
   | http://www.apache.org/licenses/LICENSE-2.0                               |
   |                                                                          |
   | Unless required by applicable law or agreed to in writing, software      |
   | distributed under the License is distributed on an "AS IS" BASIS,        |
   | WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. |
   | See the License for the specific language governing permissions and      |
   | limitations under the License.                                           |
   |                                                                          |
   ----------------------------------------------------------------------------
*/

#ifndef MG_BUN_H
#define MG_BUN_H

#define MAJORVERSION             1
#define MINORVERSION             0
#define MAINTVERSION             1
#define BUILDNUMBER              1

#define MGBUN_VERSION_MAJOR     "1"
#define MGBUN_VERSION_MINOR     "0"
#define MGBUN_VERSION_BUILD     "1"

#define MGBUN_VERSION           MGBUN_VERSION_MAJOR "." MGBUN_VERSION_MINOR "." MGBUN_VERSION_BUILD
#define MGBUN_COMPANYNAME       "M/Gateway Developments Ltd\0"
#define MGBUN_FILEDESCRIPTION   "API Abstraction for InterSystems IRIS/Cache and YottaDB\0"
#define MGBUN_FILEVERSION       MGBUN_VERSION
#define MGBUN_INTERNALNAME      "mg_bun\0"
#define MGBUN_LEGALCOPYRIGHT    "Copyright 2021-2022, M/Gateway Developments Ltd\0"
#define MGBUN_ORIGINALFILENAME  "mg_bun\0"
#define MGBUN_PLATFORM          PROCESSOR_ARCHITECTURE
#define MGBUN_PRODUCTNAME       "mg_bun\0"
#define MGBUN_PRODUCTVERSION    MGBUN_VERSION
#define MGBUN_BUILD             MGBUN_VERSION

#if defined(_WIN32)

#define BUILDING_NODE_EXTENSION     1
#if defined(_MSC_VER)
/* Check for MS compiler later than VC6 */
#if (_MSC_VER >= 1400)
#define _CRT_SECURE_NO_DEPRECATE    1
#define _CRT_NONSTDC_NO_DEPRECATE   1
#endif
#endif
#endif

#if defined(_WIN32)
#define MGBUN_EXTFUN(a)    __declspec(dllexport) a __cdecl
#else
#define MGBUN_EXTFUN(a)    a
#endif

MGBUN_EXTFUN(void)     mgbun_init       (void);
MGBUN_EXTFUN(char *)   mgbun_version    (void);
MGBUN_EXTFUN(char *)   mgbun_dbversion  (void);
MGBUN_EXTFUN(char *)   mgbun_command    (unsigned char *input, int input_len, int command, int context);
MGBUN_EXTFUN(char *)   mgbun_benchmark  (unsigned char *input, int input_len, int command, int context);

#endif


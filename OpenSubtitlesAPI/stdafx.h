// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>
#include <windows.h>


// TODO: reference additional headers your program requires here
#include <iostream>
#include <string>
#include <vector>
#include "pugixml/pugixml.hpp"
#include <WinInet.h>
#define MSG_SEARCH CMessageLoop::MSG_START
#include "Tools\StringConversion.h"
#include "Buffer.h"
#include "XMLMemoryWriter.h"
#include "InputFileInfo.h"
#include "IOpenSubtitlesAPI.h"
#include "CXMLBuilder.h"
#include "CConnection.h"
#include "MessageLoop/MessageLoop.h"
#include "COpenSubtitlesAPI_Impl.h"
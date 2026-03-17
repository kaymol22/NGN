#pragma once

#include "Core/PlatformDetection.h"

#include <iostream>
#include <memory>
#include <utility>
#include <algorithm>
#include <functional>
#include <cassert>
#include <filesystem>

#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <unordered_set>


#include "Core/Base.h"

#include "Core/Profile.h"

#ifdef NGN_PLATFORM_WINDOWS
	#include <Windows.h>
#endif 
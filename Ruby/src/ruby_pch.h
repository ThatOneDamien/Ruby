#pragma once
#include "Ruby/Main/Core.h"

#ifdef RB_PLAT_WIND
#ifndef NOMINMAX
#define NOMINMAX
#endif
#endif

#include <stdint.h>
#include <iostream>
#include <algorithm>
#include "Ruby/Utility/Pointers.h"

#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>

#ifdef RB_PLAT_WIND
#include <Windows.h>
#endif


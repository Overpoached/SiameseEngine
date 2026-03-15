//memory
#include <memory>
#include <limits>

//filesystem
#include <filesystem>

//containers
#include <string>
#include <vector>
#include <map>
#include <list>
#include <unordered_map>

//input output
#include <iostream>
#include <iomanip>
#include <sstream>
#include <istream>
#include <fstream>

//errors
#include <cassert>

//time
#include <chrono>

//others
#include <cstdint>

//windows
#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#include <process.h>
#define getpid _getpid
#else
#include <unistd.h>
#include <limits.h>
#endif
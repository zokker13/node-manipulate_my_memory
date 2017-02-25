#ifndef WINDOWS_EXTENSION_HPP_
#define WINDOWS_EXTENSION_HPP_

#include <iostream>

#include "win32_shared.hpp"
#include "win32_close_handle.hpp"
#include "win32_open_process.hpp"
#include "win32_read_process_memory.hpp"

NAN_METHOD(NanWin32OpenProcess);
NAN_METHOD(NanWin32OpenProcessSync);
NAN_METHOD(NanWin32CloseHandle);
NAN_METHOD(NanWin32ReadProcessMemory);
NAN_METHOD(NanWin32ReadProcessMemorySync);

#endif
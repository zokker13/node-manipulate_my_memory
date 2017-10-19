#ifndef MAIN_HPP_
#define MAIN_HPP_

#include <iostream>

#include "shared.hpp"
#include "close_handle.hpp"
#include "open_process.hpp"
#include "read_process_memory.hpp"
#include "write_process_memory.hpp"
#include "find_window.hpp"
#include "get_window_thread_process_id.hpp"
#include "list_processes_and_modules.hpp"

NAN_METHOD(NanWin32OpenProcess);
NAN_METHOD(NanWin32OpenProcessSync);
NAN_METHOD(NanWin32CloseHandle);
NAN_METHOD(NanWin32CloseHandleSync);
NAN_METHOD(NanWin32ReadProcessMemory);
NAN_METHOD(NanWin32ReadProcessMemorySync);
NAN_METHOD(NanWin32WriteProcessMemory);
NAN_METHOD(NanWin32WriteProcessMemorySync);
NAN_METHOD(NanWin32FindWindow);
NAN_METHOD(NanWin32FindWindowSync);
NAN_METHOD(NanWin32GetWindowThreadProcessId);
NAN_METHOD(NanWin32GetWindowThreadProcessIdSync);
NAN_METHOD(NanWin32ListProcessesAndModules);
NAN_METHOD(NanWin32ListProcessesAndModulesSync);

#endif

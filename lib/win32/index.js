const manipulateMemory = require('bindings')('ManipulateMyMemory');

// Refer to https://msdn.microsoft.com/en-us/library/windows/desktop/ms684880(v=vs.85).aspx
const PROCESS_ACCESS_RIGHTS = {
    PROCESS_TERMINATE: 0x0001
  , PROCESS_CREATE_THREAD: 0x0002
  , PROCESS_SET_SESSIONID: 0x0004
  , PROCESS_VM_OPERATION: 0x0008
  , PROCESS_VM_READ: 0x0010
  , PROCESS_VM_WRITE: 0x0020
  , PROCESS_DUP_HANDLE: 0x0040
  , PROCESS_CREATE_PROCESS: 0x0080
  , PROCESS_SET_QUOTA: 0x0100
  , PROCESS_SET_INFORMATION: 0x0200
  , PROCESS_QUERY_INFORMATION: 0x0400
  , PROCESS_SUSPEND_RESUME: 0x0800
  , PROCESS_QUERY_LIMITED_INFORMATION: 0x1000
  , PROCESS_SET_LIMITED_INFORMATION: 0x1000
};

function openProcess(desiredAccess, inheritHandle, processId, cb) {
  manipulateMemory.openProcess(desiredAccess, inheritHandle, processId, cb);
}

function openProcessSync(desiredAccess, inheritHandle, processId) {
  return manipulateMemory.openProcessSync(desiredAccess, inheritHandle, processId);
}

function readProcessMemory(handle, address, size, cb) {
  manipulateMemory.readProcessMemory(handle, address, size, cb);
}

function readProcessMemorySync(handle, address, size) {
  return manipulateMemory.readProcessMemory(handle, address, size);
}

function closeHandle(handle, cb) {
  manipulateMemory.closeHandle(handle, cb);
}

function closeHandleSync(handle) {
  return manipulateMemory.closeHandleSync(handle);
}

function findWindow(className, windowName, cb) {
  manipulateMemory.findWindow(className, windowName, cb);
}

function findWindowSync(className, windowName) {
  return manipulateMemory.findWindowSync(className, windowName);
}

module.exports = {
    openProcess
  , openProcessSync
  , readProcessMemory
  , readProcessMemorySync
  , closeHandle
  , closeHandleSync
  , findWindow
  , findWindowSync
};

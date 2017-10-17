const _ = require('underscore');
const Promise = require('bluebird');

const mmm = require('bindings')('Win32ManipulateMyMemory');

const getWindowThreadProcessId = Promise.promisify(mmm.getWindowThreadProcessId);
const findWindow = Promise.promisify(mmm.findWindow);
const openProcess = Promise.promisify(mmm.openProcess);
const readProcessMemory = Promise.promisify(mmm.readProcessMemory);
const writeProcessMemory = Promise.promisify(mmm.writeProcessMemory);

class ManipulateMyMemory {

  constructor() {
    this._openHandle = null;
  }

  findWindow(className, windowName) {
    return findWindow(className, windowName);
  }

  getWindowThreadProcessId(handle) {
    return getWindowThreadProcessId(handle);
  }

  openProcess(access, inheritHandle, processId) {
    return openProcess(access, inheritHandle, processId)
      .then((openHandle) => {
        if (!openHandle) {
          // throw
        }

        this._openHandle = openHandle;
        return openHandle;
      });
  }

  readInt8(address, handle = this._openHandle) {
    return this.readRaw(address, 1, handle)
      .then((data) => {
        return data.readInt8LE(0);
      });
  }

  readUInt8(address, handle = this._openHandle) {
    return this.readRaw(address, 1, handle)
      .then((data) => {
        return data.readUInt8LE(0);
      });
  }

  readInt16(address, handle = this._openHandle) {
    return this.readRaw(address, 2, handle)
      .then((data) => {
        return data.readInt16LE(0);
      });
  }

  readUInt16(address, handle = this._openHandle) {
    return this.readRaw(address, 2, handle)
      .then((data) => {
        return data.readUInt16LE(0);
      });
  }
  
  readInt32(address, handle = this._openHandle) {
    return this.readRaw(address, 4, handle)
      .then((data) => {
        return data.readInt32LE(0);
      });
  }

  readUInt32(address, handle = this._openHandle) {
    return this.readRaw(address, 4, handle)
      .then((data) => {
        return data.readUInt32LE(0);
      });
  }

  readFloat(address, handle = this._openHandle) {
    return this.readRaw(address, 4, handle)
      .then((data) => {
        return data.readFloatLE(0);
      });
  }

  readDouble(address, handle = this._openHandle) {
    return this.readRaw(address, 8, handle)
      .then((data) => {
        return data.readDoubleLE(0);
      });
  }

  readRaw(address, size, handle) {

    if (!Number.isInteger(address)) {
      return Promise.reject(new TypeError('Parameter "address" must be of type "integer"'));
    }

    if (!Number.isInteger(size)) {
      return Promise.reject(new TypeError('Parameter "size" must be of type "integer"'));
    }

    if (!Number.isInteger(handle)) {
      return Promise.reject(new TypeError('Parameter "handle" must be of type "integer"'));
    }

    return readProcessMemory(handle, address, size)
      .then((data) => {
        const buf = Buffer.from(data);
        return buf;
      });
  }

  writeInt8(address, value, handle = this._openHandle) {

    if (!Number.isInteger(value)) {
      return Promise.reject(new TypeError('Parameter "value" must be of type "integer"'));
    }
    
    const buf = Buffer.alloc(1);
    buf.writeInt32LE(value);
    
    return this.writeRaw(address, buf, buf.length, handle);
  }

  writeUInt8(address, value, handle = this._openHandle) {

    if (!Number.isInteger(value)) {
      return Promise.reject(new TypeError('Parameter "value" must be of type "integer"'));
    }

    const buf = Buffer.alloc(1);
    buf.writeUInt32LE(value);
    
    return this.writeRaw(address, buf, buf.length, handle);
  }

  writeInt16(address, value, handle = this._openHandle) {

    if (!Number.isInteger(value)) {
      return Promise.reject(new TypeError('Parameter "value" must be of type "integer"'));
    }

    const buf = Buffer.alloc(2);
    buf.writeInt32LE(value);
    
    return this.writeRaw(address, buf, buf.length, handle);
  }

  writeUInt16(address, value, handle = this._openHandle) {

    if (!Number.isInteger(value)) {
      return Promise.reject(new TypeError('Parameter "value" must be of type "integer"'));
    }

    const buf = Buffer.alloc(2);
    buf.writeUInt32LE(value);
    
    return this.writeRaw(address, buf, buf.length, handle);
  }

  writeInt32(address, value, handle = this._openHandle) {

    if (!Number.isInteger(value)) {
      return Promise.reject(new TypeError('Parameter "value" must be of type "integer"'));
    }

    const buf = Buffer.alloc(4);
    buf.writeInt32LE(value);
    
    return this.writeRaw(address, buf, buf.length, handle);
  }

  writeUInt32(address, value, handle = this._openHandle) {

    if (!Number.isInteger(value)) {
      return Promise.reject(new TypeError('Parameter "value" must be of type "integer"'));
    }

    const buf = Buffer.alloc(4);
    buf.writeUInt32LE(value);
    
    return this.writeRaw(address, buf, buf.length, handle);
  }

  writeFloat(address, value, handle = this._openHandle) {

    if (!_.isNumber(value)) {
      return Promise.reject('Parameter "value" must be of type "number"');
    }

    const buf = Buffer.alloc(4);
    buf.writeFloatLE(value);
    
    return this.writeRaw(address, buf, buf.length, handle);
  }

  writeDouble(address, value, handle = this._openHandle) {

    if (!_.isNumber(value)) {
      return Promise.reject('Parameter "value" must be of type "number"');
    }

    const buf = Buffer.alloc(8);
    buf.writeDoubleLE(value);
    
    return this.writeRaw(address, buf, buf.length, handle);
  }

  writeRaw(address, value, size, handle) {
    
    if (!Number.isInteger(address)) {
      return Promise.reject(new TypeError('Parameter "address" must be of type "integer"'));
    }

    if (!Buffer.isBuffer(value)) {
      return Promise.reject(new TypeError('Parameter "value" must be of type "Buffer"'));
    }

    if (!Number.isInteger(size)) {
      return Promise.reject(new TypeError('Parameter "size" must be of type "integer"'));
    }

    if (!Number.isInteger(handle)) {
      return Promise.reject(new TypeError('Parameter "handle" must be of type "integer"'));
    }

    return writeProcessMemory(handle, address, size, value);
  }

  static get processAccessRights() {
    // Refer to https://msdn.microsoft.com/en-us/library/windows/desktop/ms684880(v=vs.85).aspx
    return {
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
    }
  }
}

module.exports = ManipulateMyMemory;
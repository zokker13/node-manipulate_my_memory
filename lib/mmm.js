const os = require('os');

const _ = require('underscore');
const Promise = require('bluebird');
const snapshot = require('process-list').snapshot;
const debug = require('debug')('mmm:abstract');

const RawMmm = os.platform() === 'win32' 
  ? require('./win32/raw')
  : require('./linux/raw');

class ManipulateMyMemory {

  constructor(processName) {
    this._processName = processName;
    this._openHandle = null;
  }

  selectProcess(access, inheritHandle, processName = this._processName) {

    return snapshot('pid', 'name')
      .then((procs) => {
        return _.find(procs, (proc) => {

          debug(`Checking "${proc.name}" (${proc.pid}) with "${processName}"`);
          return proc.name === processName;
        });
      })
      .then((proc) => {
        if (!proc) {
          throw new Error('Could not get process. Selection failed.');
        }

        return RawMmm.openProcess(access, inheritHandle, proc.pid)
          .then((openHandle) => {
            if (!openHandle) {
              throw new Error('Could not open the handle.');
            }

            this._openHandle = openHandle;
            return openHandle;
          });
      });
  }

  unselectProcess(openHandle = this._openHandle) {
    return RawMmm.closeHandle(openHandle)
      .then(() => {
        this._openHandle = null;
      });
  }

  findWindow(className, windowName) {
    return findWindow(className, windowName);
  }

  getWindowThreadProcessId(handle) {
    return getWindowThreadProcessId(handle);
  }

  readInt8(address, handle = this._openHandle) {
    return this.read(address, 1, handle)
      .then((data) => {
        return data.readInt8LE(0);
      });
  }

  readUInt8(address, handle = this._openHandle) {
    return this.read(address, 1, handle)
      .then((data) => {
        return data.readUInt8LE(0);
      });
  }

  readInt16(address, handle = this._openHandle) {
    return this.read(address, 2, handle)
      .then((data) => {
        return data.readInt16LE(0);
      });
  }

  readUInt16(address, handle = this._openHandle) {
    return this.read(address, 2, handle)
      .then((data) => {
        return data.readUInt16LE(0);
      });
  }
  
  readInt32(address, handle = this._openHandle) {
    return this.read(address, 4, handle)
      .then((data) => {
        return data.readInt32LE(0);
      });
  }

  readUInt32(address, handle = this._openHandle) {
    return this.read(address, 4, handle)
      .then((data) => {
        return data.readUInt32LE(0);
      });
  }

  readFloat(address, handle = this._openHandle) {
    return this.read(address, 4, handle)
      .then((data) => {
        return data.readFloatLE(0);
      });
  }
  
  readDouble(address, handle = this._openHandle) {
    return this.read(address, 8, handle)
    .then((data) => {
      return data.readDoubleLE(0);
    });
  }

  read(address, size, handle = this._openHandle) {

    if (!Number.isInteger(address)) {
      return Promise.reject(new TypeError('Parameter "address" must be of type "integer"'));
    }

    if (!Number.isInteger(size)) {
      return Promise.reject(new TypeError('Parameter "size" must be of type "integer"'));
    }

    if (!Number.isInteger(handle)) {
      return Promise.reject(new TypeError('Parameter "handle" must be of type "integer"'));
    }

    return RawMmm.readRaw(address, size, handle);
  }

  writeInt8(address, value, handle = this._openHandle) {

    if (!Number.isInteger(value)) {
      return Promise.reject(new TypeError('Parameter "value" must be of type "integer"'));
    }
    
    const buf = Buffer.alloc(1);
    buf.writeInt32LE(value);
    
    return this.write(address, buf, buf.length, handle);
  }

  writeUInt8(address, value, handle = this._openHandle) {

    if (!Number.isInteger(value)) {
      return Promise.reject(new TypeError('Parameter "value" must be of type "integer"'));
    }

    const buf = Buffer.alloc(1);
    buf.writeUInt32LE(value);
    
    return this.write(address, buf, buf.length, handle);
  }

  writeInt16(address, value, handle = this._openHandle) {

    if (!Number.isInteger(value)) {
      return Promise.reject(new TypeError('Parameter "value" must be of type "integer"'));
    }

    const buf = Buffer.alloc(2);
    buf.writeInt32LE(value);
    
    return this.write(address, buf, buf.length, handle);
  }

  writeUInt16(address, value, handle = this._openHandle) {

    if (!Number.isInteger(value)) {
      return Promise.reject(new TypeError('Parameter "value" must be of type "integer"'));
    }

    const buf = Buffer.alloc(2);
    buf.writeUInt32LE(value);
    
    return this.write(address, buf, buf.length, handle);
  }

  writeInt32(address, value, handle = this._openHandle) {

    if (!Number.isInteger(value)) {
      return Promise.reject(new TypeError('Parameter "value" must be of type "integer"'));
    }

    const buf = Buffer.alloc(4);
    buf.writeInt32LE(value);
    
    return this.write(address, buf, buf.length, handle);
  }

  writeUInt32(address, value, handle = this._openHandle) {

    if (!Number.isInteger(value)) {
      return Promise.reject(new TypeError('Parameter "value" must be of type "integer"'));
    }

    const buf = Buffer.alloc(4);
    buf.writeUInt32LE(value);
    
    return this.write(address, buf, buf.length, handle);
  }

  writeFloat(address, value, handle = this._openHandle) {

    if (!_.isNumber(value)) {
      return Promise.reject('Parameter "value" must be of type "number"');
    }

    const buf = Buffer.alloc(4);
    buf.writeFloatLE(value);
    
    return this.write(address, buf, buf.length, handle);
  }

  writeDouble(address, value, handle = this._openHandle) {

    if (!_.isNumber(value)) {
      return Promise.reject('Parameter "value" must be of type "number"');
    }

    const buf = Buffer.alloc(8);
    buf.writeDoubleLE(value);
    
    return this.write(address, buf, buf.length, handle);
  }

  write(address, value, size, handle = this._openHandle) {

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

    return RawMmm.writeRaw(address, value, size, handle);
  }

  static listProcesses(...fields) {
    return snapshot(fields);
  }

  static get winProcessAccessRights() {
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
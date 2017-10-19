const os = require('os');

const _ = require('underscore');
const Promise = require('bluebird');
const snapshot = require('process-list').snapshot;
const debug = require('debug')('mmm:abstract');

const RawMmm = os.platform() === 'win32' 
  ? require('./win32/raw')
  : require('./linux/raw');

/**
 * 
 * The main class responsible to manipulate the memory and trace pointers.
 * 
 */
class ManipulateMyMemory {

  /**
   * Creates an instance of ManipulateMyMemory.
   * 
   * @param {String} processName The name of the process you wish to grab.
   * @constructor
   * @memberof ManipulateMyMemory
   */
  constructor(processName) {
    this._processName = processName;
    this._openHandle = null;
  }

  /**
   * Selects a process and unlocks it for manipulation (Windows).
   * It selects the first process found. So if you hve more than one process to manipulate,
   * you need to list all processes first and do your logic around that.
   * 
   * @param {Integer} access The access flags you manually set. Only needed for Windows.
   * Known flags can be taken from [ManipulateMyMemory.winProcessAccessRights] and need to be
   * binary or'ed when you need to combine them (for writing and reading).
   * @param {Boolean} [inheritHandle] The openedHandle is populated to child hadles. Windows only.
   * @param {String} [processName=this._processName] The processName you want to apply the unlocking to.
   * @returns {Promise} Returns a promise that resolves with an open handle (Windows) or nothing.
   * Might reject with `TypeError` if you pass wrong parameters.
   * @memberof ManipulateMyMemory
   */
  selectProcess(access, inheritHandle = false, processName = this._processName) {

    if (!Number.isInteger(access)) {
      return Promise.reject(new TypeError('Parameter "access" must be of type "integer"'));
    }

    if (!_.isBoolean(inheritHandle)) {
      return Promise.reject(new TypeError('Parameter "inheritHandle" must be of type "Boolean"'));
    }

    if (!_.isString(processName)) {
      return Promise.reject(new TypeError('Parameter "processName" must be of type "String"'));
    }

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

  /**
   * Unselects (closesHandle) of the open handle. Only for Windows
   * 
   * @param {Integer} [openHandle=this._openHandle] The handle you wish to close nicely. 
   * @returns {Promise} Returns a promise that resolves with nothing.
   * Might reject if closing the handle didn't work.
   * @memberof ManipulateMyMemory
   */
  unselectProcess(openHandle = this._openHandle) {
    return RawMmm.closeHandle(openHandle)
      .then(() => {
        this._openHandle = null;
      });
  }

  /**
   * A function to find a window handle based on it's title or classname.
   * Windows only!
   * 
   * @param {String} className The class name you are looking for.
   * @param {String} windowName The window name you are looking for.
   * @returns {Promise} Returns a promise that resolves with the handle (if one is found).
   * Rejects with an error containing the reason why it didn't work upon error.
   * @memberof ManipulateMyMemory
   */
  findWindow(className, windowName) {
    return findWindow(className, windowName);
  }

  /**
   * The function to find the processId associated with the window handle.
   * 
   * @param {Integer} handle The handle to the window you found earlier.
   * @returns {Promise} Returns a promise that resolves with the PID of the process
   * May reject with an Error.
   * @memberof ManipulateMyMemory
   */
  getWindowThreadProcessId(handle) {
    return getWindowThreadProcessId(handle);
  }

  /**
   * This function helps you to trace a multipointer address.
   * Just pass in your pointerchain and the target address will be presented to you.
   * 
   * @param {Integer} address The base address used as startingpoint to find the pointers.
   * @param {Array<Integer>} pointers The list of pointers you need to find the needed value.
   * @param {Integer} [handle=this._openHandle] The previously opened handle you want to look into.
   * @returns {Promise} Returns a Promise that resolves with the proper target address.
   * May reject with an Error if the memory could not be read (wrong permissions).
   * @memberof ManipulateMyMemory
   * @example
   * return mmm.selectProcess(
   *   ManipulateMyMemory.winProcessAccessRights.PROCESS_VM_READ
   *   , false
   * )
   * .then((handle) => {
   *   return mmm.pointerAddress(0x1002caa70, [0x10,0x18,0x0,0x18]);
   * })
   * .then((add) => {
   *   console.log(`Looking at 0x${add.toString(16)}`); // Looking at 0x8416528
   *   return mmm.unselectProcess();
   * });
   */
  pointerAddress(address, pointers, handle = this._openHandle) {
    return this.readUInt32(address, handle)
      .then((subAddress) => {
        debug(`Found a potential address: 0x${subAddress.toString(16)}`);

        if (!pointers.length) {
          debug(`No more pointers to follow. Choosing 0x${address.toString(16)} as desired address`);
          return address;
        }

        const offset = pointers.shift();

        return this.pointerAddress(subAddress + offset, pointers, handle);
      });
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

  monitor(interval, readFunction, cb) {
    let previousValue = null;
    setInterval(() => {
      return readFunction()
        .then((value) => {
          if (value !== previousValue) {
            previousValue = value;
            cb(null, value);
          }
        })
        .catch((err) => {
          cb(err);
        });
    }, interval);
  }

  static listProcesses(cb) {
    return RawMmm.listProcessesAndModules(cb);
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
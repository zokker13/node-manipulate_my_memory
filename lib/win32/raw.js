const _ = require('underscore');
const Promise = require('bluebird');

const mmm = require('bindings')('Win32ManipulateMyMemory');

const closeHandle = Promise.promisify(mmm.closeHandle);
const getWindowThreadProcessId = Promise.promisify(mmm.getWindowThreadProcessId);
const findWindow = Promise.promisify(mmm.findWindow);
const openProcess = Promise.promisify(mmm.openProcess);
const readProcessMemory = Promise.promisify(mmm.readProcessMemory);
const writeProcessMemory = Promise.promisify(mmm.writeProcessMemory);

class RawManipulateMyMemory {

  static findWindow(className, windowName) {
    return findWindow(className, windowName);
  }

  static getWindowThreadProcessId(handle) {
    return getWindowThreadProcessId(handle);
  }

  static closeHandle(handle) {
    return closeHandle(handle);
  }

  static openProcess(access, inheritHandle, processId) {
    return openProcess(access, inheritHandle, processId)
      .then((openHandle) => {
        if (!openHandle) {
          // throw
        }

        this._openHandle = openHandle;
        return openHandle;
      });
  }

  static readRaw(address, size, handle) {

    return readProcessMemory(handle, address, size)
      .then((data) => {
        const buf = Buffer.from(data);
        return buf;
      });
  }

  static writeRaw(address, value, size, handle) {
    return writeProcessMemory(handle, address, size, value);
  }

}

module.exports = RawManipulateMyMemory;
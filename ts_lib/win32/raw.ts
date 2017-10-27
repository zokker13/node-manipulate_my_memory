import * as _ from 'underscore';
import * as BPromise from 'bluebird';

const mmm = require('bindings')('Win32ManipulateMyMemory');

const closeHandle = BPromise.promisify(mmm.closeHandle);
const getWindowThreadProcessId = BPromise.promisify(mmm.getWindowThreadProcessId);
const findWindow = BPromise.promisify<Number, String, String>(mmm.findWindow);
const openProcess = BPromise.promisify(mmm.openProcess);
const listProcessesAndModules = BPromise.promisify(mmm.listProcessesAndModules);
const readProcessMemory = BPromise.promisify(mmm.readProcessMemory);
const writeProcessMemory = BPromise.promisify(mmm.writeProcessMemory);

export class RawManipulateMyMemory {

  public static async findWindow(className: string, windowName: string): Promise<Number> {
    return findWindow(className, windowName);
  }

}
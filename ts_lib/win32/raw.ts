import * as _ from 'underscore';
import * as BPromise from 'bluebird';

const mmm = require('bindings')('Win32ManipulateMyMemory');

const closeHandle = BPromise.promisify<boolean, number>(mmm.closeHandle);
const getWindowThreadProcessId = BPromise.promisify<number, number>(mmm.getWindowThreadProcessId);
const findWindow = BPromise.promisify<number, String, String>(mmm.findWindow);
const openProcess = BPromise.promisify<number, number, boolean, number>(mmm.openProcess);
const listProcessesAndModules = BPromise.promisify<Array<any>>(mmm.listProcessesAndModules);
const readProcessMemory = BPromise.promisify<Buffer, number, number, number>(mmm.readProcessMemory);
const writeProcessMemory = BPromise.promisify<number, number, number, number, Buffer>(mmm.writeProcessMemory);

const Raw = {
  closeHandle
  , getWindowThreadProcessId
  , findWindow
  , openProcess
  , readProcessMemory
  , writeProcessMemory
  , listProcessesAndModules
};

export { Raw };
export default Raw;
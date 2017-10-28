import * as os from 'os';

import * as _ from 'underscore';
import * as Debug from 'debug';

import { EProcessAccessRights } from './../enums';
import { 
  IManipulateMyMemory
  , IHookFunction
  , IReadFunction } from './../interfaces';

const debug = Debug('manipulate_my_memory:win32:manipulate_my_memory');

import Raw from './raw';

class ManipulateMyMemoryWin32 implements IManipulateMyMemory {

  private openHandle: number;
  constructor(private processName: string) {

  }

  async selectProcess(access: EProcessAccessRights, inheritHandle: boolean, processName: string = this.processName) {
    
    if (!Number.isInteger(access)) {
      return Promise.reject(new TypeError('Parameter "access" must be of type "integer"'));
    }

    if (!_.isBoolean(inheritHandle)) {
      return Promise.reject(new TypeError('Parameter "inheritHandle" must be of type "integer"'));
    }

    if (!_.isString(processName)) {
      return Promise.reject(new TypeError('Parameter "processName" must be of type "integer"'));
    }
  }

  async unselectProcess(openHandle: number = this.openHandle) {

    if (!Number.isInteger(openHandle)) {
      return Promise.reject(new TypeError('Parameter "openHandle" must be of type "integer"'));
    }

    return Raw.closeHandle(openHandle);
  }

  async read(
    address: number
    , size: number
    , openHandle: number = this.openHandle
  ) {
    return Raw.readProcessMemory(address, size, openHandle);
  }

  async write(
    address: number
    , size: number
    , value: Buffer
    , openHandle: number = this.openHandle
  ) {
    return Raw.writeProcessMemory(openHandle, address, size, value);
  }

  async pointerAddress(
    address: number
    , pointers: Array<number>
    , size: number = 4
    , openHandle: number = this.openHandle
  ): Promise<number> {
    const subAddress = await this.read(address, size, openHandle)
    debug(`Found a potential address: 0x${subAddress.readInt32LE(0).toString(16)}`);

    if (!pointers.length) {
      debug(`No more pointers to follow. Choosing 0x${address.toString(16)} as desired address`);
      return address;
    }

    const offset = pointers.shift();

    return await this.pointerAddress(subAddress.readInt32LE(0) + offset, pointers, size, openHandle);
  }

  monitor(interval: number, readFunc: IReadFunction, hookFunc: IHookFunction) {

    let previousValue: any = null;

    setInterval(async () => {

      const val = await readFunc();

      if (val !== previousValue) {
        hookFunc(val);
        previousValue = val;
      }
    }, interval);
  }

}
import * as os from 'os';

import * as _ from 'underscore';
import * as Debug from 'debug';

import { BigInteger, BigNumber } from 'big-integer';
import * as bigInt from 'big-integer';


import { EProcessAccessRights } from './../enums';
import { 
  ILowLevelMMM
  , IHookFunction
  , IReadFunction, 
  IProcessStructure} from './../interfaces';

const debug = Debug('manipulate_my_memory:win32:manipulate_my_memory');

import Raw from './raw';

export default class LLWin32 implements ILowLevelMMM {

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
      return Promise.reject(new TypeError('Parameter "processName" must be of type "string"'));
    }

    const procs = await this.listProcesses();
    console.log(`Got ${procs.length} procs`);
    const proc = _.find(procs, (proc) => {
      return proc.processName === processName;
    });

    if (!proc) {
      return Promise.reject(new TypeError(`Process "${processName}" not found.`));
    }

    const handle = await Raw.openProcess(access, inheritHandle, proc.pid);
    if (!handle) {
      return Promise.reject(new Error(`Could not open the handle`));
    }

    this.openHandle = handle;
    return handle;
  }

  async unselectProcess(openHandle: number = this.openHandle) {

    if (!Number.isInteger(openHandle)) {
      return Promise.reject(new TypeError('Parameter "openHandle" must be of type "integer"'));
    }

    return Raw.closeHandle(openHandle);
  }

  async read(
    address: BigInteger
    , size: number
    , openHandle: number = this.openHandle
  ) {
    return await Raw.readProcessMemory(openHandle, address, size);
  }

  async write(
    address: BigInteger
    , size: number
    , value: Buffer
    , openHandle: number = this.openHandle
  ) {
    return Raw.writeProcessMemory(openHandle, address, size, value);
  }

  async pointerAddress(
    address: BigInteger
    , pointers: Array<BigInteger>
    , size: number = 4
    , openHandle: number = this.openHandle
  ): Promise<BigInteger> {
    const subAddress = await this.read(address, size, openHandle)
    
    debug(`Byte representation of "0x${address.toString(16)}": 0x${subAddress.toString('hex')} (${subAddress.length})`);

    // Big Endian? 
    const entries: number[] = [];
    for (const add of subAddress.values()) {
      entries.unshift(add);
    }

    const numericAddress = bigInt.fromArray(entries, 256, false);
    debug(`Potential Address: 0x${numericAddress.toString(16)}`);
    
    if (!pointers.length) {
      debug(`No more pointers to follow. Choosing 0x${address.toString(16)} as desired address`);
      return address;
    }

    const offset = pointers.shift();
    debug(`Checkout out offset "${offset.toString(16)}"`);

    return await this.pointerAddress(numericAddress.add(offset), pointers, size, openHandle);
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

  async listProcesses(): Promise<Array<IProcessStructure>> {
    const entries = await Raw.listProcessesAndModules();
    const properEntires: Array<IProcessStructure> = _.map<IProcessStructure, any>(entries, (entry: any) => {
      const desiredValue: IProcessStructure = {
        pid: 0,
        processName: '',
      };

      desiredValue.processName = entry.exeFile;
      desiredValue.pid = entry.pid;

      return desiredValue;
    });

    return properEntires;
  }

}
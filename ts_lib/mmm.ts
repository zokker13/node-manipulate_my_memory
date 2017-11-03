import { BigInteger, BigNumber } from 'big-integer';

import { ILowLevelMMM, IReadFunction, IHookFunction, IProcessStructure } from './interfaces';
import { EProcessAccessRights } from './enums';

import LLWin32 from './win32/ll_win32';

export class MMM<T extends LLWin32> implements ILowLevelMMM {
  constructor(private concreteInstance: T) {
    
  }

  async selectProcess(
    access: EProcessAccessRights
    , inheritHandle: boolean
    , processName?: string
  ): Promise<any> {
    return this.concreteInstance.selectProcess(access, inheritHandle, processName);
  }

  async unselectProcess(
    openHandle: number
  ): Promise<boolean> {
    return this.concreteInstance.unselectProcess(openHandle);
  }

  async listProcesses(): Promise<Array<IProcessStructure>> {
    return this.concreteInstance.listProcesses();
  }

  async read(address: BigInteger, size: number, openHandle: number) {
    return this.concreteInstance.read(address, size, openHandle);
  }

  async write(address: BigInteger, size: number, value: Buffer, openHandle: number): Promise<number> {
    return this.concreteInstance.write(address, size, value, openHandle);
  }

  async pointerAddress(address: BigInteger, pointers: Array<BigInteger>, size: number, openHandle?: number): Promise<BigInteger> {
    return this.concreteInstance.pointerAddress(address, pointers, size, openHandle);
  }

  monitor(interval: number, inputFunc: IReadFunction, hookFunc: IHookFunction): void {
    return this.concreteInstance.monitor(interval, inputFunc, hookFunc);
  }

  async readInt32(address: BigInteger, openHandle: number) {
    return this.read(address, 4, openHandle);
  }

}

export class MMMWin32 extends MMM<LLWin32> {

  constructor(processName: string) {
    super(new LLWin32(processName));
  }
}

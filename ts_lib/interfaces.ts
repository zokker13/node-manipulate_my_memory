import { EProcessAccessRights } from './enums';
import { BigInteger } from 'big-integer';

export interface ILowLevelMMM {
  selectProcess(access: EProcessAccessRights, inheritHandle: boolean, processName: string): Promise<any>;
  unselectProcess(openHandle: Number): Promise<boolean>;
  //findWindow(className: string|null, windowName: string|null): Promise<number>;
  read(address: BigInteger, size: number, openHandle: number): Promise<Buffer>;
  write(address: BigInteger, size: number, value: Buffer, openHandle: number): Promise<number>;
  pointerAddress(address: BigInteger, pointers: Array<BigInteger>, size: number, openHandle: number): Promise<BigInteger>;
  monitor(interval: number, inputFunc: IReadFunction, hooIHookFunctionkFunc: IHookFunction): void;
  listProcesses(): Promise<Array<IProcessStructure>>
 
  /* stupid
  readInt8(address: number, openHandle: number): Promise<number>;
  readUInt8(address: number, openHandle: number): Promise<number>;
  readInt16(address: number, openHandle: number): Promise<number>;
  readUInt16(address: number, openHandle: number): Promise<number>;
  readInt32(address: number, openHandle: number): Promise<number>;
  readUInt32(address: number, openHandle: number): Promise<number>;
  */
};

export interface IReadFunction {
  (): Promise<any>;
};

export interface IHookFunction {
  (output: any): void;
};

export interface IProcessStructure {
  processName: string,
  pid: number,
}
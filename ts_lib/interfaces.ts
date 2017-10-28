import { EProcessAccessRights } from './enums';

export interface IManipulateMyMemory {
  selectProcess(access: EProcessAccessRights, inheritBoolean: boolean, processName: string): any;
  unselectProcess(openHandle: Number): Promise<boolean>;
  //findWindow(className: string|null, windowName: string|null): Promise<number>;
  read(address: number, size: number, openHandle: number): Promise<Buffer>;
  write(address: number, size: number, value: Buffer, openHandle: number): Promise<number>;
  pointerAddress(address: number, pointers: Array<number>, size: number, openHandle: number): Promise<number>;
  monitor(interval: number, inputFunc: IReadFunction, hooIHookFunctionkFunc: Function): void;
 
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
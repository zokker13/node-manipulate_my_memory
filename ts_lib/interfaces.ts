import { EProcessAccessRights } from './enums';

export interface IManipulateMyMemory {
  selectProcess(access: EProcessAccessRights, inheritBoolean: boolean, processName: string): any;
}
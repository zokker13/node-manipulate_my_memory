import * as os from 'os';

import * as _ from 'underscore';
import * as Debug from 'debug';

import { EProcessAccessRights } from './../enums';
import { IManipulateMyMemory} from './../interfaces';

const debug = Debug('manipulate_my_memory:win32:manipulate_my_memory');

class ManipulateMyMemoryWin32 implements IManipulateMyMemory {

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

}
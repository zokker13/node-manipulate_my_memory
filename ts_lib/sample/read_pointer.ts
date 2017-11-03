import * as bigInt from 'big-integer';

import { MMMWin32 } from './../index';
import {EProcessAccessRights} from './../enums';
const win32 = new MMMWin32('SC2_x64.exe');

async function start() {

  const procs = await win32.listProcesses();
  const nicer = procs.filter((proc) => {
    return proc.processName === 'SC2_x64.exe';
  })

  nicer.forEach((nice) => {
    console.dir(nice)
  })

  const handle = await win32.selectProcess(EProcessAccessRights.PROCESS_VM_OPERATION | EProcessAccessRights.PROCESS_VM_READ, false);
  console.log(`Handle: 0x${handle.toString(16)}`);

  const pointerAddr = await win32.pointerAddress(bigInt(0x7FF7D583B7C0), [bigInt(8)], 4);
  console.log(`Pointer: ${pointerAddr.toString(16)}`);
}

start();
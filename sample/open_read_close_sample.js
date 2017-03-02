const util = require('util');

const win32 = require('./../lib/win32');

const handle = win32.openProcessSync(win32.PROCESS_ACCESS_RIGHTS.PROCESS_VM_READ, false, 6692);
win32.readProcessMemory(handle, 0x14fb04, 4, (err, buf) => {

  if (!err) {
    console.log(buf);
    console.log(buf.length);
    console.log(buf.readInt32LE(0));
  } else {
    console.log('Shit: ', err);
  }

  win32.closeHandleSync(handle);
});

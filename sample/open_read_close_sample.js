const util = require('util');

const win32 = require('./../lib/win32');

const handle = win32.openProcessSync(
  win32.PROCESS_ACCESS_RIGHTS.PROCESS_VM_OPERATION
  | win32.PROCESS_ACCESS_RIGHTS.PROCESS_VM_WRITE
  , false
  , 7648
);


setInterval(() => {
  const myNum = Buffer.allocUnsafe(4);
  const numToWrite = parseInt(Math.random() * 200);
  myNum.writeInt32LE(numToWrite);
  win32.writeProcessMemorySync(handle, 0x30fc68, 4, myNum);

}, 100);

process.on('SIGINT', () => {
  console.log('bye');
  win32.closeHandleSync(handle);
  process.exit(0);
});

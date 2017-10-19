const ManipulateMyMemory = require('./../lib');
const mmm = new ManipulateMyMemory('Tutorial-x86_64.exe');  // Cheat Engine tutorial

const BASE = 0x1002caa70;

mmm.selectProcess(
  ManipulateMyMemory.winProcessAccessRights.PROCESS_VM_READ
  , false
)
.then((handle) => {
  console.log('Handle:', handle.toString('16'));
  return mmm.pointerAddress(BASE, [0x10,0x18,0x0,0x18]);
})
.then((add) => {
  console.log(`Looking at 0x${add.toString(16)}`);
  return mmm.unselectProcess();
});
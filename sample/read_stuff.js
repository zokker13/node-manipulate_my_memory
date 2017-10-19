const ManipulateMyMemory = require('./../lib');
const mmm = new ManipulateMyMemory('ac_client.exe');

const AMMO_ADDR = 0x0284A608;

mmm.selectProcess(
  ManipulateMyMemory.winProcessAccessRights.PROCESS_VM_READ
  | ManipulateMyMemory.winProcessAccessRights.PROCESS_VM_OPERATION
  | ManipulateMyMemory.winProcessAccessRights.PROCESS_VM_WRITE
  , false
)
.then((handle) => {
  console.log('Handle:', handle.toString('16'));
  return mmm.readInt32(AMMO_ADDR);
})
.then((val) => {
  console.log('Read:', val);
  return mmm.writeInt32(AMMO_ADDR, val * 2);
})
.then(() => {
  console.log('Written now');
  return mmm.readInt32(AMMO_ADDR);
})
.then((val) => {
  console.log('Value now:', val);
  return mmm.unselectProcess();
});
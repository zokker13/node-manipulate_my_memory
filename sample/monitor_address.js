const _ = require('underscore');

const ManipulateMyMemory = require('./../lib');
const mmm = new ManipulateMyMemory('ac_client.exe');

// ac_client.exe+109B74
const BASE = 0x509b74;  // Damn, need to get module addresses :(
const AMMO_PTRS = [0x374, 0x14, 0];

mmm.selectProcess(
  ManipulateMyMemory.winProcessAccessRights.PROCESS_VM_READ
  , false
)
.then(() => {

  return mmm.pointerAddress(BASE, AMMO_PTRS);
})
.then((address) => {
  
  const readFunc = mmm.readInt32.bind(mmm, address);

  // Shouldn do it too often since it will become expensive fast
  mmm.monitor(100, readFunc, (err, value) => {
    console.log('err:', err);
    console.log('value:', value);
  });
});
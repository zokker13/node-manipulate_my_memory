const ManipulateMyMemory = require('./../lib/win32/mmm');
const mmm = new ManipulateMyMemory();

const AMMO_ADDR = 0x00E7A608;

return mmm.findWindow(null, 'AssaultCube')
  .then((handle) => {
    
    if (!handle) {
      console.log('Couldnt find window. Bye');
    }

    return mmm.getWindowThreadProcessId(handle);
    
  })
  .then((ids) => {
    console.log('ids', ids);

    return mmm.openProcess(
      ManipulateMyMemory.processAccessRights.PROCESS_VM_READ
      , false
      , ids.processId);
  })
  .then((openHandle) => {

    console.log('The handle:', openHandle);
    
    if (!openHandle) {
      console.log('couldnt get handle.');
    }

    return mmm.readInt32(AMMO_ADDR, openHandle);
  })
  .then((number) => {
    console.log('my number', number);

    return mmm.writeInt32(AMMO_ADDR, 1337)
  });
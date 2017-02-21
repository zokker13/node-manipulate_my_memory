var addon = require('bindings')('NativeExtension');

const handle = addon.openProcess(0x10, false, 5620);
console.log('Handle: ', handle);

const success = addon.closeHandle(handle);
console.log('Success: ', success);

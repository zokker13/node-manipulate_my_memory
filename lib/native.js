var addon = require('bindings')('NativeExtension');

const handle = addon.openProcess(0x10, false, 6028);
console.log('Handle: ', handle);

const result = addon.readProcessMemory(handle, 0x0127CB10, 4);
console.log('Result: ', result);

const success = addon.closeHandle(handle);
console.log('Success: ', success);

const async = require('async');

const addon = require('bindings')('NativeExtension');

const steps = [
  (cb) => {
    addon.openProcess(0x10, false, 6772, cb);
  },
  (handle, cb) => {
    addon.closeHandle(handle, cb);
  }
];

async.waterfall(steps, (err, success) => {
  if (err !== null) {
    console.log('Shit: ', err);
    return;
  }

  console.log('Good boy - success:', success);
});

//  const result = addon.readProcessMemory(handle, 0x017F8650, 4);

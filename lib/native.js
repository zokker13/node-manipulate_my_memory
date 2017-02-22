const async = require('async');

const addon = require('bindings')('NativeExtension');

const steps = [
  (cb) => {
    addon.openProcess(0x10, false, 6452, cb);
  },
  (handle, cb) => {
    console.log('Open ok:', handle);
    rpm(handle, cb);
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

function rpm(handle, cb) {

  console.log('rpm yo');
  addon.readProcessMemory(handle, 0x01818650, 4, (err, success, bytesRead, arr) => {
    if (err !== null) {
      console.log('Shit:', err);
      cb(err);

    }

    console.log('Success:', success);
    console.log('BytesRead:', bytesRead);
    console.log('Array: ', arr);
    cb(null, handle);
  });
}

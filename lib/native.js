const os = require('os');

const async = require('async');

const manipulateMyMemoryAddon = require('bindings')('ManipulateMyMemory');
let handle = null;

const steps = [
  /*(cb) => {
    manipulateMyMemoryAddon.openProcess(0x10, false, 6452, cb);
  },*/
  (cb) => {
    handle = manipulateMyMemoryAddon.openProcessSync(0x10, false, 6708);
    console.log('Handle: ', handle);

    setInterval(() => {
      console.log('beep');
    }, 100);
    //const mem = manipulateMyMemoryAddon.readProcessMemorySync(handle, 0x011CCB10, 1e10);

    manipulateMyMemoryAddon.readProcessMemory(handle, 0x011CCB10, 1e10, () => {
      cb(null);
    });
  },
  (cb) => {
    const success = manipulateMyMemoryAddon.closeHandleSync(handle, cb);
    console.log('Success: ', success);
    cb(null, success);
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
  manipulateMyMemoryAddon.readProcessMemory(handle, 0x011CCB10  , 4, (err, success, buf) => {
    if (err !== null) {
      console.log('Shit:', err);
      cb(err);

    }

    console.log('Success:', success);
    cb(null, handle);
  });
}

function readIntWrap(handle, cb) {
  readInt(handle, 0x011CCB10, 4, (err, number) => {
    console.log(number);
    cb(err, handle);
  });
}

function readUIntWrap(handle, cb) {
  readUInt(handle, 0x011CCB10, 4, (err, number) => {
    console.log(number);
    cb(err, handle);
  });
}

function readInt(handle, address, size, cb) {
  manipulateMyMemoryAddon.readProcessMemory(handle, address, size, (err, success, typed) => {

    const buffer = Buffer.from(typed);
    if (err !== null) {
      cb(err);
      return;
    }

    if (os.endianness() == 'LE') {
      cb(null, buffer.readIntLE(0, size));
    } else {
      cb(null, buffer.readIntBE(0, size));
    }
  });
}

function readUInt(handle, address, size, cb) {
  manipulateMyMemoryAddon.readProcessMemory(handle, address, size, (err, success, typed) => {

    const buffer = Buffer.from(typed);
    if (err !== null) {
      cb(err);
      return;
    }

    if (os.endianness() == 'LE') {
      cb(null, buffer.readUIntLE(0, size));
    } else {
      cb(null, buffer.readUIntBE(0, size));
    }
  });
}

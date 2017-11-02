const Mmm = require('./..');

const win32 = new Mmm.MMMWin32('SC2_x64.exe');
return win32.selectProcess(0x10, false)
  .then((handle) => {
    console.log(`Handle: 0x${handle.toString(16)}`);
    return win32.pointerAddress(0x7FF7D583B7C0, [8,0x20,0x758, 0x10, 0x140], 8);
  })
  .then((addr) => {
    console.log(addr);
  })
  .catch((err) => {
    console.error(err.stack)
  })
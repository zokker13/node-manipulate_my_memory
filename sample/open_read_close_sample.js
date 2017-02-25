const util = require('util');

const win32 = require('./../lib/win32');

const hwnd = win32.findWindowSync(null, 'Step 2');
console.log('HWND:', hwnd);

win32.getWindowThreadProcessId(hwnd, (err, ids) => {
  console.log('My ids:', ids);
});

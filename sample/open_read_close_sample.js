const win32 = require('./../lib/win32');

win32.findWindow(null, 'Step 2', (err, handle) => {
    console.log('err:', err);
    console.log('handle:', handle);
});


var addon = require('bindings')('NativeExtension');

console.log(addon.hello()); // 'world'

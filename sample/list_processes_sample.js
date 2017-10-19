const ManipulateMyMemory = require('./../lib');
const mmm = new ManipulateMyMemory('ac_client.exe');

return ManipulateMyMemory.listProcesses((err, procs) => {
  console.log('hi');
  console.log('err', err);
  console.log('procs', procs);
  console.log(procs.length);
});

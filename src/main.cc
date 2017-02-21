#include "windows_extension.h"

void Init(v8::Local<v8::Object> exports) {

  exports->Set(Nan::New("openProcess").ToLocalChecked(),
    Nan::New<v8::FunctionTemplate>(ExOpenProcess)->GetFunction());

  exports->Set(Nan::New("closeHandle").ToLocalChecked(),
    Nan::New<v8::FunctionTemplate>(ExCloseHandle)->GetFunction());

  exports->Set(Nan::New("readProcessMemory").ToLocalChecked(),
    Nan::New<v8::FunctionTemplate>(ExReadProcessMemory)->GetFunction());
}

NODE_MODULE(fuck, Init)

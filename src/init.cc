#include "get.h"
#include "set.h"

void init(Local<Object> exports) {
  exports->Set(Nan::New("get").ToLocalChecked(),
               Nan::New<v8::FunctionTemplate>(Get)->GetFunction());
  exports->Set(Nan::New("set").ToLocalChecked(),
               Nan::New<v8::FunctionTemplate>(Set)->GetFunction());
}

NODE_MODULE(tagging, init)

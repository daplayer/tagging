#include "get.h"
#include "set.h"
#include "library.h"

void init(Local<Object> exports) {
  Library::Initialize(exports);

  exports->Set(Nan::New("set").ToLocalChecked(),
               Nan::New<v8::FunctionTemplate>(Set)->GetFunction());
}

NODE_MODULE(tagging, init)

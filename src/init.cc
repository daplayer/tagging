#include "get.h"
#include "set.h"

void init(Local<Object> exports) {
  NODE_SET_METHOD(exports, "get", Get);
  NODE_SET_METHOD(exports, "set", Set);
}

NODE_MODULE(tagging, init)

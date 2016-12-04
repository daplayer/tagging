#ifndef LIBRARY_H
#define LIBRARY_H

#include "utils.h"

using v8::Array;
using v8::FunctionTemplate;
using v8::Persistent;
using v8::Function;
using v8::Context;

class Library : public Nan::ObjectWrap {
  public:
    explicit Library();
    explicit Library(Local<Object> hash);

    void setArtists(Local<Object> object);
    void setSingles(Local<Object> array);

    void AddTrack(std::string artist, std::string album, Local<Object> record);
    void AddSingle(std::string artist, Local<Object> record);

    static void Initialize(Nan::ADDON_REGISTER_FUNCTION_ARGS_TYPE target);
    static void NewInstance(const Nan::FunctionCallbackInfo<Value>& args);

    static inline Nan::Persistent<Function> & constructor() {
      static Nan::Persistent<Function> my_constructor;
      return my_constructor;
    }

  private:
    Nan::Persistent<Object> internal_hash;
    Local<Object> artists();
    Local<Object> singles();

    void AddArtist(std::string name);

    ~Library();

    static void New(const Nan::FunctionCallbackInfo<Value>& args);
    static void Get(const Nan::FunctionCallbackInfo<Value>& args);
};

#endif

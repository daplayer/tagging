#include "set.h"
#include "image_file.h"

void Set(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();

  // Raise if the user doesn't supply enough arguments
  if (args.Length() < 2) {
    throwException(isolate, "Wrong number of arguments");
    return;
  }

  const char *file_name  = CString(isolate, args[0]);
  const char *location   = expand(file_name);
  const char extension[] = {file_name[strlen(file_name) - 3],
                            file_name[strlen(file_name) - 2],
                            file_name[strlen(file_name) - 1]};
  Local<Object> hash     = args[1]->ToObject();


  // Raise if the audio file doesn't exist
  if (!exist(location)) {
    throwException(isolate, "The audio file doesn't exist");
    return;
  }

  TagLib::FileRef audioFile(location);

  // Set the basic text attributes like title, etc.
  if (hash->Has(string(isolate, "title")))
    audioFile.tag()->setTitle(CString(isolate, hash, "title"));
  if (hash->Has(string(isolate, "album")))
    audioFile.tag()->setAlbum(CString(isolate, hash, "album"));
  if (hash->Has(string(isolate, "artist")))
    audioFile.tag()->setArtist(CString(isolate, hash, "artist"));
  if (hash->Has(string(isolate, "genre")))
    audioFile.tag()->setGenre(CString(isolate, hash, "genre"));
  if (hash->Has(string(isolate, "track")))
    audioFile.tag()->setTrack(hash->Get(string(isolate, "track"))->NumberValue());

  audioFile.save();

  // Set the attached picture
  if (hash->Has(string(isolate, "icon"))) {
    const char *img_name     = CString(isolate, hash, "icon");
    const char *img_location = expand(img_name);

    if (!exist(img_location)) {
      throwException(isolate, "The given artwork path doesn't exist");
      return;
    }

    ImageFile imageFile(img_location);
    TagLib::MPEG::File mpegFile(location);

    TagLib::ID3v2::Tag *tag = mpegFile.ID3v2Tag(true);
    TagLib::ID3v2::AttachedPictureFrame *frame = new TagLib::ID3v2::AttachedPictureFrame;

    if (strcmp("png", extension) == 0)
      frame->setMimeType("image/png");
    else
      frame->setMimeType("image/jpeg");

    frame->setPicture(imageFile.data());

    tag->addFrame(frame);

    mpegFile.save();
  }

  args.GetReturnValue().Set(true);
}


#include "set.h"
#include "image_file.h"

void Set(const Nan::FunctionCallbackInfo<Value>& args) {
  // Raise if the user doesn't supply enough arguments
  if (args.Length() < 2)
    return Nan::ThrowRangeError("Wrong number of arguments");

  Nan::Utf8String v8_file_name(args[0]);

  const char *file_name  = *v8_file_name;
  const char *location   = expand(file_name);
  const char extension[] = {file_name[strlen(file_name) - 3],
                            file_name[strlen(file_name) - 2],
                            file_name[strlen(file_name) - 1]};
  Local<Object> hash     = args[1]->ToObject();


  // Raise if the audio file doesn't exist
  if (!exist(location))
    return Nan::ThrowError("The audio file doesn't exist");

  TagLib::FileRef audioFile(location);

  // Set the basic text attributes like title, etc.
  if (hash->Has(string("title")))
    audioFile.tag()->setTitle(CString(hash, "title"));
  if (hash->Has(string("album")))
    audioFile.tag()->setAlbum(CString(hash, "album"));
  if (hash->Has(string("artist")))
    audioFile.tag()->setArtist(CString(hash, "artist"));
  if (hash->Has(string("genre")))
    audioFile.tag()->setGenre(CString(hash, "genre"));
  if (hash->Has(string("track")))
    audioFile.tag()->setTrack(hash->Get(string("track"))->NumberValue());

  audioFile.save();

  // Set the attached picture
  if (hash->Has(string("icon"))) {
    const char *img_name     = CString(hash, "icon");
    const char *img_location = expand(img_name);

    if (!exist(img_location)) {
      Nan::ThrowError("The given artwork path doesn't exist");
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


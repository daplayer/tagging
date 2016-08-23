#include "get.h"

void Get(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate     = args.GetIsolate();
  Local<Object> record = Object::New(isolate);

  // Ensure that we have at least the file's location
  if (args.Length() == 0) {
    throwException(isolate, "Wrong number of arguments");
    return;
  }

  char *file_name    = CString(isolate, args[0]);
  char *location     = expand(file_name);
  char *cover_folder = nullptr;

  // Raise if the file actually doesn't exist
  if (!exist(location)) {
    throwException(isolate, "The audio file doesn't exist");
    return;
  }

  if (args.Length() > 1)
    cover_folder = CString(isolate, args[1]);

  TagLib::FileRef f(location);
  TagLib::Tag *tag = f.tag();

  char *title = (char*)malloc(strlen(location) * sizeof(char));

  TagLib::String tartist = tag->artist();
  TagLib::String talbum  = tag->album();
  TagLib::String tgenre  = tag->genre();

  // Set the default title based on the file's location
  // if there's no tags for the file.
  if (tag->title().length() > 0) {
    strcpy(title, tag->title().toCString());
  } else {
    char *slash = strchr(location, '/');
    char *dot   = strchr(location, '.');

    size_t s, d, i = 0;

    // Find the position of the last slash (i.e. where the file name begins)
    while (slash != NULL) {
      s     = slash - location;
      slash = strchr(slash+1, '/');
    }

    // Find the position of the last dot (i.e. the extension)
    while (dot != NULL) {
      d   = dot - location;
      dot = strchr(dot+1, '.');
    }

    // Copy the actual file name
    for (s++; s < d; s++, i++)
      title[i] = location[s];
    title[i++] = '\0';
  }

  record->Set(string(isolate, "title"),  string(isolate, title));
  record->Set(string(isolate, "artist"), string(isolate, tartist));
  record->Set(string(isolate, "album"),  string(isolate, talbum));
  record->Set(string(isolate, "genre"),  string(isolate, tgenre));
  record->Set(string(isolate, "id"),     string(isolate, location));
  record->Set(string(isolate, "track"),  Number::New(isolate, tag->track()));

  TagLib::MPEG::File mp3_file(location);
  TagLib::ID3v2::Tag *mp3_tag = mp3_file.ID3v2Tag(true);
  TagLib::ID3v2::FrameList pictures = mp3_tag->frameList("APIC");
  TagLib::ID3v2::AttachedPictureFrame *picture_frame;

  if (args.Length() > 1) {
    const char *album  = talbum.toCString();
    const char *artist = tartist.toCString();

    // //   1 ('/')
    // // + 3 (" - ")
    // // + 1 ('.')
    // // + 3 ("png" or "jpg")
    // // + 1 ('\0')
    // // = 9
    char img_path[strlen(cover_folder) + strlen(artist)
                + strlen(strlen(album) ? album : title) + 9];

    strcpy(img_path, cover_folder);
    strcat(img_path, "/");

    int position = strlen(cover_folder) + 1;

    // Copy the name of the artist making sure that we are
    // only copying alpha-numeric chars in order to avoid
    // any filesystem errors creating the file.
    if (strlen(artist) > 0) {
      for (size_t i = 0; i < strlen(artist); i++) {
        if (isalnum(artist[i]) || artist[i] == ' ')
          img_path[position++] = artist[i];
      }

      img_path[position++] = ' ';
      img_path[position++] = '-';
      img_path[position++] = ' ';
    }

    // Ditto `artist` but for the album or the title variable;
    // we only want the alpha-numeric chars.
    if (strlen(album) > 0)
      for (size_t i = 0; i < strlen(album); i++) {
        if (isalnum(album[i]) || album[i] == ' ')
          img_path[position++] = album[i];
      }
    else
      for (size_t i = 0; i < strlen(title); i++) {
        if (isalnum(title[i]) || title[i] == ' ')
          img_path[position++] = title[i];
      }

    img_path[position] = '\0';

    if (image_exist(img_path)) {
      record->Set(string(isolate, "icon"), string(isolate, img_path));
    } else if (!pictures.isEmpty()) {
      for (TagLib::ID3v2::FrameList::ConstIterator it = pictures.begin(); it != pictures.end(); it++) {
        picture_frame    = static_cast<TagLib::ID3v2::AttachedPictureFrame *> (*it);
        const char *mime = picture_frame->mimeType().toCString();

        strcat(img_path, ".");

        if (strcmp(mime, "image/png") == 0)
          strcat(img_path, "png");
        else
          strcat(img_path, "jpg");

        if (!exist(img_path)) {
          size_t image_size = picture_frame->picture().size();
          const char *data  = picture_frame->picture().data();
          FILE *cover_file  = fopen(img_path, "wb");

          fwrite(data, image_size, 1, cover_file);
          fclose(cover_file);
        }

        record->Set(string(isolate, "icon"), string(isolate, img_path));
      }
    }
  }

  record->Set(string(isolate, "duration"), Number::New(isolate, mp3_file.audioProperties()->length()));

  args.GetReturnValue().Set(record);
}

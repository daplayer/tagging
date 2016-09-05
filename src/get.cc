#include "get.h"

void Get(const Nan::FunctionCallbackInfo<Value>& args) {
  Local<Object> record = Nan::New<Object>();

  // Ensure that we have at least the file's location
  if (args.Length() == 0)
    return Nan::ThrowRangeError("Wrong number of arguments");

  Nan::Utf8String v8_file_name(args[0]);

  char *file_name = *v8_file_name;
  char *location  = expand(file_name);

  // Raise if the file actually doesn't exist
  if (!exist(location))
    return Nan::ThrowError("The audio file doesn't exist");

  TagLib::FileRef f(location);
  TagLib::Tag *tag = f.tag();
  TagLib::AudioProperties *properties = f.audioProperties();

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

  record->Set(string("title"),    string(title));
  record->Set(string("artist"),   string(tartist.toCString()));
  record->Set(string("album"),    string(talbum.toCString()));
  record->Set(string("genre"),    string(tgenre.toCString()));
  record->Set(string("id"),       string(location));
  record->Set(string("track"),    Nan::New(tag->track()));
  record->Set(string("duration"), Nan::New(properties->length()));

  if (args.Length() > 1) {
    Nan::Utf8String v8_cover_folder(args[1]);
    char *cover_folder = *v8_cover_folder;
    const char *album  = talbum.toCString();
    const char *artist = tartist.toCString();

    //   1 ('/')
    // + 3 (" - ")
    // + 1 ('.')
    // + 3 ("png" or "jpg")
    // + 1 ('\0')
    // = 9
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
      record->Set(string("icon"), string(img_path));
    } else {
      char extension[] = {file_name[strlen(file_name - 3)],
                          file_name[strlen(file_name - 2)],
                          file_name[strlen(file_name - 1)], '\0'
                         };

      if (strcmp(extension, "mp3"))
        mp3Picture(location, img_path, record);
      else if (strcmp(extension, "m4a"))
        mp4Picture(location, img_path, record);
    }
  }

  args.GetReturnValue().Set(record);
}

void mp3Picture(char* location, char *img_path, Local<Object> record) {
  TagLib::MPEG::File mp3_file(location);
  TagLib::ID3v2::Tag *mp3_tag = mp3_file.ID3v2Tag(true);
  TagLib::ID3v2::FrameList pictures = mp3_tag->frameList("APIC");

  if (pictures.isEmpty())
    return;

  TagLib::ID3v2::AttachedPictureFrame *picture_frame;

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

    record->Set(string("icon"), string(img_path));
  }
}

void mp4Picture(char *location, char *img_path, Local<Object> record) {
  TagLib::MP4::File mp4_file(location);
  TagLib::MP4::Tag *mp4_tag = mp4_file.tag();
  TagLib::MP4::ItemListMap items_list_map = mp4_tag->itemListMap();
  TagLib::MP4::Item cover_item = items_list_map["covr"];
  TagLib::MP4::CoverArtList covert_art_list = cover_item.toCoverArtList();

  if (covert_art_list.isEmpty())
    return;

  TagLib::MP4::CoverArt cover_art = covert_art_list.front();

  strcat(img_path, ".jpg");

  if (!exist(img_path)) {
    size_t image_size = cover_art.data().size();
    const char *data  = cover_art.data().data();
    FILE *cover_file  = fopen(img_path, "wb");

    fwrite(data, image_size, 1, cover_file);
    fclose(cover_file);
  }

  record->Set(string("icon"), string(img_path));
}

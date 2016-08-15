#ifndef IMAGE_FILE_H
#define IMAGE_FILE_H

#include <taglib/tfile.h>

class ImageFile : public TagLib::File {
  public:
    ImageFile(const char *file);
    TagLib::ByteVector data();

  private:
    virtual TagLib::Tag *tag() const;
    virtual TagLib::AudioProperties *audioProperties() const;
    virtual bool save();
};

#endif

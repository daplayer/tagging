# DaPlayer Tagging Library

This library is used inside DaPlayer to read or set tags of audio files.
This is mostly just a wrapper around [TagLib](http://taglib.github.io).

## Installation

This library is not released directly on NPM, you should rather check
it out locally or reference a GitHub URL like a branch or a tar-ball:

~~~json
{
  "daplayer-tagging": "https://github.com/daplayer/daplayer-tagging"
}
~~~

Then run `npm install` to download it.

## Usage

This library reads and set tags in a synchronous way. It comes with two
handy methods: `get` and `set`.

### Getting audio tags

Reading audio tags is fairly easy, you just need to rely on the `get` method
and pass it a path to an audio file, like:

~~~javascript
const Tagging = require('daplayer-tagging');

console.log(Tagging.get('/path/to/audio/file.mp3'));
// { title: 'Maliblue',
//  artist: 'Darius',
//  album: 'Velour',
//  genre: 'Electronic',
//  id: '/path/to/audio/file.mp3',
//  track: 4,
//  duration: 259 }
~~~

The `id` field actually refers to the given path; this is a cheap way to have
an unique identifier for each of your record.

If you pass a relative path to this method, an absolute path will be computed
based on the current execution location.

It is also possible to get the cover image of an audio file passing a path to
a folder which will contain it. Then the `icon` field of the returned object
will be filled with the path to the picture.

~~~javascript
console.log(Tagging.get('/path/to/audio/file.mp3', '/path/to/cover/folder'));
// { title: 'Maliblue',
//  artist: 'Darius',
//  album: 'Velour',
//  genre: 'Electronic',
//  id: '/path/to/audio/file.mp3',
//  track: 4,
//  icon: '/path/to/cover/folder/Darius - Velour.jpg',
//  duration: 259 }
~~~

Actually the name of the file is based on the name of the artist and the album
or if it is not present, the title.

It is not possible to have access to any sort of Base64 string of the picture's
data since this is taking too much memory-consuming. You can fork this project
and change this behavior if storing cover files inside a folder is not what you
want.

The advantage of this technique is that once an audio file has been read once,
the cover file won't be computed again so it will be faster to extract tags.

### Setting audio tags

You can also define audio tags through this library very easily with the `set`
method. You just need to pass it a path to the audio file and an object with
the fields you want to define.

You don't need to know the underlying name of the frames, you can use the same
as the one returned by `get` to erase them:

~~~javascript
const Tagging = require('daplayer-tagging');

Tagging.set('/path/to/audio/file.mp3', {
  title:  'Born In Winter',
  artist: 'Gojira',
  album:  'L\'enfant sauvage',
  genre:  'Death Metal',
  track:  10,
  icon:   '/path/to/image.jpg'
});
~~~

## License

This project is a wrapper around the [TagLib](http://taglib.github.io) library which
is available either under the terms of the LGPL or the MPL license.

This project is available under the terms of the MPL license. You can check out the
LICENSE file for further information.

# DaPlayer Tagging Library

This library is used inside DaPlayer to read or set tags of audio files.
This is mostly just a wrapper around [TagLib](http://taglib.github.io).

## Installation

This library is not released directly on NPM, you should rather check
it out locally or reference a GitHub URL like a branch or a tar-ball:

~~~json
{
  "daplayer-tagging": "https://github.com/daplayer/tagging"
}
~~~

Then run `npm install` to download it.

## Usage

This library reads and set tags in a synchronous way. It comes with two
handy methods: `get` and `set`.

**Important note**: This project is usable in any of your project but since it's
mainly intented to be used inside DaPlayer, it can only deal with absolute paths
so don't expect it to automatically expand paths such as `../folder/audio.mp3`.

### Getting audio tags

Reading audio tags is fairly easy but the returned result may not be what you
would expect. Actually, the `get` method takes an array of files and will
automatically build a representation of your library. For example:

~~~javascript
const Tagging = require('daplayer-tagging');

Tagging.get([
  '/path/to/artist/album/track.mp3',
  '/path/to/artist/single.mp3',
  '/path/to/lonely_single.mp3'
]);
~~~

Will produce:

~~~javascript
{
  artists: {
    darius: {
      name: 'Darius',
      albums: {
        Velour: [
          {
            title: 'Maliblue',
            artist: 'Darius',
            genre: 'French House',
            track: 4,
            id: '/path/to/artist/album/track.mp3',
            // ...
          }
        ]
      },
      singles: [{title: 'Pyor', /* ... */}]
    }
  },
  singles: [
    { title: 'Pyor', artist: 'Darius', /* ... */ },
    { title: 'Cherokee - Take Care of You', /* ... */ }
  ]
}
~~~

Here is a couple things to keep in mind dealing with this result:

* Each artist is stored under a key which is a lower-cased version of their
  name. This avoids having two entries for the same artist if their musics
  are tagged with a different case (e.g. "Bring Me *the* Horizon" and "Bring
  Me *The* Horizon").
* The singles are both attached to their artist and stored under the `singles`
  key as DaPlayer treats singles as a single collection.

The `id` field actually refers to the given path; this is a cheap way to have
an unique identifier for each record.

#### Getting cover arts

It is also possible to get the cover arts of audio files passing a path to
a folder which will contain the images. Then the `icon` attribute of each
record will be filled automatically. For instance:

~~~javascript
Tagging.get(files, '/path/to/covers/folder');
~~~

Actually the name of the image file is based on the name of the artist and the
album's title or the track's title if the latter is not present.

It is not possible to have access to any sort of Base64 string of the picture's
data since this would take too much memory. You can fork this project and change
this behavior if storing cover files inside a folder is not what you want.

The advantage of this technique is that once an audio file has been read once,
the cover file won't be computed again so it will be faster to extract tags.

#### Getting feedback

Finally, the `get` method accepts a third argument as a callback to know the
current progression processing all the given files as this may be long when
the array gets large. For example:

~~~javascript
Tagging.get(files, cover_folder, function(index, total) {
  console.log((index / total) * 100);
});
~~~

#### Getting better performances

Processing a huge folder may take several seconds so a good approach to save
resources and time is to cache the returned hash once it has been processed
(either in memory or on the file system).

The problem is that the library isn't up-to-date when new files are introduced
in the collection so the `get` method can take an extra argument to specify
an existing hash. For instance:

~~~javascript
// The `library` variable doesn't get copied, it is barely modified.
Tagging.get(new_files, cover_folder, library);

// Or if you want to get some feedback processing the new files.
Tagging.get(new_files, cover_folder, library, function(index, total) {
  console.log("Already processed ", (index / total) * 100, "% of the new files.");
});
~~~

Removing files from the library may be a bit more difficult. You need to look
under the `singles` key, checking against the `id` field of each record or
look inside each artist's album's tracks if the file is not a single.

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

## Contributing

Contributing to this project is fairly easy. To set it up, you are done with the
following steps:

~~~
$ git clone https://github.com/daplayer/tagging
$ cd daplayer-tagging
$ npm install
$ make
~~~

`make` will actually compile the project and run all the tests.

## License

This project is a wrapper around the [TagLib](http://taglib.github.io) library which
is available either under the terms of the LGPL or the MPL license.

This project is available under the terms of the MPL license. You can check out the
LICENSE file for further information.

require('./test_helper');

describe('Tagging', () => {
  describe('#set', () => {
    describe('prototype', () => {
      beforeEach(() => {
        return helpers.cp('raw.mp3', 'to_tag.mp3');
      });

      afterEach(() => {
        helpers.rm('to_tag.mp3');
      });

      it('should take two arguments', () => {
        assert.throws(() => {
          Tagging.set();
        }, /Wrong number of arguments/);

        assert.throws(() => {
          Tagging.set('path/to/file');
        }, /Wrong number of arguments/);
      });

      it('should accept absolute paths', () => {
        Tagging.set(path.join(__dirname, 'fixtures/to_tag.mp3'), {});
      });
    });

    describe('behavior', () => {
      beforeEach(() => {
        return helpers.cp('raw.mp3', 'to_tag.mp3');
      });

      afterEach(() => {
        helpers.rm('to_tag.mp3');
      });

      it('should raise if the audio file does not exist', () => {
        assert.throws(() => {
          Tagging.set('foo/bar', {});
        }, /The audio file doesn't exist/);
      });

      it('should set the passed tag ', () => {
        var defined_tags = {
          title:  'Take Care of You',
          artist: 'Cherokee',
          album:  'Take Care of You',
          genre:  'French House',
          track:  1
        };

        Tagging.set(helpers.fixture('to_tag.mp3'), defined_tags);

        var library = new Tagging.Library();

        library.get([helpers.fixture('to_tag.mp3')]);

        var read_tags = library.artists.cherokee.albums['Take Care of You'][0];

        assert.equal(read_tags.title,  defined_tags.title);
        assert.equal(read_tags.artist, defined_tags.artist);
        assert.equal(read_tags.track,  defined_tags.track);
        assert.equal(read_tags.genre,  defined_tags.genre);
      });
    })

    describe('cover file', () => {
      beforeEach(() => {
        return helpers.cp('raw.mp3', 'to_tag.mp3').then(() => {
          helpers.createCoverFolder();
        });
      });

      afterEach(() => {
        helpers.rm('to_tag.mp3');
        helpers.clearCoverFolder();
      });

      it('should pick the given cover file', () => {
        var library = new Tagging.Library();

        Tagging.set('test/fixtures/to_tag.mp3', {
          title: 'Take Care of You',
          artist: 'Cherokee',
          icon: helpers.fixture('cover.jpg')
        });

        library.get([helpers.fixture('to_tag.mp3')], cover_folder);

        try {
          fs.accessSync(helpers.fixture('covers/227514130.jpg'));
        } catch (e) {
          throw new assert.AssertionError({
            message: 'The cover file should exist'
          });
        }
      });

      it('should not raise if an empty string is given as icon', () => {
        Tagging.set(helpers.fixture('to_tag.mp3'), {
          icon: ''
        });
      });

      it('should raise if the cover file does not exist', () => {
        assert.throws(() => {
          Tagging.set(helpers.fixture('to_tag.mp3'), {
            icon: 'i_do_not_exist'
          });
        }, /The given artwork path doesn't exist/);
      });
    });
  });
});

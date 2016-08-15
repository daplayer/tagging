global.Tagging = require('../build/Release/tagging.node');
global.assert  = require('assert');
global.path    = require('path');
global.fs      = require('fs');

global.fixtures_folder = path.join(__dirname, 'fixtures');
global.cover_folder    = path.join(fixtures_folder, 'covers');

global.helpers = {
  createCoverFolder: () => {
    try {
      fs.accessSync(cover_folder, fs.F_OK);
    } catch (e) {
      fs.mkdirSync(cover_folder);
    }
  },
  clearCoverFolder: () => {
    fs.readdirSync(cover_folder).forEach((file, index, files) => {
      if (file != 'Bakermat - Strandfeest.jpg')
        fs.unlinkSync(path.join(cover_folder, file));
    });
  },
  fixture: (file) => {
    return path.join(fixtures_folder, file);
  },
  cp: (file, destination) => {
    return new Promise((resolve, reject) => {
      var read_stream  = fs.createReadStream(helpers.fixture(file));
      var write_stream = fs.createWriteStream(helpers.fixture(destination));

      read_stream.on('close', () => {
        resolve();
      });

      read_stream.on('error', () => {
        reject();
      });

      read_stream.pipe(write_stream);
    });
  },
  rm: (file) => {
    fs.unlinkSync(helpers.fixture(file));
  }
}

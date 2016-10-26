{
  "targets": [
    {
      "target_name": "tagging",
      "sources": [ "src/get.cc", "src/image_file.cc", "src/init.cc", "src/library.cc", "src/set.cc", "src/utils.cc" ],
      "include_dirs": ['deps/include', '<!(node -e "require(\'nan\')")'],
      "conditions": [
        ['OS=="mac"', {
          "libraries": ['../deps/lib/libtag-darwin.a'],
        }],
        ['OS=="linux"', {
          "libraries": ['../deps/lib/libtag-linux-x64.so']
        }]
      ]
    }
  ]
}

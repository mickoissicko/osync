# OSync
OSync, or Obsidian Sync, is a free client to sync files between different computers running Obsidian.

--> Get [Obsidian](https://obsidian.md)

# Usage
`osync` (in daemon mode) may be run on a home server. Then you can use `osync` on your computers. Whilst you are making notes `osync` daemon fetches the files and stores them on your server.

NOTE: To use `osync` in daemon mode, just run it with the argument `--server` like so:

> `./osync --server`

Then, whilst `osync` is running, you can type `fetch` to get the latest notes. They will be stored on your computer, and that's pretty much it!

As a precaution, run `sync` before leaving your computer as you may close `osync` before it has time to send the files.

# Building
**Windows requirements**
* GCC configured + installed on Windows
* [Make](https://gnuwin32.sourceforge.net/packages/make.htm) installed

**MacOS requirementst**
* [Make](https://formulae.brew.sh/formula/make) installed

**POSIX requirements**
* [Make](https://www.gnu.org/software/make/) installed

Change your directory to `build/` and run `make`.

One file will be generated under the name `osync`.

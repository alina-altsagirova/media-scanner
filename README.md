# Media-scanner

## Build and Install

```bash
git clone https://github.com/alina-altsagirova/media-scanner.git
cd media-scanner
make all
```

## Usage

The application accepts arguments: `{number} {unit} {path}`

Units:
* `ms` - milliseconds
* `s` - seconds
* `m` - minutes
* `d` - days
* `w` - weeks

Example:
```bash
./media_scunner 5 m ~/Documents
```

## Makefile Targets

* `make all` — Build binary
* `make clean` — Remove build files
* `make rebuild` —  Rebuild project
* `make open` — Open the generated HTTP report in the default browser.
* `make clang` —  Apply clang-format to all source files.
* `make style` —  Check source code for style

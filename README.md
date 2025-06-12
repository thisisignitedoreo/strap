# libstrap
A bunch of useful stuff for casual C coding

## Overview
Library consists of modules:

- `array.h`: dynamic arrays
- `linked_list.h`: linked lists
- `hashmap.h`: hashmaps
- `fileio.h`: files and directories i/o
- `arena.h`: arena allocators
- `stringview.h`: stringview implementation
- `shell.h`: program i/o
- `utils.h`: utility functions
- `json_write.h`: JSON serialization
- `json_read.h`: JSON parsing

Check sources for more info

## Build
> [!WARNING]
> The old `build.sh` script is very rudamentary and deprecated.
> The use of zig build system is recommended.

```console
$ zig build [-Dtarget=x12_34-amogos-jlibc]
```

To add this library to your project use:
```console
$ zig fetch --save git+https://github.com/thisisignitedoreo/strap.git
```

And in your `build.zig`:
```zig
const strap = b.dependency("strap", .{
    .target = target,
    .optimize = optimize,
});
// ...
exe.linkLibrary(strap.artifact("strap"));
```
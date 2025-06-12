const std = @import("std");

pub fn build(b: *std.Build) void {
    const target = b.standardTargetOptions(.{});
    const optimize = b.standardOptimizeOption(.{});

    const strap = b.addModule("strap", .{
        .target = target,
        .optimize = optimize,
    });

    strap.addIncludePath(b.path("src"));
    strap.addCSourceFiles(.{
        .root = b.path("src"),
        .files = &.{
            "arena.c",
            "array.c",
            "fileio.c",
            "hashmap.c",
            "json_read.c",
            "json_write.c",
            "linked_list.c",
            "shell.c",
            "stringview.c",
            "utils.c",
        },
        .flags = &.{
            "-std=gnu99", "-pedantic",
        },
    });

    const lib = b.addLibrary(.{
        .name = "strap",
        .linkage = .static,
        .root_module = strap,
    });

    lib.linkLibC();

    b.installArtifact(lib);
    b.installFile("src/arena.h", "include/arena.h");
    b.installFile("src/array.h", "include/array.h");
    b.installFile("src/fileio.h", "include/fileio.h");
    b.installFile("src/hashmap.h", "include/hashmap.h");
    b.installFile("src/json_read.h", "include/json_read.h");
    b.installFile("src/json_write.h", "include/json_write.h");
    b.installFile("src/linked_list.h", "include/linked_list.h");
    b.installFile("src/shell.h", "include/shell.h");
    b.installFile("src/stringview.h", "include/stringview.h");
    b.installFile("src/utils.h", "include/utils.h");
}

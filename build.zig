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
    // lib.installHeader(b.path("src/arena.h"), "arena.h");
    // lib.installHeader(b.path("src/array.h"), "array.h");
    // lib.installHeader(b.path("src/fileio.h"), "fileio.h");
    // lib.installHeader(b.path("src/hashmap.h"), "hashmap.h");
    // lib.installHeader(b.path("src/json_read.h"), "json_read.h");
    // lib.installHeader(b.path("src/json_write.h"), "json_write.h");
    // lib.installHeader(b.path("src/linked_list.h"), "linked_list.h");
    // lib.installHeader(b.path("src/shell.h"), "shell.h");
    // lib.installHeader(b.path("src/stringview.h"), "stringview.h");
    // lib.installHeader(b.path("src/utils.h"), "utils.h");
    lib.installHeadersDirectory(b.path("src"), "", .{});

    b.installArtifact(lib);
}

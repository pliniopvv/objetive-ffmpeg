set_project("ExtratorFrameFFmpeg")
set_version("1.0.0")

add_rules("mode.debug", "mode.release")
add_requires("ffmpeg")

target("hwffmpeg")
    set_kind("binary")
    add_files("src/*.cpp")
    add_packages("ffmpeg")

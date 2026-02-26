add_rules("mode.debug", "mode.release")

add_repositories("iceblcokmc https://github.com/IceBlcokMC/xmake-repo.git")

add_requires("endstone 0.11.1")
add_requires("v8kit 8c0dba703ab9973178bf92365c75b56bc96a26d5")

add_requires(
    "nlohmann_json 3.11.3",
    "magic_enum 0.9.7"
)

if is_plat("windows") then
    if not has_config("vs_runtime") then
        set_runtimes("MD")
    end
end

target("BlueIceEngine")
    set_kind("shared")
    set_languages("cxx20")
    set_symbols("debug")

    add_headerfiles("src/**.h")
    add_includedirs("src")
    add_files("src/**.cc")

    add_packages("endstone", "v8kit")
    add_packages("nlohmann_json", "magic_enum")

    if is_plat("windows") then
        add_cxflags("/utf-8", "/W4", "/sdl")
        add_cxxflags("/Zc:__cplusplus", {force = true})
        add_cxxflags("/Zc:preprocessor");
    elseif is_plat("linux") then
        add_cxflags("-fPIC", "-stdlib=libc++", {force = true})
        add_syslinks("dl", "pthread")
    end

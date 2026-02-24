add_rules("mode.debug", "mode.release")

add_repositories("iceblcokmc https://github.com/IceBlcokMC/xmake-repo.git")

add_requires("endstone 0.11.1")
add_requires("v8kit a9b898d69b37e5b7081c7a235b08793af568e625")

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
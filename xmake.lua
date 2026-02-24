add_rules("mode.debug", "mode.release")

add_repositories("iceblcokmc https://github.com/IceBlcokMC/xmake-repo.git")

add_requires("endstone 0.11.1")

target("BlueIceEngine")
    set_kind("shared")
    set_languages("cxx20")
    set_symbols("debug")

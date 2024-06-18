set_project("magic")

set_version("0.0.1")

set_xmakever("2.8.6")

set_allowedplats("windows", "linux") -- tested in x86_64 linux
set_allowedmodes("debug", "release")

option("test", {showmenu = true,  default = false})

set_languages("c++20")

set_warnings("all")
add_rules("mode.debug", "mode.release")

if is_plat("linux") then
    set_toolchains("gcc") -- tested with gcc 
end

if is_plat("windows") then
    set_runtimes("MD")
    add_cxflags("/permissive-", {tools = "cl"})
end

if has_config("test") then
    add_requires("gtest", {system = false, configs = {main = true, gmock = false}})
end

target("magic")
    set_kind("headeronly")
    add_includedirs("include", {interface = true})
    add_headerfiles("include/(magic/*.h)")

target("test")
    set_kind("binary")
    set_default(false)
    add_files(os.files("test/*.cpp"))

    if is_plat("windows") then
        add_ldflags("/subsystem:console")
    end

    add_deps("magic")
    add_packages("gtest")

    add_tests("default")

set_project("magic")

set_version("0.0.1")

set_xmakever("2.8.5")

set_allowedplats("windows")
set_allowedmodes("debug", "release")

set_languages("c++20")

set_warnings("all")
add_rules("mode.debug", "mode.release")

if is_plat("windows") then
    set_runtimes("MD")
    add_cxflags("/permissive-", {tools = "cl"})
end

target("magic")
    set_kind("headeronly")
    add_includedirs("include", {interface = true})
    add_headerfiles("include/(magic/*.h)")

for _, file in ipairs(os.files("test/*.cpp")) do
    local name = path.basename(file)
    target(name)
        set_kind("object")
        set_default(false)
        add_files(file)

        add_deps("magic")
        add_tests("default", {build_should_pass = true})
end
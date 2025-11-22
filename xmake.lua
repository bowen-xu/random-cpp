add_rules("mode.debug", "mode.release", "mode.releasedbg")
set_languages("cxx23")

target("randomcpp")
    set_kind("headeronly")
    add_includedirs("include", {public = true})
    add_headerfiles("include/**.hpp")



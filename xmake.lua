add_rules("mode.debug", "mode.release", "mode.releasedbg")
set_languages("cxx20")

add_requires("gtest")
target("randomcpp_test")
    set_kind("binary")
    add_packages("gtest")
    add_includedirs("include")
    add_files(
        "test/test.cpp", 
        "test/integers.cpp", 
        "test/random.cpp", 
        "test/real_values.cpp", 
        "test/sequences.cpp"
    )



package("randomcpp")
    set_description("C++ library for random number generation with Pythin-like APIs")

    set_homepage("https://github.com/bowen-xu/random-cpp")
    add_urls("https://github.com/bowen-xu/random-cpp.git")
    set_license("MIT")
    set_kind("library")
    -- on_install("macosx", "linux", function (package)
    --     import("package.tools.autoconf").install(package)
    -- end)

    on_install(function (package)
        os.cp("include", package:installdir())
    end)

package_end()

-- add_rules("mode.debug", "mode.release", "mode.releasedbg")
-- set_languages("cxx20")

-- set_version("1.0.0")

-- target("randomcpp")
--     set_kind("headeronly")
--     add_includedirs("./include")

--     -- on_install(function (package)
--     --     local install_dir = package:installdir()
    
--     --     local header_src_dir = path.join(package:scriptdir(), "include") 
--     --     local header_dest_dir = path.join(install_dir, "include")
    
--     --     os.mkdir(header_dest_dir)
    
--     --     os.cp(path.join(header_src_dir, "*.h"), header_dest_dir)
--     -- end)

add_requires("randomcpp")
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



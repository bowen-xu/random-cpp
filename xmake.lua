add_rules("mode.debug", "mode.release", "mode.releasedbg")
set_languages("cxx20")

set_version("1.0.0")

target("randomcpp")
    set_kind("headeronly")
    add_includedirs("./include")

    on_install(function (package)
        local install_dir = package:installdir()
    
        local header_src_dir = path.join(package:scriptdir(), "include")  -- 假设头文件存放在 src 目录
        local header_dest_dir = path.join(install_dir, "include")    -- 目标路径是安装目录下的 include 文件夹
    
        os.mkdir(header_dest_dir)
    
        os.cp(path.join(header_src_dir, "*.h"), header_dest_dir)
    end)


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



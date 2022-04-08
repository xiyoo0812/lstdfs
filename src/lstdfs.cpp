#define LUA_LIB

#include <chrono>
#include <iostream>
#include <filesystem>

#include "sol/sol.hpp"

extern "C" {
    #include "lua.h"
    #include "lauxlib.h"
}

using namespace std;
using namespace std::chrono;

namespace lstdfs {

    struct file_info {
        std::string name;
        std::string type;
    };
    using file_vector = std::vector<file_info>;
    using path_vector = std::vector<std::string>;

    std::string lstdfs_absolute(std::string path) {
        return filesystem::absolute(path).string();
    }

    std::string lstdfs_current_path() {
        return filesystem::current_path().string();
    }

    std::string lstdfs_temp_dir() {
        return filesystem::temp_directory_path().string();
    }

    sol::variadic_results lstdfs_chdir(std::string path, sol::this_state L) {
        sol::variadic_results values;
        try {
            filesystem::current_path(path);
            values.push_back({ L, sol::in_place_type<bool>, true });
            return values;
        }
        catch (filesystem::filesystem_error const& e) {
            values.push_back({ L, sol::in_place_type<bool>, false });
            values.push_back({ L, sol::in_place_type<std::string>, e.what() });
            return values;
        }
    }

    sol::variadic_results lstdfs_mkdir(std::string path, sol::this_state L) {
        sol::variadic_results values;
        try {
            bool res = filesystem::create_directories(path);
            values.push_back({ L, sol::in_place_type<bool>, res });
            return values;
        }
        catch (filesystem::filesystem_error const& e) {
            values.push_back({ L, sol::in_place_type<bool>, false });
            values.push_back({ L, sol::in_place_type<std::string>, e.what() });
            return values;
        }
    }

    sol::variadic_results lstdfs_remove(std::string path, bool rmall, sol::this_state L) {
        sol::variadic_results values;
        try {
            if (rmall) {
                auto size = filesystem::remove_all(path);
                values.push_back({ L, sol::in_place_type<bool>, size > 0 });
                return values;
            }
            bool res = filesystem::remove(path);
            values.push_back({ L, sol::in_place_type<bool>, res });
            return values;
        }
        catch (filesystem::filesystem_error const& e) {
            values.push_back({ L, sol::in_place_type<bool>, false });
            values.push_back({ L, sol::in_place_type<std::string>, e.what() });
            return values;
        }
    }

    sol::variadic_results lstdfs_copy(std::string from, std::string to, filesystem::copy_options option, sol::this_state L) {
        sol::variadic_results values;
        try {
            filesystem::copy(from, to, option);
            values.push_back({ L, sol::in_place_type<bool>, true });
            return values;
        }
        catch (filesystem::filesystem_error const& e) {
            values.push_back({ L, sol::in_place_type<bool>, false });
            values.push_back({ L, sol::in_place_type<std::string>, e.what() });
            return values;
        }
    }
    sol::variadic_results lstdfs_copy_file(std::string from, std::string to, filesystem::copy_options option, sol::this_state L) {
        sol::variadic_results values;
        try {
            filesystem::copy_file(from, to, option);
            values.push_back({ L, sol::in_place_type<bool>, true });
            return values;
        }
        catch (filesystem::filesystem_error const& e) {
            values.push_back({ L, sol::in_place_type<bool>, false });
            values.push_back({ L, sol::in_place_type<std::string>, e.what() });
            return values;
        }
    }    

    sol::variadic_results lstdfs_rename(std::string pold, std::string pnew, sol::this_state L) {
        sol::variadic_results values;
        try {
            filesystem::rename(pold, pnew);
            values.push_back({ L, sol::in_place_type<bool>, true });
            return values;
        }
        catch (filesystem::filesystem_error const& e) {
            values.push_back({ L, sol::in_place_type<bool>, false });
            values.push_back({ L, sol::in_place_type<std::string>, e.what() });
            return values;
        }
    }

    bool lstdfs_exists(std::string path) {
        return filesystem::exists(path);
    }

    std::string lstdfs_root_name(std::string path) {
        return filesystem::path(path).root_name().string();
    }

    std::string lstdfs_filename(std::string path) {
        return filesystem::path(path).filename().string();
    }

    std::string lstdfs_extension(std::string path) {
        return filesystem::path(path).extension().string();
    }

    std::string lstdfs_root_path(std::string path) {
        return filesystem::path(path).root_path().string();
    }

    std::string lstdfs_parent_path(std::string path) {
        return filesystem::path(path).parent_path().string();
    }

    std::string lstdfs_relative_path(std::string path) {
        return filesystem::path(path).relative_path().string();
    }

    std::string lstdfs_append(std::string path, std::string append_path) {
        return filesystem::path(path).append(append_path).string();
    }

    std::string lstdfs_concat(std::string path, std::string concat_path) {
        return filesystem::path(path).concat(concat_path).string();
    }

    std::string lstdfs_remove_filename(std::string path) {
        return filesystem::path(path).remove_filename().string();
    }

    std::string lstdfs_replace_filename(std::string path, std::string filename) {
        return filesystem::path(path).replace_filename(filename).string();
    }

    std::string lstdfs_replace_extension(std::string path, std::string extens) {
        return filesystem::path(path).replace_extension(extens).string();
    }

    std::string lstdfs_stem(std::string path) {
        return filesystem::path(path).stem().string();
    }

    bool lstdfs_is_directory(std::string path) {
        return filesystem::is_directory(path);
    }

    bool lstdfs_is_absolute(std::string path) {
        return filesystem::path(path).is_absolute();
    }

    sol::variadic_results lstdfs_last_write_time(std::string path, sol::this_state L) {
        sol::variadic_results values;
        try {
            auto ftime = filesystem::last_write_time(path);
            auto sctp = time_point_cast<system_clock::duration>(ftime - filesystem::file_time_type::clock::now() + system_clock::now());
            std::time_t cftime = system_clock::to_time_t(sctp);
            values.push_back({ L, sol::in_place_type<uint64_t>, cftime });
            return values;
        }
        catch (filesystem::filesystem_error const& e) {
            values.push_back({ L, sol::in_place_type<bool>, false });
            values.push_back({ L, sol::in_place_type<std::string>, e.what() });
            return values;
        }
    }

    std::string get_file_type(const filesystem::path& path) {
        filesystem::file_status s = filesystem::status(path);
        switch (s.type()) {
        case filesystem::file_type::none: return "none";
        case filesystem::file_type::not_found: return "not_found";
        case filesystem::file_type::regular: return "regular";
        case filesystem::file_type::directory: return "directory";
        case filesystem::file_type::symlink: return "symlink";
        case filesystem::file_type::block: return "block";
        case filesystem::file_type::character: return "character";
        case filesystem::file_type::fifo: return "fifo";
        case filesystem::file_type::socket: return "socket";
        case filesystem::file_type::unknown: return "unknown";
        default: return "implementation-defined";
        }
    }

    std::string lstdfs_filetype(std::string path) {
        return get_file_type(filesystem::path(path));
    }

    sol::variadic_results lstdfs_dir(std::string path, bool recursive, sol::this_state L) {
        sol::variadic_results values;
        try {
            file_vector files;
            if (recursive) {
                for (auto entry : filesystem::recursive_directory_iterator(path)) {
                    files.push_back(file_info({ entry.path().string(), get_file_type(entry.path()) }));
                }
                values.push_back({ L, sol::in_place_type<file_vector>, files });
                return values;
            }
            for (auto entry : filesystem::directory_iterator(path)) {
                files.push_back(file_info({ entry.path().string(), get_file_type(entry.path()) }));
            }
            values.push_back({ L, sol::in_place_type<file_vector>, files });
            return values;
        }
        catch (filesystem::filesystem_error const& e) {
            values.push_back({ L, sol::in_place_type<bool>, false });
            values.push_back({ L, sol::in_place_type<std::string>, e.what() });
            return values;
        }
    }

    path_vector lstdfs_split(std::string cpath) {
        path_vector values;
        filesystem::path path = filesystem::path(cpath);
        for (auto it = path.begin(); it != path.end(); ++it) {
            values.push_back((*it).string());
        }
        return values;
    }

    sol::table open_lstdfs(sol::this_state L) {
        sol::state_view lua(L);
        auto lstdfs = lua.create_table();
        lstdfs.new_usertype<file_info>("file_info",
            "name", &file_info::name,
            "type", &file_info::type
        );
        lstdfs.new_enum("copy_options",
            "none", filesystem::copy_options::none,
            "recursive", filesystem::copy_options::recursive,
            "recursive", filesystem::copy_options::recursive,
            "copy_symlinks", filesystem::copy_options::copy_symlinks,
            "copy_symlinks", filesystem::copy_options::copy_symlinks,
            "skip_symlinks", filesystem::copy_options::skip_symlinks,
            "create_symlinks", filesystem::copy_options::create_symlinks,
            "directories_only", filesystem::copy_options::directories_only,
            "create_hard_links", filesystem::copy_options::create_hard_links,
            "overwrite_existing", filesystem::copy_options::overwrite_existing
        );
        lstdfs.set_function("dir", lstdfs_dir);
        lstdfs.set_function("stem", lstdfs_stem);
        lstdfs.set_function("copy", lstdfs_copy);
        lstdfs.set_function("mkdir", lstdfs_mkdir);
        lstdfs.set_function("chdir", lstdfs_chdir);
        lstdfs.set_function("split", lstdfs_split);
        lstdfs.set_function("rename", lstdfs_rename);
        lstdfs.set_function("exists", lstdfs_exists);
        lstdfs.set_function("remove", lstdfs_remove);
        lstdfs.set_function("append", lstdfs_append);
        lstdfs.set_function("concat", lstdfs_concat);
        lstdfs.set_function("temp_dir", lstdfs_temp_dir);
        lstdfs.set_function("absolute", lstdfs_absolute);
        lstdfs.set_function("filetype", lstdfs_filetype);
        lstdfs.set_function("filename", lstdfs_filename);
        lstdfs.set_function("copy_file", lstdfs_copy_file);
        lstdfs.set_function("extension", lstdfs_extension);
        lstdfs.set_function("root_name", lstdfs_root_name);
        lstdfs.set_function("root_path", lstdfs_root_path);
        lstdfs.set_function("is_absolute", lstdfs_is_absolute);
        lstdfs.set_function("parent_path", lstdfs_parent_path);
        lstdfs.set_function("is_directory", lstdfs_is_directory);
        lstdfs.set_function("current_path", lstdfs_current_path);
        lstdfs.set_function("relative_path", lstdfs_relative_path);
        lstdfs.set_function("last_write_time", lstdfs_last_write_time);
        lstdfs.set_function("remove_filename", lstdfs_remove_filename);
        lstdfs.set_function("replace_filename", lstdfs_replace_filename);
        lstdfs.set_function("replace_extension", lstdfs_replace_extension);
        return lstdfs;
    }
}

extern "C" LUALIB_API int luaopen_lstdfs(lua_State* L) {
    return sol::stack::call_lua(L, 1, lstdfs::open_lstdfs);
}

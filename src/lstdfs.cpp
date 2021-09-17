#include <chrono>
#include <iostream>
#include<filesystem>

extern "C"
{
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

using namespace std;
using namespace std::chrono;

#ifdef _MSC_VER
#define LSTDFS_API extern "C" _declspec(dllexport)
#else
#define LSTDFS_API extern "C"
#endif

static int lstdfs_absolute(lua_State* L) {
    filesystem::path path = filesystem::path(lua_tostring(L, 1));
    auto absolute = filesystem::absolute(path);
    lua_pushstring(L, absolute.string().c_str());
    return 1;
}

static int lstdfs_current_path(lua_State* L) {
    auto path = filesystem::current_path();
    lua_pushstring(L, path.string().c_str());
    return 1;
}

static int lstdfs_temp_dir(lua_State* L) {
    auto path = filesystem::temp_directory_path();
    lua_pushstring(L, path.string().c_str());
    return 1;
}

static int lstdfs_chdir(lua_State* L) {
    try {
        filesystem::path path = filesystem::path(lua_tostring(L, 1));
        filesystem::current_path(path);
        lua_pushboolean(L, true);
        return 1;
    }
    catch (filesystem::filesystem_error e) {
        lua_pushboolean(L, false);
        lua_pushstring(L, e.what());
        return 2;
    }
}

static int lstdfs_mkdir(lua_State* L) {
    try {
        filesystem::path path = filesystem::path(lua_tostring(L, 1));
        bool res = filesystem::create_directories(path);
        lua_pushboolean(L, res);
        return 1;
    }
    catch (filesystem::filesystem_error e) {
        lua_pushboolean(L, false);
        lua_pushstring(L, e.what());
        return 2;
    }
}

static int lstdfs_remove(lua_State* L) {
    try {
        bool rmall = false;
        if (lua_gettop(L) > 1) {
            rmall = lua_toboolean(L, 2);
        }
        filesystem::path path = filesystem::path(lua_tostring(L, 1));
        if (rmall){
            auto size = filesystem::remove_all(path);
            lua_pushboolean(L, size > 0);
            return 1;
        }
        bool res = filesystem::remove(path);
        lua_pushboolean(L, res);
        return 1;
    }
    catch (filesystem::filesystem_error e) {
        lua_pushboolean(L, false);
        lua_pushstring(L, e.what());
        return 2;
    }
}

static int lstdfs_copy(lua_State* L) {
    try {
        filesystem::path from = filesystem::path(lua_tostring(L, 1));
        filesystem::path to = filesystem::path(lua_tostring(L, 2));
        filesystem::copy_options option = (filesystem::copy_options)lua_tointeger(L, 3);
        filesystem::copy(from, to, option);
        lua_pushboolean(L, true);
        return 1;
    }
    catch (filesystem::filesystem_error e) {
        lua_pushboolean(L, false);
        lua_pushstring(L, e.what());
        return 2;
    }
}

static int lstdfs_copy_file(lua_State* L) {
    try {
        filesystem::path from = filesystem::path(lua_tostring(L, 1));
        filesystem::path to = filesystem::path(lua_tostring(L, 2));
        filesystem::copy_options option = (filesystem::copy_options)lua_tointeger(L, 3);
        filesystem::copy_file(from, to, option);
        lua_pushboolean(L, true);
        return 1;
    }
    catch (filesystem::filesystem_error e) {
        lua_pushboolean(L, false);
        lua_pushstring(L, e.what());
        return 2;
    }
}

static int lstdfs_copy_option(lua_State* L) {
    lua_newtable(L);
    lua_pushinteger(L, (lua_Integer)filesystem::copy_options::none);
    lua_setfield(L, -2, "none");
    lua_pushinteger(L, (lua_Integer)filesystem::copy_options::recursive);
    lua_setfield(L, -2, "recursive");
    lua_pushinteger(L, (lua_Integer)filesystem::copy_options::recursive);
    lua_setfield(L, -2, "recursive");
    lua_pushinteger(L, (lua_Integer)filesystem::copy_options::copy_symlinks);
    lua_setfield(L, -2, "copy_symlinks");
    lua_pushinteger(L, (lua_Integer)filesystem::copy_options::copy_symlinks);
    lua_setfield(L, -2, "copy_symlinks");
    lua_pushinteger(L, (lua_Integer)filesystem::copy_options::skip_symlinks);
    lua_setfield(L, -2, "skip_symlinks");
    lua_pushinteger(L, (lua_Integer)filesystem::copy_options::create_symlinks);
    lua_setfield(L, -2, "create_symlinks");
    lua_pushinteger(L, (lua_Integer)filesystem::copy_options::directories_only);
    lua_setfield(L, -2, "directories_only");
    lua_pushinteger(L, (lua_Integer)filesystem::copy_options::create_hard_links);
    lua_setfield(L, -2, "create_hard_links ");
    lua_pushinteger(L, (lua_Integer)filesystem::copy_options::overwrite_existing);
    lua_setfield(L, -2, "overwrite_existing");
    return 1;
}

static int lstdfs_rename(lua_State* L) {
    try {
        filesystem::path pold = filesystem::path(lua_tostring(L, 1));
        filesystem::path pnew = filesystem::path(lua_tostring(L, 2));
        filesystem::rename(pold, pnew);
        lua_pushboolean(L, true);
        return 1;
    }
    catch (filesystem::filesystem_error e) {
        lua_pushboolean(L, false);
        lua_pushstring(L, e.what());
        return 2;
    }
}

static int lstdfs_exists(lua_State* L) {
    filesystem::path path = filesystem::path(lua_tostring(L, 1));
    lua_pushboolean(L, filesystem::exists(path));
    return 1;
}

static int lstdfs_root_name(lua_State* L) {
    filesystem::path path = filesystem::path(lua_tostring(L, 1));
    lua_pushstring(L, path.root_name().string().c_str());
    return 1;
}

static int lstdfs_filename(lua_State* L) {
    filesystem::path path = filesystem::path(lua_tostring(L, 1));
    lua_pushstring(L, path.filename().string().c_str());
    return 1;
}

static int lstdfs_extension(lua_State* L) {
    filesystem::path path = filesystem::path(lua_tostring(L, 1));
    lua_pushstring(L, path.extension().string().c_str());
    return 1;
}

static int lstdfs_root_path(lua_State* L) {
    filesystem::path path = filesystem::path(lua_tostring(L, 1));
    lua_pushstring(L, path.root_path().string().c_str());
    return 1;
}

static int lstdfs_parent_path(lua_State* L) {
    filesystem::path path = filesystem::path(lua_tostring(L, 1));
    lua_pushstring(L, path.parent_path().string().c_str());
    return 1;
}

static int lstdfs_relative_path(lua_State* L) {
    filesystem::path path = filesystem::path(lua_tostring(L, 1));
    lua_pushstring(L, path.relative_path().string().c_str());
    return 1;
}

static int lstdfs_append(lua_State* L) {
    filesystem::path path = filesystem::path(lua_tostring(L, 1));
    const char* append_path = lua_tostring(L, 2);
    lua_pushstring(L, path.append(append_path).string().c_str());
    return 1;
}

static int lstdfs_concat(lua_State* L) {
    filesystem::path path = filesystem::path(lua_tostring(L, 1));
    const char* concat_path = lua_tostring(L, 2);
    lua_pushstring(L, path.concat(concat_path).string().c_str());
    return 1;
}

static int lstdfs_remove_filename(lua_State* L) {
    filesystem::path path = filesystem::path(lua_tostring(L, 1));
    lua_pushstring(L, path.remove_filename().string().c_str());
    return 1;
}

static int lstdfs_replace_filename(lua_State* L) {
    filesystem::path path = filesystem::path(lua_tostring(L, 1));
    path.replace_filename(lua_tostring(L, 2));
    lua_pushstring(L, path.string().c_str());
    return 1;
}

static int lstdfs_replace_extension(lua_State* L) {
    filesystem::path path = filesystem::path(lua_tostring(L, 1));
    path.replace_extension(lua_tostring(L, 2));
    lua_pushstring(L, path.string().c_str());
    return 1;
}

static int lstdfs_stem(lua_State* L) {
    filesystem::path path = filesystem::path(lua_tostring(L, 1));
    lua_pushstring(L, path.stem().string().c_str());
    return 1;
}

static int lstdfs_is_directory(lua_State* L) {
    filesystem::path path = filesystem::path(lua_tostring(L, 1));
    lua_pushboolean(L, filesystem::is_directory(path));
    return 1;
}

static int lstdfs_is_absolute(lua_State* L) {
    filesystem::path path = filesystem::path(lua_tostring(L, 1));
    lua_pushboolean(L, path.is_absolute());
    return 1;
}

static int lstdfs_last_write_time(lua_State* L) {
    try {
        filesystem::path path = filesystem::path(lua_tostring(L, 1));
        auto ftime = filesystem::last_write_time(path);
        auto sctp = time_point_cast<system_clock::duration>(ftime - filesystem::file_time_type::clock::now() + system_clock::now());
        std::time_t cftime = system_clock::to_time_t(sctp);
        lua_pushinteger(L, cftime);
        return 1;
    }
    catch (filesystem::filesystem_error e) {
        lua_pushnil(L);
        lua_pushstring(L, e.what());
        return 2;
    }
}

static void push_file_type(lua_State* L, filesystem::path path){
    filesystem::file_status s = filesystem::status(path);
    switch (s.type()) {
    case filesystem::file_type::none: lua_pushstring(L, "none"); break;
    case filesystem::file_type::not_found: lua_pushstring(L, "not_found"); break;
    case filesystem::file_type::regular: lua_pushstring(L, "regular"); break;
    case filesystem::file_type::directory: lua_pushstring(L, "directory"); break;
    case filesystem::file_type::symlink: lua_pushstring(L, "symlink"); break;
    case filesystem::file_type::block: lua_pushstring(L, "block"); break;
    case filesystem::file_type::character: lua_pushstring(L, "character"); break;
    case filesystem::file_type::fifo: lua_pushstring(L, "fifo"); break;
    case filesystem::file_type::socket: lua_pushstring(L, "socket"); break;
    case filesystem::file_type::unknown: lua_pushstring(L, "unknown"); break;
    default: lua_pushstring(L, "implementation-defined"); break;
    }
}

static int lstdfs_filetype(lua_State* L) {
    filesystem::path path = filesystem::path(lua_tostring(L, 1));
    push_file_type(L, path);
    return 1;
}

static int lstdfs_dir(lua_State* L) {
    bool recursive = false;
    if (lua_gettop(L) > 1) {
        recursive = lua_toboolean(L, 2);
    }
    size_t size = 0;
    int top = lua_gettop(L);
    filesystem::path path = filesystem::path(lua_tostring(L, 1));
    try {
        lua_newtable(L);
        if (recursive) {
            for (auto entry : filesystem::recursive_directory_iterator(path)) {
                lua_pushinteger(L, ++size);
                lua_newtable(L);
                lua_pushstring(L, entry.path().string().c_str());
                lua_setfield(L, -2, "name");
                push_file_type(L, entry.path());
                lua_setfield(L, -2, "type");
                lua_rawset(L, -3);
            }
            return 1;
        }
        for (auto entry : filesystem::directory_iterator(path)) {
            lua_pushinteger(L, ++size);
            lua_newtable(L);
            lua_pushstring(L, entry.path().string().c_str());
            lua_setfield(L, -2, "name");
            push_file_type(L, entry.path());
            lua_setfield(L, -2, "type");
            lua_rawset(L, -3);
        }
        return 1;
    }
    catch (filesystem::filesystem_error e) {
        lua_settop(L, top);
        lua_pushnil(L);
        lua_pushstring(L, e.what());
        return 2;
    }
}

static int lstdfs_split(lua_State* L) {
    lua_newtable(L);
    size_t size = 0;
    filesystem::path path = filesystem::path(lua_tostring(L, 1));
    for (auto it = path.begin(); it != path.end(); ++it) {
        lua_pushinteger(L, ++size);
        lua_pushstring(L, (*it).string().c_str());
        lua_rawset(L, -3);
    }
    return 1;
}

static const luaL_Reg lstdfs_funs[] = {
    { "dir", lstdfs_dir },
    { "stem", lstdfs_stem },
    { "copy", lstdfs_copy },
    { "mkdir", lstdfs_mkdir },
    { "chdir", lstdfs_chdir },
    { "split", lstdfs_split },
    { "rename", lstdfs_rename },
    { "exists", lstdfs_exists },
    { "remove", lstdfs_remove },
    { "append", lstdfs_append },
    { "concat", lstdfs_concat },
    { "temp_dir", lstdfs_temp_dir },
    { "absolute", lstdfs_absolute },
    { "filetype", lstdfs_filetype },
    { "filename", lstdfs_filename },
    { "copy_file", lstdfs_copy_file },
    { "extension", lstdfs_extension },
    { "root_name", lstdfs_root_name },
    { "root_path", lstdfs_root_path },
    { "is_absolute", lstdfs_is_absolute },
    { "copy_option", lstdfs_copy_option },
    { "parent_path", lstdfs_parent_path },
    { "is_directory", lstdfs_is_directory },
    { "current_path", lstdfs_current_path },
    { "relative_path", lstdfs_relative_path },
    { "last_write_time", lstdfs_last_write_time },
    { "remove_filename", lstdfs_remove_filename },
    { "replace_filename", lstdfs_replace_filename },
    { "replace_extension", lstdfs_replace_extension },
    { NULL, NULL }
};

LSTDFS_API int luaopen_lstdfs(lua_State* L) {
    luaL_newlib(L, lstdfs_funs);
    return 1;
}
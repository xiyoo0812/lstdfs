#include <iostream>
#include<filesystem>

extern "C"
{
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

using namespace std;

#ifdef _MSC_VER
#define LSTDFS_API extern "C" _declspec(dllexport)
#else
#define LSTDFS_API extern "C"
#endif

static int lstdfs_absolute(lua_State* L) {
    auto path = filesystem::absolute(lua_tostring(L, 1));
    lua_pushstring(L, path.string().c_str());
    return 1;
}

static int lstdfs_current_dir(lua_State* L) {
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
        filesystem::current_path(lua_tostring(L, 1));
        lua_pushboolean(L, true);
        return 1;
    }
    catch (filesystem::filesystem_error e) {
        lua_pushboolean(L, false);
        lua_pushstring(L, e.what());
        return 2;
    }
}

static int lstdfs_absolute(lua_State* L) {
    auto path = filesystem::current_path();
    lua_pushstring(L, path.string().c_str());
    return 1;
}

static int lstdfs_mkdir(lua_State* L) {
    try {
        bool res = filesystem::create_directories(lua_tostring(L, 1));
        lua_pushboolean(L, res);
        return 1;
    }
    catch (filesystem::filesystem_error e) {
        lua_pushboolean(L, false);
        lua_pushstring(L, e.what());
        return 2;
    }
}

static int lstdfs_rmdir(lua_State* L) {
    try {
        bool res = filesystem::remove(lua_tostring(L, 1));
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
        filesystem::rename(lua_tostring(L, 1), lua_tostring(L, 2));
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
    lua_pushboolean(L, filesystem::exists(lua_tostring(L, 1)));
    return 1;
}

static int lstdfs_is_directory(lua_State* L) {
    lua_pushboolean(L, filesystem::is_directory(lua_tostring(L, 1)));
    return 1;
}

static int lstdfs_is_absolute(lua_State* L) {
    filesystem::path path = filesystem::path(lua_tostring(L, 1));
    lua_pushboolean(L, path.is_absolute());
    return 1;
}

static int lstdfs_dir(lua_State* L) {
    lua_newtable(L);
    size_t size = 0;
    filesystem::path path = filesystem::path(lua_tostring(L, 1));
    for (auto entry : filesystem::directory_iterator(path)) {
        lua_pushinteger(L, ++size);
        lua_pushstring(L, entry.path().string().c_str());
        lua_rawset(L, -3);
    }
    return 1;
}

static const luaL_Reg lstdfs_funs[] = {
    { "dir", lstdfs_dir },
    { "mkdir", lstdfs_mkdir },
    { "rmdir", lstdfs_rmdir },
    { "chdir", lstdfs_chdir },
    { "rename", lstdfs_rename },
    { "exists", lstdfs_exists },
    { "temp_dir", lstdfs_temp_dir },
    { "absolute", lstdfs_absolute },
    { "current_dir", lstdfs_current_dir },
    { "copy_option", lstdfs_copy_option },
    { "is_absolute", lstdfs_is_absolute },
    { "is_directory", lstdfs_is_directory },
    { NULL, NULL }
};

LSTDFS_API int luaopen_lstdfs(lua_State* L) {
    luaL_newlib(L, lstdfs_funs);
    return 1;
}
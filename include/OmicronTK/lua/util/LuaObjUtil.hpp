#pragma once

#include <lua.hpp>

namespace OmicronTK {
namespace lua {

template<typename _Class, const char *_className>
class LuaObjUtil
{
public:
    static inline void newUserData(lua_State *L, _Class *userdata)
    {
        *static_cast<void **>(lua_newuserdata(L, sizeof(_Class *))) = userdata;

        luaL_setmetatable(L, _className);
        lua_setfield(L, -2, "__userdata");
    }

    static inline _Class *checkUserData(lua_State *L, int ud)
    {
        luaL_checktype(L, ud, LUA_TTABLE);
        lua_getfield(L, ud, "__userdata");

        return *static_cast<_Class **>(luaL_checkudata(L, -1, _className));
    }
};

}
}
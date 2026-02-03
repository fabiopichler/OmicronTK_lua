#include "OmicronTK/lua/Persistent.hpp"

#include <lua.hpp>

namespace OmicronTK {
namespace lua {

Persistent::Persistent(lua_State *L)
    : L(L)
    , m_ref(0)
{

}

Persistent::~Persistent()
{
    unref();
}

void Persistent::ref(lua_State *L)
{
    if (L)
        this->L = L;

    if (this->L)
        m_ref = luaL_ref(this->L, LUA_REGISTRYINDEX);
}

void Persistent::unref()
{
    if (L && m_ref)
    {
        luaL_unref(L, LUA_REGISTRYINDEX, m_ref);
        m_ref = 0;
    }
}

void Persistent::rawgeti()
{
    if (L)
        lua_rawgeti(L, LUA_REGISTRYINDEX, m_ref);
}

}
}

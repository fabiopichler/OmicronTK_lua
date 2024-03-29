#pragma once

#include "OmicronTK/lua/global.h"
#include <OmicronTK/lua/CallbackInfo.hpp>

namespace OmicronTK {
namespace lua {

class OTKLUA_EXPORT Callback
{
public:
    template<void (*func)(CallbackInfo &info)>
    inline static int luaCallback(lua_State *L)
    {
        CallbackInfo info(L);

        try
        {
            func(info);
        }
        catch (const std::exception &e)
        {
            return info.error(e.what());
        }

        return info.getReturnValue().length();
    }
};

}
}

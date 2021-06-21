#pragma once

#include "OmicronTK/lua/Value.hpp"
#include <OmicronTK/lua/Callback.hpp>

#include <stdexcept>

namespace OmicronTK {
namespace lua {

class Lua;
class NativeClass
{
public:
    explicit NativeClass(Lua &lua, const std::string &name);

    void setStatics(const RegVector &statics);
    void addStatic(const std::string &field, const Value &value);
    void addStatic(const LuaReg &reg);

    void setMembers(const RegVector &members);
    void addMember(const std::string &field, const Value &value);
    void addMember(const LuaReg &reg);

    void setMetamethods(const RegVector &metamethods);
    void addMetamethod(const std::string &field, const Value &value);
    void addMetamethod(const LuaReg &reg);

    void addConstructor(const Value &constructor);
    void addDestructor(const Value &__gc);

    void create();

    template<void (*value)(CallbackInfo &info)>
    inline void addConstructor()
    {
        addConstructor(&Callback::luaCallback<value>);
    }

    template<void (*value)(CallbackInfo &info)>
    inline void addDestructor()
    {
        addDestructor(&Callback::luaCallback<value>);
    }

    template<void (*value)(CallbackInfo &info)>
    inline void addStatic(const std::string &field)
    {
        addStatic(field, &Callback::luaCallback<value>);
    }

    template<void (*value)(CallbackInfo &info)>
    inline void addMember(const std::string &field)
    {
        addMember(field, &Callback::luaCallback<value>);
    }

private:
    Lua &m_lua;
    std::string m_name;
    RegVector m_statics;
    RegVector m_members;
    RegVector m_metamethods;
};

}
}
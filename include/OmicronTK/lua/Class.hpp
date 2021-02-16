#pragma once

#include "OmicronTK/lua/Value.hpp"

namespace OmicronTK {
namespace lua {

class Class
{
public:
    explicit Class(const std::string &name);

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

    template<int (*value)(const CallbackInfo &info)>
    inline void addConstructor()
    {
        addConstructor(&luaCallback<value>);
    }

    template<int (*value)(const CallbackInfo &info)>
    inline void addStatic(const std::string &field)
    {
        addStatic(field, &luaCallback<value>);
    }

    template<int (*value)(const CallbackInfo &info)>
    inline void addMember(const std::string &field)
    {
        addMember(field, &luaCallback<value>);
    }

private:
    template<int (*func)(const CallbackInfo &info)>
    inline static int luaCallback(lua_State *L)
    {
        CallbackInfo info(L);

        return func(info);
    }

    std::string m_name;
    RegVector m_statics;
    RegVector m_members;
    RegVector m_metamethods;

    friend class Lua;
};

}
}

// Author: Román Chérrez Mala
// Email: cherrezmar@coventry.ac.uk

#ifndef _SCRIPTING_MANAGER_
#define _SCRIPTING_MANAGER_

#include "lua.hpp"
#include <vector>

namespace Aloe {
  /** LuaMethod structure*/
  struct LuaMethod {
    int(*method)(lua_State* L);
    const char* method_name = "";
  };

  class ScriptingManager {
  public:
    /** Default constructor*/
    ScriptingManager();
    /** Default deconstructor*/
    ~ScriptingManager();

    /** Create function that stores all the methods to be set in LUA*/
    void create(std::vector<LuaMethod> meth);
    /** Sets the script to use*/
    void setScript(const char* script_path);
    /** Closes the current script*/
    void close();

    /** Calls a function*/
    void callFunction(const char* func_name);

  private:
    /** Lua state*/
    lua_State* state;
    /** Vector of methods*/
    std::vector<LuaMethod> methods;
  };

}

#endif
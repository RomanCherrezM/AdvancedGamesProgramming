// Author: Román Chérrez Mala
// Email: cherrezmar@coventry.ac.uk

#include "Core/scripting_manager.h"

namespace Aloe {

  ScriptingManager::ScriptingManager() {}
  ScriptingManager::~ScriptingManager() {}

  void ScriptingManager::create(std::vector<LuaMethod> meth) {
    methods = meth;
  }

  void ScriptingManager::setScript(const char* script_path) {

    if (state != nullptr) {
      close();
    }

    if (!script_path) {
      return;
    }

    // Set the script
    state = luaL_newstate();
    luaL_openlibs(state);

    //Push functions
    // Creating the table (The "class")
    lua_createtable(state, 0, 0);

    // Pushing the functions to the table
    for (int i = 0; i < methods.size(); ++i) {
      lua_register(state, methods[i].method_name, methods[i].method);
    }

    // Check the file status
    int status = luaL_dofile(state, script_path);
    if (status) {
      const char* error = lua_tostring(state, -1);
      printf("LUA ERRROR: %s\n", error);
    }
  }

  void ScriptingManager::close() {
    if (state != nullptr) {
      lua_close(state);
      state = nullptr;
    }
  }

  void ScriptingManager::callFunction(const char* func_name) {

    if (state == nullptr) { return; }

    lua_getglobal(state, func_name);
    /* call the function with 2 arguments, return 1 result */
    lua_call(state,0,0);
  }


}
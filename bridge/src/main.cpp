// myextension.cpp
// Определения расширения lib
#define LIB_NAME "bridgeext"
#define MODULE_NAME "bridge"
#define EXTENSION_NAME bridge

// включаем Defold SDK
#include <dmsdk/sdk.h>

#if defined(DM_PLATFORM_HTML5)

#include <emscripten.h>

typedef void (*NoMessage)(int call_id);


// The Emscripten JS library from .js
extern "C" {
    const char* get_language();
    void set_callbacks_in_js(NoMessage cb_string);
    void show_rewarded();
    void show_interstitial();
    void send_platform_message(const char* message);
}

dmScript::LuaCallbackInfo* no_message_callback;

static int get_language_cpp(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 1);

    const char* result = get_language();

    if (result == 0 || strcmp(result, "") == 0) {
        lua_pushnil(L);
    } else {
        lua_pushstring(L, result);
    }
    return 1;
}

static int set_callbacks_cpp(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 0);
    no_message_callback = dmScript::CreateCallback(L, 1);
    return 0;
}

static int send_platform_message_cpp(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 0);
    const char* message = luaL_checkstring(L, 1);
    send_platform_message(message);
    return 0;
}

static int show_reward_ads_cpp(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 0);
    show_rewarded();
    return 0;
}

static int show_interstitial_ads_cpp(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 0);
    show_interstitial();
    return 0;
}

// Functions exposed to Lua
static const luaL_reg Module_methods[] =
{
    {"set_callbacks", set_callbacks_cpp},
    {"get_language", get_language_cpp},
    {"send_platform_message", send_platform_message_cpp},
    {"show_reward_ads", show_reward_ads_cpp},
    {"show_interstitial_ads", show_interstitial_ads_cpp},
    {0, 0}
};

static void No_Message_Callback(int call_id)
{
    lua_State* L = dmScript::GetCallbackLuaContext(no_message_callback);
    DM_LUA_STACK_CHECK(L, 0);

    if (!dmScript::SetupCallback(no_message_callback))
    {
        return;
    }

    lua_pushnumber(L, call_id);

    dmScript::PCall(L, 2, 0); // self + # user arguments

    dmScript::TeardownCallback(no_message_callback);
}

static void LuaInit(lua_State* L)
{
    int top = lua_gettop(L);

    // Register lua names
    luaL_register(L, MODULE_NAME, Module_methods);
    lua_pop(L, 1);
    assert(top == lua_gettop(L));
}

static void RegisterCallbacks()
{
    set_callbacks_in_js((NoMessage)No_Message_Callback);
}

static dmExtension::Result InitializeExtension(dmExtension::Params* params)
{
    // Init Lua
    LuaInit(params->m_L);
    RegisterCallbacks();
    return dmExtension::RESULT_OK;
}

static dmExtension::Result FinalizeExtension(dmExtension::Params* params)
{
    return dmExtension::RESULT_OK;
}

#else

static dmExtension::Result InitializeExtension(dmExtension::Params* params)
{
    return dmExtension::RESULT_OK;
}

static dmExtension::Result FinalizeExtension(dmExtension::Params* params)
{
    return dmExtension::RESULT_OK;
}

#endif

DM_DECLARE_EXTENSION(EXTENSION_NAME, LIB_NAME, 0, 0, InitializeExtension, 0, 0, FinalizeExtension)
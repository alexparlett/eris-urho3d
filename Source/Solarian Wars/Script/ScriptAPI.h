////////////////////////////////////////////
//
// Copyright (c) 2013-2014 Homonoia Studios
//
////////////////////////////////////////////

#pragma once

class asIScriptEngine;

void RegisterScriptAPI(asIScriptEngine* engine);
void RegisterIOAPI(asIScriptEngine* engine);
void RegisterGameAPI(asIScriptEngine* engine);

inline void RegisterScriptAPI(asIScriptEngine* engine)
{
    RegisterIOAPI(engine);
    RegisterGameAPI(engine);
}
#pragma once

#include <list>
#include "ScriptArguments.h"
#include "EventHandler.h"

typedef SDK::ScriptArgument* (*fnScriptFunction)(SDK::ScriptArguments& arguments);

struct ScriptFunction
{
	char*                szName;
	fnScriptFunction     pfnFunction;
	SDK::ParameterCheck* pParameterCheck;
};

namespace SDK
{
	class ScriptClass : public virtual SDK::EventHandler
	{
	public:
		virtual char*                       GetName() = 0;
		virtual SDK::ScriptClass*           GetParent() = 0;
		virtual bool                        IsTypeOf(const char* szType) = 0;

		virtual void                        AddStaticFunction(const char* name, fnScriptFunction function, const char* szTemplate) = 0;
		virtual ScriptFunction*             GetStaticFunction(const char* name) = 0;
		virtual std::list<ScriptFunction*>* GetStaticFunctionList() = 0;

		virtual void                        AddInstanceFunction(const char* name, fnScriptFunction function, const char* szTemplate) = 0;
		virtual ScriptFunction*             GetInstanceFunction(const char* name) = 0;
		virtual std::list<ScriptFunction*>* GetInstanceFunctionList() = 0;
	};
}

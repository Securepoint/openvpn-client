#pragma once

#pragma warning(disable:4250) // C4250: 'class1' : inherits 'class2::member' via dominance

#include "ScriptArguments.h"

namespace SDK
{
	class Package;

	class EventHandler
	{
	public:
		virtual void  AddEvent(std::string eventName, SDK::Package* package, void* callback) = 0;
		virtual SDK::ScriptArguments ExecuteEvent(std::string eventName, SDK::ScriptArguments& arguments) = 0;
		virtual SDK::ScriptArguments ExecuteEvent(std::string eventName, const char* format, ...) = 0;
		virtual SDK::ScriptArguments ExecuteEvent(SDK::Package* pPackage, std::string eventName, SDK::ScriptArguments& arguments) = 0;
		virtual SDK::ScriptArguments ExecuteEvent(SDK::Package* pPackage, std::string eventName, const char* format, ...) = 0;
		virtual void  RemoveEvents(SDK::Package* package) = 0;
	};
}

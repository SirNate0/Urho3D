#include "SystemInterface.h"

#include <Core/Context.h>
#include <Core/Timer.h>
#include <IO/Log.h>
#include <Rocket/Core/Log.h>
#include <Rocket/Controls/ElementFormControlInput.h>

namespace Urho3D
{
	// Get the number of seconds elapsed since the start of the application
	float SystemInterface::GetElapsedTime()
	{
		return context_->GetSubsystem<Time>()->GetElapsedTime();
	}


	bool SystemInterface::LogMessage(::Rocket::Core::Log::Type type, const String& message)
	{
		using namespace ::Rocket::Core;
		switch (type)
		{
		case ::Rocket::Core::Log::LT_ALWAYS:
		case ::Rocket::Core::Log::LT_ERROR:
		case ::Rocket::Core::Log::LT_ASSERT:
			URHO3D_LOGERROR(message.CString());
			return false;
			break;
		case ::Rocket::Core::Log::LT_WARNING:
		case ::Rocket::Core::Log::LT_INFO:
			URHO3D_LOGINFO(message.CString());
			break;
		case ::Rocket::Core::Log::LT_DEBUG:
			URHO3D_LOGDEBUG(message.CString());
			break;
		defualt:
			URHO3D_LOGRAW(message.CString());
		}
		return true;
	}

	void SystemInterface::RegisterObject(Urho3D::Context* context)
	{
		context->RegisterFactory<SystemInterface>();
	}

	void SystemInterface::AddReference()
	{
		AddRef();
	}

	void SystemInterface::RemoveReference()
	{
		ReleaseRef();
	}
}

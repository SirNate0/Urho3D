/*
 * RocketEventListener.cpp
 *
 *  Created on: Jan 9, 2016
 *      Author: nathan
 */

#include "RocketEventListener.h"

#include <Rocket/Core/Factory.h>
#include <Core/Context.h>

#include <IO/Log.h>
#include <AngelScript/Script.h>
#include <Resource/ResourceCache.h>
#include <AngelScript/angelscript.h>

#include "RocketDocument.h"

RocketEventListenerInstancer::RocketEventListenerInstancer(Urho3D::Context* context): Object(context), executeAngelScript_(true), executeLuaScript_(false), sendUrhoEvents_(true)
{
	// TODO Auto-generated constructor stub

}

RocketEventListenerInstancer::~RocketEventListenerInstancer()
{
	while (this->GetReferenceCount())
		RemoveReference();
//	Doesn't work because the event_listener_instancer is declared static
//	if (::Rocket::Core::event_listener_instancer == this)
//		::Rocket::Core::event_listener_instancer = nullptr;
}

RocketEventListenerInstancer* RocketEventListenerInstancer::Register(Urho3D::Context* context)
{
	RocketEventListenerInstancer* l = new RocketEventListenerInstancer(context);
	context->RegisterSubsystem(l);
	::Rocket::Core::Factory::RegisterEventListenerInstancer(context->GetSubsystem<RocketEventListenerInstancer>());
	return l;
}

/// Instance an event listener object.
/// @param value Value of the event.
/// @param element Element that triggers the events.
::Rocket::Core::EventListener* RocketEventListenerInstancer::InstanceEventListener(const ::Rocket::Core::String& value, ::Rocket::Core::Element* element)
{
//	URHO3D_LOGERROR("Instanced event listerner." + Urho3D::String((intptr_t)GetSubsystem<RocketEventListener>()) + value.CString() + element->GetClassNames().CString());
//	if (GetSubsystem<RocketEventListener>())
//	{
//		element->AddEventListener(value,GetSubsystem<RocketEventListener>(),true);
//		return GetSubsystem<RocketEventListener>();
//	}
//
//	GetContext()->RegisterSubsystem(this);
//	return this;
	Urho3D::ScriptFile* sf = nullptr;
	if (element)
	{
		::Rocket::Core::String s = element->GetAttribute<::Rocket::Core::String>("context","");
		if (!s.Empty())
			sf = GetSubsystem<Urho3D::ResourceCache>()->GetResource<Urho3D::ScriptFile>(s.CString());
	}
	return new RocketEventListener(value, sf, this);
}

Urho3D::Variant ToVariant(::Rocket::Core::Variant* v)
{
	if (!v)
		return Urho3D::Variant::EMPTY;
	switch(v->GetType())
	{
	case ::Rocket::Core::Variant::INT:
		return Urho3D::Variant(v->Get<int>());
	case ::Rocket::Core::Variant::BYTE:
		return Urho3D::Variant(v->Get<::Rocket::Core::byte>());
	case ::Rocket::Core::Variant::CHAR:
		return Urho3D::Variant(v->Get<char>());
	case ::Rocket::Core::Variant::FLOAT:
		return Urho3D::Variant(v->Get<float>());
	case ::Rocket::Core::Variant::SCRIPTINTERFACE:
		return Urho3D::Variant((void*)v->Get<::Rocket::Core::ScriptInterface*>());
	case ::Rocket::Core::Variant::STRING:
		return Urho3D::Variant(v->Get<::Rocket::Core::String>().CString());
	case ::Rocket::Core::Variant::VECTOR2:
	{
		Urho3D::Vector2 ve(v->Get<::Rocket::Core::Vector2f>().x,v->Get<::Rocket::Core::Vector2f>().y);
		return Urho3D::Variant(ve);
	}
	case ::Rocket::Core::Variant::VOIDPTR:
		return Urho3D::Variant(v->Get<void*>());
	case ::Rocket::Core::Variant::WORD:
		return Urho3D::Variant(v->Get<::Rocket::Core::word>());
	case ::Rocket::Core::Variant::NONE:
	default:
		return Urho3D::Variant::EMPTY;
	}
}

Urho3D::VariantMap ToVariantMap(const ::Rocket::Core::Dictionary* d)
{
	if (!d)
		return Urho3D::VariantMap();
	int i = 0;
	::Rocket::Core::String key;
	::Rocket::Core::Variant* v;
	Urho3D::VariantMap map;
	while (d->Iterate(i,key,v))//i < d->Size())//changed from <=
	{
		//d->Iterate(i,key,v);
		map[key.CString()] = ToVariant(v);
	}
	return map;
}
/// Releases this event listener instancer.
void RocketEventListenerInstancer::Release()
{
//	GetContext()->RemoveSubsystem<RocketEventListener>();
}

void RocketEventListenerInstancer::Execute(::Rocket::Core::EventListener* l, ::Rocket::Core::Event& event, const Urho3D::String& call)
{
	if (executeAngelScript_)
	{
//		::Rocket::Core::String str = "";
//		stream->Read(str,stream->Length());
////				URHO3D_LOGRAW(ToString("%s:\n%s", source_name.CString(),str.CString()));
//		///TODO: Add a type check for the scripts to be passed to this function in native libRocket so that it can use text/angelscript vs text/luascript vs ...
//		if (source_name.Substring(source_name.Length()-3) == ".as" || source_name.Empty())
//		{
//			Urho3D::ScriptFile* script = nullptr;
//			if (!(script = context_->GetSubsystem<Urho3D::ResourceCache>()->GetResource<Urho3D::ScriptFile>(source_name.CString())))
//		{
//			Urho3D::MemoryBuffer buffer(str.CString(),str.Length());
//			script = new ScriptFile(context_);
//			script->Load(buffer);
//			script->SetName(source_name.CString());
////					if (script->GetName())
//			context_->GetSubsystem<ResourceCache>()->AddManualResource(script);
//		}
//
//			script->Execute("void Start()");
//			URHO3D_LOGRAW("DOne\n");
//		}
//		RocketEventListener* rl = (RocketEventListener*)l;
//		if (rl->GetContext())
//		{
//			if (rl->GetContext()->GetFunction(call))
//				rl->GetContext()->Execute(call);
//			else
//			{
////				CompileFunction(const char *sectionName, const char *code, int lineOffset, asDWORD compileFlags, asIScriptFunction **outFunc) = 0;
//				asIScriptFunction* outFunc = 0;
//				//todo: unique names from call hash (i.e. void __########() so that they can be reused
//				rl->GetContext()->GetScriptModule()->CompileFunction("", (Urho3D::String("void ___() { ") + call + "}").CString(), -1, 0, &outFunc);
//				if (outFunc)
//					rl->GetContext()->Execute(outFunc);
//			}
//		}
//		else
//			context_->GetSubsystem<Urho3D::Script>()->Execute(call);
		Urho3D::ScriptFile* sf = ((Urho3D::RocketDocument*)event.GetTargetElement()->GetOwnerDocument())->GetScript();
		if (sf)
		{
			if (sf->GetFunction(call))
				sf->Execute(call);
			else
			{
//				CompileFunction(const char *sectionName, const char *code, int lineOffset, asDWORD compileFlags, asIScriptFunction **outFunc) = 0;
				asIScriptFunction* outFunc = 0;
				//todo: unique names from call hash (i.e. void __########() so that they can be reused
				sf->GetScriptModule()->RemoveFunction(sf->GetFunction("void ___()"));
				sf->GetScriptModule()->CompileFunction("", (Urho3D::String("void ___() { ") + call + "\n;}").CString(), -1, 0, &outFunc);
				if (outFunc)
					sf->Execute(outFunc);
			}
		}
		else
			context_->GetSubsystem<Urho3D::Script>()->Execute(call);
	}
	if (executeLuaScript_)
	{

	}
	if (sendUrhoEvents_)
	{
		Urho3D::String name(event.GetType().CString());

//		//Print Event data
		Urho3D::String phase((event.GetPhase() == ::Rocket::Core::Event::PHASE_BUBBLE) ? "bubble" : (event.GetPhase() == ::Rocket::Core::Event::PHASE_TARGET) ? "target" : "capture");
		URHO3D_LOGRAW(Urho3D::ToString("%s_%s @ *%x: \n\tdo: %s", name.CString(), phase.CString(), event.GetCurrentElement(), call.CString()));
		int i = 0;
		::Rocket::Core::String key;
		::Rocket::Core::Variant* v;
		while (event.GetParameters()->Iterate(i,key,v))//i<= event.GetParameters()->Size())//changed from <=
		{
			//event.GetParameters()->Iterate(i,key,v);
			URHO3D_LOGRAW(Urho3D::ToString("\n\t%s=[%c]", key.CString(), v ? v->GetType() : ' '));
			//if (v->GetType() == Rocket::Core::Variant::Type::STRING)
			//	URHO3D_LOGRAW(Urho3D::ToString("%s", v->Get<Rocket::Core::String>().CString()));
			URHO3D_LOGRAW(ToVariant(v).ToString());
		}
		URHO3D_LOGRAW("\n");

		//Send the event to be handled by Urho Components
		Urho3D::VariantMap eventData = ToVariantMap(event.GetParameters());
		eventData["Element"] = (void *) event.GetCurrentElement();
		eventData["Phase"] = (int) event.GetPhase();
		eventData["Event"] = name;
		SendEvent("RocketUIEvent", eventData);
		if (eventData.Contains("StopPropagation"))
			if (eventData["StopPropagation"].GetBool() || eventData["StopPropagation"].GetInt())
				event.StopPropagation();
	}
}
/// Process the incoming Event


RocketEventListener::RocketEventListener(const ::Rocket::Core::String& str, Urho3D::ScriptFile* sf, RocketEventListenerInstancer* parent): eventCall_(str.CString()), parent_(parent), scriptContext_(sf)
{
	// TODO Auto-generated constructor stub

}

RocketEventListener::~RocketEventListener()
{
	// TODO Auto-generated destructor stub
}

void RocketEventListener::ProcessEvent(::Rocket::Core::Event& event)
{
	parent_->Execute(this, event,eventCall_);
}

/// Called when the listener has been attached to a new Element
void RocketEventListener::OnAttach(::Rocket::Core::Element* element)
{
	if (!element)
		return;
	URHO3D_LOGINFO(Urho3D::String("Attaching to element ") + element->GetTagName().CString());
}

/// Called when the listener has been detached from a Element
void RocketEventListener::OnDetach(::Rocket::Core::Element* element)
{
	if (!element)
		return;
	URHO3D_LOGINFO(Urho3D::String("Detaching from element ") + element->GetTagName().CString());
	delete this;
}

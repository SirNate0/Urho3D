/*
 * RocketEventListener.h
 *
 *  Created on: Jan 9, 2016
 *      Author: nathan
 */

#pragma once

#include "../Core/Object.h"
#include <Rocket/Core/EventListenerInstancer.h>
#include <Rocket/Core/EventListener.h>
#include "../AngelScript/ScriptFile.h"

//using namespace Rocket::Core;

class RocketEventListenerInstancer: public Urho3D::Object, public ::Rocket::Core::EventListenerInstancer//, EventInstancer
{
public:
	URHO3D_OBJECT(RocketEventListenerInstancer, Urho3D::Object);
	virtual ~RocketEventListenerInstancer();
	static RocketEventListenerInstancer* Register(Urho3D::Context* context);

	/// Instance an event listener object.
	/// @param value Value of the event.
	/// @param element Element that triggers the events.
	virtual ::Rocket::Core::EventListener* InstanceEventListener(const ::Rocket::Core::String& value, ::Rocket::Core::Element* element);

	/// Releases this event listener instancer.
	virtual void Release();

	void Execute(::Rocket::Core::EventListener* l, ::Rocket::Core::Event& e, const Urho3D::String& call);// = Urho3D::String::EMPTY);

protected:
	RocketEventListenerInstancer(Urho3D::Context* context);
	bool executeAngelScript_;
	bool executeLuaScript_;
	bool sendUrhoEvents_;
};

class RocketEventListener: public ::Rocket::Core::EventListener
{
public:
	URHO3D_OBJECT(RocketEventListener, Urho3D::Object);
	RocketEventListener(const ::Rocket::Core::String& context, Urho3D::ScriptFile* sf, RocketEventListenerInstancer* parent);
	virtual ~RocketEventListener();
//	static void Register(Urho3D::Context* context);

	/// Process the incoming Event
	virtual void ProcessEvent(::Rocket::Core::Event& event);

	/// Called when the listener has been attached to a new Element
	virtual void OnAttach(::Rocket::Core::Element* element);

	/// Called when the listener has been detached from a Element
	virtual void OnDetach(::Rocket::Core::Element* element);

	virtual Urho3D::ScriptFile* GetContext() { return scriptContext_.Get(); }


protected:
	Urho3D::String eventCall_;
	RocketEventListenerInstancer* parent_;
	Urho3D::SharedPtr<Urho3D::ScriptFile> scriptContext_;
};


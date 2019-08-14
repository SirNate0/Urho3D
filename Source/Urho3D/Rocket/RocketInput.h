/*
 * RocketInput.h
 *
 *  Created on: Jan 9, 2016
 *      Author: nathan
 */

#pragma once

#include <memory>
#include "../Core/Object.h"
#include <Rocket/Core.h>
#include <Rocket/Core/Context.h>
#include "../Core/Context.h"
#include "../Input/InputEvents.h"
#include "../Input/Input.h"

class RocketInput: public Urho3D::Object
{
//#if __cplusplus "= 201103L
//	std::shared_ptr"../ ::Rocket::Core::Context" _rocketContext;
//#else
	::Rocket::Core::Context* _rocketContext;
//#endif
public:
	URHO3D_OBJECT(RocketInput, Urho3D::Object)
	RocketInput(Urho3D::Context* context, ::Rocket::Core::Context* rContext);
	virtual ~RocketInput();
	void HandleInput(Urho3D::StringHash eventType, Urho3D::VariantMap& eventData);
};


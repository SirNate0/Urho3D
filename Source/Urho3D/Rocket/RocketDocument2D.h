#ifndef _ROCKET_ROCKETDOCUMENT2D_H
#define _ROCKET_ROCKETDOCUMENT2D_H

#pragma once

//#define _ __cplusplus
//#warning __cplusplus
//#define __cplusplus 201103L
#include <memory>
#include <unordered_map>
//#undef __cplusplus
//#define __cplusplus _
#include <Rocket/Core/Context.h>
#include <Rocket/Core/ElementDocument.h>

#include "../Scene/Component.h"


namespace Urho3D
{
	/*namespace Rocket
	{*/
		class RocketDocument2D : public Urho3D::Component
		{
			URHO3D_OBJECT(RocketDocument2D, Urho3D::Component);
		public:
			RocketDocument2D(Urho3D::Context* context);
			~RocketDocument2D();

			static void RegisterObject(Urho3D::Context* context);

			void AddReference();
			void RemoveReference();

			void LoadRML(const Urho3D::String& filename);
//			void SaveRML(
			void SetVisible(bool show);
			bool GetVisible() const { return visible_; }

			::Rocket::Core::ElementDocument* GetRocketDocument() { return document_; }


		protected:

//			void HandleUpdate(Urho3D::StringHash eventType, Urho3D::VariantMap& eventData);
//			void HandlePostRender(Urho3D::StringHash eventType, Urho3D::VariantMap& eventData);

		private:

//			std::shared_ptr<::Rocket::Core::Context> _rocketContext;
//			::Rocket::Core::Context* _rocketContext;

//			SharedPtr<RocketInput> input;
			::Rocket::Core::ElementDocument* document_;
			bool visible_;
//			int i;
		};
	/*}*/
}

#endif

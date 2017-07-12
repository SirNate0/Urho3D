#ifndef _ROCKET_ROCKETDOCUMENT_H
#define _ROCKET_ROCKETDOCUMENT_H

#pragma once

#include <Rocket/Core/ElementDocument.h>
#include <Rocket/Core/ElementInstancer.h>
#include <Scene/Component.h>
#include <AngelScript/ScriptFile.h>
#include <IO/VectorBuffer.h>

namespace Urho3D
{
	/*namespace Rocket
	{
		namespace Core
		{*/
			class RocketDocumentElementInstancer : public ::Rocket::Core::ElementInstancer
			{
				/// Instances an element given the tag name and attributes.
				/// @param[in] parent The element the new element is destined to be parented to.
				/// @param[in] tag The tag of the element to instance.
				/// @param[in] attributes Dictionary of attributes.
				virtual ::Rocket::Core::Element* InstanceElement(::Rocket::Core::Element* parent, const ::Rocket::Core::String& tag, const ::Rocket::Core::XMLAttributes& attributes);
				/// Releases an element instanced by this instancer.
				/// @param[in] element The element to release.
				virtual void ReleaseElement(::Rocket::Core::Element* element);
				/// Release the instancer.
				virtual void Release();
			};

			class RocketDocument : public Urho3D::Object, public ::Rocket::Core::ElementDocument
			{
				URHO3D_OBJECT(RocketDocument,Urho3D::Object);
			public:
				RocketDocument(Urho3D::Context* context);
				~RocketDocument();
			    RocketDocument(const ::Rocket::Core::String& tag);
			    void SetContext(Urho3D::Context* context);
			    virtual void LoadScript(::Rocket::Core::Stream* stream, const ::Rocket::Core::String& source_name);
			    Urho3D::ScriptFile* GetScript() const { return script_.Get(); }
				static void RegisterObject(Urho3D::Context* context);

				void AddReference();
				void RemoveReference();
			protected:
			private:
				static WeakPtr<Urho3D::Context> globalContext;
				SharedPtr<Urho3D::ScriptFile> script_;
//				Urho3D::VectorBuffer buffer_;
				PODVector<unsigned char> buffer_;
			};
		/*}
	}*/
}

#endif

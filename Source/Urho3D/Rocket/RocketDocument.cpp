#include "RocketDocument.h"

#include <Core/Context.h>
#include <Rocket/Core/Factory.h>
#include <IO/Log.h>
#include <AngelScript/ScriptFile.h>
#include <IO/MemoryBuffer.h>
#include <Resource/ResourceCache.h>
namespace Urho3D
{
		/// Instances an element given the tag name and attributes.
		/// @param[in] parent The element the new element is destined to be parented to.
		/// @param[in] tag The tag of the element to instance.
		/// @param[in] attributes Dictionary of attributes.
		::Rocket::Core::Element* RocketDocumentElementInstancer::InstanceElement(::Rocket::Core::Element* parent, const ::Rocket::Core::String& tag, const ::Rocket::Core::XMLAttributes& attributes)
		{
		    return new RocketDocument(tag);
		}
		/// Releases an element instanced by this instancer.
		/// @param[in] element The element to release.
		void RocketDocumentElementInstancer::ReleaseElement(::Rocket::Core::Element* element)
		{
		    delete element;
		}
		/// Release the instancer.
		void RocketDocumentElementInstancer::Release()
		{
		    delete this;
		}

		Urho3D::WeakPtr<Urho3D::Context> RocketDocument::globalContext = nullptr;

			RocketDocument::RocketDocument(Urho3D::Context* context) :
				Object(context),
				::Rocket::Core::ElementDocument("")
			{
			}
		    RocketDocument::RocketDocument(const ::Rocket::Core::String& tag): Object(globalContext), ::Rocket::Core::ElementDocument(tag), script_(new Urho3D::ScriptFile(globalContext)), buffer_()
		    {
		    	//add a script file for the rocket document. The * are to help keep it from conflicting from user's names
				if (this->GetSourceURL().Empty())
				{
					static int uniqueScriptIndex = 0;
					script_->SetName(ToString("*__ROCKET_script_%d.as*",++uniqueScriptIndex));
				}
				else
					script_->SetName(("*__" + this->GetSourceURL() + "_script.as*").CString());
				//add it to the ResourceCache
				context_->GetSubsystem<ResourceCache>()->AddManualResource(script_);
				AddReference();
		    }

			RocketDocument::~RocketDocument()
			{
				//if this is the RocketDocument subsystem, remove the subsystem so we don't have a dangling pointer/extra reference
				if (context_ && context_->GetSubsystem<RocketDocument>() == this)
				{
					AddRef();
					context_->RemoveSubsystem<RocketDocument>();
				}
				//remove the script from the resource cache
				if (context_ && context_->GetSubsystem<ResourceCache>())
					context_->GetSubsystem<ResourceCache>()->ReleaseResource(script_->GetTypeStatic(),script_->GetName(),false);

			}

			void RocketDocument::RegisterObject(Urho3D::Context* context)
			{
//				context->RegisterFactory<RocketDocument>();
				globalContext = context;
				::Rocket::Core::Factory::RegisterElementInstancer("body", new RocketDocumentElementInstancer())->RemoveReference();
				//todo: script interface
			}

			void RocketDocument::AddReference()
			{
				AddRef();
			}

			void RocketDocument::RemoveReference()
			{
				ReleaseRef();
			}
			void RocketDocument::LoadScript(::Rocket::Core::Stream* stream, const ::Rocket::Core::String& source_name)
			{
				///note: it is possible that instead of loading the script, I should simply compose a single Angelscript file of the whole script from the document
				::Rocket::Core::String str = "";
				stream->Read(str,stream->Length());
//				URHO3D_LOGRAW(ToString("%s:\n%s", source_name.CString(),str.CString()));
				///TODO: Add a type check for the scripts to be passed to this function in native libRocket so that it can use text/angelscript vs text/luascript vs ...
//				if (source_name.Substring(source_name.Length()-3) == ".as" || source_name.Empty())
//				{
//					Urho3D::ScriptFile* script = nullptr;
//					if (!(script = context_->GetSubsystem<Urho3D::ResourceCache>()->GetResource<Urho3D::ScriptFile>(source_name.CString())))
//					{
//						Urho3D::MemoryBuffer buffer(str.CString(),str.Length());
//						script = new ScriptFile(context_);
//						script->Load(buffer);
//						script->SetName(source_name.CString());
//	//					if (script->GetName())
//						context_->GetSubsystem<ResourceCache>()->AddManualResource(script);
//					}
//
//					script->Execute("void Start()");
//
//				}
				//no need to actually check for the resource, we can just read the actuall script file through rocket anywasy
//				if (source_name.Substring(source_name.Length()-3) == ".as" || source_name.Substring(source_name.Length()-4) == ".asc")
//				{
////					buffer_ += PODVector<unsigned char>((const unsigned char*)str.CString(), str.Length());
////					URHO3D_LOGERROR(GetSubsystem<ResourceCache>()->GetResource<ScriptFile>(source_name.CString())->GetName());
//					String s = "\n#include \"" + GetSubsystem<ResourceCache>()->SanitateResourceName(source_name.CString()) + "\"\n";
//					buffer_ += PODVector<unsigned char>((const unsigned char*)s.CString(), s.Length());
//					VectorBuffer b(buffer_);
//					script_->Load(b);
//				}
//				else
				{
					buffer_ += PODVector<unsigned char>((const unsigned char*)str.CString(), str.Length());
					VectorBuffer b(buffer_);
					script_->Load(b);
				}
				URHO3D_LOGINFO(ToString("%s:",source_name.CString()));
				URHO3D_LOGRAW(String((char *)&(buffer_[0]),buffer_.Size()));



			    //if it is loaded from a file
//			    if(source_name != "")
//			    {
//			        Interpreter::LoadFile(source_name);
//			    }
//			    else
//			    {
//			        String buffer = "";
//			        stream->Read(buffer,stream->Length()); //just do the whole thing
//			        Interpreter::DoString(buffer, this->GetSourceURL());
//			    }
                        }
}

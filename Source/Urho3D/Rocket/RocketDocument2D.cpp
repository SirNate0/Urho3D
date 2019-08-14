#include "RocketDocument2D.h"


#include <Urho3D.h>
#include <Rocket/Core.h>
#include <Core/Context.h>
//#include <Core/CoreEvents.h>
#include <Graphics/Graphics.h>
//#include <Graphics/GraphicsEvents.h>
#include <Scene/Node.h>
#include <Resource/ResourceCache.h>
#include <IO/Log.h>
#include <Rocket/Debugger.h>
#include <Rocket/Rocket.h>
namespace Urho3D
{
		RocketDocument2D::RocketDocument2D(Urho3D::Context* context) :
			Component(context), document_(nullptr), visible_(false)
		{
			
		}

		RocketDocument2D::~RocketDocument2D()
		{
//			if (_rocketContext)
//				delete _rocketContext;
		}


		void RocketDocument2D::LoadRML(const Urho3D::String& filename)
		{
//			document_ = _rocketContext->LoadDocument(filename.CString());//rocketDemo.html");
			if (GetSubsystem<RocketSys>())
				document_ = GetSubsystem<RocketSys>()->GetRocketContext()->LoadDocument(filename.CString());
			else
			{
				document_ = nullptr;
				return;
			}
			
			if (document_ && visible_)
			{
				document_->Show();
				//Document->RemoveReference();
			};
//			SubscribeToEvent(Urho3D::E_UPDATE, URHO3D_HANDLER(RocketDocument2D, HandleUpdate));
//			SubscribeToEvent(Urho3D::E_ENDRENDERING, URHO3D_HANDLER(RocketDocument2D, HandlePostRender));

		}
		void RocketDocument2D::SetVisible(bool show)
		{
			if (show)
			{
				if (document_)
					document_->Show();
//				SubscribeToEvent(Urho3D::E_UPDATE, URHO3D_HANDLER(RocketDocument2D, HandleUpdate));
//				SubscribeToEvent(Urho3D::E_ENDRENDERING, URHO3D_HANDLER(RocketDocument2D, HandlePostRender));
			}
			else
			{
				if (document_)
					document_->Hide();
//				UnsubscribeFromAllEvents();
			}
			visible_ = show;
				
		}

		void RocketDocument2D::RegisterObject(Urho3D::Context* context)
		{
			context->RegisterFactory<RocketDocument2D>();
		}

		void RocketDocument2D::AddReference()
		{
			AddRef();
		}

		void RocketDocument2D::RemoveReference()
		{
			ReleaseRef();
		}

		

		/*void RocketDocument2D::HandleUpdate(Urho3D::StringHash eventType, Urho3D::VariantMap& eventData)
		{      
//			i=0;
			_rocketContext->Update();
		}

		void RocketDocument2D::HandlePostRender(Urho3D::StringHash eventType, Urho3D::VariantMap &eventData)
		{
			_rocketContext->Render();
//			URHO3D_LOGERROR(String(i));
		}*/

}

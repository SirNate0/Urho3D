
#include "../Precompiled.h"

/*#include "../AngelRocket/Addons.h"
#include "../AngelRocket/Rocket.h"
#include "../AngelRocket/RocketAPI.h"
#include "../AngelRocket/RocketFile.h"
#include "../AngelRocket/RocketInstance.h"
#include "../Core/Profiler.h"
#include "../Engine/EngineEvents.h"
#include "../IO/FileSystem.h"
#include "../IO/Log.h"
#include "../Resource/ResourceCache.h"
#include "../Scene/Scene.h"*/

#include "../Core/CoreEvents.h"
#include "../Graphics/GraphicsEvents.h"
#include "../Graphics/Graphics.h"

#include "../Rocket/Rocket.h"

#include <Rocket/Core/Context.h>
#include <Rocket/Core/Core.h>
#include <Rocket/Controls.h>
#include <Scene/Node.h>
#include <Resource/ResourceCache.h>
#include <IO/Log.h>
#include <Rocket/Debugger.h>

#include "../DebugNew.h"

namespace Urho3D
{

RocketSys::RocketSys(Context* context) :
    Object(context), scissor(false)
{
    context->GetSubsystem<Urho3D::Input>()->SetMouseVisible(true);

    /*::Urho3D::RenderInterface::RegisterObject(context);
    ::Urho3D::SystemInterface::RegisterObject(context);
    ::Urho3D::RocketDocument::RegisterObject(context);
    ::Urho3D::Rocket::RocketSys::RegisterObject(context);*/
    ::Urho3D::RenderInterface::RegisterObject(context);
    ::Urho3D::SystemInterface::RegisterObject(context);
    ::Urho3D::RocketDocument::RegisterObject(context);
    ::Urho3D::RocketDocument2D::RegisterObject(context);
//    ::Urho3D::Rocket::RocketDocument3D::RegisterObject(context);

    // Initialize Rocket
	systemInterface_ = SharedPtr<Urho3D::SystemInterface>(new Urho3D::SystemInterface(context));
	context->RegisterSubsystem(systemInterface_.Get());
    ::Rocket::Core::SetSystemInterface(context->GetSubsystem<Urho3D::SystemInterface>());
	fileInterface_ = SharedPtr<RocketFileInterface>(new RocketFileInterface(context));
	context->RegisterSubsystem(fileInterface_.Get());
    ::Rocket::Core::SetFileInterface(context->GetSubsystem<RocketFileInterface>());

    ::Rocket::Core::Initialise();
    ::Rocket::Controls::Initialise();
	eventListener_ = SharedPtr<RocketEventListenerInstancer>(RocketEventListenerInstancer::Register(context));
    ::Urho3D::RocketDocument::RegisterObject(context);
    //context->RegisterSubsystem(new Urho3D::Script(context));
    Graphics* graphics = context->GetSubsystem<Graphics>();
	_rocketContext = std::shared_ptr< ::Rocket::Core::Context>(
				::Rocket::Core::CreateContext("main", 
				::Rocket::Core::Vector2i(graphics->GetWidth(), graphics->GetHeight()),
				this));

	input = new RocketInput(context, _rocketContext.get());

    SubscribeToEvent(Urho3D::E_UPDATE, URHO3D_HANDLER(RocketSys, HandleUpdate));
    SubscribeToEvent(Urho3D::E_ENDRENDERING, URHO3D_HANDLER(RocketSys, HandlePostRender));

			noTextureVS = graphics->GetShader(Urho3D::VS, "Basic", "VERTEXCOLOR");
			noTexturePS = graphics->GetShader(Urho3D::PS, "Basic", "VERTEXCOLOR");

			diffTextureVS = graphics->GetShader(Urho3D::VS, "Basic", "DIFFMAP VERTEXCOLOR");
			diffTexturePS = graphics->GetShader(Urho3D::PS, "Basic", "DIFFMAP VERTEXCOLOR");

//			noTextureVS = graphics->GetShader(Urho3D::VS, "UI", "VERTEXCOLOR");
//			noTexturePS = graphics->GetShader(Urho3D::PS, "UI", "VERTEXCOLOR");
//
//			diffTextureVS = graphics->GetShader(Urho3D::VS, "UI", "DIFFMAP VERTEXCOLOR");
//			diffTexturePS = graphics->GetShader(Urho3D::PS, "UI", "DIFFMAP VERTEXCOLOR");

			/*_rocketContext = std::shared_ptr<::Rocket::Core::Context>(
				::Rocket::Core::CreateContext("main", 
				::Rocket::Core::Vector2i(graphics->GetWidth(), graphics->GetHeight()),
				this));

			input = new RocketInput(Urho3D::Object::GetContext(), _rocketContext.get());*/

                        ::Rocket::Core::FontDatabase::LoadFontFace("Fonts/Delicious-Bold.otf");
                        ::Rocket::Core::FontDatabase::LoadFontFace("Fonts/Delicious-BoldItalic.otf");
                        ::Rocket::Core::FontDatabase::LoadFontFace("Fonts/Delicious-Italic.otf");
                        ::Rocket::Core::FontDatabase::LoadFontFace("Fonts/Delicious-Roman.otf");
                        ::Rocket::Core::FontDatabase::LoadFontFace("UI/AlegreyaSans-Regular.otf");
                        ::Rocket::Core::FontDatabase::LoadFontFace("UI/CaladeaRegular.ttf");
                        ::Rocket::Core::FontDatabase::LoadFontFace("UI/CaladeaBold.ttf");
                        ::Rocket::Core::FontDatabase::LoadFontFace("UI/CaladeaBoldItalic.ttf");
                        ::Rocket::Core::FontDatabase::LoadFontFace("UI/CaladeaItalic.ttf");
                        ::Rocket::Core::FontDatabase::LoadFontFace("UI/LiberationSerif-Regular.ttf");

                        /*::Rocket::Core::ElementDocument *Document = _rocketContext->LoadDocument("UI/DialogueDemo.html");//rocketDemo.html");

			if (Document)
			{
				Document->Show();
				//Document->RemoveReference();
			};
			SubscribeToEvent(Urho3D::E_UPDATE, URHO3D_HANDLER(RocketSys, HandleUpdate));
			SubscribeToEvent(Urho3D::E_ENDRENDERING, URHO3D_HANDLER(RocketSys, HandlePostRender));
			*/
			///Note: Very Strangely, with my custom ElementDocument, the Debugger has a sigseg related to the style sheet.
//			::Rocket::Debugger::Initialise(_rocketContext.get());
//			::Rocket::Debugger::SetVisible(true);


}

RocketSys::~RocketSys()
{

//	delete _rocketContext;
	_rocketContext.reset();
    ::Rocket::Core::Shutdown();
}

void RocketSys::HandleUpdate(Urho3D::StringHash eventType, Urho3D::VariantMap& eventData)
{      
//			i=0;
    _rocketContext->Update();
}

void RocketSys::HandlePostRender(Urho3D::StringHash eventType, Urho3D::VariantMap &eventData)
{
    _rocketContext->Render();
//			URHO3D_LOGERROR(String(i));
}

// Called by Rocket when it wants to render geometry that it does not wish to optimise.
		void RocketSys::RenderGeometry(::Rocket::Core::Vertex* vertices, int num_vertices, int* indices, int num_indices, const ::Rocket::Core::TextureHandle texture, const ::Rocket::Core::Vector2f& translation)
		{
		}

		// Called by Rocket when it wants to compile geometry it believes will be static for the forseeable future.		
		::Rocket::Core::CompiledGeometryHandle RocketSys::CompileGeometry(::Rocket::Core::Vertex* vertices, int num_vertices, int* indices, int num_indices, const ::Rocket::Core::TextureHandle texture)
		{
			VertexBuffer* vBuff = new VertexBuffer(context_);
			if (texture) {
				vBuff->SetSize(num_vertices, MASK_POSITION | MASK_COLOR | MASK_TEXCOORD1, true);
			}
			else {
				vBuff->SetSize(num_vertices, MASK_POSITION | MASK_COLOR, true);
			}

			float* dest = (float*)vBuff->Lock(0, num_vertices, true);
			for (int i = 0; i < num_vertices; ++i) {
				*dest++ = vertices[i].position.x;
				*dest++ = vertices[i].position.y;
				*dest++ = 0.f;

				Color color(vertices[i].colour.red / 255.f, vertices[i].colour.green / 255.f, vertices[i].colour.blue / 255.f, vertices[i].colour.alpha / 255.f);
				*((unsigned*)dest) = color.ToUInt(); dest++;

				if (texture) {
					*dest++ = vertices[i].tex_coord.x;
					*dest++ = vertices[i].tex_coord.y;
				}
			}
			vBuff->Unlock();

			IndexBuffer*  iBuff = new IndexBuffer(context_);
			iBuff->SetSize(num_indices, true);

			// indices
			unsigned* indices_stream = (unsigned*)iBuff->Lock(0, num_indices, true);
			for (int i = 0; i < num_indices; ++i)
				indices_stream[i] = (unsigned int)indices[i];

			iBuff->Unlock();

			Texture2D* tex = texture ? textures.at(texture - 1).Get() : nullptr;

                        Urho3DCompiledGeometry* geom = new Urho3DCompiledGeometry();
			geom->iBuff = iBuff;
			geom->vBuff = vBuff;
			geom->texture = tex;

			return (::Rocket::Core::CompiledGeometryHandle) geom;
		}

		// Called by Rocket when it wants to render application-compiled geometry.		
		void RocketSys::RenderCompiledGeometry(::Rocket::Core::CompiledGeometryHandle geometry, const ::Rocket::Core::Vector2f& translation)
		{
//			++i;
			Urho3D::Graphics* graphics = GetSubsystem<Urho3D::Graphics>();

                        Urho3DCompiledGeometry* geom = (Urho3DCompiledGeometry*)geometry;

			//This updates the screen dimensions, but is not strictly necessary...
//			_rocketContext->SetDimensions({graphics->GetWidth(), graphics->GetHeight()});
			if (GetSubsystem<RocketSys>())
				GetSubsystem<RocketSys>()->GetRocketContext()->SetDimensions({graphics->GetWidth(), graphics->GetHeight()});

			//Setup projection matrix
			Vector2 invScreenSize(1.0f / (float)graphics->GetWidth(), 1.0f / (float)graphics->GetHeight());
			Vector2 scale(2.0f * invScreenSize.x_, -2.0f * invScreenSize.y_);
			Vector2 offset(-1.0f, 1.0f);

			Matrix4 projection(Matrix4::IDENTITY);
			projection.m00_ = scale.x_;
			projection.m03_ = offset.x_;
			projection.m11_ = scale.y_;
			projection.m13_ = offset.y_;
			projection.m22_ = 1.0f;
			projection.m23_ = 0.0f;
			projection.m33_ = 1.0f;

			//Setup graphics rendering states
			graphics->ClearParameterSources();
			graphics->SetCullMode(CULL_CW);
			graphics->SetDepthTest(CMP_ALWAYS);
			graphics->SetDepthWrite(false);
			graphics->SetStencilTest(false);
			graphics->ResetRenderTargets();
			graphics->SetBlendMode(BLEND_ALPHA);
//			graphics->SetColorWrite(true);

			//Bind buffers
			graphics->SetVertexBuffer(geom->vBuff);
			graphics->SetIndexBuffer(geom->iBuff);

			ShaderVariation* ps = 0;
			ShaderVariation* vs = 0;

			if (geom->texture) {
				graphics->SetTexture(0, geom->texture);
				ps = diffTexturePS;
				vs = diffTextureVS;
			}
			else {
				graphics->SetTexture(0, NULL);
				ps = noTexturePS;
				vs = noTextureVS;
			}

			//Apply translation
			Matrix3x4 trans(Matrix3x4::IDENTITY);
			trans.SetTranslation(Vector3(translation.x, translation.y, 0.f));

			//Pass shader params
			graphics->SetShaders(vs, ps);
//			if (graphics->NeedParameterUpdate(SP_OBJECTTRANSFORM, this))
				if (graphics->NeedParameterUpdate(SP_OBJECT, this))
				graphics->SetShaderParameter(VSP_MODEL, trans);
			if (graphics->NeedParameterUpdate(SP_CAMERA, this))
				graphics->SetShaderParameter(VSP_VIEWPROJ, projection);
			if (graphics->NeedParameterUpdate(SP_MATERIAL, this))
				graphics->SetShaderParameter(PSP_MATDIFFCOLOR, Color(1.0f, 1.0f, 1.0f, 1.0f));

			if (scissor)
				graphics->SetScissorTest(true, scissors);

			//Draw indexed geometry
			graphics->Draw(TRIANGLE_LIST, 0, geom->iBuff->GetIndexCount(), 0, geom->vBuff->GetVertexCount());
		}

		// Called by Rocket when it wants to release application-compiled geometry.		
		void RocketSys::ReleaseCompiledGeometry(::Rocket::Core::CompiledGeometryHandle geometry)
		{
			Urho3D::Graphics* graphics = GetSubsystem<Urho3D::Graphics>();
			if (!graphics)
				return;//hopefully shutting down and all is well.
                        Urho3DCompiledGeometry* geom = (Urho3DCompiledGeometry*)geometry;
			graphics->SetVertexBuffer(0);
			graphics->SetIndexBuffer(0);
//			delete geom->iBuff;
//			delete geom->vBuff;
			geom->iBuff.Reset();
			geom->vBuff.Reset();
			graphics->SetVertexBuffer(0);
			graphics->SetIndexBuffer(0);
			delete geom;
		}

		// Called by Rocket when it wants to enable or disable scissoring to clip content.		
		void RocketSys::EnableScissorRegion(bool enable)
		{
//			Urho3D::Graphics* graphics = GetSubsystem<Urho3D::Graphics>();
//			if (enable)
//				graphics->SetScissorTest(true, scissors);
//			else
//				graphics->SetScissorTest(false);
			scissor = enable;
		}

		// Called by Rocket when it wants to change the scissor region.		
		void RocketSys::SetScissorRegion(int x, int y, int width, int height)
		{
//			width /= 2;
//			height /= 2;
			scissors.left_ = x;
			scissors.top_ = y;
			scissors.bottom_ = y + height;
			scissors.right_ = x + width;
//			Urho3D::Graphics* graphics = GetSubsystem<Urho3D::Graphics>();
//			graphics->SetScissorTest(true, scissors);
		}

		// Called by Rocket when a texture is required by the library.		
		bool RocketSys::LoadTexture(::Rocket::Core::TextureHandle& texture_handle, ::Rocket::Core::Vector2i& texture_dimensions, const ::Rocket::Core::String& source)
		{
			Texture2D* tex = context_->GetSubsystem<Urho3D::ResourceCache>()->GetResource<Urho3D::Texture2D>(source.CString());
			if (tex) {
				texture_dimensions.x = tex->GetWidth();
				texture_dimensions.y = tex->GetHeight();
			}

			textures.push_back(SharedPtr<Texture2D>(tex));
			texture_handle = (::Rocket::Core::TextureHandle)textures.size();   // indexing from 1, 0 means no texture

			return true;
		}

		// Called by Rocket when a texture is required to be built from an internally-generated sequence of pixels.
		bool RocketSys::GenerateTexture(::Rocket::Core::TextureHandle& texture_handle, const ::Rocket::Core::byte* source, const ::Rocket::Core::Vector2i& source_dimensions)
		{
			SharedPtr<Image> img = SharedPtr<Image>(new Image(context_));
			img->SetSize(source_dimensions.x, source_dimensions.y, 4);
			img->SetData(source);
			Texture2D* tex = new Texture2D(context_);
			tex->SetData(img);

			textures.push_back(SharedPtr<Texture2D>(tex));
			texture_handle = (::Rocket::Core::TextureHandle)textures.size();		// indexing from 1, 0 means no texture

			return true;
		}

		// Called by Rocket when a loaded texture is no longer required.		
		void RocketSys::ReleaseTexture(::Rocket::Core::TextureHandle texture_handle)
		{
			textures.at(texture_handle - 1).Reset();
		}

		float RocketSys::GetHorizontalTexelOffset()
		{
#ifdef URHO3D_OPENGL //USE_OPENGL
			return 0.f;
#else
			return 0.5f;
#endif
		}

		float RocketSys::GetVerticalTexelOffset()
		{
#ifdef URHO3D_OPENGL //USE_OPENGL
			return 0.f;
#else
			return 0.5f;
#endif
		}

}



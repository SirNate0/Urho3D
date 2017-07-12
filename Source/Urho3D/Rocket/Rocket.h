//
// Copyright (c) 2008-2016 the Urho3D project.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//

#pragma once

#include "../Core/Mutex.h"
#include "../Core/Object.h"

#include "../Rocket/RenderInterface.h"
#include "../Rocket/SystemInterface.h"
#include "../Rocket/RocketDocument.h"
#include "../Rocket/RocketDocument2D.h"
//#include "../Rocket/RocketDocument3D.h"
#include "../Rocket/RocketEventListener.h"
#include "RenderInterface.h"
#include "SystemInterface.h"
#include "RocketInput.h"
#include "../Rocket/FileInterface.h"

namespace Urho3D
{
//TODO: change RocketSys to Rocket by removing the Urho3D::Rocket namespace
/// Rocket UI subsystem. Light interface for creating the Rocket context and such.
class URHO3D_API RocketSys : public Object, public ::Rocket::Core::RenderInterface
{
    URHO3D_OBJECT(RocketSys, Object);

//    friend class RocketFile;
public:
    /// Construct.
    RocketSys(Context* context);
    /// Destruct. Release the AngelRocket engine.
    ~RocketSys();

	::Rocket::Core::Context* GetRocketContext() const { return _rocketContext.get(); }
    RocketInput* GetRocketInput() const { return input.Get(); }

			/// Called by Rocket when it wants to render geometry that it does not wish to optimise.
			virtual void RenderGeometry(::Rocket::Core::Vertex* vertices, int num_vertices, int* indices, int num_indices, ::Rocket::Core::TextureHandle texture, const ::Rocket::Core::Vector2f& translation);

			/// Called by Rocket when it wants to compile geometry it believes will be static for the forseeable future.
			virtual ::Rocket::Core::CompiledGeometryHandle CompileGeometry(::Rocket::Core::Vertex* vertices, int num_vertices, int* indices, int num_indices, ::Rocket::Core::TextureHandle texture);

			/// Called by Rocket when it wants to render application-compiled geometry.
			virtual void RenderCompiledGeometry(::Rocket::Core::CompiledGeometryHandle geometry, const ::Rocket::Core::Vector2f& translation);
			/// Called by Rocket when it wants to release application-compiled geometry.
			virtual void ReleaseCompiledGeometry(::Rocket::Core::CompiledGeometryHandle geometry);

			/// Called by Rocket when it wants to enable or disable scissoring to clip content.
			virtual void EnableScissorRegion(bool enable);
			/// Called by Rocket when it wants to change the scissor region.
			virtual void SetScissorRegion(int x, int y, int width, int height);

			/// Called by Rocket when a texture is required by the library.
			virtual bool LoadTexture(::Rocket::Core::TextureHandle& texture_handle, ::Rocket::Core::Vector2i& texture_dimensions, const ::Rocket::Core::String& source);
			/// Called by Rocket when a texture is required to be built from an internally-generated sequence of pixels.
			virtual bool GenerateTexture(::Rocket::Core::TextureHandle& texture_handle, const ::Rocket::Core::byte* source, const ::Rocket::Core::Vector2i& source_dimensions);
			/// Called by Rocket when a loaded texture is no longer required.
			virtual void ReleaseTexture(::Rocket::Core::TextureHandle texture_handle);

			/// Returns the native horizontal texel offset for the renderer.
			/// @return The renderer's horizontal texel offset. The default implementation returns 0.
			virtual float GetHorizontalTexelOffset();
			/// Returns the native vertical texel offset for the renderer.
			/// @return The renderer's vertical texel offset. The default implementation returns 0.
			virtual float GetVerticalTexelOffset();

private:
    void HandleUpdate(Urho3D::StringHash eventType, Urho3D::VariantMap& eventData);
    void HandlePostRender(Urho3D::StringHash eventType, Urho3D::VariantMap& eventData);

	std::shared_ptr< ::Rocket::Core::Context> _rocketContext;
//    ::Rocket::Core::Context* _rocketContext;
	SharedPtr<RocketInput> input;
	SharedPtr<RocketEventListenerInstancer> eventListener_;
	SharedPtr<Urho3D::SystemInterface> systemInterface_;
	SharedPtr<RocketFileInterface> fileInterface_;
			std::vector<SharedPtr<Texture2D> > textures;
//			std::vector<Texture2D*> textures;
//			Vector<Texture2D*> textures;
			IntRect scissors;
			bool scissor;

			SharedPtr<ShaderVariation> noTextureVS;
			SharedPtr<ShaderVariation> noTexturePS;
			SharedPtr<ShaderVariation> diffTextureVS;
			SharedPtr<ShaderVariation> diffTexturePS;


};

}


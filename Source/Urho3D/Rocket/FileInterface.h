#pragma once

#include <Rocket/Core/FileInterface.h>

#include "../Core/Object.h"
#include "../Container/Vector.h"
namespace Urho3D
{
	class File;
	class Context;

class RocketFileInterface: public Urho3D::Object, public ::Rocket::Core::FileInterface
{
public:
	URHO3D_OBJECT(RocketFileInterface, Urho3D::Object);

	RocketFileInterface(Urho3D::Context* context);

	// Opens a file.
	virtual Rocket::Core::FileHandle Open(const Rocket::Core::String& path);

	// Closes a previously opened file.
	virtual void Close(Rocket::Core::FileHandle file);

	// Reads data from a previously opened file.
	virtual size_t Read(void* buffer, size_t size, Rocket::Core::FileHandle file);

	// Seeks to a point in a previously opened file.
	virtual bool Seek(Rocket::Core::FileHandle file, long offset, int origin);

	// Returns the current position of the file pointer.
	virtual size_t Tell(Rocket::Core::FileHandle file);
private:
	//Vector<SharedPtr<File> > files_;
	Urho3D::Vector<Urho3D::File* > files_;
};

}

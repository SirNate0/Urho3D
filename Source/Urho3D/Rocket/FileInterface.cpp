#include "../Rocket/FileInterface.h"

#include "../Resource/ResourceCache.h"
#include "../IO/File.h"
#include "../Container/Ptr.h"

namespace Urho3D
{

RocketFileInterface::RocketFileInterface(Urho3D::Context* context): Urho3D::Object(context)
{}

Rocket::Core::FileHandle RocketFileInterface::Open(const Rocket::Core::String& path)
{
	Urho3D::ResourceCache* c = GetSubsystem<Urho3D::ResourceCache>();
	Urho3D::SharedPtr<Urho3D::File> file = c->GetFile(path.CString(), false);
	if (!file)
		return 0;
//	files_.Push(file);
	file->AddRef();
	files_.Push(file.Get());
	return reinterpret_cast<intptr_t>(file.Get());
}

// Closes a previously opened file.
void RocketFileInterface::Close(Rocket::Core::FileHandle file)
{
	if (file == 0)
		return;
	//SharedPtr<File> f(file);
	//files_.Remove(f);
	((Urho3D::File*)file)->ReleaseRef();
	files_.Remove((Urho3D::File*)file);
}

// Reads data from a previously opened file.
size_t RocketFileInterface::Read(void* buffer, size_t size, Rocket::Core::FileHandle file)
{
	//if (files_.Contains(SharedPtr<File>(file))
	//	return files_.Find(SharedPtr<File>(file))Read (void *dest, unsigned size)
	if (file)
		return ((Urho3D::File*)file)->Read(buffer,size);
	else
		return 0;
}

// Seeks to a point in a previously opened file.
bool RocketFileInterface::Seek(Rocket::Core::FileHandle filehandle, long offset, int origin)
{
	if (!filehandle)
		return false;
	Urho3D::File* file = (Urho3D::File*) filehandle;
	switch(origin)
	{
	case SEEK_SET:
		if (offset >= file->GetSize())
			return false;//? what does Seek do with invalid position?
		file->Seek(offset);
		return true;
	case SEEK_CUR:
		offset += file->GetPosition();
		if (offset >= file->GetSize())
			return false;//? what does Seek do with invalid position?
		file->Seek(offset);
		return true;
	case SEEK_END:
		if (offset >= file->GetSize())
			return false;//? what does Seek do with invalid position?
		offset = file->GetSize() - offset - 1;//is that right?
		file->Seek(offset);
		return true;
	}
}

// Returns the current position of the file pointer.
size_t RocketFileInterface::Tell(Rocket::Core::FileHandle file)
{
	if (!file)
		return -1;
	return ((Urho3D::File*)file)->GetPosition();
}

}

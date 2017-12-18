//
// Copyright (c) 2008-2017 the Urho3D project.
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

#include "../Precompiled.h"

#include "../Core/Profiler.h"
#include "../IO/File.h"
#include "../IO/FileSystem.h"
#include "../IO/Log.h"
#include "../IO/MemoryBuffer.h"
#include "../IO/PackageFile.h"
#include "../IO/PackedFile.h"

#ifdef __ANDROID__
#include <SDL/SDL_rwops.h>
#endif

#include <cstdio>
#include <LZ4/lz4.h>

#include "../DebugNew.h"

namespace Urho3D
{


#ifdef __ANDROID__
const char* APK = "/apk/";
static const unsigned READ_BUFFER_SIZE = 32768;
#endif
static const unsigned SKIP_BUFFER_SIZE = 1024;

PackedFile::PackedFile(Context* context) :
    File(context),
    handle_(nullptr),
#ifdef __ANDROID__
    assetHandle_(0),
#endif
    readBufferOffset_(0),
    readBufferSize_(0),
    offset_(0),
    checksum_(0),
    compressed_(false),
    readSyncNeeded_(false)
{
}

PackedFile::PackedFile(Context *context, const String &fileName, FileMode mode) :
    File(context),
    handle_(nullptr),
#ifdef __ANDROID__
    assetHandle_(0),
#endif
    readBufferOffset_(0),
    readBufferSize_(0),
    offset_(0),
    checksum_(0),
    compressed_(false),
    readSyncNeeded_(false)
{
    Open(fileName, mode);
}

PackedFile::PackedFile(Context* context, FileSource* package, const String& fileName, FileMode mode) :
    File(context),
    handle_(nullptr),
#ifdef __ANDROID__
    assetHandle_(0),
#endif
    readBufferOffset_(0),
    readBufferSize_(0),
    offset_(0),
    checksum_(0),
    compressed_(false),
    readSyncNeeded_(false)
{
    Open(package, fileName, mode);
}

PackedFile::~PackedFile()
{
    Close();
}


bool PackedFile::Open(FileSource *source, const String &fileName, FileMode mode)
{
    if (!source)
        return false;

    if (!source->IsInstanceOf(PackageFile::GetTypeStatic()))
    {
        URHO3D_LOGERROR("Could not open packed file " + fileName + " from non-PackageFile source");
        return false;
    }

    PackageFile* package = (PackageFile*)(source);

    const PackageEntry* entry = package->GetEntry(fileName);
    if (!entry)
        return false;

    bool success = OpenInternal(package->GetName());
    if (!success)
    {
        URHO3D_LOGERROR("Could not open package file " + fileName);
        return false;
    }

    fileName_ = fileName;
    offset_ = entry->offset_;
    checksum_ = entry->checksum_;
    size_ = entry->size_;
    compressed_ = package->IsCompressed();

    // Seek to beginning of package entry's file data
    SeekInternal(offset_);
    return true;
}

unsigned PackedFile::Read(void* dest, unsigned size)
{
    if (!IsOpen())
    {
        // If file not open, do not log the error further here to prevent spamming the stderr stream
        return 0;
    }

    if (mode_ == FILE_WRITE)
    {
        URHO3D_LOGERROR("PackedFile not opened for reading");
        return 0;
    }

    if (size + position_ > size_)
        size = size_ - position_;
    if (!size)
        return 0;

#ifdef __ANDROID__
    if (assetHandle_ && !compressed_)
    {
        // If not using a compressed package file, buffer file reads on Android for better performance
        if (!readBuffer_)
        {
            readBuffer_ = new unsigned char[READ_BUFFER_SIZE];
            readBufferOffset_ = 0;
            readBufferSize_ = 0;
        }

        unsigned sizeLeft = size;
        unsigned char* destPtr = (unsigned char*)dest;

        while (sizeLeft)
        {
            if (readBufferOffset_ >= readBufferSize_)
            {
                readBufferSize_ = Min(size_ - position_, READ_BUFFER_SIZE);
                readBufferOffset_ = 0;
                ReadInternal(readBuffer_.Get(), readBufferSize_);
            }

            unsigned copySize = Min((readBufferSize_ - readBufferOffset_), sizeLeft);
            memcpy(destPtr, readBuffer_.Get() + readBufferOffset_, copySize);
            destPtr += copySize;
            sizeLeft -= copySize;
            readBufferOffset_ += copySize;
            position_ += copySize;
        }

        return size;
    }
#endif

    if (compressed_)
    {
        unsigned sizeLeft = size;
        unsigned char* destPtr = (unsigned char*)dest;

        while (sizeLeft)
        {
            if (!readBuffer_ || readBufferOffset_ >= readBufferSize_)
            {
                unsigned char blockHeaderBytes[4];
                ReadInternal(blockHeaderBytes, sizeof blockHeaderBytes);

                MemoryBuffer blockHeader(&blockHeaderBytes[0], sizeof blockHeaderBytes);
                unsigned unpackedSize = blockHeader.ReadUShort();
                unsigned packedSize = blockHeader.ReadUShort();

                if (!readBuffer_)
                {
                    readBuffer_ = new unsigned char[unpackedSize];
                    inputBuffer_ = new unsigned char[LZ4_compressBound(unpackedSize)];
                }

                /// \todo Handle errors
                ReadInternal(inputBuffer_.Get(), packedSize);
                LZ4_decompress_fast((const char*)inputBuffer_.Get(), (char*)readBuffer_.Get(), unpackedSize);

                readBufferSize_ = unpackedSize;
                readBufferOffset_ = 0;
            }

            unsigned copySize = Min((readBufferSize_ - readBufferOffset_), sizeLeft);
            memcpy(destPtr, readBuffer_.Get() + readBufferOffset_, copySize);
            destPtr += copySize;
            sizeLeft -= copySize;
            readBufferOffset_ += copySize;
            position_ += copySize;
        }

        return size;
    }

    // Need to reassign the position due to internal buffering when transitioning from writing to reading
    if (readSyncNeeded_)
    {
        SeekInternal(position_ + offset_);
        readSyncNeeded_ = false;
    }

    if (!ReadInternal(dest, size))
    {
        // Return to the position where the read began
        SeekInternal(position_ + offset_);
        URHO3D_LOGERROR("Error while reading from file " + GetName());
        return 0;
    }

    position_ += size;
    return size;
}

unsigned PackedFile::Seek(unsigned position)
{
    if (!IsOpen())
    {
        // If file not open, do not log the error further here to prevent spamming the stderr stream
        return 0;
    }

    // Allow sparse seeks if writing
    if (mode_ == FILE_READ && position > size_)
        position = size_;

    if (compressed_)
    {
        // Start over from the beginning
        if (position == 0)
        {
            position_ = 0;
            readBufferOffset_ = 0;
            readBufferSize_ = 0;
            SeekInternal(offset_);
        }
        // Skip bytes
        else if (position >= position_)
        {
            unsigned char skipBuffer[SKIP_BUFFER_SIZE];
            while (position > position_)
                Read(skipBuffer, Min(position - position_, SKIP_BUFFER_SIZE));
        }
        else
            URHO3D_LOGERROR("Seeking backward in a compressed file is not supported");

        return position_;
    }

    SeekInternal(position + offset_);
    position_ = position;
    readSyncNeeded_ = false;
    return position_;
}

unsigned PackedFile::Write(const void* data, unsigned size)
{
    if (!IsOpen())
    {
        // If file not open, do not log the error further here to prevent spamming the stderr stream
        return 0;
    }

    URHO3D_LOGERROR("PackedFile does not support writing. Tried to write to " + GetName());
    return 0;
}

unsigned PackedFile::GetChecksum()
{
    if (offset_ || checksum_)
        return checksum_;
#ifdef __ANDROID__
    if ((!handle_ && !assetHandle_) || mode_ == FILE_WRITE)
#else
    if (!handle_ || mode_ == FILE_WRITE)
#endif
        return 0;

    URHO3D_PROFILE(CalculateFileChecksum);

    unsigned oldPos = position_;
    checksum_ = 0;

    Seek(0);
    while (!IsEof())
    {
        unsigned char block[1024];
        unsigned readBytes = Read(block, 1024);
        for (unsigned i = 0; i < readBytes; ++i)
            checksum_ = SDBMHash(checksum_, block[i]);
    }

    Seek(oldPos);
    return checksum_;
}

void PackedFile::Close()
{
#ifdef __ANDROID__
    if (assetHandle_)
    {
        SDL_RWclose(assetHandle_);
        assetHandle_ = 0;
    }
#endif

    readBuffer_.Reset();
    inputBuffer_.Reset();

    if (handle_)
    {
        fclose((FILE*)handle_);
        handle_ = nullptr;
        position_ = 0;
        size_ = 0;
        offset_ = 0;
        checksum_ = 0;
    }
}

void PackedFile::Flush()
{
    if (handle_)
        fflush((FILE*)handle_);
}

bool PackedFile::IsOpen() const
{
#ifdef __ANDROID__
    return handle_ != 0 || assetHandle_ != 0;
#else
    return handle_ != nullptr;
#endif
}

bool PackedFile::OpenInternal(const String& fileName)
{
    Close();

    compressed_ = false;
    readSyncNeeded_ = false;

    FileSystem* fileSystem = GetSubsystem<FileSystem>();
    if (fileSystem && !fileSystem->CheckAccess(GetPath(fileName)))
    {
        URHO3D_LOGERRORF("Access denied to %s", fileName.CString());
        return false;
    }

    if (fileName.Empty())
    {
        URHO3D_LOGERROR("Could not open file with empty name");
        return false;
    }

#ifdef __ANDROID__
    if (URHO3D_IS_ASSET(fileName))
    {
        if (mode != FILE_READ)
        {
            URHO3D_LOGERROR("Only read mode is supported for Android asset files");
            return false;
        }

        assetHandle_ = SDL_RWFromFile(URHO3D_ASSET(fileName), "rb");
        if (!assetHandle_)
        {
            URHO3D_LOGERRORF("Could not open Android asset file %s", fileName.CString());
            return false;
        }
        else
        {
            fileName_ = fileName;
            mode_ = mode;
            position_ = 0;
            if (!fromPackage)
            {
                size_ = SDL_RWsize(assetHandle_);
                offset_ = 0;
            }
            checksum_ = 0;
            return true;
        }
    }
#endif

#ifdef _WIN32
    handle_ = _wfopen(GetWideNativePath(fileName).CString(), L"rb");
#else
    handle_ = fopen(GetNativePath(fileName).CString(), "rb");
#endif

    if (!handle_)
    {
        URHO3D_LOGERRORF("Could not open file %s", fileName.CString());
        return false;
    }

    fileName_ = fileName;
    position_ = 0;
    checksum_ = 0;

    return true;
}

bool PackedFile::ReadInternal(void* dest, unsigned size)
{
#ifdef __ANDROID__
    if (assetHandle_)
    {
        return SDL_RWread(assetHandle_, dest, size, 1) == 1;
    }
    else
#endif
        return fread(dest, size, 1, (FILE*)handle_) == 1;
}

void PackedFile::SeekInternal(unsigned newPosition)
{
#ifdef __ANDROID__
    if (assetHandle_)
    {
        SDL_RWseek(assetHandle_, newPosition, SEEK_SET);
        // Reset buffering after seek
        readBufferOffset_ = 0;
        readBufferSize_ = 0;
    }
    else
#endif
        fseek((FILE*)handle_, newPosition, SEEK_SET);
}

}

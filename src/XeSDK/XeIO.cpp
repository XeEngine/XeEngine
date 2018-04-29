#include "pch.h"
#include <XeSDK/XeIO.h>
#include <XeSDK/IIOMemoryStream.h>
#include <XeSDK/XeMemory.h>

namespace Xe {
    namespace IO {
        void Copy(IStream* dst, IStream* src) {
            s64 bytesToCopy = src->GetLength() - src->GetPosition();
            if (dst->CanOverflow() == false) {
                s64 remainsOut = dst->GetLength() - dst->GetPosition();
                if (bytesToCopy >= remainsOut)
                    bytesToCopy = remainsOut;
            }
            IO::Copy(dst, src, bytesToCopy);
        }
        void Copy(IStream* dst, IStream* src, s64 length) {
            const s64 BufferSize = 4096;
            u8 buffer[BufferSize];

            IO::IMemoryStream *mem1, *mem2;
            if (src->Query((IObject**)&mem1, IO::IMemoryStream::ID) &&
                dst->Query((IObject**)&mem2, IO::IMemoryStream::ID)) {

                void* pIn = (u8*)((IMemoryStream*)src)->GetMemory() + src->GetPosition();
                void* pOut = (u8*)((IMemoryStream*)dst)->GetMemory() + dst->GetPosition();

                s64 remainsOut = dst->GetLength() - dst->GetPosition() - length;
                if (remainsOut < 0) {
                    if (dst->SetLength(dst->GetLength() - remainsOut))
                        pOut = (u8*)((IMemoryStream*)src)->GetMemory();
                    else
                        length = dst->GetLength() - dst->GetPosition();
                }

                Xe::Memory::Copy(pOut, pIn, (uvar)length);
                src->SetPosition(length, Seek_Cur);
                dst->SetPosition(length, Seek_Cur);
            }
            else {
                while (length > 0) {
                    s32 toCopy = (s32)(length > BufferSize ? BufferSize : length);
                    src->Read(buffer, 0, toCopy);
                    dst->Write(buffer, 0, toCopy);
                    length -= BufferSize;
                }
            }
        }
    }
}
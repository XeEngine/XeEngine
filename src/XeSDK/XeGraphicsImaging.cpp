#include "pch.h"
#include <XeSDK/XeGraphicsImaging.h>
#include <XeSDK/XeMemory.h>

namespace Xe {
    namespace Graphics {
        namespace Imaging {
            bool CreateSurface(IContext *context, ISurface **surface, SurfaceType type, IImage *image) {
                context->AddRef();
                image->AddRef();

                const Size &size = image->GetSize();
                Color::Format format = image->GetFormat();
                svar bpp = Color::GetBitsPerPixel(format);
                if (bpp == 24) bpp = 32; // APPLY CONVERSION
                else if (bpp == 4) bpp = 8;
                DataDesc desc;
                desc.pitch = size.x * bpp / 8;
                u8 *data = (u8*)Memory::Alloc(desc.pitch * size.y);
                desc.data = data;

                switch (format) {
                    case Color::Format_RGB888: {
                        uvar bufferPitch = size.x * 3;
                        u8 *buffer = (u8*)Memory::Alloc(bufferPitch);
                        u8 *pData = data;
                        for (s32 i = 0; i < size.y; i++) {
                            u8 *pBuffer = buffer;
                            image->ReadLine(buffer, 0);
                            for (s32 j = 0; j < size.x; j++) {
                                *pData++ = *pBuffer++;
                                *pData++ = *pBuffer++;
                                *pData++ = *pBuffer++;
                                *pData++ = 0xFF;
                            }
                        }
                        Memory::Free(buffer);
                        format = Color::Format_RGBX8888;
                    } break;
                    case Color::Format_BGR888: {
                        uvar bufferPitch = size.x * 3;
                        u8 *buffer = (u8*)Memory::Alloc(bufferPitch);
                        u8 *pData = data;
                        for (s32 i = 0; i < size.y; i++) {
                            u8 *pBuffer = buffer;
                            image->ReadLine(buffer, 0);
                            for (s32 j = 0; j < size.x; j++) {
                                *pData++ = *pBuffer++;
                                *pData++ = *pBuffer++;
                                *pData++ = *pBuffer++;
                                *pData++ = 0xFF;
                            }
                        }
                        Memory::Free(buffer);
                        format = Color::Format_BGRX8888;
                    } break;
                    case Color::Format_Indexed4: {
                        uvar bufferPitch = (size.x + 1) >> 1;
                        u8 *buffer = (u8*)Memory::Alloc(bufferPitch);
                        for (s32 i = 0; i < size.y; i++) {
                            image->ReadLine(buffer, 0);
                            u8 *pSrc = buffer;
                            u8 *pDst = data + desc.pitch * i;
                            for (uvar i = 0; i < bufferPitch; i++) {
                                *pDst++ = (*pSrc >> 4) & 0x0F;
                                *pDst++ = *pSrc & 0x0F;
                                pSrc++;
                            }
                        }
                        Memory::Free(buffer);
                        format = Color::Format_Indexed8;
                    } break;
                    default:
                        for (s32 i = 0; i < size.y; i++)
                            image->ReadLine(data, desc.pitch * i);
                }
                bool res = context->CreateSurface(surface, type, size, format, desc);

                Memory::Free(data);
                image->Release();
                context->Release();
                return res;
            }
        }
    }
}
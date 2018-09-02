#include "pch.h"
#include <XeSDK/XeGraphicsImagingPng.h>
#include <XeSDK/IGraphicsImagingImage.h>

#include "png.h"

namespace Xe {
    namespace Graphics {
        namespace Imaging {
            class Png : public IImage {
                IO::IStream *m_stream;
                png_structp m_pPngStruct;
                png_infop m_pPngInfo;
                s32 m_rowsRead;
            public:
                static void ReadDataFromStream(png_structp png_ptr, png_bytep outBytes, png_size_t byteCountToRead)
                {
                    //if (png_ptr != nullptr)
                    {
                        IO::IStream* stream = (IO::IStream*)png_get_io_ptr(png_ptr);
                        //if (stream != nullptr)
                        stream->Read(outBytes, 0, byteCountToRead);
                    }
                }

                Png(const Size& size, Color::Format format, uvar colorsCount, uvar stride, IO::IStream *stream,
                    png_structp pPngStruct, png_infop pPngInfo) :
                        IImage(size, format, colorsCount, stride), m_stream(stream),
                        m_pPngStruct(pPngStruct), m_pPngInfo(pPngInfo),
                        m_rowsRead(0) {
                }
                ~Png() {
                    png_destroy_read_struct(&m_pPngStruct, &m_pPngInfo, nullptr);
                    m_stream->Release();
                }
                bool ReadLine(void *dst, svar offset) {
                    if (m_rowsRead < GetSize().y)
                    {
                        m_rowsRead++;
                        png_read_row(m_pPngStruct, (png_bytep)dst + offset, NULL);
                        return true;
                    }
                    return false;
                }
                uvar ReadPalette(u32 *dst, uvar count) {
                    png_colorp color_ptr;
                    int num_palette;
                    // Extract palette
                    if (png_get_PLTE(m_pPngStruct, m_pPngInfo, &color_ptr, &num_palette) == PNG_INFO_PLTE)
                    {
                        if (num_palette > (int)count)
                            num_palette = (int)count;
                        for (svar i = 0; i < num_palette; i++)
                        {
                            dst[i] = 0xFF000000 |
                                     (color_ptr[i].blue << 0) |
                                     (color_ptr[i].green << 8) |
                                     (color_ptr[i].red << 16);
                        }
                        png_bytep trans_ptr; // array of transparent entries for palette
                        int numtrans; // gray value or color sample values of the single transparent color (non-palette)
                        png_color_16p trans_color; // number of transparent entries
                        // Extract transparency info
                        if (png_get_tRNS(m_pPngStruct, m_pPngInfo, &trans_ptr, &numtrans, &trans_color) != 0) {
                            //dst[numtrans - 1] &= ~0xFF000000;
                            for (int i = 0; i < numtrans; i++) {
                                png_byte index = trans_ptr[i];
                                if (num_palette > index)
                                    dst[index] &= ~0xFF000000;
                            }
                        }
                        return (uvar)num_palette;
                    }
                    else return 0;
                }
            };

            bool PngCheck(IO::IStream *stream) {
                static const int PNG_BYTES_TO_CHECK = 4;
                u8 buf[PNG_BYTES_TO_CHECK];
                svar read = stream->Read(buf, 0, PNG_BYTES_TO_CHECK);
                stream->SetPosition(-read, Xe::IO::Seek_Cur);
                if (read == PNG_BYTES_TO_CHECK)
                    return !png_sig_cmp(buf, (png_size_t)0, PNG_BYTES_TO_CHECK);
                return false;
            }
            bool PngOpen(IImage **image, IO::IStream *stream) {
                stream->AddRef();
                if (PngCheck(stream)) {
                    // XTODO error handling; check last three params
                    /* Create and initialize the png_struct with the desired error handler
                    * functions.  If you want to use the default stderr and longjump method,
                    * you can supply NULL for the last three parameters.  We also supply the
                    * the compiler header file version, so that we know if the application
                    * was compiled with a compatible version of the library.  REQUIRED
                    */
                    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
                    if (png_ptr != nullptr)
                    {
                        /* Allocate/initialize the memory for image information.  REQUIRED. */
                        png_infop info_ptr = png_create_info_struct(png_ptr);
                        if (info_ptr != nullptr) {
                            /* Set error handling if you are using the setjmp/longjmp method (this is
                            * the normal method of doing things with libpng).  REQUIRED unless you
                            * set up your own error handlers in the png_create_read_struct() earlier.
                            */
                            if (!setjmp(png_jmpbuf(png_ptr)))
                            {
                                png_uint_32 width, height;
                                Color::Format format;
                                int bit_depth, color_type, interlace_type;

                                /* If you are using replacement read functions, instead of calling
                                * png_init_io() here you would call:
                                */
                                png_set_read_fn(png_ptr, (png_voidp)stream, Png::ReadDataFromStream);

                                /* The call to png_read_info() gives us all of the information from the
                                * PNG file before the first IDAT (image data chunk).  REQUIRED
                                */
                                png_read_info(png_ptr, info_ptr);
                                png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type,
                                             &interlace_type, NULL, NULL);
                                bit_depth = png_get_bit_depth(png_ptr, info_ptr);
                                color_type = png_get_color_type(png_ptr, info_ptr);

                                switch (color_type)
                                {
                                    case PNG_COLOR_TYPE_GRAY:
                                        png_set_gray_to_rgb(png_ptr);
                                        bit_depth = 24;
                                        format = Color::Format_RGB888;
                                        break;
                                    case PNG_COLOR_TYPE_GRAY_ALPHA:
                                        png_set_gray_to_rgb(png_ptr);
                                        bit_depth = 32;
                                        format = Color::Format_RGBA8888;
                                        break;
                                    case PNG_COLOR_TYPE_PALETTE:
                                        switch (bit_depth)
                                        {
                                            case 1:
                                                format = Color::Format_Indexed1;
                                                break;
                                            case 2:
                                                format = Color::Format_Indexed2;
                                                break;
                                            case 4:
                                                format = Color::Format_Indexed4;
                                                break;
                                            case 8:
                                                format = Color::Format_Indexed8;
                                                break;
                                            case 16:
                                                format = Color::Format_Indexed16;
                                                break;
                                        }
                                        break;
                                    case PNG_COLOR_TYPE_RGB:
                                        bit_depth = 24;
                                        format = Color::Format_RGB888;
                                        break;
                                    case PNG_COLOR_TYPE_RGB_ALPHA:
                                        bit_depth = 32;
                                        format = Color::Format_RGBA8888;
                                        break;
                                }

                                /* Optional call to gamma correct and add the background to the palette
                                * and update info structure.  REQUIRED if you are expecting libpng to
                                * update the palette for you (ie you selected such a transform above).
                                */
                                png_read_update_info(png_ptr, info_ptr);

                                int colorsCounti = 0;
                                png_get_PLTE(png_ptr, info_ptr, nullptr, &colorsCounti);
                                *image = new Png(Size(width, height), format, (uvar)colorsCounti, png_get_rowbytes(png_ptr, info_ptr),
                                                 stream, png_ptr, info_ptr);
                                return true;
                            }
                        }
                        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
                    }
                }
                stream->Release();
                *image = nullptr;
                return false;
            }
            bool PngSave(IO::IStream *stream, IImage *image) {
                stream->AddRef();
                image->AddRef();
                image->Release();
                stream->Release();
                return false;
            }
        }
    }
}
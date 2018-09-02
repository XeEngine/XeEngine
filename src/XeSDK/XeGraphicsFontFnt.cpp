#include "pch.h"
#include <XeSDK/XeGraphicsFont.h>
#include <XeSDK/IGraphicsContext.h>
#include <XeSDK/XeIO.h>
#include <XeSDK/IIOMemoryStream.h>
#include <XeSDK/XeMath.h>

namespace Xe {
    namespace Graphics {
        namespace Font {
            class Fnt : public IFont {
                // https://msdn.microsoft.com/en-us/library/windows/desktop/ms648014(v=vs.85).aspx
                struct FONTINFO16_SVAR {
                    // Specify the type of font file.
                    // Lowest bit specify if it's raster (reset) or vectorial (set)
                    // The other bits are reserved from GDI use
                    svar dfType;
                    // Nominal point size at which this character set looks best.
                    svar dfPoints;
                    // Nominal vertical resolution at which this character set was digitized.
                    svar dfVertRes;
                    // Nominal horizontal resolution at which this character set was digitized.
                    svar dfHorizRes;
                    // Specify the distance from the top of a character definition cell to the baseline of the typographical font.
                    svar dfAscent;
                    // Specifies the amount of leading inside the bounds set by dfPixHeight.
                    // Accent marks may occur in this area. This may be zero at the designer's option.
                    svar dfInternalLeading;
                    // Specifies the amount of extra leading that the designer requests the application
                    // add between rows. Since this area is outside of the font proper, it contains no
                    // marks and will not be altered by text output calls in either the OPAQUE or
                    // TRANSPARENT mode.This may be zero at the designer's option.
                    svar dfExternalLeading;
                    // Specify whether or not the character definition data represent an italic font.
                    svar dfItalic;
                    // Specify whether or not the character definition data represent an underlined font.
                    svar dfUnderline;
                    // Specify whether or not the character definition data represent an struckout font.
                    svar dfStrikeOut;
                    // Specify the weight of the characters in the character definition data,
                    // on a scale of 1 to 1000. 400 specify a regular weight.
                    svar dfWeight;
                    // Specifying the character set defined by this font.
                    svar dfCharSet;
                    // For vector fonts, specifies the width of the grid on which the font was
                    // digitized. For raster fonts, if dfPixWidth is nonzero, it represents the
                    // width for all the characters in the bitmap; if it is zero, the font has
                    // variable width characters whose widths are specified in the dfCharTable array.
                    svar dfPixWidth;
                    // Specify the height of the character bitmap or of the grid on which a vector font was digitized.
                    svar dfPixHeight;
                    svar dfPitchAndFamily;
                    // For fixed-pitch fonts, this is the same as dfPixWidth. For variable pitch
                    // fonts, this is the width of the character "X."
                    svar dfAvgWidth;
                    // Specify the maximum pixel width of any character in the font. For fixed
                    // pitch fonts, this is simply dfPixWidth.
                    svar dfMaxWidth;
                    // Specify the first character code defined by this font.
                    svar dfFirstChar;
                    // Specify the last character code defined by this font. Note that all the
                    // characters with codes between dfFirstChar and dfLastChar must be present
                    // in the font character definitions.
                    svar dfLastChar;
                    // Character to substitute whenever a string contains a character out of the
                    // range.The character is given relative to dfFirstChar so that dfDefaultChar
                    // is the actual value of the character, less dfFirstChar.The dfDefaultChar
                    // should indicate a special character that is not a space
                    svar dfDefaultChar;
                    // Character that will define word breaks. This character defines word breaks
                    // for word wrapping and word spacing justification.The character is given
                    // relative to dfFirstChar so that dfBreakChar is the actual value of the
                    // character, less that of dfFirstChar.The dfBreakChar is normally(32 -
                    // dfFirstChar), which is an ASCII space.
                    svar dfBreakChar;
                    // Number of bytes in each row of the bitmap.This is always even, so that the
                    // rows start on WORD boundaries.For vector fonts, this field has no meaning.
                    svar dfWidthBytes;
                    // Offset in the file to the string giving the device name.For a generic font,
                    // this value is zero.
                    svar dfDevice;
                    // Specify the offset in the file to the null-terminated string that names the face.
                    svar dfFace;
                    // Specify the absolute machine address of the bitmap. This is set by GDI at load time.
                    svar dfBitsPointer;
                    // Specify the offset in the file to the beginning of the bitmap information.
                    // If the 04H bit in dfType is set, then it's an absolute address of the bitmap.
                    svar dfBitsOffset;
                    // Not used.
                    svar dfReserved;
                };

                struct CharEntry {
                    short width;
                    short offset;
                };

                IO::IMemoryStream *m_stream;
                FONTINFO16_SVAR m_prop;
                String m_name;

                svar m_posDiff;
                svar m_chByteLength;
                uint *m_charTable;
            public:
                Fnt(IO::IMemoryStream *stream) :
                        m_stream(stream) {
                    m_stream->AddRef();
                }
                bool Open() {
                    m_posDiff = (svar)(m_stream->GetPosition() - 66);
                    m_stream->Read(&m_prop.dfType, 0, 2);
                    m_stream->Read(&m_prop.dfPoints, 0, 2);
                    m_stream->Read(&m_prop.dfVertRes, 0, 2);
                    m_stream->Read(&m_prop.dfHorizRes, 0, 2);
                    m_stream->Read(&m_prop.dfAscent, 0, 2);
                    m_stream->Read(&m_prop.dfInternalLeading, 0, 2);
                    m_stream->Read(&m_prop.dfExternalLeading, 0, 2);
                    m_stream->Read(&m_prop.dfItalic, 0, 1);
                    m_stream->Read(&m_prop.dfUnderline, 0, 1);
                    m_stream->Read(&m_prop.dfStrikeOut, 0, 1);
                    m_stream->Read(&m_prop.dfWeight, 0, 2);
                    m_stream->Read(&m_prop.dfCharSet, 0, 1);
                    m_stream->Read(&m_prop.dfPixWidth, 0, 2);
                    m_stream->Read(&m_prop.dfPixHeight, 0, 2);
                    m_stream->Read(&m_prop.dfPitchAndFamily, 0, 1);
                    m_stream->Read(&m_prop.dfAvgWidth, 0, 2);
                    m_stream->Read(&m_prop.dfMaxWidth, 0, 2);
                    m_stream->Read(&m_prop.dfFirstChar, 0, 1);
                    m_stream->Read(&m_prop.dfLastChar, 0, 1);
                    m_stream->Read(&m_prop.dfDefaultChar, 0, 1);
                    m_stream->Read(&m_prop.dfBreakChar, 0, 1);
                    m_stream->Read(&m_prop.dfWidthBytes, 0, 2);
                    m_stream->Read(&m_prop.dfDevice, 0, 4);
                    m_stream->Read(&m_prop.dfFace, 0, 4);
                    m_stream->Read(&m_prop.dfBitsPointer, 0, 4);
                    m_stream->Read(&m_prop.dfBitsOffset, 0, 4);
                    m_stream->Read(&m_prop.dfReserved, 0, 1);
                    m_charTable = (uint*)((u8*)m_stream->GetMemory() + m_stream->GetPosition());
                    m_name = (const char*)m_stream->GetMemory() + m_prop.dfFace - 66;

                    if (m_prop.dfType & 1) {
                        LOG(Debug::Log::Priority_Warning, Debug::Log::Type_Font,
                            _T("FNT %s is vectorial and it's not currently supported."), m_name.GetData());
                        return false;
                    }

                    m_isFixedWidth = m_prop.dfPixWidth != 0;
                    m_pxWidth = m_prop.dfMaxWidth;
                    m_pxHeight = m_prop.dfPixHeight;
                    m_chFirst = m_prop.dfFirstChar;
                    m_chCount = m_prop.dfLastChar - m_prop.dfFirstChar + 1;
                    m_chDefault = m_prop.dfFirstChar + m_prop.dfDefaultChar;
                    m_format = Color::Format_BGRA8888;
                    if (m_isFixedWidth)
                        m_chByteLength = m_pxWidth * m_pxHeight * Color::GetBitsPerPixel(m_format) / 8;
                    else {
                        LOG(Debug::Log::Priority_Warning, Debug::Log::Type_Font,
                            _T("FNT %s uses variable width that's not currently supported."), m_name.GetData());
                        m_chByteLength = 0;
                        return false;
                    }
                    LOG(Debug::Log::Priority_Diagnostics, Debug::Log::Type_Font,
                        _T("FNT details: %s %ix%i count %i."), m_name.GetData(), m_pxWidth, m_pxHeight, m_chCount);
                    return true;
                }

                const String &GetName() const {
                    return m_name;
                }
                bool GetCharacter(CharacterDesc &chDesc, uint ch) const {
                    unsigned char CH = (unsigned char)ch;
                    if (CH >= m_prop.dfFirstChar && CH <= m_prop.dfLastChar) {
                        CharEntry *entry = (CharEntry*)(m_charTable + CH - m_prop.dfFirstChar);
                        chDesc.width = entry->width;
                        chDesc.ch = ch;
                        return true;
                    }
                    else {
                        tchar ch = m_prop.dfDefaultChar;
                        LOG(Debug::Log::Priority_Info, Debug::Log::Type_Font,
                            _T("FNT::GetCharacter %p (%s) character %04X not found. Returing default character %04X"), this, m_name.GetData(), ch);
                    }
                    return false;
                }
                bool ReadCharater(void *dst, svar pitch, svar height, Color::Format format, uint ch) {
                    unsigned char CH = (unsigned char)ch;
                    if (CH >= m_prop.dfFirstChar && CH <= m_prop.dfLastChar) {
                        CharEntry *entry = (CharEntry*)(m_charTable + CH - m_prop.dfFirstChar);
                        u8 *pSrc = (u8*)m_stream->GetMemory() + entry->offset - 66;
                        switch (format) {
                            case Color::Format_BGRA8888:
                            case Color::Format_RGBA8888:
                            case Color::Format_BGRX8888:
                            case Color::Format_RGBX8888:
                            {
                                svar h = Math::Min(height, m_pxHeight);
                                for (svar x = 0; x < m_pxWidth; x += 8) {
                                    for (svar y = 0; y < h; y++) {
                                        uint *pDst = (uint*)((u8*)dst + pitch * y) + x;
                                        u8 d = *pSrc++;
                                        for (svar i = 0; i < 8; i++) {
                                            int n = d & (1 << (7 - i));
                                            *pDst++ = n != 0 ? 0xFFFFFFFF : 0;
                                        }
                                    }
                                }
                            }
                                break;
                            case Color::Format_Indexed8:
                            default:
                                return false;
                        }
                        return true;
                    }
                    LOG(Debug::Log::Priority_Info, Debug::Log::Type_Font,
                        _T("FNT::ReadCharater %p (%s) character %04X not found. Returing default character %04X"), this, m_name.GetData(), ch);
                    return false;
                }
            };
            bool OpenFnt(IFont **font, IO::IStream *stream) {
                stream->AddRef();
                s64 remain = stream->GetLength() - stream->GetPosition();
                LOG(Debug::Log::Priority_Info, Debug::Log::Type_Font,
                    _T("Opening FNT from stream %p..."), stream);
                if (remain >= 66) {
                    short dfVersion;
                    long dfSize;
                    char copyright[60];
                    stream->Read(&dfVersion, 0, 2);
                    dfVersion = Math::Swap16(dfVersion);
                    if (dfVersion == 2 || dfVersion == 3) {
                        stream->Read(&dfSize, 0, 4);
                        stream->Read(&copyright, 0, 60);
                        if (remain < dfSize) {
                            LOG(Debug::Log::Priority_Error, Debug::Log::Type_Font,
                                _T("%p dfSize is greater than the stream."), stream);
                        }
                        else {
                            LOG(Debug::Log::Priority_Diagnostics, Debug::Log::Type_Font,
                                _T("FNT seems to be a valid file."));
                            IO::IMemoryStream *memStream = IO::IMemoryStream::New(dfSize - 66, false);
                            IO::Copy(memStream, stream, memStream->GetLength());
                            LOG(Debug::Log::Priority_Diagnostics, Debug::Log::Type_Font,
                                _T("FNT copied into main memory."));
                            stream->Release();
                            memStream->SetPosition(0);
                            Fnt *fnt = new Fnt(memStream);
                            memStream->Release();
                            if (fnt->Open()) {
                                *font = fnt;
                                LOG(Debug::Log::Priority_Diagnostics, Debug::Log::Type_Font,
                                    _T("FNT opened with success!"), stream);
                                return true;
                            }
                            else {
                                LOG(Debug::Log::Priority_Error, Debug::Log::Type_Font,
                                    _T("%p unable to open FNT."), stream);
                            }
                            fnt->Release();
                        }
                    }
                    else {
                        LOG(Debug::Log::Priority_Error, Debug::Log::Type_Font,
                            _T("%p version %i is not supported."), dfVersion);
                    }
                }
                else {
                    LOG(Debug::Log::Priority_Error, Debug::Log::Type_Font,
                        _T("%p not enough bytes to be valid."), stream);
                }
                *font = nullptr;
                stream->Release();
                return false;
            }
        }
    }
}  
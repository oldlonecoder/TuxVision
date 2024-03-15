//
// Created by oldlonecoder on 11/13/23.
//

//#ifndef TUXVISION_BLOCDC_H
//#define TUXVISION_BLOCDC_H
/******************************************************************************************
 *   Copyright (C) 1965/1987/2023 by Serge Lussier                                        *
 *   serge.lussier@oldlonecoder.club                                                      *
 *                                                                                        *
 *                                                                                        *
 *   Unless otherwise specified, all code IsIn this project is written                      *
 *   by the author (Serge Lussier)                                                        *
 *   and no one else then not even {copilot, chatgpt, or any other AI calamities}         *
 *   ----------------------------------------------------------------------------------   *
 *   Copyrights from authors other than Serge Lussier also apply here                     *
 *   Open source FREE licences also apply To the code from the same author (Serge Lussier)*
 *   ----------------------------------------------------------------------------------   *
 ******************************************************************************************/

#pragma once

#include <TuxVision/UiDefs.h>
#include <AppBook/Util/Geometry.h>
#include <AppBook/Util/Object.h>

using Util::Object;

namespace Tux
{

class TV_API WinDC
{


    Object* OwnerParent{nullptr};
    
    Rect    BlocGeometry;
    Point   ScreenCoord;
    int           Z{0};

public:

    using Type = uint32_t*;


    struct TV_API Pixel
    {
        using Type = uint32_t;
        Type  Mem  = 0;


        static constexpr uint32_t CharMask	= 0x000000FF;
        static constexpr uint32_t FGMask	= 0x0000FF00;
        static constexpr uint32_t BGMask	= 0x00FF0000;
        static constexpr uint32_t CMask		= 0x00FFFF00;
        static constexpr uint32_t AttrMask	= 0xFF000000;
        static constexpr uint32_t UTFMASK	= 0x11000000;
        // ==================Attributes==========================
        static constexpr uint32_t  UGlyph 	= 0x01000000;
        static constexpr uint32_t  Underline= 0x02000000;
        static constexpr uint32_t  Stroke 	= 0x04000000;
        static constexpr uint32_t  Blink 	= 0x08000000;
        static constexpr uint32_t  Accent 	= 0x10000000;
        // ==============Bit Shift ============================
        static constexpr int FGShift = 0x08;
        static constexpr int BGShift = 0x10;
        static constexpr int ATShift = 0x18;

        Pixel() =default;

        explicit Pixel(WinDC::Pixel::Type _Ch);
        explicit Pixel(WinDC::Pixel* _C);
        ~Pixel() =default;

        WinDC::Pixel& SetFg(Color::Code fg_);
        WinDC::Pixel& SetBg(Color::Code bg_);

        [[maybe_unused]] WinDC::Pixel& SetAttributes(WinDC::Pixel::Type d_);

        [[maybe_unused]] WinDC::Pixel& SetColors(Color::Pair&& c_);

        [[maybe_unused]] WinDC::Pixel& SetColors(Color::Pair& c_);

        WinDC::Pixel& operator=(WinDC::Pixel::Type d_);
        WinDC::Pixel& operator=(WinDC::Type d_);
        WinDC::Pixel& operator=(char d_);

        [[nodiscard]] Color::Code Fg() const;
        [[nodiscard]] Color::Code Bg() const;

        [[maybe_unused]] [[nodiscard]] Color::Pair Colors() const;

        [[maybe_unused]] Utf::Glyph::Type IconID();

        [[maybe_unused]] [[nodiscard]] Utf::AccentFR::Type AccentID() const;
        [[nodiscard]] uint8_t Ascii() const;

        WinDC::Pixel::Type& operator*() { return Mem; }

        [[maybe_unused]] [[nodiscard]] uint16_t Attributes() const;

        WinDC::Pixel& operator << (Utf::Glyph::Type i_);
        WinDC::Pixel& operator << (Utf::AccentFR::Type a_);
        WinDC::Pixel& operator << (char c) { Mem = Mem & (~CharMask) | c; return *this; }

        [[maybe_unused]] [[nodiscard]] std::string RenderColors() const;

        [[maybe_unused]] WinDC::Pixel& ResetAttributes(WinDC::Pixel::Type bits_);
        explicit operator std::string();

        [[nodiscard]] std::string Details() const;

    };


    struct TV_API Painter
    {
        WinDC*          Dc{nullptr};
        Rect            Wh{};
        WinDC::Pixel    Pixel{0x20};
        WinDC::Pixel*   DefAttr{nullptr};
        WinDC::Pixel*   Cursor{nullptr};
        Justify::Type   JBits{0};

        Point     Position() const;

    private:
        Painter() = default;
        Painter(WinDC* DC, Rect R = {}, WinDC::Pixel* DefAttr={});
        Painter(Painter&&) noexcept = default;
        Painter(const Painter&) = default;
    public:
        Painter& operator = (Painter&&) noexcept = default;
        Painter& operator = (const Painter&) = default;

        Painter& operator << (Color::Code aColorID);
        Painter& operator << (Utf::Glyph::Type aIconID);
        Painter& operator << (const std::string& aStr);
        Painter& operator << (const StrAcc& aStr);
        Painter& operator << (const Point& XY);
        Painter& operator << (const char* aStr);
        Painter& operator << (Utf::AccentFR::Type aAcc);

        Painter& NextChar();

        Painter& GotoXY(Point XY);
        Painter& Home() { return GotoXY({}); }

        template<typename T> Painter& operator << (const T& aStr)
        {
            std::ostringstream Out;
            Out << aStr;
            return (*this) << Out.str();
        }

        Painter& SetColors(Color::Pair pair_);

        Book::Result SetupGeometry();
        Painter& Clear();
        // void Update();
        Justify::Type& JustifyBits() { return JBits; }
        Painter& SetBits(Justify::Type aBits);// { Bits = aBits; return *this; }
        Painter& SetHcenter(bool S);// { Bits = S ? (Bits & ~HCenter) | HCenter : (Bits & ~HCenter); return *this; }
        Painter& SetVcenter(bool S);
        Painter& SetCenter(bool S);
        Painter& SetWordWrap(bool S);
        Painter& SetBg(Color::Code aBg);
        Painter& SetFg(Color::Code aFg);

    };



    WinDC()= default;
    WinDC(Object* ParentObject);
    ~WinDC();

    Dim Dimensions();

    WinDC::Pixel* MemoryBloc{nullptr};
    
    Pixel::Type D {0x20};

    WinDC::Pixel* Alloc(Dim dim_ = {});
    WinDC::Pixel*  Realloc(Dim dim_);
    void Dealloc();
    WinDC::Pixel* Peek(const Point& pt_);

    Rect Geometry();

    int Width() const;
    int Height() const;
    Point CursorPosition();
    Book::Result SetPosition(Point pt_);
    void SetLocation(Point pt_) { ScreenCoord = pt_;}
    Point Location() { return ScreenCoord;}
    Book::Result Clear(); ///< clear with current attr
    Book::Result Clear(const Rect& r_); ///< clear sub-region with the current attr;
    Book::Result Clear(WinDC::Pixel::Type a_, const Rect& r_ = {});

private:
    friend class WinDC::Painter;
};

} // Tux

//#endif //TUXVISION_BLOCDC_H

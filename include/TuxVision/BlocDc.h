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
#include <AppBook/Core/Geometry.h>
#include <AppBook/Core/Object.h>


namespace Tux
{

class TUXV_EXPORT BlocDC
{


    Core::Object* OwnerParent{nullptr};
    //Core::Rect    VisibleArea;
    Core::Rect    BlocGeometry;
    Core::Point   ScreenCoord;
    int           Z{0};

public:

    using Type = uint32_t*;


    struct TUXV_EXPORT Char
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

        Char() =default;

        explicit Char(BlocDC::Char::Type _Ch);
        explicit Char(BlocDC::Type _C);
        ~Char() =default;

        BlocDC::Char& SetFg(Core::Color::code fg_);
        BlocDC::Char& SetBg(Core::Color::code bg_);

        [[maybe_unused]] BlocDC::Char& SetAttributes(BlocDC::Char::Type d_);

        [[maybe_unused]] BlocDC::Char& SetColors(Core::Color::Pair&& c_);

        [[maybe_unused]] BlocDC::Char& SetColors(Core::Color::Pair& c_);

        BlocDC::Char& operator=(BlocDC::Char::Type d_);
        BlocDC::Char& operator=(BlocDC::Type d_);
        BlocDC::Char& operator=(char d_);

        [[nodiscard]] Core::Color::code Fg() const;
        [[nodiscard]] Core::Color::code Bg() const;

        [[maybe_unused]] [[nodiscard]] Core::Color::Pair Colors() const;

        [[maybe_unused]] Utf::Glyph::Type IconID();

        [[maybe_unused]] [[nodiscard]] Utf::AccentFR::Type AccentID() const;
        [[nodiscard]] uint8_t Ascii() const;

        BlocDC::Char::Type& operator*() { return Mem; }

        [[maybe_unused]] [[nodiscard]] uint16_t Attributes() const;

        BlocDC::Char& operator << (Utf::Glyph::Type i_);
        BlocDC::Char& operator << (Utf::AccentFR::Type a_);
        BlocDC::Char& operator << (char c) { Mem = Mem & (~CharMask) | c; return *this; }

        [[maybe_unused]] [[nodiscard]] std::string RenderColors() const;

        [[maybe_unused]] BlocDC::Char& ResetAttributes(BlocDC::Char::Type bits_);
        explicit operator std::string();

        [[nodiscard]] std::string Details() const;

    };


    struct TUXV_EXPORT Painter
    {
        BlocDC*         Dc{nullptr};
        Core::Rect      Wh;
        BlocDC::Char    C;
        BlocDC::Type    DefAttr{nullptr};
        BlocDC::Type    Cursor{nullptr};
        Justify::Type   JBits{0};
        Core::Point     Position() const;

    private:
        Painter() = default;
        Painter(BlocDC* DC, Core::Rect R = {}, BlocDC::Type DefAttr={});
        Painter(Painter&&) noexcept = default;
        Painter(const Painter&) = default;
    public:
        Painter& operator = (Painter&&) noexcept = default;
        Painter& operator = (const Painter&) = default;

        Painter& operator << (Core::Color::code aColorID);
        Painter& operator << (Utf::Glyph::Type aIconID);
        Painter& operator << (const std::string& aStr);
        Painter& operator << (const Core::StrAcc& aStr);
        Painter& operator << (const Core::Point& XY);
        Painter& operator << (const char* aStr);
        Painter& operator << (Utf::AccentFR::Type aAcc);

        Painter& NextChar();

        Painter& GotoXY(Core::Point XY);
        Painter& Home() { return GotoXY({}); }

        template<typename T> Painter& operator << (const T& aStr)
        {
            std::ostringstream Out;
            Out << aStr;
            return (*this) << Out.str();
        }

        Painter& SetColors(Core::Color::Pair pair_);

        Book::Result SetupGeometry();
        Painter& Clear();
        // void Update();
        Justify::Type& JustifyBits() { return j_bits; }
        Painter& SetBits(Justify::Type aBits);// { Bits = aBits; return *this; }
        Painter& SetHcenter(bool S);// { Bits = S ? (Bits & ~HCenter) | HCenter : (Bits & ~HCenter); return *this; }
        Painter& SetVcenter(bool S);
        Painter& SetCenter(bool S);
        Painter& SetWordWrap(bool S);
        Painter& SetBg(Core::Color::code aBg);
        Painter& SetFg(Core::Color::code aFg);

    };



    BlocDC()= default;
    BlocDC(Core::Object* ParentObject);
    ~BlocDC();

    Core::Dim Dimensions();

    BlocDC::Type MemoryBloc{nullptr};
    
    Char::Type D {0x20};

    BlocDC::Type Alloc(Core::Dim dim_ = {});
    BlocDC::Type Realloc(Core::Dim dim_);
    void Dealloc();
    BlocDC::Type Peek(const Core::Point& pt_);

    Core::Rect Geometry();

    int Width() const;
    int Height() const;
    Core::Point CursorPosition();
    Book::Result SetPosition(Core::Point pt_);
    void SetLocation(Core::Point pt_) { ScreenCoord = pt_;}
    Core::Point Location() { return ScreenCoord;}
    Book::Result Clear(); ///< clear with current attr
    Book::Result Clear(const Core::Rect& r_); ///< clear sub-region with the current attr;
    Book::Result Clear(BlocDC::Char::Type a_, const Core::Rect& r_ = {});

private:
    friend class BlocDC::Painter;
};

} // Tux

//#endif //TUXVISION_BLOCDC_H


#include "TuxVision/WinDC.h"

namespace Tux
{



#pragma region Pixel

[[maybe_unused]] WinDC::Pixel::Pixel(WinDC::Pixel::Type _Ch):Mem(_Ch){}

WinDC::Pixel::Pixel(WinDC::Pixel* _C): Mem(*_C){}

WinDC::Pixel &WinDC::Pixel::SetFg(Color::Code fg_)
{
    Mem = (Mem & ~FGMask) | (static_cast<Pixel::Type>(fg_) << FGShift);
    return *this;
}

WinDC::Pixel &WinDC::Pixel::SetBg(Color::Code bg_)
{
    Mem = (Mem & ~BGMask) | (static_cast<Pixel::Type>(bg_) << BGShift);
    return *this;
}

[[maybe_unused]] WinDC::Pixel &WinDC::Pixel::SetAttributes(WinDC::Pixel::Type d_)
{
    Mem = (Mem & ~AttrMask) | d_;
    return *this;
}


[[maybe_unused]] WinDC::Pixel &WinDC::Pixel::SetColors(Color::Pair &&c_)
{
    return SetFg(c_.Fg).SetBg(c_.Bg);
}

[[maybe_unused]] WinDC::Pixel &WinDC::Pixel::SetColors(Color::Pair &c_)
{
    return SetFg(c_.Fg).SetBg(c_.Bg);
}

WinDC::Pixel &WinDC::Pixel::operator=(WinDC::Pixel::Type d_)
{
    Mem = d_;
    return *this;
}

WinDC::Pixel &WinDC::Pixel::operator=(WinDC::Type d_)
{
    Mem = *d_;
    return *this;
}

WinDC::Pixel &WinDC::Pixel::operator=(char d_)
{
    Mem = (Mem & ~CharMask) | d_;
    return *this;
}

Color::Code WinDC::Pixel::Fg() const
{
    return static_cast<Color::Code>((Mem & FGMask) >> FGShift);
}

Color::Code WinDC::Pixel::Bg() const
{
    return static_cast<Color::Code>((Mem & BGMask) >> BGShift);
}

[[maybe_unused]] Color::Pair WinDC::Pixel::Colors() const
{
    return {Fg(),Bg()};
}

[[maybe_unused]] Utf::Glyph::Type WinDC::Pixel::IconID()
{
    if (!(Mem & Pixel::UGlyph)) return Utf::Glyph::Poop;
    auto Ic = Mem & Pixel::CharMask;
    if (Ic > 59) Ic = Utf::Glyph::Bug;
    return  Ic;
}

[[maybe_unused]] Utf::AccentFR::Type WinDC::Pixel::AccentID() const
{
    auto AID = Mem & Pixel::CharMask;
    if (AID > Utf::AccentFR::Ucirc) return Utf::AccentFR::Agrave;
    return static_cast<Utf::AccentFR::Type>(AID);
}

uint8_t WinDC::Pixel::Ascii() const
{
    return Mem & CharMask;
}

[[maybe_unused]] uint16_t WinDC::Pixel::Attributes() const
{
    return (Mem & AttrMask) >> ATShift;
}

WinDC::Pixel &WinDC::Pixel::operator<<(Utf::Glyph::Type i_)
{
    return *this;
}

WinDC::Pixel &WinDC::Pixel::operator<<(Utf::AccentFR::Type a_)
{
    return *this;
}

[[maybe_unused]] std::string WinDC::Pixel::RenderColors() const
{
    std::string str;
    str += Color::Ansi({Bg(),Fg()});
    //Color::AnsiBg(Bg()) += Color::Ansi(Fg());
    return str;
}

[[maybe_unused]] WinDC::Pixel &WinDC::Pixel::ResetAttributes(WinDC::Pixel::Type bits_)
{
    return *this;
}

WinDC::Pixel::operator std::string()
{
    return Details();
}

std::string WinDC::Pixel::Details() const
{
    StrAcc Str = "%s[Pixel:'%s%d%s'/Fg:'%s%s%s'/Bg:'%s%s%s']";
    Str << Color::White <<Color::Yellow << (int)Ascii() << Color::White
        << Color::Yellow << Color::Name(Fg()) << Color::White
        << Color::Yellow << Color::Name(Bg()) << Color::White;

    return Str();
}


#pragma endregion Pixel

WinDC::WinDC(Object *ParentObject): OwnerParent(ParentObject){}

WinDC::~WinDC()
{
    delete [] MemoryBloc;
}

Dim WinDC::Dimensions()
{

    return BlocGeometry.Dwh;
}

WinDC::Pixel* WinDC::Alloc(Dim dim_)
{
    if(dim_)
        BlocGeometry.Assign({}, dim_);

    delete [] MemoryBloc;
    MemoryBloc = new Pixel [dim_.W * dim_.H + 1];
    return MemoryBloc;
}


WinDC::Pixel* WinDC::Realloc(Dim dim_)
{
    if(!dim_)
        throw AppBook::Exception()[ AppBook::Except() << Book::Result::Unexpected << " unset Dimensions" ];

    BlocGeometry.Assign({},dim_);

    delete [] MemoryBloc;
    MemoryBloc = new Pixel[dim_.W * dim_.H + 1];
    return MemoryBloc;
}



void WinDC::Dealloc()
{
    delete [] MemoryBloc;
    MemoryBloc = nullptr;
    BlocGeometry = {};
}


WinDC::Pixel* WinDC::Peek(const Point &pt_)
{
    auto* p = MemoryBloc + (pt_.X + pt_.Y * BlocGeometry.Dwh.W);
    if (!BlocGeometry[pt_])
        return nullptr;

    return p;
}

Rect WinDC::Geometry()
{
    return BlocGeometry;
}

int WinDC::Width() const
{
    return BlocGeometry.Dwh.W;
}

int WinDC::Height() const
{
    return BlocGeometry.Dwh.H;
}



Book::Result WinDC::Clear()
{
    if (!MemoryBloc)
        throw AppBook::Exception()[ AppBook::Fatal() << " Attempt to clear a null VDC"];

    auto* C = MemoryBloc;
    auto blk = BlocGeometry.Dwh.W * BlocGeometry.Dwh.H;
    for (int x = 0; x < blk; x++) *C++ = D;
    return Book::Result::Accepted;
}

Book::Result WinDC::Clear(const Rect &r_)
{
    Rect R;
    if(r_)
        R = Rect({}, BlocGeometry.Dwh) & r_;
    else
        return Clear();
    if(!MemoryBloc)
        throw AppBook::Exception()[AppBook::Fatal() << "Attempt to clear a null WinDC"];

    if(!R)
        return Book::Result::Rejected;

    for(int y = R.A.Y; y <= R.B.Y; y++)
    {
        Pixel* C = Peek({R.A.X, y});
        for(int x = 0; x < R.Width(); x++) *C++ = D;
    }

    return Book::Result::Accepted;
}



Book::Result WinDC::Clear(WinDC::Pixel::Type a_, const Rect &r_)
{
    auto A = D;
    D = a_;
    auto Ok = Clear(r_);
    D = A;
    return Ok;
}


WinDC::Painter::Painter(WinDC *DC, Rect R, WinDC::Pixel* Attr):Dc(DC), Wh(R), DefAttr(Attr)
{

}

WinDC::Painter &WinDC::Painter::operator<<(Color::Code aColorID)
{
    Pixel.SetFg(aColorID);
    return *this;
}



/**
 * @brief Assigns the IconAttr to the cell's attr bit field and the Icon Code ID to the ascci bit field
 *
 * @param aIconID
 * @return painter& ( ref to self )
 * @author &copy; 2022, Serge Lussier, lussier.serge@gmail.com ( oldlonecoder )
 */
WinDC::Painter &WinDC::Painter::operator<<(Utf::Glyph::Type aIconID)
{
    WinDC::Pixel Cell = Pixel;

    *Cursor = (Cell << aIconID).Mem;
    Cell = *Cursor;
    ++Cursor;
    Pixel = Cell;
    *Cursor = (Pixel << ' ').Mem;

    return *this;
}

/**
 * @brief Writes string \a aStr into the painter's backbuffer vdc
 *
 * @param aStr
 * @return painter& (ref to self )
 *
 * @note This painter version does not handle justify flags, nor checks the logical line and columns boundaries - Only the linear-bloc limit.
 * @author &copy; 2022, Serge Lussier, lussier.serge@gmail.com ( oldlonecoder )
 */
WinDC::Painter &WinDC::Painter::operator<<(const std::string &aStr)
{
    Point Pt = Position();
    auto C = aStr.begin();
    //diagnostic::debug() << "painter <<  \"" << aStr << "\" @" << (std::string)pt << " -> in " << _r.to_string();
    while ((C != aStr.end()) && Wh[Pt])
    {
        *Cursor = (Pixel << *C++).Mem; // Ouais... Pas mal pété; A bit risky, doing three things in the very same statement...
        ++Cursor;
        Pt.X++;
    }
    return *this;
}

WinDC::Painter &WinDC::Painter::operator<<(const StrAcc &aStr)
{
    return *this;
}

WinDC::Painter &WinDC::Painter::operator<<(const Point &XY)
{
    return *this;
}

WinDC::Painter &WinDC::Painter::operator<<(const char *aStr)
{
    return *this;
}

WinDC::Painter &WinDC::Painter::operator<<(Utf::AccentFR::Type aAcc)
{
    return *this;
}


/**
 * @brief operator << overload acting as Painter::GotoXY
 *
 * @param XY coordinate point into the painter's subregion.
 * @return WinDC::Painter& ( ref to self )
 * @author &copy; 2022, Serge Lussier, lussier.serge@gmail.com ( oldlonecoder )
 */
WinDC::Painter &WinDC::Painter::GotoXY(Point XY)
{
    Point Pt = XY + Wh.A; // + {0,0} by default;
    if (!Wh[Pt])
        throw AppBook::Exception()[AppBook::Fatal() << Book::Fn::Endl << " : " << (std::string)XY << " is out of range in " << Wh.to_string()];

    Cursor = Dc->Peek(Pt);
    return *this;
}

WinDC::Painter &WinDC::Painter::SetColors(Color::Pair pair_)
{
    Pixel.SetColors(pair_);

    return *this;
}

Book::Result WinDC::Painter::SetupGeometry()
{
    //diagnostic::debug(sfnl) < " painter _dc's '" < color::Chartreuse6 <  "Geometry: " <  _dc->geometry();
    if(!Wh)
        Wh = Dc->Geometry();
    else
        Wh = Dc->Geometry()  & Wh;

    if (!Wh)
        throw AppBook::Exception()[AppBook::Fatal() << ": " << Book::Fn::Endl << " - Attempt to < Construct > a painter object using invalid Geometry : " << (std::string)Wh];

    //diagnostic::debug(sfnll) < " Configured Geometry:" < color::Yellow < _r < color::Reset < " :";
    Cursor = Dc->Peek(Wh.A);
    //diagnostic::debug(SourceLocation) <<  VDC::Cell{ *_cursor }.Details();
    return Book::Result::Accepted;
}

WinDC::Painter &WinDC::Painter::Clear()
{
    AppBook::Debug() << (std::string)Wh;
    auto* CC = Cursor;
    Cursor = Dc->Peek(Wh.A);
    Pixel << ' ';
    Pixel.SetFg(WinDC::Pixel(DefAttr).Fg());

    for(int y = 0; y< Wh.Height(); y++)
    {
        GotoXY({ 0,y });
        for (int X = 0; X < Wh.Width(); X++) *Cursor++ = Pixel.Mem;
    }

    Cursor = CC;
    return *this;
}

WinDC::Painter &WinDC::Painter::SetBits(Justify::Type aBits)
{
    return *this;
}

WinDC::Painter &WinDC::Painter::SetHcenter(bool S)
{
    return *this;
}

WinDC::Painter &WinDC::Painter::SetVcenter(bool S)
{
    return *this;
}

WinDC::Painter &WinDC::Painter::SetCenter(bool S)
{
    return *this;
}

WinDC::Painter &WinDC::Painter::SetWordWrap(bool S)
{
    return *this;
}

WinDC::Painter &WinDC::Painter::SetBg(Color::Code aBg)
{
    return *this;
}

WinDC::Painter &WinDC::Painter::SetFg(Color::Code aFg)
{
    return *this;
}



/*!
    @brief Computes coordinates within the painter's bloc from the linear index (cursor) location.

    @param none
    @return computed point.
    @note none
    @author &copy; 2018,2023; Serge Lussier;oldlonecoder (lussier.serge@gmail.com)
*/
Point WinDC::Painter::Position() const
{
    auto* B = Dc->MemoryBloc;
    int dx = static_cast<int>(Cursor-B);
    return { dx % Dc->Width(), dx / Dc->Width() };
}

WinDC::Painter &WinDC::Painter::NextChar()
{
    auto Pt = Position();
    Pt.X++;
    if(!Wh[Pt])
    {

    }
    return *this;
}


} // Tux
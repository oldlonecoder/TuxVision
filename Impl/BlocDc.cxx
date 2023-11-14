
#include <AppBook/Book/AppBook.h>

#include "TuxVision/BlocDc.h"

namespace Tux
{



#pragma region Char

[[maybe_unused]] BlocDC::Char::Char(BlocDC::Char::Type _Ch):Mem(_Ch){}

BlocDC::Char::Char(BlocDC::Type _C): Mem(*_C){}

BlocDC::Char &BlocDC::Char::SetFg(Core::Color::code fg_)
{
    Mem = (Mem & ~FGMask) | (static_cast<Char::Type>(fg_) << FGShift);
    return *this;
}

BlocDC::Char &BlocDC::Char::SetBg(Core::Color::code bg_)
{
    Mem = (Mem & ~BGMask) | (static_cast<Char::Type>(bg_) << BGShift);
    return *this;
}

[[maybe_unused]] BlocDC::Char &BlocDC::Char::SetAttributes(BlocDC::Char::Type d_)
{
    Mem = (Mem & ~AttrMask) | d_;
    return *this;
}


[[maybe_unused]] BlocDC::Char &BlocDC::Char::SetColors(Color::Pair &&c_)
{
    return SetFg(c_.Fg).SetBg(c_.Bg);
}

[[maybe_unused]] BlocDC::Char &BlocDC::Char::SetColors(Color::Pair &c_)
{
    return SetFg(c_.Fg).SetBg(c_.Bg);
}

BlocDC::Char &BlocDC::Char::operator=(BlocDC::Char::Type d_)
{
    Mem = d_;
    return *this;
}

BlocDC::Char &BlocDC::Char::operator=(BlocDC::Type d_)
{
    Mem = *d_;
    return *this;
}

BlocDC::Char &BlocDC::Char::operator=(char d_)
{
    Mem = (Mem & ~CharMask) | d_;
    return *this;
}

Core::Color::code BlocDC::Char::Fg() const
{
    return static_cast<Core::Color::code>((Mem & FGMask) >> FGShift);
}

Core::Color::code BlocDC::Char::Bg() const
{
    return static_cast<Core::Color::code>((Mem & BGMask) >> BGShift);
}

[[maybe_unused]] Core::Color::Pair BlocDC::Char::Colors() const
{
    return {Fg(),Bg()};
}

[[maybe_unused]] Utf::Glyph::Type BlocDC::Char::IconID()
{
    if (!(Mem & Char::UGlyph)) return Utf::Glyph::Poop;
    auto Ic = Mem & Char::CharMask;
    if (Ic > 59) Ic = Utf::Glyph::Bug;
    return  Ic;
}

[[maybe_unused]] Utf::AccentFR::Type BlocDC::Char::AccentID() const
{
    auto AID = Mem & Char::CharMask;
    if (AID > Utf::AccentFR::Ucirc) return Utf::AccentFR::Agrave;
    return static_cast<Utf::AccentFR::Type>(AID);
}

uint8_t BlocDC::Char::Ascii() const
{
    return Mem & CharMask;
}

[[maybe_unused]] uint16_t BlocDC::Char::Attributes() const
{
    return (Mem & AttrMask) >> ATShift;
}

BlocDC::Char &BlocDC::Char::operator<<(Utf::Glyph::Type i_)
{
    return *this;
}

BlocDC::Char &BlocDC::Char::operator<<(Utf::AccentFR::Type a_)
{
    return *this;
}

[[maybe_unused]] std::string BlocDC::Char::RenderColors() const
{
    std::string str;
    str += Core::Color::Ansi({Bg(),Fg()});
    //Core::Color::AnsiBg(Bg()) += Core::Color::Ansi(Fg());
    return str;
}

[[maybe_unused]] BlocDC::Char &BlocDC::Char::ResetAttributes(BlocDC::Char::Type bits_)
{
    return *this;
}

BlocDC::Char::operator std::string()
{
    return Details();
}

std::string BlocDC::Char::Details() const
{
    Core::StrAcc Str = "%s[Char:'%s%d%s'/Fg:'%s%s%s'/Bg:'%s%s%s']";
    Str << Core::Color::White <<Core::Color::Yellow << (int)Ascii() << Core::Color::White
        << Core::Color::Yellow << Core::Color::Name(Fg()) << Core::Color::White
        << Core::Color::Yellow << Core::Color::Name(Bg()) << Core::Color::White;

    return Str();
}


#pragma endregion C

BlocDC::BlocDC(Core::Object *ParentObject): OwnerParent(ParentObject){}

BlocDC::~BlocDC()
{
    delete [] MemoryBloc;
}

Core::Dim BlocDC::Dimensions()
{

    return BlocGeometry.Dwh;
}

BlocDC::Type BlocDC::Alloc(Core::Dim dim_)
{
    if(dim_)
        BlocGeometry.Assign({}, dim_);

    delete [] MemoryBloc;
    MemoryBloc = new Char::Type [dim_.W * dim_.H +1];
    return MemoryBloc;
}


BlocDC::Type BlocDC::Realloc(Core::Dim dim_)
{
    if(!dim_)
        throw AppBook::Exception()[ AppBook::Except() << Book::Result::Unexpected << " unset Dimensions" ];

    BlocGeometry.Assign({},dim_);

    delete [] MemoryBloc;
    MemoryBloc = new Char::Type [dim_.W * dim_.H +1];
    return MemoryBloc;
}



void BlocDC::Dealloc()
{
    delete [] MemoryBloc;
    MemoryBloc = nullptr;
    BlocGeometry = {};
}


BlocDC::Type BlocDC::Peek(const Core::Point &pt_)
{
    auto* p = MemoryBloc + (pt_.X + pt_.Y * BlocGeometry.Dwh.W);
    if (!BlocGeometry[pt_])
        return nullptr;

    return p;
}

Core::Rect BlocDC::Geometry()
{
    return BlocGeometry;
}

int BlocDC::Width() const
{
    return BlocGeometry.Dwh.W;
}

int BlocDC::Height() const
{
    return BlocGeometry.Dwh.H;
}



Book::Result BlocDC::Clear()
{
    if (!MemoryBloc)
        throw AppBook::Exception()[ AppBook::Fatal() << " Attempt to clear a null VDC"];

    auto* C = MemoryBloc;
    auto blk = BlocGeometry.Dwh.W * BlocGeometry.Dwh.H;
    for (int x = 0; x < blk; x++) *C++ = D;
    return Book::Result::Accepted;
}

Book::Result BlocDC::Clear(const Core::Rect &r_)
{
    Core::Rect R;
    if(r_)
        R = Core::Rect({}, BlocGeometry.Dwh) & r_;
    else
        return Clear();
    if(!MemoryBloc)
        throw AppBook::Exception()[AppBook::Fatal() << "Attempt to clear a null BlocDC"];

    if(!R)
        return Book::Result::Rejected;

    for(int y = R.A.Y; y <= R.B.Y; y++)
    {
        Char::Type* C = Peek({R.A.X, y});
        for(int x = 0; x < R.Width(); x++) *C++ = D;
    }

    return Book::Result::Accepted;
}



Book::Result BlocDC::Clear(BlocDC::Char::Type a_, const Core::Rect &r_)
{
    auto A = D;
    D = a_;
    auto Ok = Clear(r_);
    D = A;
    return Ok;
}


BlocDC::Painter::Painter(BlocDC *DC, Core::Rect R, BlocDC::Type Attr):Dc(DC), Wh(R), DefAttr(Attr)
{

}

BlocDC::Painter &BlocDC::Painter::operator<<(Core::Color::code aColorID)
{
    C.SetFg(aColorID);
    return *this;
}



/**
 * @brief Assigns the IconAttr to the cell's attr bit field and the Icon Code ID to the ascci bit field
 *
 * @param aIconID
 * @return painter& ( ref to self )
 * @author &copy; 2022, Serge Lussier, lussier.serge@gmail.com ( oldlonecoder )
 */
BlocDC::Painter &BlocDC::Painter::operator<<(Utf::Glyph::Type aIconID)
{
    BlocDC::Char Cell = C;

    *Cursor = (Cell << aIconID).Mem;
    Cell = Cursor;
    ++Cursor;
    C = Cell;
    *Cursor = (C << ' ').Mem;

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
BlocDC::Painter &BlocDC::Painter::operator<<(const std::string &aStr)
{
    Core::Point Pt = Position();
    auto c = aStr.begin();
    //diagnostic::debug() << "painter <<  \"" << aStr << "\" @" << (std::string)pt << " -> in " << _r.to_string();
    while ((c != aStr.end()) && Wh[Pt])
    {
        *Cursor = (C << *c++).Mem; // Ouais... Pas mal pété; A bit risky, doing three things in the very same statement...
        ++Cursor;
        Pt.X++;
    }
    return *this;
}

BlocDC::Painter &BlocDC::Painter::operator<<(const Core::StrAcc &aStr)
{
    return *this;
}

BlocDC::Painter &BlocDC::Painter::operator<<(const Core::Point &XY)
{
    return *this;
}

BlocDC::Painter &BlocDC::Painter::operator<<(const char *aStr)
{
    return *this;
}

BlocDC::Painter &BlocDC::Painter::operator<<(Utf::AccentFR::Type aAcc)
{
    return *this;
}


/**
 * @brief operator << overload acting as Painter::GotoXY
 *
 * @param XY coordinate point into the painter's subregion.
 * @return BlocDC::Painter& ( ref to self )
 * @author &copy; 2022, Serge Lussier, lussier.serge@gmail.com ( oldlonecoder )
 */
BlocDC::Painter &BlocDC::Painter::GotoXY(Core::Point XY)
{
    Core::Point Pt = XY + Wh.A; // + {0,0} by default;
    if (!Wh[Pt])
        throw AppBook::Exception()[AppBook::Fatal() << Book::Fn::Endl << " : " << (std::string)XY << " is out of range in " << Wh.to_string()];

    Cursor = Dc->Peek(Pt);
    return *this;
}

BlocDC::Painter &BlocDC::Painter::SetColors(Core::Color::Pair pair_)
{
    C.SetColors(pair_);

    return *this;
}

Book::Result BlocDC::Painter::SetupGeometry()
{
    //diagnostic::debug(sfnl) < " painter _dc's '" < color::Chartreuse6 <  "Geometry: " <  _dc->geometry();
    if(!Wh)
        Wh = Dc->Geometry();
    else
        Wh = Dc->Geometry()  & Wh;

    if (!Wh)
        throw AppBook::Exception()[AppBook::Fatal() << ": " << Book::Fn::Endl << " - Attempt to < Contruct > a painter object using invalid Geometry : " << Wh];

    //diagnostic::debug(sfnll) < " Configured Geometry:" < color::Yellow < _r < color::Reset < " :";
    Cursor = Dc->Peek(Wh.A);
    //diagnostic::debug(SourceLocation) <<  VDC::Cell{ *_cursor }.Details();
    return Book::Result::Accepted;
}

BlocDC::Painter &BlocDC::Painter::Clear()
{
    AppBook::Debug() << Wh;
    auto* CC = Cursor;
    Cursor = Dc->Peek(Wh.A);
    C << ' ';
    C.SetFg(BlocDC::Char(DefAttr).Fg());

    for(int y = 0; y< Wh.Height(); y++)
    {
        GotoXY({ 0,y });
        for (int X = 0; X < Wh.Width(); X++) *Cursor++ = C.Mem;
    }

    Cursor = CC;
    return *this;
}

BlocDC::Painter &BlocDC::Painter::SetBits(Justify::Type aBits)
{
    return *this;
}

BlocDC::Painter &BlocDC::Painter::SetHcenter(bool S)
{
    return *this;
}

BlocDC::Painter &BlocDC::Painter::SetVcenter(bool S)
{
    return *this;
}

BlocDC::Painter &BlocDC::Painter::SetCenter(bool S)
{
    return *this;
}

BlocDC::Painter &BlocDC::Painter::SetWordWrap(bool S)
{
    return *this;
}

BlocDC::Painter &BlocDC::Painter::SetBg(Core::Color::code aBg)
{
    return *this;
}

BlocDC::Painter &BlocDC::Painter::SetFg(Core::Color::code aFg)
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
Core::Point BlocDC::Painter::Position() const
{
    auto* B = Dc->MemoryBloc;
    int dx = static_cast<int>(Cursor-B);
    return { dx % Dc->Width(), dx / Dc->Width() };
}

BlocDC::Painter &BlocDC::Painter::NextChar()
{
    auto Pt = Position();
    Pt.X++;
    if(!Wh[Pt])
    {

    }
    return *this;
}


} // Tux
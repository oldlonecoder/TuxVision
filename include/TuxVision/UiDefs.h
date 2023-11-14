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
#include <AppBook/Core/ChAttr.h>
#include "TuxVision/Exports.h"
#include <cstdint>
#include <map>


namespace Tux
{


using Core::Color;


/**
 * @brief Widget ui justification enums or'ed flags
 * @author &copy; 2008,2022,2023, Serge Lussier, lussier.serge@gmail.com ( oldlonecoder )
 */
namespace Justify
{
using Type = uint8_t;
static constexpr uint8_t Left 		= 0x01;
static constexpr uint8_t Right 		= 0x02;
static constexpr uint8_t HCenter 	= 0x04;
static constexpr uint8_t VCenter 	= 0x08;
static constexpr uint8_t Center 	= 0x0C;
static constexpr uint8_t WordWRap 	= 0x10;
static constexpr uint8_t Prefix 	= 0x20;
static constexpr uint8_t Suffix 	= 0x40;
}


/**
 * @brief Direction enums values
 * @author &copy; 2008,2022,2023, Serge Lussier, lussier.serge@gmail.com ( oldlonecoder )
 */
namespace Direction
{
using Type = int8_t;
static constexpr int8_t Left = -1;
static constexpr int8_t Right = 1;
static constexpr int8_t Up = 2;
static constexpr int8_t Down = -2;
}

/**
 * @brief Widget ui class enums or'ed flags
 * @author &copy; 2008,2022,2023, Serge Lussier, lussier.serge@gmail.com ( oldlonecoder )
 */
namespace wclass
{
using type = uint16_t;
static constexpr type TopLevel 	= 0x0001;
static constexpr type Floating 	= 0x0002;
static constexpr type Child 	= 0x0004;
static constexpr type Parent 	= 0x0008;
static constexpr type Caption 	= 0x0010;
static constexpr type Frame 	= 0x0020;
static constexpr type Input 	= 0x0040;
//...
}
/**
 * @brief Widget ui state enums or'ed flags
 * @author &copy; 2008,2022,2023, Serge Lussier, lussier.serge@gmail.com ( oldlonecoder )
 */
namespace State
{
using Type = uint16_t;
static constexpr Type Normal  = 0x00;
static constexpr Type Disable = 0x01;
static constexpr Type Active  = 0x02;
static constexpr Type Focus   = 0x04;
static constexpr Type Error   = 0x08;
static constexpr Type Warning = 0x10;
static constexpr Type Illegal = 0x20;
static constexpr Type Invalid = 0x40;
static constexpr Type Visible = 0x80;
static constexpr Type Success = 0x100;

}

namespace Colors
{

struct TUXV_EXPORT Db
    {
        using Components = std::map<State::Type,Color::Pair>;
        using Elements  = std::map<std::string_view, Db::Components>;
        using Themes    = std::map<std::string_view, Db::Elements>;
        //using theme     = std::map<std::string_view, db::dictionary>;
        static Db::Themes Data;
    };
}



class TUXV_EXPORT UIDefs
{

};


}// Tux




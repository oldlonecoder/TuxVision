//
// Created by oldlonecoder on 11/10/23.
//

#include "TuxVision/UiDefs.h"

namespace Tux
{

Colors::Db::Themes Colors::Db::Data =
{
    {
        "default",
        {
            {
                "widget",
                {
                    {State::Active, {Color::Grey100,Color::Grey27}},
                    {State::Disable,{Color::Grey30, Color::Grey35}},    // Keep coherance with widget base
                    {State::Normal,{Color::Grey70,Color::Grey27}},      // Keep coherance with widget base
                    {State::Focus,{Color::Red4, Color::Grey27}}

                    //...
                }
            },
            {
                "label",
                {
                    {State::Active, {Color::Yellow, Color::Grey27}},// Active label base coherance with composite/complex ui
                    {State::Disable,{Color::Grey19, Color::Grey35}},    // Keep coherance with widget base
                    {State::Normal,{Color::Grey70,Color::Grey27}},      // Keep coherance with widget base
                    {State::Focus,{Color::Grey70, Color::Grey27}}
                    //...
                }
            },
            {
                "shortkey",
                {
                    {State::Active, {Color::HotPink4,Color::Grey27}},
                    {State::Disable,{Color::Grey19, Color::Grey35}},    // Keep coherance with widget base
                    {State::Normal,{Color::Grey70,Color::Grey27}},
                    {State::Focus,{Color::Red4, Color::Grey27}}
                }
            },
            {
                "icon",
                {
                    {State::Active, {Color::Blue1, Color::Grey27}},    // Keep coherance with widget base
                    {State::Disable,{Color::Grey19, Color::Grey35}},
                    {State::Normal,{Color::Blue1, Color::Grey35}},
                    {State::Focus,{Color::Aquamarine3, Color::Grey27}},
                    {State::Success ,{Color::Green5, Color::Grey27}}

                    //...
                }
            },
            {
                "input-area", // widget::text_input
                {
                    {State::Active, {Color::White, Color::Grey30}},    // Keep coherance with widget base
                    {State::Disable,{Color::Grey39, Color::Grey35}},
                    {State::Normal,{Color::Grey37, Color::Grey30}},
                    {State::Focus,{Color::White, Color::Grey30}}
                }
            },
            {
                "input-field",
                {
                    {State::Active, {Color::Grey70, Color::Grey27}},    // Keep coherance with widget base
                    {State::Disable, {Color::Grey39, Color::Grey35}},
                    {State::Normal, {Color::Grey70, Color::Grey27}},
                    {State::Focus, {Color::White, Color::Grey27}}
                }
            }
        }
    }
};

}
// Tux

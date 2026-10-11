//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: IBaseMenu bits that cannot live in the header -- they reach for
//          IToolkitMenus and CCSPlayerController, both of which are only
//          declared by the time the class body is parsed.
//
//=============================================================================//
#include "source2toolkit/IToolkitApi.h"
#include "source2toolkit/IToolkitMenus.h"
#include "source2toolkit/IToolkitPlugin.h"
#include "source2toolkit/schema/entity/classes/CCSPlayerController.h"
TOOLKIT_GLOBALVARS();

std::function<bool(CCSPlayerController*)> IBaseMenu::s_canSelect =
    [](CCSPlayerController*) { return true; };

std::function<void(CCSPlayerController*)> IBaseMenu::s_onSelect =
    [](CCSPlayerController*) {};

ChatMenuOption& IBaseMenu::AddMenuOptionWithCooldown(
    std::string optionText,
    std::function<void(CCSPlayerController*, ChatMenuOption&)> action,
    bool disabled,
    bool close,
    std::function<bool()> disabledEvaluator)
{
    ChatMenuOption& opt = AddMenuOption(
        std::move(optionText),
        [action = std::move(action), close](CCSPlayerController* player, ChatMenuOption& optRef)
        {
            if (s_canSelect && !s_canSelect(player))
                return;

            action(player, optRef);

            if (s_onSelect)
                s_onSelect(player);

            if (close)
            {
                if (g_pToolkitMenus)
                    g_pToolkitMenus->CloseActiveMenu(player);

                if (player)
                    player->PrintToCenterHtml(optRef.Text.c_str(), 5, true);
            }
        },
        disabled);

    opt.DisabledEvaluator = std::move(disabledEvaluator);
    return opt;
}

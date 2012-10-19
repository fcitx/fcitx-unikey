/***************************************************************************
 *   Copyright (C) 2012~2012 by CSSlayer                                   *
 *   wengxt@gmail.com                                                      *
 *                                                                         *
 *  This program is free software: you can redistribute it and/or modify   *
 *  it under the terms of the GNU General Public License as published by   *
 *  the Free Software Foundation, either version 3 of the License, or      *
 *  (at your option) any later version.                                    *
 *                                                                         *
 *  This program is distributed in the hope that it will be useful,        *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 *  GNU General Public License for more details.                           *
 *                                                                         *
 *  You should have received a copy of the GNU General Public License      *
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.  *
 *                                                                         *
 ***************************************************************************/

#include "unikey-im.h"

const char*          Unikey_IMNames[]    = {"Telex", "Vni", "STelex", "STelex2"};
const UkInputMethod   Unikey_IM[]         = {UkTelex, UkVni, UkSimpleTelex, UkSimpleTelex2};
const unsigned int    NUM_INPUTMETHOD     = sizeof(Unikey_IM)/sizeof(Unikey_IM[0]);

const char*          Unikey_OCNames[]    = {"Unicode",
                                            "TCVN3",
                                            "VNI Win",
                                            "VIQR",
                                            "BK HCM 2",
                                            "CString",
                                            "NCR Decimal",
                                            "NCR Hex"};
const unsigned int    NUM_OUTPUTCHARSET   = sizeof(Unikey_OCNames)/sizeof(Unikey_OCNames[0]);

void UpdateIMMenu(FcitxUIMenu *menu)
{
    FcitxUnikey* unikey = (FcitxUnikey*) menu->priv;
    menu->mark = unikey->config.im;
}

boolean IMMenuAction(FcitxUIMenu *menu, int index)
{
    FcitxUnikey* unikey = (FcitxUnikey*) menu->priv;
    unikey->config.im = (UkInputMethod) index;
    UpdateUnikeyConfig(unikey);
    return true;
}

void UpdateOCMenu(FcitxUIMenu *menu)
{
    FcitxUnikey* unikey = (FcitxUnikey*) menu->priv;
    menu->mark =  unikey->config.oc;
}

boolean OCMenuAction(FcitxUIMenu *menu, int index)
{
    FcitxUnikey* unikey = (FcitxUnikey*) menu->priv;
    unikey->config.oc =(UkConv) index;
    UpdateUnikeyConfig(unikey);
    return true;
}

static const char* GetIMIconName(void* arg)
{
    return "";
}

static const char* GetOCIconName(void* arg)
{
    return "";
}

void ToggleSpellCheck(void *arg)
{
    FcitxUnikey* unikey = (FcitxUnikey*) arg;
    unikey->config.spellCheck = !unikey->config.spellCheck;
    UpdateUnikeyConfig(unikey);
}

boolean GetSpellCheck(void *arg)
{
    FcitxUnikey* unikey = (FcitxUnikey*) arg;
    return unikey->config.spellCheck;
}

void ToggleMacro(void *arg)
{
    FcitxUnikey* unikey = (FcitxUnikey*) arg;
    unikey->config.macro = !unikey->config.macro;
    UpdateUnikeyConfig(unikey);
}

boolean GetMacro(void *arg)
{
    FcitxUnikey* unikey = (FcitxUnikey*) arg;
    return unikey->config.spellCheck;
}

void InitializeBar(FcitxUnikey* unikey) {
    FcitxInstance* instance = unikey->owner;
    FcitxUIRegisterComplexStatus(instance, unikey,
        "unikey-input-method",
        "",
        _("Choose input method"),
        NULL,
        GetIMIconName
    );

    FcitxUIRegisterComplexStatus(instance, unikey,
        "unikey-output-charset",
        "",
        _("Choose output charset"),
        NULL,
        GetOCIconName
    );
    FcitxUIRegisterStatus(instance, unikey,
        "unikey-spell-check",
        _("Spell Check"),
        _("Enable Spell Check"),
        ToggleSpellCheck,
        GetSpellCheck
    );

    FcitxUIRegisterStatus(instance, unikey,
        "unikey-macro",
        _("Macro"),
        _("Enable Macro"),
        ToggleMacro,
        GetMacro
    );
    FcitxUISetStatusVisable(instance, "unikey-input-method", false);
    FcitxUISetStatusVisable(instance, "unikey-output-charset", false);
    FcitxUISetStatusVisable(instance, "unikey-spell-check", false);
    FcitxUISetStatusVisable(instance, "unikey-macro", false);
}

void InitializeMenu(FcitxUnikey* unikey) {
    FcitxInstance* instance = unikey->owner;
    FcitxMenuInit(&unikey->imMenu);
    FcitxUIMenu* imMenu = &unikey->imMenu;
    imMenu->name = strdup(_("Unikey Input Method"));
    imMenu->candStatusBind = strdup("unikey-input-method");
    imMenu->UpdateMenu = UpdateIMMenu;
    imMenu->MenuAction = IMMenuAction;
    imMenu->priv = unikey;
    imMenu->isSubMenu = false;
    int i;
    for (i = 0; i < NUM_INPUTMETHOD; i ++)
        FcitxMenuAddMenuItem(imMenu, _(Unikey_IMNames[i]), MENUTYPE_SIMPLE, NULL);

    FcitxUIRegisterMenu(instance, imMenu);

    FcitxMenuInit(&unikey->ocMenu);
    FcitxUIMenu* ocMenu = &unikey->ocMenu;
    ocMenu->name = strdup(_("Output Charset"));
    ocMenu->candStatusBind = strdup("unikey-output-charset");
    ocMenu->UpdateMenu = UpdateOCMenu;
    ocMenu->MenuAction = OCMenuAction;
    ocMenu->priv = unikey;
    ocMenu->isSubMenu = false;
    for (i = 0; i < NUM_INPUTMETHOD; i ++)
        FcitxMenuAddMenuItem(ocMenu, _(Unikey_OCNames[i]), MENUTYPE_SIMPLE, NULL);
    FcitxUIRegisterMenu(instance, ocMenu);
}

void UpdateUnikeyUI(FcitxUnikey* unikey)
{
    FcitxUISetStatusString(unikey->owner,
                            "unikey-input-method",
                           _(Unikey_IMNames[unikey->config.im]),
                           _("Choose input method"));


    FcitxUISetStatusString(unikey->owner,
                            "unikey-output-charset",
                           _(Unikey_OCNames[unikey->config.oc]),
                           _("Choose output charset"));

    FcitxUISetStatusString(unikey->owner,
                           "unikey-spell-check",
                           unikey->config.spellCheck? _("Spell Check"): _("No Spell Check"),
                           _("Enable Spell Check"));

    FcitxUISetStatusString(unikey->owner,
                            "unikey-macro",
                           unikey->config.macro? _("Macro"): _("No Macro"),
                           _("Enable Macro"));
}

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

#ifndef UNIKEY_IM_H
#define UNIKEY_IM_H
#include <iconv.h>
#include <string>
#include <fcitx/instance.h>
#include <libintl.h>

#define _(x) dgettext("fcitx-unikey", (x))
#include "unikey-config.h"

struct FcitxUnikey
{
    UnikeyConfig config;
    std::string* preeditstr;
    UnikeyOptions ukopt;
    FcitxInstance* owner;
    bool auto_commit;
    bool last_key_with_shift;
    FcitxUIMenu imMenu;
    FcitxUIMenu ocMenu;
};

void UpdateUnikeyConfig(FcitxUnikey* unikey);

#endif //UNIKEY_IM_H

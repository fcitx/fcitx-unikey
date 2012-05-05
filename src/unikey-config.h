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

#ifndef UNIKEY_CONFIG_H
#define UNIKEY_CONFIG_H
#include <keycons.h>
#include <fcitx-config/fcitx-config.h>

enum UkConv {
    UKCONV_XUTF8,
    UKCONV_TCVN3,
    UKCONV_VNIWIN,
    UKCONV_VIQR,
    UKCONV_BKHCM2,
    UKCONV_UNI_CSTRING,
    UKCONV_UNIREF,
    UKCONV_UNIREF_HEX
};

struct UnikeyConfig
{
    FcitxGenericConfig gconfig;
    UkInputMethod im;
    UkConv oc;
    boolean spellCheck;
    boolean macro;
    boolean process_w_at_begin;
    boolean autoNonVnRestore;
    boolean modernStyle;
    boolean freeMarking;
};


CONFIG_BINDING_DECLARE(UnikeyConfig);

#endif // UNIKEY_CONFIG_H
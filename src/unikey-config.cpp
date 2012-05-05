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

#include "unikey-config.h"

/* USE fcitx provided macro to bind config and variable */
CONFIG_BINDING_BEGIN(UnikeyConfig)
CONFIG_BINDING_REGISTER("Unikey", "InputMethod", im)
CONFIG_BINDING_REGISTER("Unikey", "OuputCharset", oc)
CONFIG_BINDING_REGISTER("Unikey", "SpellCheck", spellCheck)
CONFIG_BINDING_REGISTER("Unikey", "Macro", macro)
CONFIG_BINDING_REGISTER("Unikey", "ModernStyle", modernStyle)
CONFIG_BINDING_REGISTER("Unikey", "FreeMarking", freeMarking)
CONFIG_BINDING_REGISTER("Unikey", "AutoNonVnRestore", autoNonVnRestore)
CONFIG_BINDING_REGISTER("Unikey", "ProcessWAtBegin", process_w_at_begin)
CONFIG_BINDING_END()
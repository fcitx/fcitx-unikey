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

#include <QApplication>
#include <libintl.h>
#include <fcitx-utils/utils.h>
#include "editor.h"
int main(int argc, char* argv[])
{
    setlocale(LC_ALL, "");
    char* path = fcitx_utils_get_fcitx_path("localedir");
    bindtextdomain("fcitx-unikey", path);
    free(path);
    bind_textdomain_codeset("fcitx-unikey", "UTF-8");
    textdomain("fcitx-unikey");

    QApplication app(argc, argv);
    fcitx_unikey::MacroEditor window;
    window.show();
    return app.exec();
}
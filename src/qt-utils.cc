/*
 * art-frame -- an artful sands image emulation
 *
 * Copyright (C) 2019 Jürgen Reuter
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published
 * by the Free Software Foundation; either version 2 of the License,
 * or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
 * 02111-1307, USA.
 *
 * As a special exception to the GNU General Public License, if you
 * distribute this file as part of a program that contains a
 * configuration script generated by Autoconf, you may include it
 * under the same distribution terms that you use for the rest of that
 * program.
 *
 * For updates and more info or contacting the author, visit:
 * <https://github.com/soundpaint/art-frame>
 *
 * Author's web site: www.juergen-reuter.de
 */

#include <qt-utils.hh>
#include <log.hh>

void
Qt_utils::create_pixmap_and_icon(const char *image_path,
                                 QPixmap **pixmap,
                                 QIcon **icon)
{
  *pixmap = new QPixmap(image_path);
  if (!*pixmap) {
    Log::fatal("Status_line::create:icon(): not enough memory");
  }
  *icon = new QIcon(**pixmap);
  if (!*icon) {
    Log::fatal("Status_line::create_icon(): not enough memory");
  }
}

void
Qt_utils::create_button(QWidget *parent,
                        QPushButton **button,
                        const char *tool_tip,
                        QPixmap **pixmap,
                        const char *image_path,
                        QIcon **icon)
{
  create_pixmap_and_icon(image_path, pixmap, icon);
  *button = new QPushButton(/*tr(label)*/);
  if (!*button) {
    Log::fatal("Qt_utils::create_button(): not enough memory");
  }
  (*button)->setIcon(**icon);
  (*button)->setIconSize((*pixmap)->rect().size());
  (*button)->setToolTip(QPushButton::tr(tool_tip));
}

/*
 * Local variables:
 *   mode: c++
 *   coding: utf-8
 * End:
 */
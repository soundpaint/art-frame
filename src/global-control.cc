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

#include <global-control.hh>
#include <QtWidgets/QLabel>
#include <log.hh>
#include <qt-utils.hh>

Global_control::Global_control(QWidget *parent, const IConfig *config)
  : QGroupBox(tr("Art Frame"), parent)
{
  QBoxLayout *layout = new QHBoxLayout();
  if (!layout) {
    Log::fatal("Global_control::Global_control(): not enough memory");
  }
  setLayout(layout);

  if (config->get_enable_button_quit()) {
    Qt_utils::create_button(this,
                            &_button_quit, "shut down system",
                            &_pixmap_quit, "quit.png", &_icon_quit);
    layout->addWidget(_button_quit);
  } else {
    _button_quit = 0;
    _pixmap_quit = 0;
    _icon_quit = 0;
  }

  Qt_utils::create_button(this,
                          &_button_about, "about this application",
                          &_pixmap_about, "about.png", &_icon_about);
  layout->addWidget(_button_about);

  Qt_utils::create_button(this,
                          &_button_license, "show license",
                          &_pixmap_license, "license.png", &_icon_license);
  layout->addWidget(_button_license);
}

Global_control::~Global_control()
{
  // Q objects will be deleted by Qt, just set them to 0
  _button_quit = 0;
  _icon_quit = 0;
  _pixmap_quit = 0;
  _button_about = 0;
  _icon_about = 0;
  _pixmap_about = 0;
  _button_license = 0;
  _icon_license = 0;
  _pixmap_license = 0;
}

QPushButton *
Global_control::get_button_quit() const
{
  return _button_quit;
}

QIcon *
Global_control::get_icon_quit() const
{
  return _icon_quit;
}

QPixmap *
Global_control::get_pixmap_quit() const
{
  return _pixmap_quit;
}

QPushButton *
Global_control::get_button_about() const
{
  return _button_about;
}

QIcon *
Global_control::get_icon_about() const
{
  return _icon_about;
}

QPixmap *
Global_control::get_pixmap_about() const
{
  return _pixmap_about;
}

QPushButton *
Global_control::get_button_license() const
{
  return _button_license;
}

QIcon *
Global_control::get_icon_license() const
{
  return _icon_license;
}

QPixmap *
Global_control::get_pixmap_license() const
{
  return _pixmap_license;
}

/*
 * Local variables:
 *   mode: c++
 *   coding: utf-8
 * End:
 */
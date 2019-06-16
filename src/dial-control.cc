/*
 * art-frame -- an artful sands image emulation
 *
 * Copyright (C) 2016, 2019 Jürgen Reuter
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

#include <dial-control.hh>
#include <QtWidgets/QBoxLayout>
#include <log.hh>

Dial_control::Dial_control(QWidget *parent,
                           const char *label,
                           const double initial_value,
                           const char *tool_tip)
  : QGroupBox(parent)
{
  if (!label) {
    Log::fatal("Dial_control::Dial_control(): label is NULL");
  }
  setTitle(QString(tr(label)));

  _dial = new QDial();
  if (!_dial) {
    Log::fatal("Dial_control::Dial_control(): not enough memory");
  }
  if (tool_tip) {
    _dial->setToolTip(tr(tool_tip));
  }
  _dial->setValue((int)(initial_value * _dial->maximum()));
  QBoxLayout *layout = new QBoxLayout(QBoxLayout::TopToBottom);
  layout->addWidget(_dial);
  setLayout(layout);
  connect(_dial,
	  SIGNAL(valueChanged(int)),
	  this,
	  SIGNAL(value_changed(int)));
}

Dial_control::~Dial_control()
{
  // Q objects will be deleted by Qt, just set them to 0
  _dial = 0;
}

int
Dial_control::minimum() const
{
  return _dial->minimum();
}

int
Dial_control::maximum() const
{
  return _dial->maximum();
}

void
Dial_control::set_value(const int value)
{
  return _dial->setValue(value);
}

int
Dial_control::value() const
{
  return _dial->value();
}

/*
 * Local variables:
 *   mode: c++
 *   coding: utf-8
 * End:
 */

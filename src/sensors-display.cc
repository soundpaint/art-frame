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

#include <QtGui/QPalette>
#include <sensors-display.hh>
#include <log.hh>

Sensors_display::Sensors_display(QWidget *parent) : QGroupBox(parent)
{
  _layout = new QHBoxLayout();
  if (!_layout) {
    Log::fatal("Sensors_display::Sensors_display(): not enough memory");
  }
  setLayout(_layout);

  _col1 = new QWidget();
  if (!_col1) {
    Log::fatal("Sensors_display::Sensors_display(): not enough memory");
  }
  _layout->addWidget(_col1);

  _col1_layout = new QFormLayout();
  if (!_col1_layout) {
    Log::fatal("Sensors_display::Sensors_display(): not enough memory");
  }
  _col1->setLayout(_col1_layout);

  _label_pitch_key = new QLabel(tr("Pitch:"));
  if (!_label_pitch_key) {
    Log::fatal("Sensors_display::Sensors_display(): not enough memory");
  }
  _label_pitch_value = new QLabel();
  if (!_label_pitch_value) {
    Log::fatal("Sensors_display::Sensors_display(): not enough memory");
  }
  _col1_layout->addRow(_label_pitch_key, _label_pitch_value);

  _label_roll_key = new QLabel(tr("Roll:"));
  if (!_label_roll_key) {
    Log::fatal("Sensors_display::Sensors_display(): not enough memory");
  }
  _label_roll_value = new QLabel();
  if (!_label_roll_value) {
    Log::fatal("Sensors_display::Sensors_display(): not enough memory");
  }
  _col1_layout->addRow(_label_roll_key, _label_roll_value);

  _col2 = new QWidget();
  if (!_col2) {
    Log::fatal("Sensors_display::Sensors_display(): not enough memory");
  }
  _layout->addWidget(_col2);

  _col2_layout = new QFormLayout();
  if (!_col2_layout) {
    Log::fatal("Sensors_display::Sensors_display(): not enough memory");
  }
  _col2->setLayout(_col2_layout);

  _label_ax_key = new QLabel(tr("ax:"));
  if (!_label_ax_key) {
    Log::fatal("Sensors_display::Sensors_display(): not enough memory");
  }
  _label_ax_value = new QLabel();
  if (!_label_ax_value) {
    Log::fatal("Sensors_display::Sensors_display(): not enough memory");
  }
  _col2_layout->addRow(_label_ax_key, _label_ax_value);

  _label_ay_key = new QLabel(tr("ay:"));
  if (!_label_ay_key) {
    Log::fatal("Sensors_display::Sensors_display(): not enough memory");
  }
  _label_ay_value = new QLabel();
  if (!_label_ay_value) {
    Log::fatal("Sensors_display::Sensors_display(): not enough memory");
  }
  _col2_layout->addRow(_label_ay_key, _label_ay_value);
}

Sensors_display::~Sensors_display()
{
  // Q objects will be deleted by Qt, just set them to 0
  _layout = 0;
  _label_pitch_key = 0;
  _label_pitch_value = 0;
  _label_roll_key = 0;
  _label_roll_value = 0;
  _label_ax_key = 0;
  _label_ax_value = 0;
  _label_ay_key = 0;
  _label_ay_value = 0;
  _col1_layout = 0;
  _col1 = 0;
  _col2_layout = 0;
  _col2 = 0;
}

void
Sensors_display::slot_update(const double pitch,
			     const double roll,
			     const double ax,
			     const double ay)
{
  _label_pitch_value->setNum(pitch);
  _label_roll_value->setNum(roll);
  _label_ax_value->setNum(ax);
  _label_ay_value->setNum(ay);
  update();
}

/*
 * Local variables:
 *   mode: c++
 *   coding: utf-8
 * End:
 */

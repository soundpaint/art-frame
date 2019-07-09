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

#include <thermal-display.hh>
#include <log.hh>

Thermal_display::Thermal_display(QWidget *parent)
  : QGroupBox(tr("Temperature"), parent)
{
  _layout = new QHBoxLayout();
  if (!_layout) {
    Log::fatal("Thermal_display::Thermal_display(): not enough memory");
  }
  setLayout(_layout);

  _column = new QWidget();
  if (!_column) {
    Log::fatal("Thermal_display::Thermal_display(): not enough memory");
  }
  _layout->addWidget(_column);

  _column_layout = new QFormLayout();
  if (!_column_layout) {
    Log::fatal("Thermal_display::Thermal_display(): not enough memory");
  }
  _column->setLayout(_column_layout);

  _label_cpu_temperature_key = new QLabel(tr("CPU[℃]:"));
  if (!_label_cpu_temperature_key) {
    Log::fatal("Thermal_display::Thermal_display(): not enough memory");
  }
  _field_cpu_temperature_value = new QLineEdit();
  if (!_field_cpu_temperature_value) {
    Log::fatal("Thermal_display::Thermal_display(): not enough memory");
  }
  _field_cpu_temperature_value->setReadOnly(true);
  _field_cpu_temperature_value->setEnabled(false);
  _column_layout->addRow(_label_cpu_temperature_key,
                         _field_cpu_temperature_value);

  _label_case_temperature_key = new QLabel(tr("Case [℃]:"));
  if (!_label_case_temperature_key) {
    Log::fatal("Thermal_display::Thermal_display(): not enough memory");
  }
  _field_case_temperature_value = new QLineEdit();
  if (!_field_case_temperature_value) {
    Log::fatal("Thermal_display::Thermal_display(): not enough memory");
  }
  _field_case_temperature_value->setReadOnly(true);
  _field_case_temperature_value->setEnabled(false);
  _column_layout->addRow(_label_case_temperature_key,
                         _field_case_temperature_value);
}

Thermal_display::~Thermal_display()
{
  // Q objects will be deleted by Qt, just set them to 0
  _layout = 0;
  _label_cpu_temperature_key = 0;
  _field_cpu_temperature_value = 0;
  _label_case_temperature_key = 0;
  _field_case_temperature_value = 0;
  _column_layout = 0;
  _column = 0;
}

void
Thermal_display::slot_update_cpu_temperature(const double cpu_temperature)
{
  _field_cpu_temperature_value->setText(QString::number(cpu_temperature));
  update();
}

void
Thermal_display::slot_update_case_temperature(const double case_temperature)
{
  _field_case_temperature_value->setText(QString::number(case_temperature));
  update();
}

/*
 * Local variables:
 *   mode: c++
 *   coding: utf-8
 * End:
 */

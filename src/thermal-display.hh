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

#ifndef THERMAL_DISPLAY_HH
#define THERMAL_DISPLAY_HH

#include <QtCore/QObject>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>

class Thermal_display : public QGroupBox
{
  Q_OBJECT
public:
  explicit Thermal_display(QWidget *parent);
  virtual ~Thermal_display();
public slots:
  void slot_update_cpu_temperature(const double cpu_temperature);
  void slot_update_case_temperature(const double case_temperature);
private:
  QHBoxLayout *_layout;
  QWidget *_column;
  QFormLayout *_column_layout;
  QLabel *_label_cpu_temperature_key;
  QLineEdit *_field_cpu_temperature_value;
  QLabel *_label_case_temperature_key;
  QLineEdit *_field_case_temperature_value;
};

#endif /* THERMAL_DISPLAY_HH */

/*
 * Local variables:
 *   mode: c++
 *   coding: utf-8
 * End:
 */
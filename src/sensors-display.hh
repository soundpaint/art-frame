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

#ifndef SENSORS_DISPLAY_HH
#define SENSORS_DISPLAY_HH

#include <QtCore/QObject>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>

class Sensors_display : public QGroupBox
{
  Q_OBJECT
public:
  explicit Sensors_display(QWidget *parent);
  virtual ~Sensors_display();
public slots:
  void slot_update(const double pitch,
                   const double roll,
                   const double ax,
                   const double ay);
private:
  QHBoxLayout *_layout;
  QFormLayout *_col1_layout, *_col2_layout;
  QWidget *_col1, *_col2;
  QLabel *_label_pitch_key;
  QLineEdit *_field_pitch_value;
  QLabel *_label_roll_key;
  QLineEdit *_field_roll_value;
  QLabel *_label_ax_key;
  QLineEdit *_field_ax_value;
  QLabel *_label_ay_key;
  QLineEdit *_field_ay_value;
};

#endif /* SENSORS_DISPLAY_HH */

/*
 * Local variables:
 *   mode: c++
 *   coding: utf-8
 * End:
 */

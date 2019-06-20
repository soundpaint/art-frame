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

#ifndef SENSORS_HH
#define SENSORS_HH

#include <inttypes.h>
#include <QtCore/QTimer>
#include <RTIMULib.h>
#include <iconfig.hh>

class Sensors : public QTimer
{
  Q_OBJECT
public:
  explicit Sensors(QObject *parent, const IConfig *config);
  virtual ~Sensors();
  const double get_pitch() const;
  const double get_roll() const;
  const double get_ax() const;
  const double get_ay() const;
  const double get_temperature() const;
private slots:
  void sample_and_hold();
signals:
  void sample_updated(const double pitch,
                      const double roll,
                      const double ax,
                      const double ay,
                      const double temperature);
private:
  const IConfig *_config;
  uint64_t _display_timer;
  RTIMUSettings *_settings;
  RTIMU *_imu;
  double _pitch;
  double _roll;
  double _acceleration_x;
  double _acceleration_y;
  double _temperature;
};

#endif /* SENSORS_HH */

/*
 * Local variables:
 *   mode: c++
 *   coding: utf-8
 * End:
 */

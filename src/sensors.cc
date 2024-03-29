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

#include <sensors.hh>
#include <cmath>
#include <log.hh>

Sensors::Sensors(QObject *parent, const IConfig *config) : QTimer(parent)
{
  if (!config) {
    Log::fatal("Sensors::Sensors(): config is NULL");
  }
  _config = config;

  _pitch = 0.0;
  _roll = 0.0;
  _acceleration_x = 0.0;
  _acceleration_y = 0.0;
  _temperature = 0.0;

  start(10);

  _settings = new RTIMUSettings("RTIMULib");
  if (!_settings) {
    Log::fatal("Sensors::Sensors(): not enough memory");
  }

  _imu = RTIMU::createIMU(_settings);
  if (!_imu || (_imu->IMUType() == RTIMU_TYPE_NULL)) {
    Log::warn("Sensors::Sensors(): no IMU found => sensors will not work");
    if (_config->get_enable_sensors_fake_data()) {
      Log::warn("Sensors::Sensors(): using fake sensor data as configured");
    }
  } else {
    _imu->IMUInit();
    _imu->setSlerpPower(0.02);
    _imu->setGyroEnable(true);
    _imu->setAccelEnable(true);
    _imu->setCompassEnable(true);
  }
  _display_timer = RTMath::currentUSecsSinceEpoch();
  QObject::connect(this, SIGNAL(timeout()),
                   this, SLOT(sample_and_hold()));
  QObject::connect(this,
                   SIGNAL(signal_sample_updated(double, double, double,
                                                double, double)),
                   parent,
                   SLOT(slot_update_sensors_display(double, double, double,
                                                    double, double)));
}

Sensors::~Sensors()
{
  if (_imu) {
    delete _imu;
    _imu = 0;
  }

  if (_settings) {
    delete _settings;
    _settings = 0;
  }

  _display_timer = 0;
  _pitch = 0.0;
  _roll = 0.0;
  _acceleration_x = 0.0;
  _acceleration_y = 0.0;
  _temperature = 0.0;
  _config = 0;
}

void
Sensors::sample_and_hold()
{
  const uint64_t now = RTMath::currentUSecsSinceEpoch();
  if ((now - _display_timer) > 100000) {
    if (_imu && (_imu->IMUType() != RTIMU_TYPE_NULL)) {
      if (_imu->IMURead()) {
        const RTIMU_DATA imuData = _imu->getIMUData();
        _pitch = imuData.fusionPose.x(); // * RTMATH_RAD_TO_DEGREE;
        _roll = imuData.fusionPose.y(); // * RTMATH_RAD_TO_DEGREE;
        _acceleration_x = imuData.accel.x();
        _acceleration_y = imuData.accel.y();
        _temperature = imuData.temperature;
        emit signal_sample_updated(_pitch, _roll,
                                   _acceleration_x, _acceleration_y,
                                   _temperature); // update sensors display
      } else {
        // keep sensor data unchanged
      }
    } else {
      // no sensors available
      if (_config->get_enable_sensors_fake_data()) {
        _pitch = _config->get_fake_pitch();
        _roll = _config->get_fake_roll();
        _acceleration_x = _config->get_fake_acceleration_x();
        _acceleration_y = _config->get_fake_acceleration_y();
        _temperature = 0.0; // TODO
        emit signal_sample_updated(_pitch, _roll,
                                   _acceleration_x, _acceleration_y,
                                   _temperature); // update sensors display
      } else {
        // keep sensor data unchanged
      }
    }
    _display_timer = now;
  }
}

// return value is in the range (-0.5 * M_PI .. 0.5 * M_PI].
const double
Sensors::get_pitch() const
{
  return _pitch;
}

// return value is in the range (-0.5 * M_PI .. 0.5 * M_PI].
const double
Sensors::get_roll() const
{
  return _roll;
}

const double
Sensors::get_ax() const
{
  return _acceleration_x;
}

const double
Sensors::get_ay() const
{
  return _acceleration_y;
}

const double
Sensors::get_temperature() const
{
  return _temperature;
}

/*
 * Local variables:
 *   mode: c++
 *   coding: utf-8
 * End:
 */

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

#ifndef SIMULATION_HH
#define SIMULATION_HH

#include <isimulation-control.hh>
#include <QtCore/QThread>
#include <sys/timeb.h>
#include <particles.hh>

class Simulation : public QThread, public ISimulation_control
{
  Q_OBJECT
public:
  explicit Simulation(const uint16_t width,
                      const uint16_t height,
                      const IConfig *config,
                      const Sensors *sensors,
                      const Thermal_sensors *thermal_sensors);
  virtual ~Simulation();
  Particles *get_particles();
  void pause();
  void resume();
  const bool is_running() const;
  const bool is_pausing() const;
  void reset_image();
  void load_image(const Config_image *image);
  void set_gravity(const int8_t gravity);
  const int8_t get_gravity() const;
  const double get_activity_level() const;
  const uint64_t started_at() const;
  const uint64_t stopped_at() const;
private:
  static const uint16_t FAILBACK_THREADS_SUPPORTED;
  enum Status {starting, running, pausing, stopped};
  Particles *_particles;
  Status _status;
  int8_t _gravity;
  uint16_t _oversampling;
  uint64_t _started_at;
  uint64_t _stopped_at;
  void set_status(const Status status);
  const bool has_status(const Status status) const;
  static Particles *
  create_particles(const uint16_t width,
                   const uint16_t height,
                   const IConfig *config,
                   const Sensors *sensors,
                   const Thermal_sensors *thermal_sensors);
  void update_status_time();
  virtual void run() override;
};

#endif /* SIMULATION_HH */

/*
 * Local variables:
 *   mode: c++
 *   coding: utf-8
 * End:
 */

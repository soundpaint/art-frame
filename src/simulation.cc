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

#include <simulation.hh>
#include <thread>
#include <frame-display.hh>
#include <log.hh>

Simulation::Simulation(const uint16_t width,
                       const uint16_t height,
                       const IConfig *config,
                       const Sensors *sensors,
                       const Cpu_status *cpu_status)
  : QTimer()
{
  _oversampling = 1;
  _started_at = RTMath::currentUSecsSinceEpoch();
  _stopped_at = RTMath::currentUSecsSinceEpoch();
  _particles = create_particles(width, height, config, sensors, cpu_status);
  set_status(pausing);
  connect(this, SIGNAL(timeout()),
          this, SLOT(slot_update_particles()));
}

Simulation::~Simulation()
{
  set_status(stopped);
  delete _particles;
  _particles = 0;
  _oversampling = 0;
  _started_at = 0;
  _stopped_at = 0;
}

const uint16_t
Simulation::FAILBACK_THREADS_SUPPORTED = 4;

Particles *
Simulation::create_particles(const uint16_t width,
                             const uint16_t height,
                             const IConfig *config,
                             const Sensors *sensors,
                             const Cpu_status *cpu_status)
{
  const int concurent_threads_supported = std::thread::hardware_concurrency();
  const int num_threads =
    concurent_threads_supported ?
    concurent_threads_supported :
    FAILBACK_THREADS_SUPPORTED;
  Particles *particles = new Particles(width, height, config, sensors,
                                       cpu_status, num_threads);
  if (!particles) {
    Log::fatal("Simulation::create_particles(): not enough memory");
  }
  return particles;
}

Particles *
Simulation::get_particles()
{
  return _particles;
}

void
Simulation::update_status_time()
{
  const uint64_t now = RTMath::currentUSecsSinceEpoch();
  if (_status == running) {
    _started_at = now;
  } else if (_status == pausing) {
    _stopped_at = now;
  } else {
    std::stringstream msg;
    msg << "Simulation::update_status_time(): unexpected status: " << _status;
    Log::warn(msg.str());
  }
}

void
Simulation::set_status(const Status status)
{
  // TODO: check for invalid state change
  _status = status;
  update_status_time();
}

const bool
Simulation::has_status(const Status status) const
{
  return _status == status;
}

void
Simulation::reset_image()
{
  _particles->reset();
  update_status_time();
}

void
Simulation::load_image(const Config_image *image)
{
  _particles->load_image(image);
  update_status_time();
}

const bool
Simulation::is_running() const
{
  return has_status(running);
}

void
Simulation::pause()
{
  if (has_status(running)) {
    set_status(pausing);
  } else {
    std::stringstream msg;
    msg << "Simulation::pause(): unexpected status: " << _status;
    Log::warn(msg.str());
  }
}

const bool
Simulation::is_pausing() const
{
  return has_status(pausing);
}

void
Simulation::resume()
{
  if (has_status(pausing)) {
    set_status(running);
  } else {
    // pause function disabled during other states
  }
}

void
Simulation::set_speed(const double speed)
{
  if ((speed < 0.0) || (speed > 1.0)) {
    Log::fatal("Simulation::set_speed(): speed out of range");
  }
  _speed = speed;
  _oversampling = (uint16_t)(exp(speed * log(UINT16_MAX)) + 0.5);
  // TODO: simulation does not yet consider oversampling value.
}

const double
Simulation::get_speed() const
{
  return _speed;
}

void
Simulation::slot_update_particles()
{
  switch (_status)
  {
    case starting:
      // frame display may not yet have been initialized => do nothing
      break;
    case running:
      {
        _particles->update();
      }
      break;
    case pausing:
      // pause => do nothing
      break;
    case stopped:
      // done => do nothing
      break;
    default:
      Log::fatal("Simulation::update(): unexpected case fall-through");
  }
}

const double
Simulation::get_activity_level() const
{
  return _particles->get_activity_level();
}

const uint64_t
Simulation::started_at() const
{
  return _started_at;
}

const uint64_t
Simulation::stopped_at() const
{
  return _stopped_at;
}

/*
 * Local variables:
 *   mode: c++
 *   coding: utf-8
 * End:
 */

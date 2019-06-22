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

#include <activity-monitor.hh>
#include <RTIMULib.h>
#include <log.hh>

Activity_monitor::Activity_monitor(QObject *parent,
                                   const IConfig *config,
                                   const ISimulation_control *simulation)
  : QTimer(parent)
{
  if (!config) {
    Log::fatal("Activity_monitor::Activity_monitor(): config is NULL");
  }
  _config = config;

  if (!simulation) {
    Log::fatal("Activity_monitor::Activity_monitor(): simulation is NULL");
  }
  _simulation = simulation;

  QObject::connect(this, SIGNAL(timeout()),
                   this, SLOT(slot_check_activity()));
  start(1000);
}

Activity_monitor::~Activity_monitor()
{
  _simulation = 0;
  _config = 0;
}

const bool
Activity_monitor::running_at_least(const uint32_t seconds)
{
  const uint64_t started_at = _simulation->started_at();
  const uint64_t now = RTMath::currentUSecsSinceEpoch();
  return now - started_at > 1000000 * seconds;
}

void
Activity_monitor::slot_check_activity()
{
  if (_simulation->is_running()) {
    const double activity_level = _simulation->get_activity_level();
    if (activity_level <= _config->get_stop_below_activity()) {
      if (running_at_least(_config->get_check_activity_only_after())) {
        emit signal_low_activity();
      }
    }
  }
}

/*
 * Local variables:
 *   mode: c++
 *   coding: utf-8
 * End:
 */

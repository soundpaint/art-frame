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

#include <cpu-status.hh>
#include <log.hh>
#include <fcntl.h>
#include <unistd.h>
#include <QtCore/QDateTime>

Cpu_status::Cpu_status(QObject *parent) : QTimer(parent)
{
  _vc_temperature = 0.0;

  start(1000);

  _display_timer = QDateTime::currentMSecsSinceEpoch();
  QObject::connect(this, SIGNAL(timeout()),
		   this, SLOT(sample_and_hold()));
  QObject::connect(this, SIGNAL(sample_updated(double)),
		   parent, SLOT(slot_update_cpu_status_display(double)));
}

Cpu_status::~Cpu_status()
{
  _vc_temperature = 0.0;
  _display_timer = 0;
}

#define BUF_LEN 4096

void
Cpu_status::sample_and_hold()
{
  const uint64_t now = QDateTime::currentMSecsSinceEpoch();
  if ((now - _display_timer) > 100) {
    const int file_handle =
      open("/sys/class/thermal/thermal_zone0/temp", O_RDONLY);
    if (file_handle >= 0) {
      double vc_value = 0.0;
      char buffer[BUF_LEN];
      const int bytes_read = read(file_handle, buffer, sizeof(buffer) - 1);
      if (bytes_read >= 0) {
	buffer[bytes_read] = 0; // ensure string terminates with zero byte
	sscanf(buffer, "%lf", &vc_value);
	_vc_temperature = vc_value * 0.001;
	emit sample_updated(_vc_temperature);
      } else {
        Log::warn("Cpu_status::Cpu_status(): _vc_temperature not available");
      }
      const int close_result = close(file_handle);
      if (close_result) {
        Log::warn("Cpu_status::Cpu_status(): "
	          "failed closing temperature file");
      }
      _display_timer = now;
    } else {
      Log::warn("Cpu_status::Cpu_status(): _vc_temperature not supported");
    }
  }
}

const double
Cpu_status::get_vc_temperature() const
{
  return _vc_temperature;
}

/*
 * Local variables:
 *   mode: c++
 *   coding: utf-8
 * End:
 */
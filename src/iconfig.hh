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

#ifndef ICONFIG_HH
#define ICONFIG_HH

#include <inttypes.h>
#include <config-image.hh>

class IConfig
{
public:
  virtual ~IConfig() = 0;
  virtual const double get_start_fan_temperature() const = 0;
  virtual const double get_stop_fan_temperature() const = 0;
  virtual const double get_start_cooling_break_temperature() const = 0;
  virtual const double get_stop_cooling_break_temperature() const = 0;
  virtual const uint32_t get_frame_usleep_min() const = 0;
  virtual const uint32_t get_frame_usleep_max() const = 0;
  virtual const bool get_enable_cursor() const = 0;
  virtual const bool get_enable_button_quit() const = 0;
  virtual const bool get_enable_key_quit() const = 0;
  virtual const bool get_simulation_start_on_application_start() const = 0;
  virtual const double get_simulation_initial_speed() const = 0;
  virtual const bool get_enable_audio() const = 0;
  virtual const double get_audio_initial_volume() const = 0;
  virtual const uint32_t get_images_count() const = 0;
  virtual const Config_image *get_image(const uint32_t index) const = 0;
};

inline IConfig::~IConfig() {}

#endif /* ICONFIG_HH */

/*
 * Local variables:
 *   mode: c++
 *   coding: utf-8
 * End:
 */

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

#ifndef CONFIG_HH
#define CONFIG_HH

#include <iconfig.hh>
#include <config-image.hh>
#include <config-image-browser.hh>

class Config : public IConfig
{
public:
  static const double DEFAULT_START_FAN_TEMPERATURE;
  static const double DEFAULT_STOP_FAN_TEMPERATURE;
  static const double DEFAULT_START_COOLING_BREAK_TEMPERATURE;
  static const double DEFAULT_STOP_COOLING_BREAK_TEMPERATURE;
  static const uint32_t DEFAULT_FRAME_USLEEP_MIN;
  static const uint32_t DEFAULT_FRAME_USLEEP_MAX;
  static const bool DEFAULT_ENABLE_BUTTON_QUIT;
  static const bool DEFAULT_ENABLE_KEY_QUIT;
  static const bool DEFAULT_ENABLE_AUDIO;
  Config(const char *path);
  virtual ~Config();
  void set_start_fan_temperature(const double start_fan_temperature);
  const double get_start_fan_temperature() const;
  void set_stop_fan_temperature(const double stop_fan_temperature);
  const double get_stop_fan_temperature() const;
  void set_start_cooling_break_temperature(const double start_cooling_break_temperature);
  const double get_start_cooling_break_temperature() const;
  void set_stop_cooling_break_temperature(const double stop_cooling_break_temperature);
  const double get_stop_cooling_break_temperature() const;
  void set_frame_usleep_min(const uint32_t frame_usleep_min);
  const uint32_t get_frame_usleep_min() const;
  void set_frame_usleep_max(const uint32_t frame_usleep_max);
  const uint32_t get_frame_usleep_max() const;
  void set_enable_button_quit(const bool enable_button_quit);
  const bool get_enable_button_quit() const;
  void set_enable_key_quit(const bool enable_key_quit);
  const bool get_enable_key_quit() const;
  void set_enable_audio(const bool enable_audio);
  const bool get_enable_audio() const;
  void add_image(const Config_image *image);
  const uint32_t get_images_count() const;
  const Config_image *get_image(const uint32_t index) const;
private:
  double _start_fan_temperature;
  double _stop_fan_temperature;
  double _start_cooling_break_temperature;
  double _stop_cooling_break_temperature;
  uint32_t _frame_usleep_min;
  uint32_t _frame_usleep_max;
  bool _enable_button_quit;
  bool _enable_key_quit;
  bool _enable_audio;
  std::vector<const Config_image *> *_images;
};

#endif /* CONFIG_HH */

/*
 * Local variables:
 *   mode: c++
 *   coding: utf-8
 * End:
 */

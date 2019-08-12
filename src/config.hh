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
  static const bool DEFAULT_FULL_SCREEN;
  static const uint16_t DEFAULT_CONTROL_SHOW_AFTER_PRESSING;
  static const uint16_t DEFAULT_CONTROL_AUTOHIDE_AFTER;
  static const bool DEFAULT_ENABLE_CURSOR;
  static const bool DEFAULT_ENABLE_BUTTON_QUIT;
  static const bool DEFAULT_ENABLE_KEY_QUIT;
  static const bool DEFAULT_ENABLE_SENSORS_FAKE_DATA;
  static const double DEFAULT_FAKE_ROLL;
  static const double DEFAULT_FAKE_PITCH;
  static const double DEFAULT_FAKE_ACCELERATION_X;
  static const double DEFAULT_FAKE_ACCELERATION_Y;
  static const bool DEFAULT_SIMULATION_START_ON_APPLICATION_START;
  static const int8_t DEFAULT_SIMULATION_INITIAL_GRAVITY;
  static const double DEFAULT_SWEEP_SENSITIVITY;
  static const int DEFAULT_SWEEP_WIDTH;
  static const double DEFAULT_SWEEP_FADE_FACTOR;
  static const double DEFAULT_STOP_BELOW_ACTIVITY;
  static const int DEFAULT_CHECK_ACTIVITY_ONLY_AFTER;
  static const int DEFAULT_CHANGE_IMAGE_WHEN_PAUSED;
  static const int DEFAULT_CHANGE_IMAGE_WHEN_RUNNING;
  static const bool DEFAULT_ENABLE_AUDIO;
  static const double DEFAULT_AUDIO_SAMPLE_SCALE;
  static const int DEFAULT_AUDIO_SAMPLE_RATE;
  static const int DEFAULT_AUDIO_CHANNELS;
  static const bool DEFAULT_ALSA_VERBOSE;
  static const char *DEFAULT_ALSA_PLAYBACK_DEVICE;
  static const int DEFAULT_ALSA_BUFFER_TIME;
  static const int DEFAULT_ALSA_PERIOD_TIME;
  static const double DEFAULT_AUDIO_INITIAL_VOLUME;
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
  void set_full_screen(const bool full_screen);
  const bool get_full_screen() const;
  void set_control_show_after_pressing(const uint16_t control_show_after_pressing);
  const uint16_t get_control_show_after_pressing() const;
  void set_control_autohide_after(const uint16_t control_autohide_after);
  const uint16_t get_control_autohide_after() const;
  void set_enable_cursor(const bool enable_cursor);
  const bool get_enable_cursor() const;
  void set_enable_button_quit(const bool enable_button_quit);
  const bool get_enable_button_quit() const;
  void set_enable_key_quit(const bool enable_key_quit);
  const bool get_enable_key_quit() const;
  void bind_key_to_action(const int key, const Key_bindings::Action action);
  const Key_bindings::Action get_action_for_key(const int key) const;
  void set_enable_sensors_fake_data(const bool enable_sensors_fake_data);
  const bool get_enable_sensors_fake_data() const;
  void set_fake_roll(const double fake_roll);
  const double get_fake_roll() const;
  void set_fake_pitch(const double fake_pitch);
  const double get_fake_pitch() const;
  void set_fake_acceleration_x(const double fake_acceleration_x);
  const double get_fake_acceleration_x() const;
  void set_fake_acceleration_y(const double fake_acceleration_y);
  const double get_fake_acceleration_y() const;
  void set_simulation_start_on_application_start(const bool simulation_start_on_application_start);
  const bool get_simulation_start_on_application_start() const;
  void set_simulation_initial_gravity(const int8_t simulation_initial_gravity);
  const int8_t get_simulation_initial_gravity() const;
  void set_sweep_sensitivity(const double sweep_sensitivity);
  const double get_sweep_sensitivity() const;
  void set_sweep_width(const int sweep_width);
  const int get_sweep_width() const;
  void set_sweep_fade_factor(const double sweep_fade_factor);
  const double get_sweep_fade_factor() const;
  void set_stop_below_activity(const double stop_below_activity);
  const double get_stop_below_activity() const;
  void set_check_activity_only_after(const int check_activity_only_after);
  const int get_check_activity_only_after() const;
  void set_change_image_when_paused(const int change_image_when_paused);
  const int get_change_image_when_paused() const;
  void set_change_image_when_running(const int change_image_when_running);
  const int get_change_image_when_running() const;
  void set_enable_audio(const bool enable_audio);
  const bool get_enable_audio() const;
  void set_audio_sample_scale(const double sample_scale);
  const double get_audio_sample_scale() const;
  void set_audio_sample_rate(const int sample_rate);
  const int get_audio_sample_rate() const;
  void set_audio_channels(const int channels);
  const int get_audio_channels() const;
  void set_alsa_verbose(const bool alsa_verbose);
  const bool get_alsa_verbose() const;
  void set_alsa_playback_device(const char *alsa_playback_device);
  const char *get_alsa_playback_device() const;
  void set_alsa_buffer_time(const int alsa_buffer_time);
  const int get_alsa_buffer_time() const;
  void set_alsa_period_time(const int alsa_period_time);
  const int get_alsa_period_time() const;
  void set_audio_initial_volume(const double audio_initial_volume);
  const double get_audio_initial_volume() const;
  void add_image(const Config_image *image);
  const uint32_t get_images_count() const;
  const Config_image *get_image(const uint32_t index) const;
  void to_string(std::stringstream *buffer) const;
private:
  double _start_fan_temperature;
  double _stop_fan_temperature;
  double _start_cooling_break_temperature;
  double _stop_cooling_break_temperature;
  uint32_t _frame_usleep_min;
  uint32_t _frame_usleep_max;
  bool _full_screen;
  uint16_t _control_show_after_pressing;
  uint16_t _control_autohide_after;
  bool _enable_cursor;
  bool _enable_button_quit;
  bool _enable_key_quit;
  bool _enable_sensors_fake_data;
  double _fake_roll;
  double _fake_pitch;
  double _fake_acceleration_x;
  double _fake_acceleration_y;
  bool _simulation_start_on_application_start;
  int8_t _simulation_initial_gravity;
  double _sweep_sensitivity;
  int _sweep_width;
  double _sweep_fade_factor;
  double _stop_below_activity;
  int _check_activity_only_after;
  int _change_image_when_paused;
  int _change_image_when_running;
  bool _enable_audio;
  double _audio_sample_scale;
  int _audio_sample_rate;
  int _audio_channels;
  bool _alsa_verbose;
  const char *_alsa_playback_device;
  int _alsa_buffer_time;
  int _alsa_period_time;
  double _audio_initial_volume;
  Key_bindings *_key_bindings;
  std::vector<const Config_image *> *_images;
};

#endif /* CONFIG_HH */

/*
 * Local variables:
 *   mode: c++
 *   coding: utf-8
 * End:
 */

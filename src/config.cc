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

#include <config.hh>
#include <log.hh>
#include <config-reader.hh>

const double
Config::DEFAULT_START_FAN_TEMPERATURE = 60.0;

const double
Config::DEFAULT_STOP_FAN_TEMPERATURE = 55.0;

const double
Config::DEFAULT_START_COOLING_BREAK_TEMPERATURE = 80.0;

const double
Config::DEFAULT_STOP_COOLING_BREAK_TEMPERATURE = 75.0;

const uint32_t
Config::DEFAULT_FRAME_USLEEP_MIN = 0;

const uint32_t
Config::DEFAULT_FRAME_USLEEP_MAX = 500000;

const bool
Config::DEFAULT_FULL_SCREEN = true;

const uint16_t
Config::DEFAULT_WINDOW_WIDTH = 800;

const uint16_t
Config::DEFAULT_WINDOW_HEIGHT = 640;

const uint16_t
Config::DEFAULT_CONTROL_SHOW_AFTER_PRESSING = 0;

const uint16_t
Config::DEFAULT_CONTROL_AUTOHIDE_AFTER = 10;

const bool
Config::DEFAULT_ENABLE_CURSOR = false;

const bool
Config::DEFAULT_ENABLE_BUTTON_QUIT = true;

const bool
Config::DEFAULT_ENABLE_SENSORS_FAKE_DATA = true;

const double
Config::DEFAULT_FAKE_ROLL = -2.2;

const double
Config::DEFAULT_FAKE_PITCH = -0.02;

const double
Config::DEFAULT_FAKE_ACCELERATION_X = 0.02;

const double
Config::DEFAULT_FAKE_ACCELERATION_Y = -0.8;

const bool
Config::DEFAULT_SIMULATION_START_ON_APPLICATION_START = true;

const bool
Config::DEFAULT_ENABLE_GRAVITY_CONTROL = true;

const int8_t
Config::DEFAULT_SIMULATION_INITIAL_GRAVITY = 2;

const double
Config::DEFAULT_SWEEP_SENSITIVITY = 10.0;

const int
Config::DEFAULT_SWEEP_WIDTH = 5;

const double
Config::DEFAULT_SWEEP_FADE_FACTOR = 0.8;

const double
Config::DEFAULT_STOP_BELOW_ACTIVITY = 0.0003;

const int
Config::DEFAULT_CHECK_ACTIVITY_ONLY_AFTER = 10;

const int
Config::DEFAULT_CHANGE_IMAGE_WHEN_PAUSED = 15;

const int
Config::DEFAULT_CHANGE_IMAGE_WHEN_RUNNING = 600;

const bool
Config::DEFAULT_ENABLE_AUDIO = true;

const bool
Config::DEFAULT_ENABLE_VOLUME_CONTROL = true;

const double
Config::DEFAULT_AUDIO_SAMPLE_SCALE = 1.0;

const int
Config::DEFAULT_AUDIO_SAMPLE_RATE = 44100;

const int
Config::DEFAULT_AUDIO_CHANNELS = 2;

const bool
Config::DEFAULT_ALSA_VERBOSE = true;

const std::string
Config::DEFAULT_ALSA_PLAYBACK_DEVICE = std::string("default");

const int
Config::DEFAULT_ALSA_BUFFER_TIME = 250000;

const int
Config::DEFAULT_ALSA_PERIOD_TIME = 50000;

const double
Config::DEFAULT_AUDIO_INITIAL_VOLUME = 0.5;

const std::string
Config::DEFAULT_CAPTURING_PATH = std::string(".");

Config::Config(const char *path)
{
  set_start_fan_temperature(DEFAULT_START_FAN_TEMPERATURE);
  set_stop_fan_temperature(DEFAULT_STOP_FAN_TEMPERATURE);
  set_start_cooling_break_temperature(DEFAULT_START_COOLING_BREAK_TEMPERATURE);
  set_stop_cooling_break_temperature(DEFAULT_STOP_COOLING_BREAK_TEMPERATURE);
  set_frame_usleep_min(DEFAULT_FRAME_USLEEP_MIN);
  set_frame_usleep_max(DEFAULT_FRAME_USLEEP_MAX);
  set_full_screen(DEFAULT_FULL_SCREEN);
  set_window_width(DEFAULT_WINDOW_WIDTH);
  set_window_height(DEFAULT_WINDOW_HEIGHT);
  set_control_show_after_pressing(DEFAULT_CONTROL_SHOW_AFTER_PRESSING);
  set_control_autohide_after(DEFAULT_CONTROL_AUTOHIDE_AFTER);
  set_enable_cursor(DEFAULT_ENABLE_CURSOR);
  set_enable_button_quit(DEFAULT_ENABLE_BUTTON_QUIT);
  set_enable_sensors_fake_data(DEFAULT_ENABLE_SENSORS_FAKE_DATA);
  set_fake_roll(DEFAULT_FAKE_ROLL);
  set_fake_pitch(DEFAULT_FAKE_PITCH);
  set_fake_acceleration_x(DEFAULT_FAKE_ACCELERATION_X);
  set_fake_acceleration_y(DEFAULT_FAKE_ACCELERATION_Y);
  set_simulation_start_on_application_start(DEFAULT_SIMULATION_START_ON_APPLICATION_START);
  set_enable_gravity_control(DEFAULT_ENABLE_GRAVITY_CONTROL);
  set_simulation_initial_gravity(DEFAULT_SIMULATION_INITIAL_GRAVITY);
  set_sweep_sensitivity(DEFAULT_SWEEP_SENSITIVITY);
  set_sweep_width(DEFAULT_SWEEP_WIDTH);
  set_sweep_fade_factor(DEFAULT_SWEEP_FADE_FACTOR);
  set_stop_below_activity(DEFAULT_STOP_BELOW_ACTIVITY);
  set_check_activity_only_after(DEFAULT_CHECK_ACTIVITY_ONLY_AFTER);
  set_change_image_when_paused(DEFAULT_CHANGE_IMAGE_WHEN_PAUSED);
  set_change_image_when_running(DEFAULT_CHANGE_IMAGE_WHEN_RUNNING);
  set_enable_audio(DEFAULT_ENABLE_AUDIO);
  set_enable_volume_control(DEFAULT_ENABLE_VOLUME_CONTROL);
  set_audio_sample_scale(DEFAULT_AUDIO_SAMPLE_SCALE);
  set_audio_sample_rate(DEFAULT_AUDIO_SAMPLE_RATE);
  set_audio_channels(DEFAULT_AUDIO_CHANNELS);
  set_alsa_verbose(DEFAULT_ALSA_VERBOSE);
  set_alsa_playback_device(DEFAULT_ALSA_PLAYBACK_DEVICE);
  set_alsa_buffer_time(DEFAULT_ALSA_BUFFER_TIME);
  set_alsa_period_time(DEFAULT_ALSA_PERIOD_TIME);
  set_audio_initial_volume(DEFAULT_AUDIO_INITIAL_VOLUME);
  set_capturing_path(DEFAULT_CAPTURING_PATH);
  _key_bindings = new Key_bindings();
  if (!_key_bindings) {
    Log::fatal("Config::Config(): not enough memory");
  }
  _images = new std::vector<const Config_image *>();
  if (!_images) {
    Log::fatal("Config::Config(): not enough memory");
  }
  Config_reader *config_reader = new Config_reader(this);
  if (!config_reader) {
    Log::fatal("Config::Config(): not enough memory");
  }
  config_reader->parse(path);
  delete config_reader;
}

Config::~Config()
{
  for (uint32_t image_index = 0; image_index < _images->size(); image_index++) {
    delete _images->at(image_index);
    _images->at(image_index) = 0;
  }
  delete _images;
  _start_fan_temperature = 0.0;
  _stop_fan_temperature = 0.0;
  _start_cooling_break_temperature = 0.0;
  _stop_cooling_break_temperature = 0.0;
  _frame_usleep_min = 0;
  _frame_usleep_max = 0;
  _full_screen = 0;
  _window_width = 0;
  _window_height = 0;
  _control_show_after_pressing = 0;
  _control_autohide_after = 0;
  _enable_cursor = false;
  _enable_button_quit = false;
  _enable_sensors_fake_data = false;
  _fake_roll = 0.0;
  _fake_pitch = 0.0;
  _fake_acceleration_x = 0.0;
  _fake_acceleration_y = 0.0;
  _simulation_start_on_application_start = false;
  _enable_gravity_control = false;
  _simulation_initial_gravity = 0;
  _sweep_width = 0;
  _sweep_fade_factor = 0.0;
  _stop_below_activity = 0.0;
  _check_activity_only_after = 0;
  _change_image_when_paused = 0;
  _change_image_when_running = 0;
  _enable_audio = false;
  _enable_volume_control = false;
  _audio_sample_scale = 1.0;
  _audio_sample_rate = 0;
  _audio_channels = 0;
  _alsa_verbose = false;
  _alsa_playback_device = std::string();
  _alsa_buffer_time = 0;
  _alsa_period_time = 0;
  _audio_initial_volume = 0.0;
  _capturing_path = std::string();
}

const int8_t
Config::GRAVITY_MIN_VALUE() const { return 1; }

const int8_t
Config::GRAVITY_MAX_VALUE() const { return 8; }

void
Config::set_start_fan_temperature(const double start_fan_temperature)
{
  _start_fan_temperature = start_fan_temperature;
}

const double
Config::get_start_fan_temperature() const
{
  return _start_fan_temperature;
}

void
Config::set_stop_fan_temperature(const double stop_fan_temperature)
{
  _stop_fan_temperature = stop_fan_temperature;
}

const double
Config::get_stop_fan_temperature() const
{
  return _stop_fan_temperature;
}

void
Config::set_start_cooling_break_temperature(const double start_cooling_break_temperature)
{
  _start_cooling_break_temperature = start_cooling_break_temperature;
}

const double
Config::get_start_cooling_break_temperature() const
{
  return _start_cooling_break_temperature;
}

void
Config::set_stop_cooling_break_temperature(const double stop_cooling_break_temperature)
{
  _stop_cooling_break_temperature = stop_cooling_break_temperature;
}

const double
Config::get_stop_cooling_break_temperature() const
{
  return _stop_cooling_break_temperature;
}

void
Config::set_frame_usleep_min(const uint32_t frame_usleep_min)
{
  _frame_usleep_min = frame_usleep_min;
}

const uint32_t
Config::get_frame_usleep_min() const
{
  return _frame_usleep_min;
}

void
Config::set_frame_usleep_max(const uint32_t frame_usleep_max)
{
  _frame_usleep_max = frame_usleep_max;
}

const uint32_t
Config::get_frame_usleep_max() const
{
  return _frame_usleep_max;
}

void
Config::set_full_screen(const bool full_screen)
{
  _full_screen = full_screen;
}

const bool
Config::get_full_screen() const
{
  return _full_screen;
}

void
Config::set_window_width(const uint16_t window_width)
{
  _window_width = window_width;
}

const uint16_t
Config::get_window_width() const
{
  return _window_width;
}

void
Config::set_window_height(const uint16_t window_height)
{
  _window_height = window_height;
}

const uint16_t
Config::get_window_height() const
{
  return _window_height;
}

void
Config::set_control_show_after_pressing(const uint16_t control_show_after_pressing)
{
  _control_show_after_pressing = control_show_after_pressing;
}

const uint16_t
Config::get_control_show_after_pressing() const
{
  return _control_show_after_pressing;
}

void
Config::set_control_autohide_after(const uint16_t control_autohide_after)
{
  _control_autohide_after = control_autohide_after;
}

const uint16_t
Config::get_control_autohide_after() const
{
  return _control_autohide_after;
}

void
Config::set_enable_cursor(const bool enable_cursor)
{
  _enable_cursor = enable_cursor;
}

const bool
Config::get_enable_cursor() const
{
  return _enable_cursor;
}

void
Config::set_enable_button_quit(const bool enable_button_quit)
{
  _enable_button_quit = enable_button_quit;
}

const bool
Config::get_enable_button_quit() const
{
  return _enable_button_quit;
}

void
Config::bind_key_to_action(const int key, const Key_bindings::Action action)
{
  _key_bindings->bind_key_to_action(key, action);
}

const Key_bindings::Action
Config::get_action_for_key(const int key) const
{
  return _key_bindings->get_action_for_key(key);
}

void
Config::set_enable_sensors_fake_data(const bool enable_sensors_fake_data)
{
  _enable_sensors_fake_data = enable_sensors_fake_data;
}

const bool
Config::get_enable_sensors_fake_data() const
{
  return _enable_sensors_fake_data;
}

void
Config::set_fake_roll(const double fake_roll)
{
  _fake_roll = fake_roll;
}

const double
Config::get_fake_roll() const
{
  return _fake_roll;
}

void
Config::set_fake_pitch(const double fake_pitch)
{
  _fake_pitch = fake_pitch;
}

const double
Config::get_fake_pitch() const
{
  return _fake_pitch;
}

void
Config::set_fake_acceleration_x(const double fake_acceleration_x)
{
  _fake_acceleration_x = fake_acceleration_x;
}

const double
Config::get_fake_acceleration_x() const
{
  return _fake_acceleration_x;
}

void
Config::set_fake_acceleration_y(const double fake_acceleration_y)
{
  _fake_acceleration_y = fake_acceleration_y;
}

const double
Config::get_fake_acceleration_y() const
{
  return _fake_acceleration_y;
}

void
Config::set_simulation_start_on_application_start(const bool simulation_start_on_application_start)
{
  _simulation_start_on_application_start =
    simulation_start_on_application_start;
}

const bool
Config::get_simulation_start_on_application_start() const
{
  return _simulation_start_on_application_start;
}

void
Config::set_enable_gravity_control(const bool enable_gravity_control)
{
  _enable_gravity_control = enable_gravity_control;
}

const bool
Config::get_enable_gravity_control() const
{
  return _enable_gravity_control;
}

void
Config::set_simulation_initial_gravity(const int8_t simulation_initial_gravity)
{
  if (simulation_initial_gravity < GRAVITY_MIN_VALUE()) {
    std::stringstream msg;
    msg << "Config::set_simulation_initial_gravity(): "
      "simulation_initial_gravity < " << (int)GRAVITY_MIN_VALUE();
    Log::fatal(msg.str());
  }
  if (simulation_initial_gravity > GRAVITY_MAX_VALUE()) {
    std::stringstream msg;
    msg << "Config::set_simulation_initial_gravity(): "
      "simulation_initial_gravity > " << (int)GRAVITY_MAX_VALUE();
    Log::fatal(msg.str());
  }
  _simulation_initial_gravity = simulation_initial_gravity;
}

const int8_t
Config::get_simulation_initial_gravity() const
{
  return _simulation_initial_gravity;
}

void
Config::set_sweep_sensitivity(const double sweep_sensitivity)
{
  _sweep_sensitivity = sweep_sensitivity;
}

const double
Config::get_sweep_sensitivity() const
{
  return _sweep_sensitivity;
}

void
Config::set_sweep_width(const int sweep_width)
{
  _sweep_width = sweep_width;
}

const int
Config::get_sweep_width() const
{
  return _sweep_width;
}

void
Config::set_sweep_fade_factor(const double sweep_fade_factor)
{
  _sweep_fade_factor = sweep_fade_factor;
}

const double
Config::get_sweep_fade_factor() const
{
  return _sweep_fade_factor;
}

void
Config::set_stop_below_activity(const double stop_below_activity)
{
  _stop_below_activity = stop_below_activity;
}

const double
Config::get_stop_below_activity() const
{
  return _stop_below_activity;
}

void
Config::set_check_activity_only_after(const int check_activity_only_after)
{
  _check_activity_only_after = check_activity_only_after;
}

const int
Config::get_check_activity_only_after() const
{
  return _check_activity_only_after;
}

void
Config::set_change_image_when_paused(const int change_image_when_paused)
{
  _change_image_when_paused = change_image_when_paused;
}

const int
Config::get_change_image_when_paused() const
{
  return _change_image_when_paused;
}

void
Config::set_change_image_when_running(const int change_image_when_running)
{
  _change_image_when_running = change_image_when_running;
}

const int
Config::get_change_image_when_running() const
{
  return _change_image_when_running;
}

void
Config::set_enable_audio(const bool enable_audio)
{
  _enable_audio = enable_audio;
}

const bool
Config::get_enable_audio() const
{
  return _enable_audio;
}

void
Config::set_enable_volume_control(const bool enable_volume_control)
{
  _enable_volume_control = enable_volume_control;
}

const bool
Config::get_enable_volume_control() const
{
  return _enable_volume_control;
}

void
Config::set_audio_sample_scale(const double audio_sample_scale)
{
  _audio_sample_scale = audio_sample_scale;
}

const double
Config::get_audio_sample_scale() const
{
  return _audio_sample_scale;
}

void
Config::set_audio_sample_rate(const int audio_sample_rate)
{
  _audio_sample_rate = audio_sample_rate;
}

const int
Config::get_audio_sample_rate() const
{
  return _audio_sample_rate;
}

void
Config::set_audio_channels(const int audio_channels)
{
  _audio_channels = audio_channels;
}

const int
Config::get_audio_channels() const
{
  return _audio_channels;
}

void
Config::set_alsa_verbose(const bool alsa_verbose)
{
  _alsa_verbose = alsa_verbose;
}

const bool
Config::get_alsa_verbose() const
{
  return _alsa_verbose;
}

void
Config::set_alsa_playback_device(const std::string alsa_playback_device)
{
  _alsa_playback_device.clear();
  _alsa_playback_device.append(alsa_playback_device);
}

const std::string
Config::get_alsa_playback_device() const
{
  return _alsa_playback_device;
}

void
Config::set_alsa_buffer_time(const int alsa_buffer_time)
{
  _alsa_buffer_time = alsa_buffer_time;
}

const int
Config::get_alsa_buffer_time() const
{
  return _alsa_buffer_time;
}

void
Config::set_alsa_period_time(const int alsa_period_time)
{
  _alsa_period_time = alsa_period_time;
}

const int
Config::get_alsa_period_time() const
{
  return _alsa_period_time;
}

void
Config::set_audio_initial_volume(const double audio_initial_volume)
{
  _audio_initial_volume = audio_initial_volume;
}

const double
Config::get_audio_initial_volume() const
{
  return _audio_initial_volume;
}

void
Config::set_capturing_path(const std::string capturing_path)
{
  _capturing_path.clear();
  _capturing_path.append(capturing_path);
}

const std::string
Config::get_capturing_path() const
{
  return _capturing_path;
}

void
Config::add_image(const Config_image *image)
{
  _images->push_back(image);
}

const uint32_t
Config::get_images_count() const
{
  return _images->size();
}

const Config_image *
Config::get_image(const uint32_t index) const
{
  return _images->at(index);
}

void
Config::to_string(std::stringstream *buffer) const
{
  (*buffer) << std::endl << "==== Power Save ====" << std::endl;
  (*buffer) << "start fan [°C]: " << _start_fan_temperature << std::endl;
  (*buffer) << "stop fan [°C]: "  << _stop_fan_temperature << std::endl;
  (*buffer) << "start cooling break [°C]: " <<
    _start_cooling_break_temperature << std::endl;
  (*buffer) << "stop cooling break [°C]: " <<
    _stop_cooling_break_temperature << std::endl;
  (*buffer) << "frame sleep min [µs]: " << _frame_usleep_min << std::endl;
  (*buffer) << "frame sleep max [µs]: " << _frame_usleep_max << std::endl;
  (*buffer) << std::endl << "==== Kiosk Mode ====" << std::endl;
  (*buffer) << "full screen [y/n]: " << _full_screen << std::endl;
  (*buffer) << "window width [px]: " << _window_width << std::endl;
  (*buffer) << "window height [px]: " << _window_height << std::endl;
  (*buffer) << "control show after pressing [s]: " <<
    _control_show_after_pressing << std::endl;
  (*buffer) << "control autohide after [s]: " <<
    _control_autohide_after << std::endl;
  (*buffer) << "enable cursor [y/n]: " << _enable_cursor << std::endl;
  (*buffer) << "enable button 'quit' [y/n]: " <<
    _enable_button_quit << std::endl;
  (*buffer) << std::endl << "==== Sensors ====" << std::endl;
  (*buffer) << "enable fake data [y/n]: " <<
    _enable_sensors_fake_data << std::endl;
  (*buffer) << "fake roll: " << _fake_roll << std::endl;
  (*buffer) << "fake pitch: " << _fake_pitch << std::endl;
  (*buffer) << "fake acceleration x: " << _fake_acceleration_x << std::endl;
  (*buffer) << "fake acceleration y: " << _fake_acceleration_y << std::endl;
  (*buffer) << std::endl << "==== Simulation ====" << std::endl;
  (*buffer) << "start on app start [y/n]: " <<
    _simulation_start_on_application_start << std::endl;
  (*buffer) << "enable gravity control [y/n]: " <<
    _enable_gravity_control << std::endl;
  (*buffer) << "simulation initial gravity [" <<
    (int)GRAVITY_MIN_VALUE() << "…" << (int)GRAVITY_MAX_VALUE() << "]: " <<
    ((int16_t)_simulation_initial_gravity) << std::endl;
  (*buffer) << "sweep sensitivity [double]: " <<
    _sweep_sensitivity << std::endl;
  (*buffer) << "sweep width [1..31]: " <<
    _sweep_width << std::endl;
  (*buffer) << "sweep fade factor [0.0…1.0]: " <<
    _sweep_fade_factor << std::endl;
  (*buffer) << "stop below activity [0.0…1.0]: " <<
    _stop_below_activity << std::endl;
  (*buffer) << "check activity only after [s]: " <<
    _check_activity_only_after << std::endl;
  (*buffer) << "change image when paused [s]: " <<
    _change_image_when_paused << std::endl;
  (*buffer) << "change image when running [s]: " <<
    _change_image_when_running << std::endl;
  (*buffer) << std::endl << "==== Audio ====" << std::endl;
  (*buffer) << "enable [y/n]: " << _enable_audio << std::endl;
  (*buffer) << "enable volume control [y/n]: " <<
    _enable_volume_control << std::endl;
  (*buffer) << "sample scale [1]: " << _audio_sample_scale << std::endl;
  (*buffer) << "sample rate [#/s]: " << _audio_sample_rate << std::endl;
  (*buffer) << "channels [#]: " << _audio_channels << std::endl;
  (*buffer) << "alsa verbose [y/n]: " << _alsa_verbose << std::endl;
  (*buffer) << "alsa playback device: " << _alsa_playback_device << std::endl;
  (*buffer) << "alsa buffer time [µs]: " << _alsa_buffer_time << std::endl;
  (*buffer) << "alsa period time [µs]: " << _alsa_period_time << std::endl;
  (*buffer) << "initial volume [0.0…1.0]: " <<
    _audio_initial_volume << std::endl;
  (*buffer) << std::endl << "==== Capturing ====" << std::endl;
  (*buffer) << "capturing path: " << _capturing_path << std::endl;
  (*buffer) << std::endl << "==== Key Bindings ====" << std::endl;
  (*buffer) << "key bindings: " << _key_bindings << std::endl;
  (*buffer) << std::endl << "==== Images ====" << std::endl;
  (*buffer) << "images: " << _images << std::endl;
  (*buffer) << std::endl;
}

/*
 * Local variables:
 *   mode: c++
 *   coding: utf-8
 * End:
 */

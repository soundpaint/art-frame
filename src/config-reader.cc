/*
 * art-frame -- an artful sands image emulation
 *
 * Copyright (C) 2015, 2016, 2019 Jürgen Reuter
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

#include <config-reader.hh>
#include <cerrno>
#include <unistd.h>
#include <log.hh>

#define CONFIG_SCHEMA_LOCATION "http://soundpaint.org/schema/soundcolumn/config.xsd"

Config_reader::Config_reader(Config *config)
  : Abstract_config_reader()
{
  if (!config) {
    Log::fatal("Config_reader::Config_reader(): config is NULL");
  }
  _config = config;
}

Config_reader::~Config_reader()
{
  // Q objects will be deleted by Qt, just set them to 0

  _config = 0;
}

void
Config_reader::parse_document(const xercesc::DOMElement *elem_config)
{
  const XMLCh *node_name = elem_config->getLocalName();

  char *node_name_as_c_star = xercesc::XMLString::transcode(node_name);
  if (strcmp(node_name_as_c_star, "config")) {
    std::stringstream message;
    message << "Config_reader::parse_document(): "
      "unexpected document element: " << node_name_as_c_star;
    Log::fatal(message.str());
  }

  xercesc::XMLString::release(&node_name_as_c_star);
  node_name_as_c_star = 0;

  const xercesc::DOMElement *elem_power_save =
    get_single_child_element(elem_config, "power-save");
  if (elem_power_save) {
    parse_power_save(elem_power_save);
  }

  const xercesc::DOMElement *elem_kiosk_mode =
    get_single_child_element(elem_config, "kiosk-mode");
  if (elem_kiosk_mode) {
    parse_kiosk_mode(elem_kiosk_mode);
  }

  const xercesc::DOMElement *elem_key_bindings =
    get_single_child_element(elem_config, "key-bindings");
  if (elem_key_bindings) {
    parse_key_bindings(elem_key_bindings);
  }

  const xercesc::DOMElement *elem_sensors =
    get_single_child_element(elem_config, "sensors");
  if (elem_sensors) {
    parse_sensors(elem_sensors);
  }

  const xercesc::DOMElement *elem_simulation =
    get_single_child_element(elem_config, "simulation");
  if (elem_simulation) {
    parse_simulation(elem_simulation);
  }

  const xercesc::DOMElement *elem_audio =
    get_single_child_element(elem_config, "audio");
  if (elem_audio) {
    parse_audio(elem_audio);
  }

  const xercesc::DOMElement *elem_capturing =
    get_single_child_element(elem_config, "capturing");
  if (elem_capturing) {
    parse_capturing(elem_capturing);
  }

  const xercesc::DOMElement *elem_images =
    get_single_child_element(elem_config, "images");
  if (elem_images) {
    parse_images(elem_images);
  } else {
    fatal("missing \"images\" element");
  }
}

void
Config_reader::parse_power_save(const xercesc::DOMElement *elem_power_save)
{
  // start_fan_temperature
  const xercesc::DOMElement *elem_start_fan_temperature =
    get_single_child_element(elem_power_save, "start-fan-temperature");
  if (elem_start_fan_temperature) {
    const XMLCh *start_fan_temperature =
      elem_start_fan_temperature->getTextContent();
    const double start_fan_temperature_value =
      parse_double(start_fan_temperature);
    if (start_fan_temperature_value < 0) {
      Log::fatal("Config_reader::parse_power_save(): "
                 "start fan temperature value < 0");
    }
    _config->set_start_fan_temperature(start_fan_temperature_value);
  }

  // stop_fan_temperature
  const xercesc::DOMElement *elem_stop_fan_temperature =
    get_single_child_element(elem_power_save, "stop-fan-temperature");
  if (elem_stop_fan_temperature) {
    const XMLCh *stop_fan_temperature =
      elem_stop_fan_temperature->getTextContent();
    const double stop_fan_temperature_value =
      parse_double(stop_fan_temperature);
    if (stop_fan_temperature_value < 0) {
      Log::fatal("Config_reader::parse_power_save(): "
                 "stop fan temperature value < 0");
    }
    if (_config->get_start_fan_temperature() <
        stop_fan_temperature_value) {
      Log::fatal("Config_reader::parse_power_save(): "
                 "fan temperature: start value < stop value");
    }
    _config->set_stop_fan_temperature(stop_fan_temperature_value);
  }

  // start_cooling_break_temperature
  const xercesc::DOMElement *elem_start_cooling_break_temperature =
    get_single_child_element(elem_power_save,
                             "start-cooling-break-temperature");
  if (elem_start_cooling_break_temperature) {
    const XMLCh *start_cooling_break_temperature =
      elem_start_cooling_break_temperature->getTextContent();
    const double start_cooling_break_temperature_value =
      parse_double(start_cooling_break_temperature);
    if (start_cooling_break_temperature_value < 0) {
      Log::fatal("Config_reader::parse_power_save(): "
                 "start cooling break temperature value < 0");
    }
    _config->set_start_cooling_break_temperature(start_cooling_break_temperature_value);
  }

  // stop_cooling_break_temperature
  const xercesc::DOMElement *elem_stop_cooling_break_temperature =
    get_single_child_element(elem_power_save,
                             "stop-cooling-break-temperature");
  if (elem_stop_cooling_break_temperature) {
    const XMLCh *stop_cooling_break_temperature =
      elem_stop_cooling_break_temperature->getTextContent();
    const double stop_cooling_break_temperature_value =
      parse_double(stop_cooling_break_temperature);
    if (stop_cooling_break_temperature_value < 0) {
      Log::fatal("Config_reader::parse_power_save(): "
                 "stop cooling break temperature value < 0");
    }
    if (_config->get_start_cooling_break_temperature() <
        stop_cooling_break_temperature_value) {
      Log::fatal("Config_reader::parse_power_save(): "
                 "cooling break temperature: start value < stop value");
    }
    _config->set_stop_cooling_break_temperature(stop_cooling_break_temperature_value);
  }

  // frame_usleep_min
  const xercesc::DOMElement *elem_frame_usleep_min =
    get_single_child_element(elem_power_save, "frame-usleep-min");
  if (elem_frame_usleep_min) {
    const XMLCh *frame_usleep_min = elem_frame_usleep_min->getTextContent();
    const uint32_t frame_usleep_min_value =
      parse_decimal_uint32(frame_usleep_min);
    if (frame_usleep_min_value < 0) {
      Log::fatal("Config_reader::parse_power_save(): "
                 "frame usleep min value < 0");
    }
    _config->set_frame_usleep_min(frame_usleep_min_value);
  }

  // frame_usleep_max
  const xercesc::DOMElement *elem_frame_usleep_max =
    get_single_child_element(elem_power_save, "frame-usleep-max");
  if (elem_frame_usleep_max) {
    const XMLCh *frame_usleep_max = elem_frame_usleep_max->getTextContent();
    const uint32_t frame_usleep_max_value =
      parse_decimal_uint32(frame_usleep_max);
    if (frame_usleep_max_value < 0) {
      Log::fatal("Config_reader::parse_power_save(): "
                 "frame usleep max value < 0");
    }
    if (_config->get_frame_usleep_min() > frame_usleep_max_value) {
      Log::fatal("Config_reader::parse_power_save(): "
                 "frame usleep: min value < max value");
    }
    _config->set_frame_usleep_max(frame_usleep_max_value);
  }
}

void
Config_reader::parse_kiosk_mode(const xercesc::DOMElement *elem_kiosk_mode)
{
  // full_screen
  const xercesc::DOMElement *elem_full_screen =
    get_single_child_element(elem_kiosk_mode, "full-screen");
  if (elem_full_screen) {
    const XMLCh *full_screen = elem_full_screen->getTextContent();
    const bool full_screen_value = parse_bool(full_screen);
    _config->set_full_screen(full_screen_value);
  }

  // window_width
  const xercesc::DOMElement *elem_window_width =
    get_single_child_element(elem_kiosk_mode, "window-width");
  if (elem_window_width) {
    const XMLCh *window_width =
      elem_window_width->getTextContent();
    const uint16_t window_width_value =
      parse_decimal_uint16(window_width);
    if (window_width_value <= 0) {
      Log::fatal("Config_reader::parse_kiosk_mode(): "
                 "window_width <= 0");
    }
    _config->set_window_width(window_width_value);
  }

  // window_height
  const xercesc::DOMElement *elem_window_height =
    get_single_child_element(elem_kiosk_mode, "window-height");
  if (elem_window_height) {
    const XMLCh *window_height =
      elem_window_height->getTextContent();
    const uint16_t window_height_value =
      parse_decimal_uint16(window_height);
    if (window_height_value <= 0) {
      Log::fatal("Config_reader::parse_kiosk_mode(): "
                 "window_height <= 0");
    }
    _config->set_window_height(window_height_value);
  }

  // control_show_after_pressing
  const xercesc::DOMElement *elem_control_show_after_pressing =
    get_single_child_element(elem_kiosk_mode, "control-show-after-pressing");
  if (elem_control_show_after_pressing) {
    const XMLCh *control_show_after_pressing =
      elem_control_show_after_pressing->getTextContent();
    const uint16_t control_show_after_pressing_value =
      parse_decimal_uint16(control_show_after_pressing);
    if (control_show_after_pressing_value < 0) {
      Log::fatal("Config_reader::parse_kiosk_mode(): "
                 "control show after pressing < 0");
    }
    _config->set_control_show_after_pressing(control_show_after_pressing_value);
  }

  // control_autohide_after
  const xercesc::DOMElement *elem_control_autohide_after =
    get_single_child_element(elem_kiosk_mode, "control-autohide-after");
  if (elem_control_autohide_after) {
    const XMLCh *control_autohide_after =
      elem_control_autohide_after->getTextContent();
    const uint16_t control_autohide_after_value =
      parse_decimal_uint16(control_autohide_after);
    if (control_autohide_after_value < 0) {
      Log::fatal("Config_reader::parse_kiosk_mode(): "
                 "control autohide after < 0");
    }
    _config->set_control_autohide_after(control_autohide_after_value);
  }

  // enable_cursor
  const xercesc::DOMElement *elem_enable_cursor =
    get_single_child_element(elem_kiosk_mode, "enable-cursor");
  if (elem_enable_cursor) {
    const XMLCh *enable_cursor = elem_enable_cursor->getTextContent();
    const bool enable_cursor_value = parse_bool(enable_cursor);
    _config->set_enable_cursor(enable_cursor_value);
  }

  // enable_button_quit
  const xercesc::DOMElement *elem_enable_button_quit =
    get_single_child_element(elem_kiosk_mode, "enable-button-quit");
  if (elem_enable_button_quit) {
    const XMLCh *enable_button_quit = elem_enable_button_quit->getTextContent();
    const bool enable_button_quit_value = parse_bool(enable_button_quit);
    _config->set_enable_button_quit(enable_button_quit_value);
  }

  // enable_key_quit
  const xercesc::DOMElement *elem_enable_key_quit =
    get_single_child_element(elem_kiosk_mode, "enable-key-quit");
  if (elem_enable_key_quit) {
    const XMLCh *enable_key_quit = elem_enable_key_quit->getTextContent();
    const bool enable_key_quit_value = parse_bool(enable_key_quit);
    _config->set_enable_key_quit(enable_key_quit_value);
  }
}

void
Config_reader::parse_key_bindings(const xercesc::DOMElement *elem_key_bindings)
{
  XMLCh *node_name_action = xercesc::XMLString::transcode("action");
  const xercesc::DOMNodeList *node_list =
    get_children_by_tag_name(elem_key_bindings, node_name_action);
  if (node_list) {
    const XMLSize_t length = node_list->getLength();
    for (uint32_t node_index = 0; node_index < length; node_index++) {
      const xercesc::DOMNode *node = node_list->item(node_index);
      const xercesc::DOMElement *action =
        dynamic_cast<const xercesc::DOMElement *>(node);
      if (!action) {
        fatal("Config_reader::parse_key_bindings(): action is NULL");
      }
      parse_action(action);
    }
  } else {
    // no actions => nothing to parse
  }
  xercesc::XMLString::release(&node_name_action);
}

void
Config_reader::parse_action(const xercesc::DOMElement *elem_action)
{
  // id
  const xercesc::DOMElement *elem_id =
    get_single_child_element(elem_action, "id");
  if (!elem_id) {
    fatal("Config_reader::parse_action(): id is NULL");
  }
  const XMLCh *id = elem_id->getTextContent();
  char *id_as_c_star = xercesc::XMLString::transcode(id);
  const IKey_bindings::Action action =
    IKey_bindings::action_from_string(id_as_c_star);
  if (action == IKey_bindings::None) {
    std::stringstream msg;
    msg << "Config_reader::parse_action(): unknown action id: " <<
      id_as_c_star;
    fatal(msg.str());
  }
  xercesc::XMLString::release(&id_as_c_star);

  // keys
  const xercesc::DOMElement *elem_keys =
    get_single_child_element(elem_action, "keys");
  if (!elem_keys) {
    fatal("Config_reader::parse_action(): keys is NULL");
  }
  parse_keys(elem_keys, action);

}

void
Config_reader::parse_keys(const xercesc::DOMElement *elem_keys,
                          const IKey_bindings::Action action)
{
  XMLCh *node_name_key = xercesc::XMLString::transcode("key");
  const xercesc::DOMNodeList *node_list =
    get_children_by_tag_name(elem_keys, node_name_key);
  if (node_list) {
    const XMLSize_t length = node_list->getLength();
    for (uint32_t node_index = 0; node_index < length; node_index++) {
      const xercesc::DOMNode *node = node_list->item(node_index);
      const xercesc::DOMElement *key =
        dynamic_cast<const xercesc::DOMElement *>(node);
      if (!key) {
        fatal("Config_reader::parse_keys(): key is NULL");
      }
      parse_key(key, action);
    }
  } else {
    // no key => nothing to parse
  }
  xercesc::XMLString::release(&node_name_key);
}

void
Config_reader::parse_key(const xercesc::DOMElement *elem_key,
                         const IKey_bindings::Action action)
{
  const XMLCh *key = elem_key->getTextContent();
  const uint32_t key_code = parse_hex_or_dec_or_oct_int32(key);
  _config->bind_key_to_action(key_code, action);
}

void
Config_reader::parse_sensors(const xercesc::DOMElement *elem_sensors)
{
  // enable_sensors_fake_data
  const xercesc::DOMElement *elem_enable_sensors_fake_data =
    get_single_child_element(elem_sensors, "enable-sensors-fake-data");
  if (elem_enable_sensors_fake_data) {
    const XMLCh *enable_sensors_fake_data =
      elem_enable_sensors_fake_data->getTextContent();
    const bool enable_sensors_fake_data_value =
      parse_bool(enable_sensors_fake_data);
    _config->set_enable_sensors_fake_data(enable_sensors_fake_data_value);
  }

  // fake_roll
  const xercesc::DOMElement *elem_fake_roll =
    get_single_child_element(elem_sensors, "fake-roll");
  if (elem_fake_roll) {
    const XMLCh *fake_roll = elem_fake_roll->getTextContent();
    const double fake_roll_value = parse_double(fake_roll);
    _config->set_fake_roll(fake_roll_value);
  }

  // fake_pitch
  const xercesc::DOMElement *elem_fake_pitch =
    get_single_child_element(elem_sensors, "fake-pitch");
  if (elem_fake_pitch) {
    const XMLCh *fake_pitch = elem_fake_pitch->getTextContent();
    const double fake_pitch_value = parse_double(fake_pitch);
    _config->set_fake_pitch(fake_pitch_value);
  }

  // fake_acceleration_x
  const xercesc::DOMElement *elem_fake_acceleration_x =
    get_single_child_element(elem_sensors, "fake-acceleration-x");
  if (elem_fake_acceleration_x) {
    const XMLCh *fake_acceleration_x =
      elem_fake_acceleration_x->getTextContent();
    const double fake_acceleration_x_value = parse_double(fake_acceleration_x);
    _config->set_fake_acceleration_x(fake_acceleration_x_value);
  }

  // fake_acceleration_y
  const xercesc::DOMElement *elem_fake_acceleration_y =
    get_single_child_element(elem_sensors, "fake-acceleration-y");
  if (elem_fake_acceleration_y) {
    const XMLCh *fake_acceleration_y =
      elem_fake_acceleration_y->getTextContent();
    const double fake_acceleration_y_value = parse_double(fake_acceleration_y);
    _config->set_fake_acceleration_y(fake_acceleration_y_value);
  }
}

void
Config_reader::parse_simulation(const xercesc::DOMElement *elem_simulation)
{
  // simulation_start_on_application_start
  const xercesc::DOMElement *elem_start_on_application_start =
    get_single_child_element(elem_simulation,
                             "start-on-application-start");
  if (elem_start_on_application_start) {
    const XMLCh *start_on_application_start =
      elem_start_on_application_start->getTextContent();
    const bool start_on_application_start_value =
      parse_bool(start_on_application_start);
    _config->set_simulation_start_on_application_start(start_on_application_start_value);
  }

  // initial_gravity
  const xercesc::DOMElement *elem_initial_gravity =
    get_single_child_element(elem_simulation, "initial-gravity");
  if (elem_initial_gravity) {
    const XMLCh *initial_gravity = elem_initial_gravity->getTextContent();
    const int8_t initial_gravity_value = parse_decimal_int8(initial_gravity);
    if (initial_gravity_value < -32) {
      Log::fatal("Config_reader::parse_simulation(): initial_gravity < -32");
    }
    if (initial_gravity_value > 31) {
      Log::fatal("Config_reader::parse_simulation(): initial_gravity > 31");
    }
    _config->set_simulation_initial_gravity(initial_gravity_value);
  }

  // sweep_sensitivity
  const xercesc::DOMElement *elem_sweep_sensitivity =
    get_single_child_element(elem_simulation, "sweep-sensitivity");
  if (elem_sweep_sensitivity) {
    const XMLCh *sweep_sensitivity = elem_sweep_sensitivity->getTextContent();
    const double sweep_sensitivity_value = parse_double(sweep_sensitivity);
    if (sweep_sensitivity_value < 0.0) {
      Log::fatal("Config_reader::parse_simulation(): sweep sensitivity < 0.0");
    }
    _config->set_sweep_sensitivity(sweep_sensitivity_value);
  }

  // sweep_width
  const xercesc::DOMElement *elem_sweep_width =
    get_single_child_element(elem_simulation, "sweep-width");
  if (elem_sweep_width) {
    const XMLCh *sweep_width = elem_sweep_width->getTextContent();
    const int sweep_width_value = parse_decimal_uint8(sweep_width);
    if (sweep_width_value < 1) {
      Log::fatal("Config_reader::parse_simulation(): sweep width < 1");
    }
    if (sweep_width_value > 31) {
      Log::fatal("Config_reader::parse_simulation(): sweep width > 31");
    }
    _config->set_sweep_width(sweep_width_value);
  }

  // sweep_fade_factor
  const xercesc::DOMElement *elem_sweep_fade_factor =
    get_single_child_element(elem_simulation, "sweep-fade-factor");
  if (elem_sweep_fade_factor) {
    const XMLCh *sweep_fade_factor =
      elem_sweep_fade_factor->getTextContent();
    const double sweep_fade_factor_value = parse_double(sweep_fade_factor);
    if (sweep_fade_factor_value < 0.0) {
      Log::fatal("Config_reader::parse_simulation(): "
                 "sweep fade factor < 0.0");
    }
    if (sweep_fade_factor_value > 1.0) {
      Log::fatal("Config_reader::parse_simulation(): "
                 "sweep fade factor > 1.0");
    }
    _config->set_sweep_fade_factor(sweep_fade_factor_value);
  }

  // stop_below_activity
  const xercesc::DOMElement *elem_stop_below_activity =
    get_single_child_element(elem_simulation, "stop-below-activity");
  if (elem_stop_below_activity) {
    const XMLCh *stop_below_activity =
      elem_stop_below_activity->getTextContent();
    const double stop_below_activity_value = parse_double(stop_below_activity);
    if (stop_below_activity_value < 0.0) {
      Log::fatal("Config_reader::parse_simulation(): "
                 "stop below activity < 0.0");
    }
    if (stop_below_activity_value > 1.0) {
      Log::fatal("Config_reader::parse_simulation(): "
                 "stop below activity > 1.0");
    }
    _config->set_stop_below_activity(stop_below_activity_value);
  }

  // check_activity_only_after
  const xercesc::DOMElement *elem_check_activity_only_after =
    get_single_child_element(elem_simulation, "check_activity_only_after");
  if (elem_check_activity_only_after) {
    const XMLCh *check_activity_only_after =
      elem_check_activity_only_after->getTextContent();
    const uint16_t check_activity_only_after_value =
      parse_decimal_uint32(check_activity_only_after);
    if (check_activity_only_after_value < 0) {
      Log::fatal("Config_reader::parse_simulation(): "
                 "check activity only after value < 0");
    }
    _config->set_check_activity_only_after(check_activity_only_after_value);
  }

  // change_image_when_paused
  const xercesc::DOMElement *elem_change_image_when_paused =
    get_single_child_element(elem_simulation, "change-image-when-paused");
  if (elem_change_image_when_paused) {
    const XMLCh *change_image_when_paused =
      elem_change_image_when_paused->getTextContent();
    const uint16_t change_image_when_paused_value =
      parse_decimal_uint32(change_image_when_paused);
    if (change_image_when_paused_value < 0) {
      Log::fatal("Config_reader::parse_simulation(): "
                 "change image when paused < 0");
    }
    _config->set_change_image_when_paused(change_image_when_paused_value);
  }

  // change_image_when_running
  const xercesc::DOMElement *elem_change_image_when_running =
    get_single_child_element(elem_simulation, "change-image-when-running");
  if (elem_change_image_when_running) {
    const XMLCh *change_image_when_running =
      elem_change_image_when_running->getTextContent();
    const uint16_t change_image_when_running_value =
      parse_decimal_uint32(change_image_when_running);
    if (change_image_when_running_value < 0) {
      Log::fatal("Config_reader::parse_simulation(): "
                 "change image when running < 0");
    }
    _config->set_change_image_when_running(change_image_when_running_value);
  }
}

void
Config_reader::parse_audio(const xercesc::DOMElement *elem_audio)
{
  // enable_audio
  const xercesc::DOMElement *elem_enable_audio =
    get_single_child_element(elem_audio, "enable-audio");
  if (elem_enable_audio) {
    const XMLCh *enable_audio = elem_enable_audio->getTextContent();
    const bool enable_audio_value = parse_bool(enable_audio);
    _config->set_enable_audio(enable_audio_value);
  }

  // audio_sample_scale
  const xercesc::DOMElement *elem_sample_scale =
    get_single_child_element(elem_audio, "sample-scale");
  if (elem_sample_scale) {
    const XMLCh *sample_scale = elem_sample_scale->getTextContent();
    const double sample_scale_value = parse_double(sample_scale);
    _config->set_audio_sample_scale(sample_scale_value);
  }

  // audio_sample_rate
  const xercesc::DOMElement *elem_audio_sample_rate =
    get_single_child_element(elem_audio, "audio-sample-rate");
  if (elem_audio_sample_rate) {
    const XMLCh *audio_sample_rate = elem_audio_sample_rate->getTextContent();
    const uint32_t audio_sample_rate_value =
      parse_decimal_uint32(audio_sample_rate);
    if (audio_sample_rate_value < 0) {
      Log::fatal("Config_reader::parse_audio(): "
                 "audio sample rate value < 0");
    }
    _config->set_audio_sample_rate(audio_sample_rate_value);
  }

  // audio_channels
  const xercesc::DOMElement *elem_audio_channels =
    get_single_child_element(elem_audio, "audio-channels");
  if (elem_audio_channels) {
    const XMLCh *audio_channels = elem_audio_channels->getTextContent();
    const uint32_t audio_channels_value =
      parse_decimal_uint32(audio_channels);
    if (audio_channels_value < 0) {
      Log::fatal("Config_reader::parse_audio(): "
                 "audio channels value < 0");
    }
    _config->set_audio_channels(audio_channels_value);
  }

  // bool alsa_verbose
  const xercesc::DOMElement *elem_alsa_verbose =
    get_single_child_element(elem_audio, "alsa-verbose");
  if (elem_alsa_verbose) {
    const XMLCh *alsa_verbose = elem_alsa_verbose->getTextContent();
    const bool alsa_verbose_value = parse_bool(alsa_verbose);
    _config->set_alsa_verbose(alsa_verbose_value);
  }

  // alsa_playback_device
  const xercesc::DOMElement *elem_alsa_playback_device =
    get_single_child_element(elem_audio, "alsa-playback-device");
  if (elem_alsa_playback_device) {
    const XMLCh *alsa_playback_device =
      elem_alsa_playback_device->getTextContent();
    const char *alsa_playback_device_value =
      xercesc::XMLString::transcode(alsa_playback_device);
    // TODO: Put a strdup()'d copy of this (char *) into config and
    // release it here with xercesc::XMLString::release(), in case it
    // turns out that releasing it at application exit with ordinary
    // free() causes problems.
    _config->set_alsa_playback_device(alsa_playback_device_value);
  }

  // alsa_buffer_time
  const xercesc::DOMElement *elem_alsa_buffer_time =
    get_single_child_element(elem_audio, "alsa-buffer-time");
  if (elem_alsa_buffer_time) {
    const XMLCh *alsa_buffer_time = elem_alsa_buffer_time->getTextContent();
    const uint32_t alsa_buffer_time_value =
      parse_decimal_uint32(alsa_buffer_time);
    if (alsa_buffer_time_value < 0) {
      Log::fatal("Config_reader::parse_audio(): "
                 "alsa buffer time value < 0");
    }
    _config->set_alsa_buffer_time(alsa_buffer_time_value);
  }

  // alsa_period_time
  const xercesc::DOMElement *elem_alsa_period_time =
    get_single_child_element(elem_audio, "alsa-period-time");
  if (elem_alsa_period_time) {
    const XMLCh *alsa_period_time = elem_alsa_period_time->getTextContent();
    const uint32_t alsa_period_time_value =
      parse_decimal_uint32(alsa_period_time);
    if (alsa_period_time_value < 0) {
      Log::fatal("Config_reader::parse_audio(): "
                 "alsa period time value < 0");
    }
    _config->set_alsa_period_time(alsa_period_time_value);
  }

  // initial_volume
  const xercesc::DOMElement *elem_initial_volume =
    get_single_child_element(elem_audio, "initial-volume");
  if (elem_initial_volume) {
    const XMLCh *initial_volume = elem_initial_volume->getTextContent();
    const double initial_volume_value = parse_double(initial_volume);
    if (initial_volume_value < 0.0) {
      Log::fatal("Config_reader::parse_audio(): initial volume < 0.0");
    }
    if (initial_volume_value > 1.0) {
      Log::fatal("Config_reader::parse_audio(): initial volume > 1.0");
    }
    _config->set_audio_initial_volume(initial_volume_value);
  }
}

void
Config_reader::parse_capturing(const xercesc::DOMElement *elem_capturing)
{
  // path
  const xercesc::DOMElement *elem_path =
    get_single_child_element(elem_capturing, "path");
  if (elem_path) {
    const XMLCh *path = parse_path(elem_path);
    char *str_path = xercesc::XMLString::transcode(path);
    _config->set_capturing_path(str_path);
    xercesc::XMLString::release(&str_path);
    str_path = 0;
  }
}

void
Config_reader::parse_images(const xercesc::DOMElement *elem_config)
{
  XMLCh *node_name_image = xercesc::XMLString::transcode("image");
  const xercesc::DOMNodeList *node_list =
    get_children_by_tag_name(elem_config, node_name_image);
  if (node_list) {
    const XMLSize_t length = node_list->getLength();
    for (uint32_t node_index = 0; node_index < length; node_index++) {
      const xercesc::DOMNode *node = node_list->item(node_index);
      const xercesc::DOMElement *image =
        dynamic_cast<const xercesc::DOMElement *>(node);
      if (!image) {
        fatal("Config_reader::parse_images(): image is NULL");
      }
      const Config_image *config_image = parse_image(image);
      if (config_image) {
        _config->add_image(config_image);
      } else {
        Log::warn("Config_reader::parse_images(): skipping image");
      }
    }
  } else {
    // no images => nothing to parse
  }
  if (!_config->get_images_count()) {
    Log::fatal("Config_reader::parse_images(): no image loaded");
  }
  xercesc::XMLString::release(&node_name_image);
}

const Config_image *
Config_reader::parse_image(const xercesc::DOMElement *elem_image)
{
  const Config_image *config_image;
  const xercesc::DOMElement *elem_from_file =
    get_single_child_element(elem_image, "from-file");
  if (elem_from_file) {
    config_image = parse_from_file(elem_from_file);
  } else {
    fatal("for now, image definition must contain from-file definition");
  }
  return config_image;
}

const Config_image *
Config_reader::parse_from_file(const xercesc::DOMElement *elem_from_file)
{
  const XMLCh *path;
  const xercesc::DOMElement *elem_path =
    get_single_child_element(elem_from_file, "path");
  if (elem_path) {
    path = parse_path(elem_path);
  } else {
    fatal("for now, from-file definition must contain path definition");
  }
  char *str_path = xercesc::XMLString::transcode(path);
  const Config_image *config_image = Config_image::create(str_path);
  xercesc::XMLString::release(&str_path);
  str_path = 0;
  if (!config_image) {
    Log::warn("Config_reader::parse_document(): failed accessing image");
  }
  return config_image;
}

const XMLCh *
Config_reader::parse_path(const xercesc::DOMElement *elem_path)
{
  const XMLCh *node_value_path = elem_path->getTextContent();
  return node_value_path;
}

void
Config_reader::print_config() const
{
  std::stringstream msg;
  msg << std::endl;
  msg << "######## CONFIG BEGIN ########" << std::endl;
  _config->to_string(&msg);
  msg << "######### CONFIG END ###########" << std::endl;
  Log::info(msg.str());
}

/*
 * Local variables:
 *   mode: c++
 *   coding: utf-8
 * End:
 */

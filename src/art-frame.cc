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

#include <art-frame.hh>
#include <cmath>
#include <fstream>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDesktopWidget>
#include <config.hh>
#include <alsa-player.hh>
#include <unistd.h>
#include <log.hh>

const char *
Art_frame::STYLE_SHEET_FILE_PATH = "style.qss";

Art_frame::Art_frame(int &argc, char **argv)
  : QApplication(argc, argv, 0)
{
  srand(1);
  _style_sheet = read_style_sheet(STYLE_SHEET_FILE_PATH);
  setStyleSheet(_style_sheet);
  const QRect screen_geometry = QApplication::desktop()->screenGeometry();
  const uint16_t width = 19 * screen_geometry.width() / 20;
  const uint16_t height = 19 * screen_geometry.height() / 20;
  std::cout << "[screen_width=" << width <<
    ", screen_height=" << height << "]\n";

  _fan_running = false;

  _config = new Config("config.xml");
  if (!_config) {
    Log::fatal("Art_frame::Art_frame(): not enough memory");
  }

  _sensors = new Sensors(this, _config);
  if (!_sensors) {
    Log::fatal("Art_frame::Art_frame(): not enough memory");
  }

  _thermal_sensors = new Thermal_sensors(this);
  if (!_thermal_sensors) {
    Log::fatal("Art_frame::Art_frame(): not enough memory");
  }

  _simulation =
    new Simulation(width, height, _config, _sensors, _thermal_sensors);
  if (!_simulation) {
    Log::fatal("Art_frame::Art_frame(): not enough memory");
  }
  _simulation->set_gravity(_config->get_simulation_initial_gravity());
  _simulation->start();
  Particles *particles = _simulation->get_particles();

  if (_config->get_enable_audio()) {
    _audio_player = new Alsa_player(_config);
    if (!_audio_player) {
      Log::fatal("Art_frame::Art_frame(): not enough memory");
    }
    _audio_player->set_volume(_config->get_audio_initial_volume());
    _audio_player->connect_to(particles);
  } else {
    _audio_player = 0;
  }

  _main_window = new Main_window(width, height, this, _config, _sensors,
                                 _simulation, _audio_player);
  if (!_main_window) {
    Log::fatal("Art_frame::Art_frame(): not enough memory");
  }
  if (_config->get_full_screen()) {
  _main_window->showFullScreen();
  } else {
  _main_window->show();
  }
  if (!_main_window->isVisible()) {
    Log::fatal("Art_frame::Art_frame(): "
               "main window must be visible to determine particles amount of size");
  }

  connect(this, SIGNAL(signal_start_fan()),
          this, SLOT(slot_start_fan()));
  connect(this, SIGNAL(signal_stop_fan()),
          this, SLOT(slot_stop_fan()));
  connect(this, SIGNAL(signal_start_cooling_break()),
          this, SLOT(slot_start_cooling_break()));
  connect(this, SIGNAL(signal_stop_cooling_break()),
          this, SLOT(slot_stop_cooling_break()));
}

Art_frame::~Art_frame()
{
  if (_simulation->is_running()) {
    _audio_player->pause();
  }

  // Q objects will be deleted by Qt, just set them to 0
  _main_window = 0;
  _simulation = 0;

  _fan_running = false;

  delete _config;
  _config = 0;

  delete _audio_player;
  _audio_player = 0;

  delete _sensors;
  _sensors = 0;

  delete[] _style_sheet;
  _style_sheet = 0;
}

const char *
Art_frame::read_style_sheet(const char *file_path)
{
  std::ifstream in_file(file_path);
  if (!in_file.good()) {
    Log::warn("Art_frame::read_style_sheet(): no style sheet \"style.qss\"");
    return NULL;
  }
  in_file.seekg(0, std::ios::end);
  const int file_length = in_file.tellg();
  in_file.seekg(0, std::ios::beg);
  char *file_data = new char[file_length + 1];
  if (!file_data) {
    Log::fatal("Art_frame::read_style_sheet(): not enough memory");
  }
  in_file.read(file_data, file_length);
  file_data[file_length] = 0; // null-terminated string
  in_file.close();
  return file_data;
}

void
Art_frame::slot_update_sensors_display(const double pitch,
                                       const double roll,
                                       const double ax,
                                       const double ay,
                                       const double temperature)
{
  _main_window->slot_update_sensors_display(pitch, roll, ax, ay, temperature);
}

void
Art_frame::slot_update_thermal_display(const double vc_temperature)
{
  _main_window->slot_update_thermal_display(vc_temperature);
  if (vc_temperature >= _config->get_start_cooling_break_temperature()) {
    emit signal_start_cooling_break();
  } else if (vc_temperature <= _config->get_stop_cooling_break_temperature()) {
    emit signal_stop_cooling_break();
  }
  if (vc_temperature >= _config->get_start_fan_temperature()) {
    if (!_fan_running) {
      emit signal_start_fan();
    }
  } else if (vc_temperature <= _config->get_stop_fan_temperature()) {
    if (_fan_running) {
      emit signal_stop_fan();
    }
  }
}

void
Art_frame::slot_start_fan()
{
  Log::info("turning on fan");
  const int result = system("../bin/fan-on.sh");
  if (result) {
    std::stringstream msg;
    msg << "Art_frame::slot_start_fan(): result=" << result;
    Log::warn(msg.str());
  }
  _fan_running = true;
  Log::info("fan now turned on");
}

void
Art_frame::slot_stop_fan()
{
  Log::info("turning off fan");
  const int result = system("../bin/fan-off.sh");
  if (result) {
    std::stringstream msg;
    msg << "Art_frame::slot_stop_fan(): result=" << result;
    Log::warn(msg.str());
  }
  _fan_running = false;
  Log::info("fan now turned off");
}

void
Art_frame::slot_start_cooling_break()
{
  _main_window->start_cooling_break();
}

void
Art_frame::slot_stop_cooling_break()
{
  _main_window->stop_cooling_break();
}

void
Art_frame::confirm_quit()
{
  QMessageBox::StandardButton reply;
  reply = QMessageBox::question(_main_window, tr("Confirm Shutdown"),
                                tr("Are you sure to shut down the system?"),
                                QMessageBox::Yes | QMessageBox::No);
  if (reply == QMessageBox::Yes) {
    if (_audio_player->is_running()) {
      _audio_player->pause();
    }
    if (_simulation->is_running()) {
      _simulation->stop();
      _simulation->wait();
    }
    QApplication::quit();
  } else {
    // continue
  }
}

int main(int argc, char *argv[])
{
  Art_frame *art_frame = new Art_frame(argc, argv);
  const int result = art_frame->exec();

  delete art_frame;
  art_frame = 0;

  exit(result);
}

/*
 * Local variables:
 *   mode: c++
 *   coding: utf-8
 * End:
 */

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

#include <status-line.hh>
#include <log.hh>
#include <QtWidgets/QApplication>
#include <main-window.hh>

QMessageBox *
Status_line::create_cool_message()
{
  QMessageBox *cool_message = new QMessageBox();
  if (!cool_message) {
    Log::fatal("Status_line::create_cool_message(): not enough memory");
  }
  QPalette palette;
  palette.setColor(QPalette::Window, Qt::yellow);
  cool_message->setPalette(palette);
  cool_message->setStandardButtons(0);
  cool_message->setWindowFlags(cool_message->windowFlags() |
                               Qt::FramelessWindowHint |
                               Qt::WindowStaysOnTopHint);
  QFont font = QFont();
  font.setPixelSize(32);
  font.setBold(true);
  cool_message->setFont(font);
  cool_message->setText(tr("CPU temperature too high.\n"
                           "Pausing simulation for a short while\n"
                           "for cooling down the CPU."));
  return cool_message;
}

void
Status_line::create_dialogs()
{
  _about_dialog = new About_dialog(this);
  if (!_about_dialog) {
    Log::fatal("Status_line::create_dialogs(): not enough memory");
  }
  _about_dialog->setModal(true);

  _license_dialog = new License_dialog(this);
  if (!_license_dialog) {
    Log::fatal("Status_line::create_dialogs(): not enough memory");
  }
  _license_dialog->setModal(true);

  _cool_message = create_cool_message();
}

void
Status_line::create_info_row()
{
  _info_row = new QWidget();
  if (!_info_row) {
    Log::fatal("Status_line::create_info_row(): not enough memory");
  }
  _layout->addWidget(_info_row);
  _info_row_layout = new QHBoxLayout();
  if (!_info_row_layout) {
    Log::fatal("Status_line::create_info_row(): not enough memory");
  }
  _info_row->setLayout(_info_row_layout);

  _sensors_display = new Sensors_display(this);
  if (!_sensors_display) {
    Log::fatal("Status_line::create_info_row(): not enough memory");
  }
  _info_row_layout->addWidget(_sensors_display);

  _thermal_display = new Thermal_display(this);
  if (!_thermal_display) {
    Log::fatal("Status_line::create_info_row(): not enough memory");
  }
  _info_row_layout->addWidget(_thermal_display);
  _info_row_layout->addStretch();

  _label_app_title =
    new QLabel(tr("<h1>Paint it Yourself!</h1>"
                  "<h2>Interactive Art Frame V0.2</h2>"));
  if (!_label_app_title) {
    Log::fatal("Status_line::create_info_row(): not enough memory");
  }
  _label_app_title->setTextFormat(Qt::RichText);
  _info_row_layout->addWidget(_label_app_title);

  _sidebar_column = new QWidget();
  if (!_sidebar_column) {
    Log::fatal("Status_line::create_info_row(): not enough memory");
  }
  _info_row_layout->addWidget(_sidebar_column);
  _sidebar_column_layout = new QVBoxLayout();
  if (!_sidebar_column_layout) {
    Log::fatal("Status_line::create_info_row(): not enough memory");
  }
  _sidebar_column->setLayout(_sidebar_column_layout);
  _button_close =
    new Titled_button(this, "close control panel", "Close", "close.png");
  _sidebar_column_layout->addWidget(_button_close);
  _sidebar_column_layout->addStretch();
}

void
Status_line::create_button_row()
{
  _button_row = new QWidget();
  if (!_button_row) {
    Log::fatal("Status_line::create_button_row(): not enough memory");
  }

  _button_row_layout = new QHBoxLayout();
  if (!_button_row_layout) {
    Log::fatal("Status_line::create_button_row(): not enough memory");
  }
  _button_row->setLayout(_button_row_layout);

  Global_control *global_control = new Global_control(this, _config);
  if (!global_control) {
    Log::fatal("Status_line::create_button_row(): not enough memory");
  }
  _button_quit = global_control->get_button_quit();
  _button_about = global_control->get_button_about();
  _button_license = global_control->get_button_license();
  _button_row_layout->addWidget(global_control);

  Simulation_control *simulation_control =
    new Simulation_control(this, _config);
  if (!simulation_control) {
    Log::fatal("Status_line::create_button_row(): not enough memory");
  }
  _button_pause_resume = simulation_control->get_button_pause_resume();
  _image_pause = simulation_control->get_image_pause();
  _image_resume = simulation_control->get_image_resume();
  _dial_gravity = simulation_control->get_dial_gravity();
  _button_row_layout->addWidget(simulation_control);

  Image_control *image_control = new Image_control(this);
  if (!image_control) {
    Log::fatal("Status_line::create_button_row(): not enough memory");
  }
  _button_previous_image = image_control->get_button_previous();
  _button_reset_image = image_control->get_button_reset();
  _button_next_image = image_control->get_button_next();
  _button_row_layout->addWidget(image_control);

  if (_config->get_enable_audio()) {
    Audio_control *audio_control = new Audio_control(this, _config);
    if (!audio_control) {
      Log::fatal("Status_line::create_button_row(): not enough memory");
    }
    _dial_volume = audio_control->get_dial_volume();
    _button_mute_unmute = audio_control->get_button_mute_unmute();
    _image_mute = audio_control->get_image_mute();
    _image_unmute = audio_control->get_image_unmute();
    _button_row_layout->addWidget(audio_control);
  } else {
    _dial_volume = 0;
    _button_mute_unmute = 0;
    _image_mute = 0;
    _image_unmute = 0;
  }

  _button_row_layout->addStretch();
  _layout->addWidget(_button_row);
}

Status_line::Status_line(QWidget *parent,
                         IParticles_change_listener *particles_change_listener,
                         IApp_control *app_control,
                         const IConfig *config,
                         const Activity_monitor *activity_monitor,
                         const Simulation_pause_monitor *simulation_pause_monitor,
                         const Simulation_execution_monitor *simulation_execution_monitor)
  : QWidget(parent)
{
  if (!parent) {
    Log::fatal("Status_line::Status_line(): parent is NULL");
  }
  _parent = parent;

  if (!app_control) {
    Log::fatal("Status_line::Status_line(): app_control is NULL");
  }
  _app_control = app_control;

  if (!config) {
    Log::fatal("Status_line::Status_line(): config is NULL");
  }
  _config = config;

  if (!activity_monitor) {
    Log::fatal("Status_line::Status_line(): activity_monitor is NULL");
  }
  _activity_monitor = activity_monitor;

  if (!simulation_pause_monitor) {
    Log::fatal("Status_line::Status_line(): simulation_pause_monitor is NULL");
  }
  _simulation_pause_monitor = simulation_pause_monitor;

  if (!simulation_execution_monitor) {
    Log::fatal("Status_line::Status_line(): "
               "simulation_execution_monitor is NULL");
  }
  _simulation_execution_monitor = simulation_execution_monitor;

  if (!particles_change_listener) {
    Log::fatal("Status_line::Status_line(): particles_change_listener is NULL");
  }
  _particles_change_listener = particles_change_listener;

  _simulation_control = 0;
  _transport_control = 0;

  _config_image_browser = new Config_image_browser(config);
  if (!_config_image_browser) {
    Log::fatal("Status_line::Status_line(): not enough memory");
  }

  QPalette background = palette();
  background.setColor(backgroundRole(), Qt::lightGray);
  setAutoFillBackground(true);
  setPalette(background);

  create_dialogs();

  _layout = new QVBoxLayout();
  if (!_layout) {
    Log::fatal("Status_line::Status_line(): not enough memory");
  }
  setLayout(_layout);

  create_info_row();
  create_button_row();

  adjustSize();

  _is_muted = false;
  _is_cooling = false;
  _is_running = false;
  if (_config->get_enable_audio()) {
    _dial_volume->setEnabled(false);
  }
  _dial_gravity->setEnabled(false);
  gettimeofday(&_menue_button_last_pressed, NULL);

  create_actions();
}

Status_line::~Status_line()
{
  // Q objects will be deleted by Qt, just set them to 0
  _layout = 0;
  _tool_bar = 0;
  _label_app_title = 0;
  _sensors_display = 0;
  _thermal_display = 0;
  _about_dialog = 0;
  _license_dialog = 0;
  _button_pause_resume = 0;
  _image_pause = 0;
  _image_resume = 0;
  _button_previous_image = 0;
  _button_reset_image = 0;
  _button_next_image = 0;
  _dial_volume = 0;
  _button_mute_unmute = 0;
  _image_mute = 0;
  _image_unmute = 0;
  _dial_gravity = 0;
  _button_quit = 0;
  _button_about = 0;
  _button_license = 0;
  _info_row_layout = 0;
  _info_row = 0;
  _sidebar_column_layout = 0;
  _sidebar_column = 0;

  _simulation_control = 0;
  _transport_control = 0;
  _parent = 0;
  _cool_message = 0;
  _is_muted = false;
  _is_running = false;
  _is_cooling = false;
  _menue_button_last_pressed.tv_sec = 0;
  _menue_button_last_pressed.tv_usec = 0;
  _activity_monitor = 0;
  _simulation_pause_monitor = 0;
  _simulation_execution_monitor = 0;
  _app_control = 0;
  _config = 0;
}

void
Status_line::setVisible(const bool visible)
{
  if (visible) setFixedWidth(_parent->geometry().width());
  QWidget::setVisible(visible);
}

void
Status_line::resizeEvent(QResizeEvent* event)
{
  setFixedWidth(_parent->geometry().width());
  QWidget::resizeEvent(event);
}

void
Status_line::set_simulation_control(ISimulation_control *simulation_control)
{
  if (!simulation_control) {
    Log::fatal("Status_line::set_simulation_control(): "
               "simulation_control is NULL");
  }
  _simulation_control = simulation_control;
  _simulation_control->load_image(_config_image_browser->get_current_image());
  if (_config->get_simulation_start_on_application_start()) {
    resume();
  }
}

ISimulation_control *
Status_line::get_simulation_control()
{
  return _simulation_control;
}

void
Status_line::set_transport_control(ITransport_control *transport_control)
{
  if (!transport_control) {
    Log::fatal("Status_line::set_transport_control(): "
               "transport_control is NULL");
  }
  _transport_control = transport_control;
  unmute();
}

void
Status_line::create_actions()
{
  connect(_activity_monitor,
          SIGNAL(signal_low_activity()),
          this,
          SLOT(slot_handle_low_activity()));
  connect(_simulation_pause_monitor,
          SIGNAL(signal_deadline_exceeded()),
          this,
          SLOT(slot_handle_pause_deadline_exceeded()));
  connect(_simulation_execution_monitor,
          SIGNAL(signal_deadline_exceeded()),
          this,
          SLOT(slot_handle_execution_deadline_exceeded()));
  connect(_button_close,
          SIGNAL(clicked()),
          this,
          SLOT(slot_close()));
  connect(_button_pause_resume,
          SIGNAL(clicked()),
          this,
          SLOT(slot_toggle_pause_resume()));
  connect(_button_previous_image,
          SIGNAL(clicked()),
          this,
          SLOT(slot_previous_image()));
  connect(_button_reset_image,
          SIGNAL(clicked()),
          this,
          SLOT(slot_reset_image()));
  connect(_button_next_image,
          SIGNAL(clicked()),
          this,
          SLOT(slot_next_image()));
  if (_config->get_enable_audio()) {
    connect(_dial_volume,
            SIGNAL(valueChanged(int)),
            this,
            SLOT(slot_volume_change()));
    connect(_button_mute_unmute,
            SIGNAL(clicked()),
            this,
            SLOT(slot_toggle_mute_unmute()));
  }
  connect(_dial_gravity,
          SIGNAL(valueChanged(int)),
          this,
          SLOT(slot_gravity_change()));
  if (_config->get_enable_button_quit()) {
    connect(_button_quit,
            SIGNAL(clicked()),
            this,
            SLOT(slot_handle_quit()));
  }
  connect(_button_about,
          SIGNAL(clicked()),
          this,
          SLOT(slot_show_about()));
  connect(_button_license,
          SIGNAL(clicked()),
          this,
          SLOT(slot_show_license()));
}

void
Status_line::particles_changed()
{
  _particles_change_listener->particles_changed();
}

void
Status_line::keyPressEvent(QKeyEvent* event)
{
  const QString label = event->text();
  const int key = event->key();
  gettimeofday(&_menue_button_last_pressed, NULL);
  // TODO: Special keys like Qt::Key_Left and Qt::Key_Right are
  // already caught by QWidget parent class.  Need to change parent
  // class's key filtering policy for retrieving them here.
  const Key_bindings::Action action = _config->get_action_for_key(key);
  switch (action) {
  case Key_bindings::Menu:
    setVisible(!isVisible());
    break;
  case Key_bindings::None:
    {
      std::stringstream msg;
      msg << "unhandled key: " << key;
      Log::warn(msg.str());
    }
    break;
  case Key_bindings::Quit:
    if (_config->get_enable_key_quit()) {
      slot_handle_quit();
    }
    break;
  case Key_bindings::About:
    _about_dialog->show();
    break;
  case Key_bindings::License:
    _license_dialog->show();
    break;
  case Key_bindings::Simulation_start_stop:
    slot_toggle_pause_resume();
    break;
  case Key_bindings::Simulation_decrement_gravity:
    adjust_gravity(-1);
    break;
  case Key_bindings::Simulation_increment_gravity:
    adjust_gravity(+1);
    break;
  case Key_bindings::Image_previous:
    slot_previous_image();
    break;
  case Key_bindings::Image_reset:
    slot_reset_image();
    break;
  case Key_bindings::Image_next:
    slot_next_image();
    break;
  case Key_bindings::Audio_decrement_volume:
    adjust_volume(-1);
    break;
  case Key_bindings::Audio_increment_volume:
    adjust_volume(+1);
    break;
  case Key_bindings::Audio_mute_unmute:
    if (_config->get_enable_audio()) {
      slot_toggle_mute_unmute();
    }
    break;
  default:
    {
      std::stringstream msg;
      msg << "unknown action: " << action;
      Log::fatal(msg.str());
    }
  }
};

void
Status_line::adjust_gravity(const int steps)
{
  const int gravity = _dial_gravity->value();
  const int singleStep = _dial_gravity->singleStep();
  _dial_gravity->setValue(gravity + steps * singleStep);
}

void
Status_line::adjust_volume(const int steps)
{
  const int volume = _dial_volume->value();
  const int singleStep = _dial_volume->singleStep();
  _dial_volume->setValue(volume + steps * singleStep);
  if (_is_muted) slot_toggle_mute_unmute();
}

void
Status_line::start_cooling_break()
{
  if (!_is_cooling) {
    if (!_simulation_control) {
      Log::fatal("Status_line::start_cooling_break(): "
                 "_simulation_control is NULL");
    }
    if (!_cool_message) {
      Log::fatal("Status_line::start_cooling_break(): "
                 "_cool_message is NULL");
    }
    _cool_message->show();
    _is_cooling = true;
    if (_is_running) {
      _simulation_control->pause();
    } else {
      // already pausing because of user pause
    }
    if (!_is_muted) {
      _transport_control->pause();
    }
  } else {
    // we are outside of the hystheresis => keep current state
  }
}

void
Status_line::stop_cooling_break()
{
  if (_is_cooling) {
    if (!_simulation_control) {
      Log::fatal("Status_line::stop_cooling_break(): "
                 "_simulation_control is NULL");
    }
    if (!_cool_message) {
      Log::fatal("Status_line::stop_cooling_break(): "
                 "_cool_message is NULL");
    }
    if (_is_running) {
      _simulation_control->resume();
    } else {
      // leave pausing because of user pause
    }
    if (!_is_muted) {
      _transport_control->resume();
    }
    _is_cooling = false;
    _cool_message->hide();
  } else {
    // we are outside of the hystheresis => keep current state
  }
}

void
Status_line::slot_auto_hide_status_line(const struct timeval mouse_last_moved)
{
  const uint16_t control_autohide_after =
    _config->get_control_autohide_after();
  if ((control_autohide_after > 0) && isVisible()) {
    struct timeval now;
    gettimeofday(&now, NULL);
    const time_t delta1_sec = now.tv_sec - mouse_last_moved.tv_sec;
    const time_t delta2_sec = now.tv_sec - _menue_button_last_pressed.tv_sec;
    if ((delta1_sec >= control_autohide_after) &&
        (delta2_sec >= control_autohide_after)) {
      setVisible(false);
    }
  }
}

void
Status_line::resume()
{
  _simulation_control->resume();
  if (_config->get_enable_audio()) {
    if (!_transport_control) {
      Log::fatal("Status_line::resume(): _transport_control is NULL");
    }
    _transport_control->resume();
  }
  _button_pause_resume->set_image(*_image_pause);
  _button_pause_resume->set_title(tr("Pause"));
  _is_running = true;
}

void
Status_line::pause()
{
  if (_config->get_enable_audio()) {
    if (!_transport_control) {
      Log::fatal("Status_line::pause(): _transport_control is NULL");
    }
    _transport_control->pause();
  }
  _simulation_control->pause();
  _button_pause_resume->set_image(*_image_resume);
  _button_pause_resume->set_title(tr("Resume"));
  _is_running = false;
}

void
Status_line::slot_toggle_pause_resume()
{
  if (!_simulation_control) {
    Log::fatal("Status_line::slot_toggle_pause_resume(): "
               "_simulation_control is NULL");
  }
  if (!_is_cooling) {
    if (_is_running) {
      pause();
    } else {
      resume();
    }
  } else {
    // can not change running state during cooling break
  }
}

void
Status_line::slot_previous_image()
{
  if (!_simulation_control) {
    Log::fatal("Status_line::slot_previous_image(): "
               "_simulation_control is NULL");
  }
  _config_image_browser->move_to_previous_image();
  _simulation_control->load_image(_config_image_browser->get_current_image());
  particles_changed();
}

void
Status_line::slot_reset_image()
{
  if (!_simulation_control) {
    Log::fatal("Status_line::slot_reset_image(): _simulation_control is NULL");
  }
  _simulation_control->reset_image();
  particles_changed();
}

void
Status_line::slot_next_image()
{
  if (!_simulation_control) {
    Log::fatal("Status_line::slot_next_image(): _simulation_control is NULL");
  }
  _config_image_browser->move_to_next_image();
  _simulation_control->load_image(_config_image_browser->get_current_image());
  particles_changed();
}

void
Status_line::slot_close()
{
  setVisible(false);
}

void
Status_line::slot_handle_quit()
{
  _app_control->confirm_quit();
}

void
Status_line::slot_show_about()
{
  _about_dialog->show();
}

void
Status_line::slot_show_license()
{
  _license_dialog->show();
}

void
Status_line::slot_volume_change()
{
  if (!_config->get_enable_audio()) {
    Log::fatal("Status_line::slot_volume_change(): audio not enabled");
  }
  if (!_transport_control) {
    Log::fatal("Status_line::slot_volume_change(): _transport_control is NULL");
  }
  const double value =
    ((double)_dial_volume->value()) /
    (_dial_volume->maximum() - _dial_volume->minimum());
  _transport_control->set_volume(value);
}

void
Status_line::unmute()
{
  _transport_control->unmute();
  _button_mute_unmute->set_image(*_image_mute);
  _button_mute_unmute->set_title(tr("Mute"));
  _is_muted = false;
}

void
Status_line::mute()
{
  _transport_control->mute();
  _button_mute_unmute->set_image(*_image_unmute);
  _button_mute_unmute->set_title(tr("Unmute"));
  _is_muted = true;
}

void
Status_line::slot_toggle_mute_unmute()
{
  if (!_config->get_enable_audio()) {
    Log::fatal("Status_line::slot_toggle_mute_unmute(): audio not enabled");
  }
  if (!_transport_control) {
    Log::fatal("Status_line::slot_toggle_mute_unmute(): "
               "_transport_control is NULL");
  }
  if (_is_muted) {
    unmute();
  } else {
    mute();
  }
}

void
Status_line::slot_gravity_change()
{
  if (!_simulation_control) {
    Log::fatal("Status_line::slot_gravity_change(): "
               "_simulation_control is NULL");
  }
  const double value =
    ((double)_dial_gravity->value()) /
    (1 + _dial_gravity->maximum() - _dial_gravity->minimum());
  _simulation_control->set_gravity((int8_t)(64.0 * value) - 32);
}

void
Status_line::slot_update_sensors_display(const double pitch,
                                         const double roll,
                                         const double ax,
                                         const double ay,
                                         const double temperature)
{
  _sensors_display->slot_update(pitch, roll, ax, ay);
  _thermal_display->slot_update_case_temperature(temperature);
}

void
Status_line::slot_update_thermal_display(const double vc_temperature)
{
  _thermal_display->slot_update_cpu_temperature(vc_temperature);
}

void
Status_line::slot_handle_low_activity()
{
  if (!_is_cooling) {
    if (_is_running) {
      pause();
    }
  }
}

void
Status_line::slot_handle_pause_deadline_exceeded()
{
  if (!_is_cooling) {
    slot_next_image();
    if (!_is_running) {
      resume();
    }
  }
}

void
Status_line::slot_handle_execution_deadline_exceeded()
{
  if (!_is_cooling) {
    slot_next_image();
    if (!_is_running) {
      resume();
    }
  }
}

/*
 * Local variables:
 *   mode: c++
 *   coding: utf-8
 * End:
 */

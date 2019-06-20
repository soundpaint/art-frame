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
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QApplication>
#include <qt-utils.hh>
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
  /*
  palette.setColor(QPalette::WindowText, Qt::blue);
  palette.setColor(QPalette::ToolTipText, Qt::cyan);
  palette.setColor(QPalette::Text, Qt::green);
  palette.setColor(QPalette::BrightText, Qt::red);
  palette.setColor(QPalette::ButtonText, Qt::white);
  */
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

  _cpu_status_display = new Cpu_status_display(this);
  if (!_cpu_status_display) {
    Log::fatal("Status_line::create_info_row(): not enough memory");
  }
  _info_row_layout->addWidget(_cpu_status_display);
  _info_row_layout->addStretch();

  _label_keys = new QLabel(tr("<h1>Interactive Art Frame</h1>"));
  if (!_label_keys) {
    Log::fatal("Status_line::create_info_row(): not enough memory");
  }
  _label_keys->setTextFormat(Qt::RichText);
  _info_row_layout->addWidget(_label_keys);
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
  _pixmap_quit = global_control->get_pixmap_quit();
  _icon_quit = global_control->get_icon_quit();
  _button_about = global_control->get_button_about();
  _pixmap_about = global_control->get_pixmap_about();
  _icon_about = global_control->get_icon_about();
  _button_license = global_control->get_button_license();
  _pixmap_license = global_control->get_pixmap_license();
  _icon_license = global_control->get_icon_license();
  _button_row_layout->addWidget(global_control);

  Simulation_control *simulation_control =
    new Simulation_control(this, _config);
  if (!simulation_control) {
    Log::fatal("Status_line::create_button_row(): not enough memory");
  }
  _button_mode = simulation_control->get_button_mode();
  _pixmap_pause = simulation_control->get_pixmap_pause();
  _icon_pause = simulation_control->get_icon_pause();
  _pixmap_resume = simulation_control->get_pixmap_resume();
  _icon_resume = simulation_control->get_icon_resume();
  _dial_speed = simulation_control->get_dial_speed();
  _button_row_layout->addWidget(simulation_control);

  Image_control *image_control = new Image_control(this);
  if (!image_control) {
    Log::fatal("Status_line::create_button_row(): not enough memory");
  }
  _button_previous = image_control->get_button_previous();
  _pixmap_previous = image_control->get_pixmap_previous();
  _icon_previous = image_control->get_icon_previous();
  _button_reset = image_control->get_button_reset();
  _pixmap_reset = image_control->get_pixmap_reset();
  _icon_reset = image_control->get_icon_reset();
  _button_next = image_control->get_button_next();
  _pixmap_next = image_control->get_pixmap_next();
  _icon_next = image_control->get_icon_next();
  _button_row_layout->addWidget(image_control);

  if (_config->get_enable_audio()) {
    Audio_control *audio_control = new Audio_control(this, _config);
    if (!audio_control) {
      Log::fatal("Status_line::create_button_row(): not enough memory");
    }
    _dial_volume = audio_control->get_dial_volume();
    _button_mute = audio_control->get_button_mute();
    _pixmap_unmuted = audio_control->get_pixmap_unmuted();
    _icon_unmuted = audio_control->get_icon_unmuted();
    _pixmap_muted = audio_control->get_pixmap_muted();
    _icon_muted = audio_control->get_icon_muted();
    _button_row_layout->addWidget(audio_control);
  } else {
    _dial_volume = 0;
    _button_mute = 0;
    _pixmap_unmuted = 0;
    _icon_unmuted = 0;
    _pixmap_muted = 0;
    _icon_muted = 0;
  }

  _button_row_layout->addStretch();
  _layout->addWidget(_button_row);
}

Status_line::Status_line(QWidget *parent,
                         IParticles_change_listener *particles_change_listener,
                         const IConfig *config)
  : QWidget(parent)
{
  if (!parent) {
    Log::fatal("Status_line::Status_line(): parent is NULL");
  }
  _parent = parent;

  if (!config) {
    Log::fatal("Status_line::Status_line(): config is NULL");
  }
  _config = config;

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
  _dial_speed->setEnabled(false);
  gettimeofday(&_menue_button_last_pressed, NULL);

  create_actions();
}

Status_line::~Status_line()
{
  // Q objects will be deleted by Qt, just set them to 0
  _layout = 0;
  _tool_bar = 0;
  _label_keys = 0;
  _sensors_display = 0;
  _cpu_status_display = 0;
  _pixmap_resume = 0;
  _icon_resume = 0;
  _pixmap_pause = 0;
  _icon_pause = 0;
  _pixmap_previous = 0;
  _icon_previous = 0;
  _pixmap_reset = 0;
  _icon_reset = 0;
  _pixmap_next = 0;
  _icon_next = 0;
  _pixmap_unmuted = 0;
  _icon_unmuted = 0;
  _pixmap_muted = 0;
  _icon_muted = 0;
  _pixmap_quit = 0;
  _icon_quit = 0;
  _pixmap_about = 0;
  _icon_about = 0;
  _about_dialog = 0;
  _pixmap_license = 0;
  _icon_license = 0;
  _license_dialog = 0;
  _button_mode = 0;
  _button_previous = 0;
  _button_reset = 0;
  _button_next = 0;
  _dial_volume = 0;
  _button_mute = 0;
  _dial_speed = 0;
  _button_quit = 0;
  _button_about = 0;
  _button_license = 0;

  _simulation_control = 0;
  _transport_control = 0;
  _parent = 0;
  _cool_message = 0;
  _is_muted = false;
  _is_running = false;
  _is_cooling = false;
  _menue_button_last_pressed.tv_sec = 0;
  _menue_button_last_pressed.tv_usec = 0;
}

void
Status_line::setVisible(const bool visible)
{
  setFixedWidth(_parent->geometry().width());
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
}

void
Status_line::create_actions()
{
  connect(_button_mode,
          SIGNAL(clicked()),
          this,
          SLOT(slot_toggle_mode()));
  connect(_button_previous,
          SIGNAL(clicked()),
          this,
          SLOT(slot_previous()));
  connect(_button_reset,
          SIGNAL(clicked()),
          this,
          SLOT(slot_reset()));
  connect(_button_next,
          SIGNAL(clicked()),
          this,
          SLOT(slot_next()));
  if (_config->get_enable_audio()) {
    connect(_dial_volume,
            SIGNAL(valueChanged(int)),
            this,
            SLOT(slot_volume_change()));
    connect(_button_mute,
            SIGNAL(clicked()),
            this,
            SLOT(slot_toggle_mute()));
  }
  connect(_dial_speed,
          SIGNAL(valueChanged(int)),
          this,
          SLOT(slot_speed_change()));
  if (_config->get_enable_button_quit()) {
    connect(_button_quit,
            SIGNAL(clicked()),
            this,
            SLOT(slot_confirm_quit()));
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
  const int code = event->key();
  switch (code) {
    // TODO: Special keys like Qt::Key_Left and Qt::Key_Right are
    // already caught by QWidget parent class.  Need to change parent
    // class's key filtering policy for catching them in this switch
    // statement.
  case Qt::Key_Back:
  case Qt::Key_Left:
  case Qt::Key_P:
  case Qt::Key_Backtab:
    slot_previous();
    break;
  case Qt::Key_Forward:
  case Qt::Key_N:
  case Qt::Key_Right:
  case Qt::Key_Tab:
    slot_next();
    break;
  case Qt::Key_A:
  case Qt::Key_Info:
    _about_dialog->show();
    break;
  case Qt::Key_C:
    _license_dialog->show();
    break;
  case Qt::Key_PageUp:
    adjust_speed(+1);
    break;
  case Qt::Key_PageDown:
    adjust_speed(-1);
    break;
  case Qt::Key_L:
  case Qt::Key_Pause:
    slot_toggle_mode();
    break;
  case Qt::Key_Plus:
    adjust_volume(+1);
    break;
  case Qt::Key_Minus:
    adjust_volume(-1);
    break;
  case Qt::Key_M:
  case Qt::Key_VolumeMute:
    if (_config->get_enable_audio()) {
      slot_toggle_mute();
    }
    break;
  case Qt::Key_Q:
  case Qt::Key_Escape:
  case Qt::Key_PowerOff:
    if (_config->get_enable_key_quit()) {
      slot_confirm_quit();
    }
    break;
  case Qt::Key_R:
    slot_reset();
    break;
  case Qt::Key_MenuKB:
  case Qt::Key_F1:
  case Qt::Key_Question:
  case Qt::Key_NumberSign:
    setVisible(!isVisible());
    gettimeofday(&_menue_button_last_pressed, NULL);
    break;
  default:
    // no action
    break;
  }
};

void
Status_line::adjust_speed(const int steps)
{
  if (_dial_speed->isEnabled()) {
    const int speed = _dial_speed->value();
    const int singleStep = _dial_speed->singleStep();
    _dial_speed->setValue(speed + steps * singleStep);
  }
}

void
Status_line::adjust_volume(const int steps)
{
  if (_dial_volume->isEnabled()) {
    const int volume = _dial_volume->value();
    const int singleStep = _dial_volume->singleStep();
    _dial_volume->setValue(volume + steps * singleStep);
    if (_is_muted) slot_toggle_mute();
  }
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
    setEnabled(false);
    _cool_message->show();
    if (_is_running) {
      _simulation_control->pause();
    } else {
      // already pausing because of user pause
    }
    _is_cooling = true;
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
    _cool_message->hide();
    setEnabled(true);
    _is_cooling = false;
  } else {
    // we are outside of the hystheresis => keep current state
  }
}

const double
Status_line::STATUS_LINE_AUTO_HIDE_INTERVAL = 10; // [s]

void
Status_line::slot_auto_hide_status_line(const struct timeval mouse_last_moved)
{
  if (isVisible()) {
    struct timeval now;
    gettimeofday(&now, NULL);
    const time_t delta1_sec = now.tv_sec - mouse_last_moved.tv_sec;
    const time_t delta2_sec = now.tv_sec - _menue_button_last_pressed.tv_sec;
    if ((delta1_sec >= STATUS_LINE_AUTO_HIDE_INTERVAL) &&
        (delta2_sec >= STATUS_LINE_AUTO_HIDE_INTERVAL)) {
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
  //_button_mode->setText(tr("Pause"));
  _button_mode->setIcon(*_icon_pause);
  _button_mode->setIconSize(_pixmap_pause->rect().size());
  //_button_previous->setEnabled(true);
  //_button_reset->setEnabled(true);
  //_button_next->setEnabled(true);
  if (_config->get_enable_audio()) {
    _dial_volume->setEnabled(true);
  }
  _dial_speed->setEnabled(true);
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
  //_button_mode->setText(tr("Resume"));
  _button_mode->setIcon(*_icon_resume);
  _button_mode->setIconSize(_pixmap_resume->rect().size());
  //_button_previous->setEnabled(false);
  //_button_reset->setEnabled(false);
  //_button_next->setEnabled(false);
  if (_config->get_enable_audio()) {
    _dial_volume->setEnabled(false);
  }
  _dial_speed->setEnabled(false);
  _is_running = false;
}

void
Status_line::slot_toggle_mode()
{
  if (!_simulation_control) {
    Log::fatal("Status_line::slot_toggle_mode(): _simulation_control is NULL");
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
Status_line::slot_previous()
{
  if (!_simulation_control) {
    Log::fatal("Status_line::slot_previous(): _simulation_control is NULL");
  }
  _config_image_browser->move_to_previous_image();
  _simulation_control->load_image(_config_image_browser->get_current_image());
  particles_changed();
}

void
Status_line::slot_reset()
{
  if (!_simulation_control) {
    Log::fatal("Status_line::slot_reset(): _simulation_control is NULL");
  }
  _simulation_control->reset_image();
  particles_changed();
}

void
Status_line::slot_next()
{
  if (!_simulation_control) {
    Log::fatal("Status_line::slot_next(): _simulation_control is NULL");
  }
  _config_image_browser->move_to_next_image();
  _simulation_control->load_image(_config_image_browser->get_current_image());
  particles_changed();
}

void
Status_line::slot_confirm_quit()
{
  QMessageBox::StandardButton reply;
  reply = QMessageBox::question(this, tr("Confirm Shutdown"),
                                tr("Are you sure to shut down the system?"),
                                QMessageBox::Yes | QMessageBox::No);
  if (reply == QMessageBox::Yes) {
    QApplication::quit();
  } else {
    // continue
  }
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
Status_line::slot_toggle_mute()
{
  if (!_config->get_enable_audio()) {
    Log::fatal("Status_line::slot_toggle_mute(): audio not enabled");
  }
  if (!_transport_control) {
    Log::fatal("Status_line::slot_toggle_mute(): _transport_control is NULL");
  }
  if (_is_muted) {
    _transport_control->unmute();
    //_button_mute->setText(tr("Mute"));
    _button_mute->setIcon(*_icon_unmuted);
    _button_mute->setIconSize(_pixmap_unmuted->rect().size());
    _is_muted = false;
  } else {
    _transport_control->mute();
    //_button_mute->setText(tr("Unmute"));
    _button_mute->setIcon(*_icon_muted);
    _button_mute->setIconSize(_pixmap_muted->rect().size());
    _is_muted = true;
  }
}

void
Status_line::slot_speed_change()
{
  if (!_simulation_control) {
    Log::fatal("Status_line::slot_speed_change(): _simulation_control is NULL");
  }
  const double value =
    ((double)_dial_speed->value()) /
    (_dial_speed->maximum() - _dial_speed->minimum());
  _simulation_control->set_speed(value);
}

void
Status_line::slot_update_sensors_display(const double pitch,
                                         const double roll,
                                         const double ax,
                                         const double ay,
                                         const double temperature)
{
  _sensors_display->slot_update(pitch, roll, ax, ay);
  _cpu_status_display->slot_update_case_temperature(temperature);
}

void
Status_line::slot_update_cpu_status_display(const double vc_temperature)
{
  _cpu_status_display->slot_update_cpu_temperature(vc_temperature);
}

/*
 * Local variables:
 *   mode: c++
 *   coding: utf-8
 * End:
 */

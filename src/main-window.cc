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

#include <main-window.hh>
#include <log.hh>

Main_window::Main_window(const uint16_t width,
                         const uint16_t height,
                         const IConfig *config,
                         const Sensors *sensors,
                         Simulation *simulation,
                         ITransport_control *transport_control,
                         QWidget *parent)
  : QMainWindow(parent)
{
  setWindowFlags(windowFlags() | Qt::FramelessWindowHint);
  if (!config->get_enable_cursor()) {
    setCursor(Qt::BlankCursor);
  }
  _mouse_pressed = false;
  _press_pos = QPointF(0.0, 0.0);
  _release_pos = QPointF(0.0, 0.0);
  setMouseTracking(true);

  setWindowTitle(tr("Art Frame"));

  _window_icon = new QIcon("piy.png");
  if (!_window_icon) {
    Log::fatal("Main_window::Main_window(): not enough memory");
  }
  setWindowIcon(*_window_icon);

  _central_widget_layout = new QVBoxLayout();
  if (!_central_widget_layout) {
    Log::fatal("Main_window::Main_window(): not enough memory");
  }

  _central_widget = new QWidget();
  if (!_central_widget) {
    Log::fatal("Main_window::Main_window(): not enough memory");
  }
  _central_widget->setMouseTracking(true);
  _central_widget->setLayout(_central_widget_layout);
  setCentralWidget(_central_widget);

  _frame_display =
    new Frame_display(width, height, config, sensors,
                      simulation->get_particles(), this);
  if (!_frame_display) {
    Log::fatal("Main_window::Main_window(): not enough memory");
  }
  _frame_display->setMouseTracking(true);
  _central_widget_layout->addWidget(_frame_display, 1.0);

  _activity_monitor = new Activity_monitor(this, config, simulation);
  if (!_activity_monitor) {
    Log::fatal("Main_window::Main_window(): not enough memory");
  }

  _simulation_pause_monitor =
    new Simulation_pause_monitor(this, config, simulation);
  if (!_simulation_pause_monitor) {
    Log::fatal("Main_window::Main_window(): not enough memory");
  }

  _simulation_execution_monitor =
    new Simulation_execution_monitor(this, config, simulation);
  if (!_simulation_execution_monitor) {
    Log::fatal("Main_window::Main_window(): not enough memory");
  }

  _status_line =
    new Status_line(this, _frame_display, config,
                    _activity_monitor, _simulation_pause_monitor,
                    _simulation_execution_monitor);
  if (!_status_line) {
    Log::fatal("Main_window::Main_window(): not enough memory");
  }
  if (config->get_enable_audio()) {
    _status_line->set_transport_control(transport_control);
  }
  _status_line->set_simulation_control(simulation);
  //_central_widget_layout->addWidget(_status_line, 0.0);

  gettimeofday(&_mouse_last_moved, NULL);
  gettimeofday(&_mouse_last_pressed, NULL);
  _have_prev_pos = false;
  _prev_pos = QPointF(0.0, 0.0);
}

Main_window::~Main_window()
{
  // Q objects will be deleted by Qt, just set them to 0
  _window_icon = 0;
  _frame_display = 0;
  _central_widget_layout = 0;
  _central_widget = 0;

  _press_pos = QPointF(0.0, 0.0);
  _release_pos = QPointF(0.0, 0.0);
  _mouse_pressed = false;
  _mouse_last_moved.tv_sec = 0;
  _mouse_last_moved.tv_usec = 0;
  _mouse_last_pressed.tv_sec = 0;
  _mouse_last_pressed.tv_usec = 0;
  _have_prev_pos = false;
  _prev_pos = QPointF(0.0, 0.0);
  delete _activity_monitor;
  _activity_monitor = 0;
  delete _simulation_pause_monitor;
  _simulation_pause_monitor = 0;
  delete _simulation_execution_monitor;
  _simulation_execution_monitor = 0;
}

const struct timeval
Main_window::difftime(const struct timeval time1, const struct timeval time2)
{
  struct timeval diff;
  double tv_sec = time1.tv_sec - time2.tv_sec;
  double tv_usec = time2.tv_usec - time2.tv_usec;
  if (tv_sec < 0.0) {
    Log::fatal("Main_window::difftime: time1 must be after time2");
  }
  if (tv_usec < 0.0) {
    tv_usec += 1000000.0;
    tv_sec -= 1.0;
  }
  diff.tv_sec = (time_t)tv_sec;
  diff.tv_usec = (suseconds_t)tv_usec;
  return diff;
}

void
Main_window::mouseDoubleClickEvent(QMouseEvent *event)
{
  if (event->button() == Qt::LeftButton) {
    gettimeofday(&_mouse_last_pressed, NULL);
    if (!_status_line->isVisible()) {
      _status_line->setVisible(true);
    }
  }
  QMainWindow::mouseDoubleClickEvent(event);
}

void
Main_window::mouseMoveEvent(QMouseEvent *event)
{
  if (event->buttons() & Qt::LeftButton) {
    if (event->button() == Qt::NoButton) {
      /*
        if (!_status_line->isVisible()) {
        _status_line->setVisible(true);
        }
      */
      struct timeval now;
      gettimeofday(&now, NULL);
      const time_t delta_sec = now.tv_sec - _mouse_last_moved.tv_sec;
      const suseconds_t delta_usec = now.tv_usec - _mouse_last_moved.tv_usec;
      const QPointF current_pos = event->localPos();
      if (_status_line->get_simulation_control()->is_running()) {
        if (_have_prev_pos) {
          _frame_display->handle_sweep(_prev_pos, current_pos,
                                       delta_sec, delta_usec);
        }
      }
      _prev_pos = current_pos;
      _have_prev_pos = true;
      _mouse_last_moved = now;
    }
  }
  QMainWindow::mouseMoveEvent(event);
}

void
Main_window::mousePressEvent(QMouseEvent *event)
{
  gettimeofday(&_mouse_last_moved, NULL);
  gettimeofday(&_mouse_last_pressed, NULL);
  _press_pos = event->localPos();
  _mouse_pressed = true;
  _have_prev_pos = false;
}

void
Main_window::mouseReleaseEvent(QMouseEvent *event)
{
  struct timeval now;
  gettimeofday(&now, NULL);
  _release_pos = event->localPos();
  _mouse_pressed = false;

  struct timeval diff = difftime(now, _mouse_last_pressed);
  if (diff.tv_sec >= 3) {
    if (!_status_line->isVisible()) {
      gettimeofday(&_mouse_last_moved, NULL);
      _status_line->setVisible(true);
    }
  }

  /*
  std::cout << "[wipe: " <<
    "press.x=" << _press_pos.x() <<
    ", press.y=" << _press_pos.y() <<
    ", release.x=" << _release_pos.x() <<
    ", release.y=" << _release_pos.y() << "]\n"; // DEBUG
  */
  _have_prev_pos = false;
}

Frame_display *
Main_window::get_frame_display() const
{
  return _frame_display;
}

void
Main_window::slot_update_sensors_display(const double pitch,
                                         const double roll,
                                         const double ax,
                                         const double ay,
                                         const double temperature)
{
  _status_line->slot_update_sensors_display(pitch, roll, ax, ay, temperature);

  // FIXME: The following code should be called independently from here.
  slot_auto_hide_status_line();
}

void
Main_window::slot_auto_hide_status_line()
{
#if 0
  // auto hide after last cursor movement
  struct timeval now;
  gettimeofday(&now, NULL);
  if (_status_line->isVisible()) {
    const time_t delta_sec = now.tv_sec - _mouse_last_moved.tv_sec;
    if (delta_sec >= Status_line::STATUS_LINE_AUTO_HIDE_INTERVAL) {
#if 0 // auto hide only if cursor outside of status line
      const QRect status_line_geometry = _status_line->geometry();
      const QPoint mouse_pos = mapFromGlobal(QCursor::pos());
      const bool in_status_line = status_line_geometry.contains(mouse_pos);
      if (!in_status_line) {
        _status_line->setVisible(false);
      }
#else 0 // auto hide independent of cursor position
      _status_line->setVisible(false);
#endif
    }
  }
#else
  // auto hide after last menu key press
  _status_line->slot_auto_hide_status_line(_mouse_last_moved);
#endif
}

void
Main_window::slot_update_thermal_display(const double vc_temperature)
{
  _status_line->slot_update_thermal_display(vc_temperature);
}

void
Main_window::start_cooling_break()
{
  setEnabled(false);
  _status_line->start_cooling_break();
}

void
Main_window::stop_cooling_break()
{
  _status_line->stop_cooling_break();
  setEnabled(true);
}

/*
 * Local variables:
 *   mode: c++
 *   coding: utf-8
 * End:
 */

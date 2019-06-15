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

#ifndef MAIN_WINDOW_HH
#define MAIN_WINDOW_HH

#include <time.h>
#include <QtCore/QObject>
#include <QtCore/QProcess>
#include <QtCore/QString>
#include <QtGui/QIcon>
#include <QtGui/QMouseEvent>
#include <QtWidgets/QAction>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include <frame-display.hh>
#include <status-line.hh>
#include <sensors.hh>
#include <simulation.hh>
#include <audio-player.hh>

class Main_window : public QMainWindow
{
  Q_OBJECT
public:
  explicit Main_window(const uint16_t width,
                       const uint16_t height,
                       const IConfig *config,
		       const Sensors *sensors,
                       Simulation *simulation,
		       Audio_player *audio_player,
		       QWidget *parent = 0);
  virtual ~Main_window();
  Frame_display *get_frame_display() const;
  Status_line *get_status_line() const;
  void start_cooling_break();
  void stop_cooling_break();
public slots:
  void slot_update_sensors_display(const double pitch,
				   const double roll,
				   const double ax,
				   const double ay,
				   const double temperature);
  void slot_auto_hide_status_line();
  void slot_update_cpu_status_display(const double vc_temperature);
protected:
  void mouseMoveEvent(QMouseEvent *event);
  void mousePressEvent(QMouseEvent *event);
  void mouseReleaseEvent(QMouseEvent *event);
  void mouseDoubleClickEvent(QMouseEvent *event);
private:
  QIcon *_window_icon;
  QVBoxLayout *_central_widget_layout;
  QWidget *_central_widget;
  Frame_display *_frame_display;
  Status_line *_status_line;
  bool _mouse_pressed;
  QPointF _press_pos, _release_pos, _prev_pos;
  struct timeval _mouse_last_moved, _mouse_last_pressed;
  bool _have_prev_pos;
  const struct timeval difftime(const struct timeval time1,
				const struct timeval time2);
  bool is_simulation_running();
};

#endif /* MAIN_WINDOW_HH */

/*
 * Local variables:
 *   mode: c++
 *   coding: utf-8
 * End:
 */

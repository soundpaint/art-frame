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

#ifndef STATUS_LINE_HH
#define STATUS_LINE_HH

#include <QtCore/QObject>
#include <QtCore/QProcess>
#include <QtCore/QString>
#include <QtWidgets/QAction>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>
#include <frame-display.hh>
#include <iconfig.hh>
#include <activity-monitor.hh>
#include <simulation-pause-monitor.hh>
#include <simulation-execution-monitor.hh>
#include <config-image-browser.hh>
#include <global-control.hh>
#include <simulation-control.hh>
#include <image-control.hh>
#include <audio-control.hh>
#include <sensors-display.hh>
#include <cpu-status-display.hh>
#include <about-dialog.hh>
#include <license-dialog.hh>
#include <isimulation-control.hh>
#include <itransport-control.hh>
#include <iparticles-change-listener.hh>

class Status_line : public QWidget
{
  Q_OBJECT
public:
  static const double STATUS_LINE_AUTO_HIDE_INTERVAL;
  explicit Status_line(QWidget *parent,
                       IParticles_change_listener *particles_change_listener,
                       const IConfig *config,
                       const Activity_monitor *activity_monitor,
                       const Simulation_pause_monitor *simulation_pause_monitor,
                       const Simulation_execution_monitor *simulation_execution_monitor);
  virtual ~Status_line();
  void set_simulation_control(ISimulation_control *simulation_control);
  ISimulation_control *get_simulation_control();
  void set_transport_control(ITransport_control *transport_control);
  virtual void setVisible(const bool visible);
  virtual void resizeEvent(QResizeEvent* event);
  virtual void keyPressEvent(QKeyEvent* event);
  void start_cooling_break();
  void stop_cooling_break();
public slots:
  void slot_toggle_mode();
  void slot_previous();
  void slot_reset();
  void slot_next();
  void slot_confirm_quit();
  void slot_show_about();
  void slot_show_license();
  void slot_volume_change();
  void slot_toggle_mute();
  void slot_speed_change();
  void slot_update_sensors_display(const double pitch,
                                   const double roll,
                                   const double ax,
                                   const double ay,
                                   const double temperature);
  void slot_update_cpu_status_display(const double vc_temperature);
  void slot_auto_hide_status_line(const struct timeval mouse_last_moved);
private slots:
  void slot_handle_low_activity();
  void slot_handle_pause_deadline_exceeded();
  void slot_handle_execution_deadline_exceeded();
private:
  QWidget *_parent;
  const IConfig *_config;
  const Activity_monitor *_activity_monitor;
  const Simulation_pause_monitor *_simulation_pause_monitor;
  const Simulation_execution_monitor *_simulation_execution_monitor;
  IParticles_change_listener *_particles_change_listener;
  Config_image_browser *_config_image_browser;
  ISimulation_control *_simulation_control;
  ITransport_control *_transport_control;
  QVBoxLayout *_layout;
  QHBoxLayout *_info_row_layout;
  QWidget *_info_row;
  QHBoxLayout *_button_row_layout;
  QWidget *_button_row;
  QToolBar *_tool_bar;
  QPushButton *_button_quit;
  QPushButton *_button_about;
  QPushButton *_button_license;
  QPushButton *_button_mode;
  QDial *_dial_speed;
  QPushButton *_button_previous;
  QPushButton *_button_reset;
  QPushButton *_button_next;
  QDial *_dial_volume;
  QPushButton *_button_mute;
  QPixmap *_pixmap_quit;
  QPixmap *_pixmap_about;
  QPixmap *_pixmap_license;
  QPixmap *_pixmap_pause;
  QPixmap *_pixmap_resume;
  QPixmap *_pixmap_previous;
  QPixmap *_pixmap_reset;
  QPixmap *_pixmap_next;
  QPixmap *_pixmap_unmuted;
  QPixmap *_pixmap_muted;
  QIcon *_icon_quit;
  QIcon *_icon_about;
  QIcon *_icon_license;
  QIcon *_icon_pause;
  QIcon *_icon_resume;
  QIcon *_icon_previous;
  QIcon *_icon_reset;
  QIcon *_icon_next;
  QIcon *_icon_unmuted;
  QIcon *_icon_muted;
  bool _is_muted;
  bool _is_running;
  bool _is_cooling;
  Sensors_display *_sensors_display;
  Cpu_status_display *_cpu_status_display;
  QLabel *_label_keys;
  About_dialog *_about_dialog;
  License_dialog *_license_dialog;
  QMessageBox *_cool_message;
  struct timeval _menue_button_last_pressed;
  static QMessageBox *create_cool_message();
  void adjust_speed(const int steps);
  void adjust_volume(const int steps);
  void create_dialogs();
  void create_info_row();
  void create_button_row();
  void create_actions();
  void pause();
  void resume();
  void particles_changed();
};

#endif /* STATUS_LINE_HH */

/*
 * Local variables:
 *   mode: c++
 *   coding: utf-8
 * End:
 */

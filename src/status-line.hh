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
#include <QtWidgets/QAbstractButton>
#include <QtWidgets/QAction>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>
#include <frame-display.hh>
#include <about-dialog.hh>
#include <activity-monitor.hh>
#include <audio-control.hh>
#include <config-image-browser.hh>
#include <global-control.hh>
#include <iapp-control.hh>
#include <iconfig.hh>
#include <image-control.hh>
#include <iparticles-change-listener.hh>
#include <isimulation-control.hh>
#include <itransport-control.hh>
#include <license-dialog.hh>
#include <sensors-display.hh>
#include <simulation-execution-monitor.hh>
#include <simulation-pause-monitor.hh>
#include <simulation-control.hh>
#include <thermal-display.hh>
#include <titled-button.hh>

class Status_line : public QWidget
{
  Q_OBJECT
public:
  static const double STATUS_LINE_AUTO_HIDE_INTERVAL;
  explicit Status_line(QWidget *parent,
                       IParticles_change_listener *particles_change_listener,
                       IApp_control *app_control,
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
  void slot_toggle_pause_resume();
  void slot_previous_image();
  void slot_reset_image();
  void slot_next_image();
  void slot_capture_image();
  void slot_show_about();
  void slot_show_license();
  void slot_volume_change();
  void slot_toggle_mute_unmute();
  void slot_gravity_change();
  void slot_update_sensors_display(const double pitch,
                                   const double roll,
                                   const double ax,
                                   const double ay,
                                   const double temperature);
  void slot_update_thermal_display(const double vc_temperature);
  void slot_auto_hide_status_line(const struct timeval mouse_last_pressed);
private slots:
  void slot_close();
  void slot_handle_quit();
  void slot_handle_low_activity();
  void slot_handle_pause_deadline_exceeded();
  void slot_handle_execution_deadline_exceeded();
protected:
  void mouseMoveEvent(QMouseEvent *event);
  void mousePressEvent(QMouseEvent *event);
  void mouseReleaseEvent(QMouseEvent *event);
private:
  QWidget *_parent;
  IApp_control *_app_control;
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
  QVBoxLayout *_sidebar_column_layout;
  QWidget *_sidebar_column;
  QHBoxLayout *_button_row_layout;
  QWidget *_button_row;
  QToolBar *_tool_bar;
  QAbstractButton *_button_quit;
  QAbstractButton *_button_about;
  QAbstractButton *_button_license;
  Titled_button *_button_pause_resume;
  QImage *_image_pause;
  QImage *_image_resume;
  QDial *_dial_gravity;
  QAbstractButton *_button_previous_image;
  QAbstractButton *_button_reset_image;
  QAbstractButton *_button_next_image;
  QDial *_dial_volume;
  Titled_button *_button_mute_unmute;
  QImage *_image_mute;
  QImage *_image_unmute;
  bool _is_muted;
  bool _is_running;
  bool _is_cooling;
  Sensors_display *_sensors_display;
  Thermal_display *_thermal_display;
  QLabel *_label_app_title;
  QAbstractButton *_button_close;
  About_dialog *_about_dialog;
  License_dialog *_license_dialog;
  QMessageBox *_cool_message;
  struct timeval _keyboard_last_event;
  struct timeval _mouse_last_event;
  static QMessageBox *create_cool_message();
  void adjust_gravity(const int steps);
  void adjust_volume(const int steps);
  void create_dialogs();
  void create_info_row();
  void create_button_row();
  void create_actions();
  void set_pause_image();
  void pause();
  void resume();
  void unmute();
  void mute();
  void particles_changed();
};

#endif /* STATUS_LINE_HH */

/*
 * Local variables:
 *   mode: c++
 *   coding: utf-8
 * End:
 */

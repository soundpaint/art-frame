/*
 * art-frame -- an artful sands image emulation
 *
 * Copyright (C) 2019 Jürgen Reuter
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

#ifndef ACTIVITY_MONITOR_HH
#define ACTIVITY_MONITOR_HH

#include <QtCore/QTimer>
#include <iconfig.hh>
#include <isimulation-control.hh>

class Activity_monitor : public QTimer
{
  Q_OBJECT
public:
  explicit Activity_monitor(QObject *parent,
                            const IConfig *config,
                            const ISimulation_control *simulation);
  virtual ~Activity_monitor();
private slots:
  void slot_check_activity();
private:
  const IConfig *_config;
  const ISimulation_control *_simulation;
signals:
  void signal_stop_simulation();
};

#endif /* ACTIVITY_MONITOR_HH */

/*
 * Local variables:
 *   mode: c++
 *   coding: utf-8
 * End:
 */

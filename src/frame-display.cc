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

#include <QtGui/QBrush>
#include <QtGui/QPainter>
#include <QtGui/QPaintEvent>
#include <QtWidgets/QApplication>
#include <frame-display.hh>
#include <log.hh>

Frame_display::Frame_display(const uint16_t minimum_width,
                             const uint16_t minimum_height,
                             const IConfig *config,
                             const Sensors *sensors,
                             Particles *particles,
                             QWidget *parent)
  : QWidget(parent)
{
  // to compute frame display size correctly, parent must not be NULL
  if (!parent) {
    Log::fatal("Frame_display::Frame_display(): parent is NULL");
  }

  if (!config) {
    Log::fatal("Frame_display::Frame_display(): config is NULL");
  }
  _config = config;

  if (minimum_width <= 0) {
    Log::fatal("Frame_display::Frame_display(): minimum_width <= 0");
  }
  _minimum_width = minimum_width;

  if (minimum_height <= 0) {
    Log::fatal("Frame_display::Frame_display(): minimum_height <= 0");
  }
  _minimum_height = minimum_height;

  if (!sensors) {
    Log::fatal("Frame_display::Frame_display(): sensors is NULL");
  }
  _sensors = sensors;

  if (!particles) {
    Log::fatal("Frame_display::Frame_display(): particles is NULL");
  }
  _particles = particles;
  _particles->set_change_listener(this);

  setBackgroundRole(QPalette::Base);
  setAutoFillBackground(true);
}

Frame_display::~Frame_display()
{
  _minimum_width = 0;
  _minimum_height = 0;
  _particles = 0;
  _sensors = 0;
  _config = 0;
}

QSize
Frame_display::minimumSizeHint() const
{
  return QSize(_minimum_width, _minimum_height);
}

void
Frame_display::draw_particles(QPainter *painter, const QRect rect)
{
  const QImage *image = _particles->get_image();
  // TODO: Consider window resize
  /*
  const uint16_t current_width = width();
  const uint16_t current_height = height();
  if ((current_width != image->width()) ||
      (current_height != image->height())) {
    delete _particles;
    }
    _particles = create_background(current_width, current_height);
    if (!_particles) {
      Log::fatal("Frame_display::draw_background(): not enough memory");
    }
  }
  */
  painter->drawImage(rect, *image, rect);
}

void
Frame_display::paintEvent(QPaintEvent *event)
{
  const QRect rect = event->rect();
  QPainter painter(this);
  draw_particles(&painter, rect);
  painter.end();
}

void
Frame_display::particles_changed()
{
  const QRect paintRect(0, 0, width(), height());
  update_display(paintRect);
}

void
Frame_display::update_display(const QRect paintRect)
{
  update(paintRect);
  sweep_fade_step();
}

void
Frame_display::handle_sweep(const QPointF pos0,
                            const QPointF pos1,
                            const double time)
{
  _particles->handle_sweep(pos0, pos1, time);
}

void
Frame_display::sweep_fade_step()
{
  _particles->sweep_fade_step();
}

/*
 * Local variables:
 *   mode: c++
 *   coding: utf-8
 * End:
 */

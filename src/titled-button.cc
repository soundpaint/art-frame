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

#include <titled-button.hh>
#include <QtGui/QFont>
#include <log.hh>
#include <iconfig.hh>

const int
Titled_button::BUTTON_WIDTH = 72;

const int
Titled_button::BUTTON_HEIGHT = 72;

const int
Titled_button::ICON_WIDTH = 40;

const int
Titled_button::ICON_HEIGHT = 40;

Titled_button::Titled_button(QWidget *parent,
                             const char *tool_tip_text,
                             const char *title,
                             const char *image_path)
  : QToolButton(parent)
{
  _label_font_point_size = font().pointSize();
  ensurePolished();
  QFont widgetFont = font();
  widgetFont.setPointSize(get_label_font_point_size());
  widgetFont.setCapitalization(QFont::AllUppercase);

  _label = new QLabel();
  if (!_label) {
    Log::fatal("Titled_button::Titled_button(): not enough memory");
  }
  _label->setAlignment(Qt::AlignBottom | Qt::AlignHCenter);
  _label->setWordWrap(true);
  _label->setTextInteractionFlags(Qt::NoTextInteraction);
  _label->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  _label->setFont(widgetFont);

  _layout = new QHBoxLayout();
  if (!_layout) {
    Log::fatal("Titled_button::Titled_button(): not enough memory");
  }
  _layout->addWidget(_label);
  _layout->setSpacing(0);
  _layout->setMargin(0);
  _layout->setContentsMargins(2, 2, 2, 2);

  setFixedSize(BUTTON_WIDTH, BUTTON_HEIGHT);
  setToolTip(tr(tool_tip_text));
  setLayout(_layout);
  setIconSize(QSize(ICON_WIDTH, ICON_HEIGHT));

  _pixmap = new QPixmap();
  if (!_pixmap) {
    Log::fatal("Titled_button::Titled_button(): not enough memory");
  }

  if (title) {
    set_title(title);
  }
  if (image_path) {
    set_image(QImage(image_path));
  }
}

Titled_button::~Titled_button()
{
  _label = 0;
  _pixmap = 0;
  _layout = 0;
}

void
Titled_button::set_title(const QString &title)
{
  _label->setText(title);
}

bool
Titled_button::set_image(const QImage &image)
{
  const bool ok = _pixmap->convertFromImage(image);
  if (ok) {
    setIcon(QIcon(*_pixmap));
  } else {
    std::stringstream msg;
    msg << "failed loading image";
    Log::warn(msg.str());
  }
  return ok;
}

void
Titled_button::set_label_font_point_size(const int size)
{
  _label_font_point_size = size;
}

const int
Titled_button::get_label_font_point_size() const
{
  return _label_font_point_size;
}

void
Titled_button::mouseMoveEvent(QMouseEvent *event)
{
  QToolButton::mouseMoveEvent(event);
  // let status line display receive events for auto hide
  event->ignore();
}

void
Titled_button::mousePressEvent(QMouseEvent *event)
{
  QToolButton::mousePressEvent(event);
  // let status line display receive events for auto hide
  event->ignore();
}

void
Titled_button::mouseReleaseEvent(QMouseEvent *event)
{
  QToolButton::mouseReleaseEvent(event);
  // let status line display receive events for auto hide
  event->ignore();
}

/*
 * Local variables:
 *   mode: c++
 *   coding: utf-8
 * End:
 */

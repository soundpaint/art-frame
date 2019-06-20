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

#include <about-dialog.hh>
#include <QtCore/QSizeF>
#include <QtCore/QUrl>
#include <log.hh>
#include <ABOUT.h>

About_dialog::About_dialog(QWidget *parent) : QDialog(parent)
{
  setWindowTitle(QString(tr("About")));
  _layout = new QVBoxLayout();
  if (!_layout) {
    Log::fatal("About_dialog::About_dialog(): not enough memory");
  }
  setLayout(_layout);

  _app_icon = new QImage();
  if (!_app_icon) {
    Log::fatal("About_dialog::About_dialog(): not enough memory");
  }
  _app_icon->load("we-paint.png");

  _about_text = new QTextEdit(this);
  if (!_about_text) {
    Log::fatal("About_dialog::About_dialog(): not enough memory");
  }
  _about_text->setReadOnly(true);
  _about_text->setDocumentTitle(QString(tr("About")));
  _about_text->setMinimumSize(400, 100);

  _about_document = new QTextDocument(_about_text);
  if (!_about_document) {
    Log::fatal("About_dialog::About_dialog(): not enough memory");
  }
  _about_document->addResource(QTextDocument::ImageResource,
                               QUrl("resources://app-icon.png"),
                               QVariant(*_app_icon));
  _about_document->setHtml(QString((const char *)ABOUT_html));

  _about_text->setDocument(_about_document);
  _layout->addWidget(_about_text);

  _button_box = new QDialogButtonBox(QDialogButtonBox::Ok);
  if (!_button_box) {
    Log::fatal("About_dialog::About_dialog(): not enough memory");
  }
  _layout->addWidget(_button_box);

  adjustSize();

  create_actions();
}

About_dialog::~About_dialog()
{
  // Q objects will be deleted by Qt, just set them to 0
  _action_done = 0;
  _about_document = 0;
  _about_text = 0;
  _button_box = 0;
  _app_icon = 0;
}

void
About_dialog::create_actions()
{
  connect(_button_box,
          SIGNAL(accepted()),
          this,
          SLOT(accept()));
  connect(_button_box,
          SIGNAL(rejected()),
          this,
          SLOT(reject()));
}

/*
 * Local variables:
 *   mode: c++
 *   coding: utf-8
 * End:
 */

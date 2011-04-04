// XMazed - Simple maze game using gtkmm & cairomm libraries
// Copyright (C) 2011 Tvrtko Majstorovic <tvrtkom@gmail.com>

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#ifndef XMMAINWINDOW_HH
#define XMMAINWINDOW_HH

#include <string>
#include "gtkmm.h"

#include "XMMazeWidget.hh"

const std::string gladefile = "glade/XMMainWindow.glade";

class XMMainWindow : public Gtk::Window 
{
  Glib::RefPtr<Gtk::Builder> mBuilder;
  Gtk::VBox *mMainVBox;
  Gtk::AspectFrame *mAFrame;
  Gtk::MenuItem *mQuitItem;
  Gtk::MenuItem *mNewItem;
  Gtk::MenuItem *mAboutItem;
  Gtk::RadioMenuItem *mEasyRadioItem;
  Gtk::RadioMenuItem *mMediumRadioItem;
  Gtk::RadioMenuItem *mHardRadioItem;
  Gtk::AboutDialog *mAboutDialog;

  // maze widget
  XMMazeWidget mXMaze;

  // signal handlers
  bool OnDeleteEvent(GdkEventAny *event);
  void OnQuitItemActivate();
  void OnNewItemActivate();
  void OnAboutItemActivate();
  void OnLevelRadioActivate(Level level);
  void OnFinishReached(int movenum);

public:
  XMMainWindow();
  ~XMMainWindow() { /* */ }
  void BuildUI();
};

#endif /* XMMAINWINDOW_HH */

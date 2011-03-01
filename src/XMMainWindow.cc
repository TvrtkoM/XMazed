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

#include <iostream>
#include "XMMainWindow.hh"

using namespace std;

XMMainWindow::XMMainWindow() 
  : builder(Gtk::Builder::create()), MainVBox(0L), AFrame(0L), xmmaze(HARD)
{
  this->signal_delete_event().connect
    (sigc::mem_fun(*this, &XMMainWindow::OnQuit));
  set_resizable(false);
}

void XMMainWindow::BuildUI() 
{
  builder->add_from_file(gladefile);
  builder->get_widget("MainVBox", MainVBox);
  builder->get_widget("AFrame", AFrame);
  AFrame->add(xmmaze);
  add(*MainVBox);
  show_all();
}

XMMainWindow::~XMMainWindow()
{
  delete MainVBox;
}

//
// signal handlers
//
bool XMMainWindow::OnQuit(GdkEventAny *event) 
{
  hide();
  return true;
}


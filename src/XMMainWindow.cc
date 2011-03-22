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
#include <sstream>
#include "XMMainWindow.hh"

using namespace std;

XMMainWindow::XMMainWindow() 
  : mBuilder(Gtk::Builder::create()), mMainVBox(0L), mAFrame(0L), mXMaze(HARD)
{
  this->signal_delete_event().connect
    (sigc::mem_fun(*this, &XMMainWindow::OnQuit));
  mXMaze.FinishReached().connect
    (sigc::mem_fun(*this, &XMMainWindow::OnFinishReached));
  set_resizable(false);
}

void XMMainWindow::BuildUI() 
{
  mBuilder->add_from_file(gladefile);
  mBuilder->get_widget("MainVBox", mMainVBox);
  mBuilder->get_widget("AFrame", mAFrame);
  mAFrame->add(mXMaze);
  add(*mMainVBox);
  show_all();
}

XMMainWindow::~XMMainWindow()
{
  delete mMainVBox;
}

//
// signal handlers
//
bool XMMainWindow::OnQuit(GdkEventAny *event) 
{
  hide();
  return true;
}

void XMMainWindow::OnFinishReached(int movenum)
{
  stringstream moven_in;
  moven_in << movenum;
  string msg = "You reached finish in " + moven_in.str() + " moves!";
  Gtk::MessageDialog finish_msg(*this, msg, false, Gtk::MESSAGE_INFO,
				Gtk::BUTTONS_YES_NO, true);
  finish_msg.set_secondary_text("Do you wish to play again?");
  int response = finish_msg.run();
  switch(response)
    {
    case(Gtk::RESPONSE_YES): mXMaze.New(); break;
    case(Gtk::RESPONSE_NO): mXMaze.SetBlocked(true); break;
    }
}

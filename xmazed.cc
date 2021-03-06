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

#include "XMMainWindow.hh"
#include <iostream>

using namespace std;

int main(int argc, char *argv[]) 
{

  Gtk::Main MainApp(argc, argv);

  XMMainWindow W;

  try 
    {
      W.BuildUI();
    }
  catch (const Glib::FileError &e) 
    {
      cerr << "Glade file not found. Exiting...\n";
      return 1;
    }

  MainApp.run(W);

  return 0;
}

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

#include <algorithm>
#include <stack>
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <cairomm/context.h>
#include "XMMazeWidget.hh"

//
// XMMazeLevel
//

XMMazeLevel::XMMazeLevel(Level level) 
{
  SetLevel(level);
}

void XMMazeLevel::SetLevel(Level level) 
{
  mLevel = level;
  switch(mLevel) 
    {
    case(EASY):
      mCellNumSide = 5;
      mLevelStr = "Easy";
      break;
    case(MEDIUM):
      mCellNumSide = 15;
      mLevelStr = "Medium";
      break;
    case(HARD):
      mCellNumSide = 25;
      mLevelStr = "Hard";
      break;
    }
}

inline const Glib::ustring &XMMazeLevel::GetLevelStr() const 
{
  return mLevelStr;
}

inline int XMMazeLevel::CellNum() const 
{
  return (mCellNumSide * mCellNumSide);
}

inline int XMMazeLevel::CellNumSide() const 
{
  return mCellNumSide;
}

//
// XMSquare
//

inline void XMSquare::SetX(int x)
{
  mX = x;
}

inline void XMSquare::SetY(int y)
{
  mY = y;
}

inline void XMSquare::SetPos(int x, int y)
{
  mX = x;
  mY = y;
}

inline int XMSquare::GetX() const
{
  return mX;
}

inline int XMSquare::GetY() const
{
  return mY;
}

inline void XMSquare::SetSideLen(int sidelen)
{
  mSideLen = sidelen;
}

inline int XMSquare::GetSideLen() const
{
  return mSideLen;
}

inline bool XMSquare::operator!=(const XMSquare &square) const
{
  return (mX != square.GetX() || 
	  mY != square.GetY() || 
	  mSideLen != square.GetSideLen());
}

inline bool XMSquare::operator<(const XMSquare &square) const
{
  if(mSideLen != square.GetSideLen())
    return (mSideLen < square.GetSideLen());
  else
    return (mY < square.GetY() || (mY == square.GetY() && mX < square.GetX()));
}

inline bool XMSquare::operator==(const XMSquare &square) const
{
  return (mX == square.GetX() &&
	  mY == square.GetY() &&
	  mSideLen == square.GetSideLen());
}

//
// XMDrawables
//

void XMActor::Draw(Cairo::RefPtr<Cairo::Context> cr) const
{
  // TODO: use png image as actor
  cr->save();
  cr->rectangle(mX, mY, mSideLen, mSideLen);
  cr->set_source_rgb(0.8, 0.4, 0.6);
  cr->stroke();
  cr->restore();
}

int XMMazeCell::mNumAdjusted = 0;

inline void XMMazeCell::SetWall(Wall wall, bool set)
{
  mWalls[wall] = set;
}

inline void XMMazeCell::SetWalls(bool set)
{
  SetWall(WEST, set);
  SetWall(EAST, set);
  SetWall(SOUTH, set);
  SetWall(NORTH, set);
}

inline const XMMazeCell &XMMazeCell::Set(int x, int y, int sidelen)
{
  mX = x;
  mY = y;
  mSideLen = sidelen;
  return *this;
}

inline bool XMMazeCell::IsWall(Wall wall) const
{
  return mWalls.find(wall)->second;
}

inline int XMMazeCell::Adjust()
{
  mAdjusted = true;
  return ++mNumAdjusted;
}

std::vector<Wall> XMMazeCell::Walls() const
{
  std::vector<Wall> walls;
  std::map<Wall, bool>::const_iterator it;
  for(it = mWalls.begin(); it != mWalls.end(); ++it)
    {
      if((*it).second)
	walls.push_back((*it).first);
    }
  return walls;
}

inline void XMMazeCell::DrawLine(int x1, int y1, int x2, int y2, 
			  Cairo::RefPtr<Cairo::Context> cr) const
{
  cr->move_to(x1, y1);
  cr->line_to(x2, y2);
}

void XMMazeCell::Draw(Cairo::RefPtr<Cairo::Context> cr) const
{
  int x = mX;
  int y = mY;
  int sl = mSideLen;
  cr->save();
  cr->set_line_width(2.0);
  cr->set_source_rgb(0.0, 0.0, 0.0);
  if(IsWall(WEST) == true)
    DrawLine(x, y, x, y + sl, cr);
  if(IsWall(EAST) == true)
    DrawLine(x + sl, y, x + sl, y + sl, cr);
  if(IsWall(SOUTH) == true)
    DrawLine(x, y + sl, x + sl, y + sl, cr);
  if(IsWall(NORTH) == true)
    DrawLine(x, y, x + sl, y, cr);
  cr->stroke();
  cr->restore();
}

//
// XMMazeWidget
//

XMMazeWidget::XMMazeWidget(Level level) 
  : XMMazeLevel(level), mMoveNum(0), mNewMaze(true), mLen(600)
{
  mSideLen = mLen/CellNumSide();
  mA.SetSideLen(mSideLen);

  add_events(Gdk::KEY_PRESS_MASK);
  set_size_request(mLen, mLen);
  set_can_focus(true);
  signal_key_press_event().connect
    (sigc::mem_fun(*this, &XMMazeWidget::OnKeyPress));
}

bool XMMazeWidget::on_expose_event(GdkEventExpose *event) 
{
  Glib::RefPtr<Gdk::Window> window = get_window();
  if(window) 
    {
      Gdk::Rectangle allocation = get_allocation();

      if(mMoveNum == 0)
	mA.SetPos(event->area.x, event->area.y);

      Cairo::RefPtr<Cairo::Context> cr = window->create_cairo_context();

      if(mNewMaze)
	BuildMaze();

      std::set<XMMazeCell>::iterator it;
      for(it = mCells.begin(); it != mCells.end(); ++it)
	  it->Draw(cr);

      mA.Draw(cr);
    }
  return true;
}

// used in BuildMaze func for debugging 
void print_walls(const std::vector<Wall> &w)
{
  std::vector<Wall>::const_iterator it;
  for(it = w.begin(); it != w.end(); ++it)
    {
      switch(*it)
	{
	case EAST: std::cout << "EAST"; break;
	case WEST: std::cout << "WEST"; break;
	case SOUTH: std::cout << "SOUTH"; break;
	case NORTH: std::cout << "NORTH"; break;
	}
      std::cout << " ";
    }
  std::cout << std::endl;
}

bool XMMazeWidget::OnKeyPress(GdkEventKey *event)
{
  int curr_x = mA.GetX();
  int curr_y = mA.GetY();
  // keys defined in "gdk/gdkkeysyms.h"
  // if key is right and position not on far right
  if(event->keyval == 0xff53 && (curr_x < (mLen - mSideLen - 1)))
    {
      mA.SetX(curr_x + mSideLen);
      mMoveNum++;
    }
  // if key is left and position not on far left
  if(event->keyval == 0xff51 && (curr_x > 0))
    {
      mA.SetX(curr_x - mSideLen);
      mMoveNum++;
    }
  // if key is down and position not bottom
  if(event->keyval == 0xff54 && (curr_y < (mLen - mSideLen - 1)))
    {
      mA.SetY(curr_y + mSideLen);
      mMoveNum++;
    }
  // if key is up and position not up
  if(event->keyval == 0xff52 && (curr_y > 0)) 
    {
      mA.SetY(curr_y - mSideLen);
      mMoveNum++;
    }
  queue_draw();
  return true;
}

// build maze using recursive backtracker technique
void XMMazeWidget::BuildMaze()
{
  int adjusted = 0;
  std::stack<XMMazeCell> cell_stack;
  XMMazeCell next_cell;
  std::srand(time(0L));
  while(adjusted != CellNum() - 1)
    {
      XMMazeCell curr_cell = next_cell;
      std::vector<Wall> exits = curr_cell.Walls();
      int end = mLen - mSideLen;
      int x = curr_cell.GetX(); int y = curr_cell.GetY();
      // if((y > mLen - mSideLen || x > mLen - mSideLen) ||
      // (y < 0 || x < 0))
      // throw std::runtime_error("x or y out of range\n");
      Wall carve;
      XMMazeCell test_cell;
      if(x == 0 || 
	 mCells.find(test_cell.Set(x - mSideLen, y, mSideLen)) != mCells.end())
	exits.erase(std::remove(exits.begin(), exits.end(), WEST), exits.end());
      if(y == 0 || 
	 mCells.find(test_cell.Set(x, y - mSideLen, mSideLen)) != mCells.end())
	exits.erase(std::remove(exits.begin(), exits.end(), NORTH), exits.end());
      if(x == end ||
	 mCells.find(test_cell.Set(x + mSideLen, y, mSideLen)) != mCells.end())
	exits.erase(std::remove(exits.begin(), exits.end(), EAST), exits.end());
      if(y == end ||
	 mCells.find(test_cell.Set(x, y + mSideLen, mSideLen)) != mCells.end())
	exits.erase(std::remove(exits.begin(), exits.end(), SOUTH), exits.end());
      //std::cout << "possible exits: " << exits.size() << std::endl;
      //std::cout << " -> ";
      //print_walls(exits);
      if(exits.size() != 0)
	{
	  int c = std::rand() % exits.size();
	  carve = exits[c];
	  curr_cell.SetWall(carve, false);
	  //print_walls(curr_cell.Walls());
	  curr_cell.SetSideLen(mSideLen);
	  cell_stack.push(curr_cell);
	  Wall nextw;
	  if(carve == WEST || carve == SOUTH)
	    nextw = (Wall)(carve + 1);
	  else
	    nextw = (Wall)(carve - 1);
	  int next_x = x, next_y = y;
	  switch(nextw)
	    {
	    case(WEST):
	      next_x = x + mSideLen; break;
	    case(EAST):
	      next_x = x - mSideLen; break;
	    case(SOUTH):
	      next_y = y - mSideLen; break;
	    case(NORTH):
	      next_y = y + mSideLen; break;
	    }
	  next_cell.SetWalls(true);
	  next_cell.SetWall(nextw, false);
	  //print_walls(next_cell.Walls());
	  next_cell.SetPos(next_x, next_y);
	}
      else
	{
	  adjusted = curr_cell.Adjust();
	  curr_cell.SetSideLen(mSideLen);
	  next_cell = cell_stack.top();
	  cell_stack.pop();
	}
      std::pair<std::set<XMMazeCell>::iterator, bool> ret;
      ret = mCells.insert(curr_cell);
      if(!ret.second) 
	{
	  mCells.erase(ret.first);
	  mCells.insert(curr_cell);
	}
    };
  mNewMaze = false;
}

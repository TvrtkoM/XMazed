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

#ifndef XMMAZEWIDGET_HH
#define XMMAZEWIDGET_HH

#include <gtkmm/drawingarea.h>
#include <glibmm/ustring.h>
#include <vector>
#include <set>
#include <map>

enum Wall 
{
  WEST, EAST, SOUTH, NORTH
};

enum Level 
{
  EASY, MEDIUM, HARD
};

//
// XMMazeLevel
//

class XMMazeLevel 
{
private:
  Level mLevel;
  int mCellNumSide; // cell number on one side of the maze.
  Glib::ustring mLevelStr;
public:
  XMMazeLevel(Level level = EASY);
  void SetLevel(Level level);
  const Glib::ustring &GetLevelStr() const;
  int CellNumSide() const;
  int CellNum() const;
};

//
// XMSquare
//

class XMSquare
{
protected:
  int mX, mY;
  int mSideLen;
public:
  XMSquare(int x = 0, int y = 0, int sidelen = 0) 
    : mX(x), mY(y), mSideLen(sidelen) { /* */ }
  void SetX(int x);
  void SetY(int y);
  void SetPos(int x, int y);
  int GetX() const;
  int GetY() const;
  void SetSideLen(int sidelen);
  int GetSideLen() const;
  bool operator!=(const XMSquare &square) const;
  bool operator<(const XMSquare &square) const;
  bool operator==(const XMSquare &square) const;
};

//
// XMDrawables
//

class XMDrawable 
{
public:
  virtual void Draw(Cairo::RefPtr<Cairo::Context> cr) const = 0;
  virtual ~XMDrawable() { /* */ }
};

class XMActor : public XMDrawable, public XMSquare
{
private:
public:
  XMActor(int x = 0, int y = 0, int sidelen = 0)
    : XMSquare(x, y, sidelen) { /* */ }
  void Draw(Cairo::RefPtr<Cairo::Context> cr) const;
};

class XMMazeCell : public XMDrawable, public XMSquare 
{
private:
  bool mAdjusted;
  static int mNumAdjusted;
  std::map<Wall, bool> mWalls;
  void DrawLine(int x1, int y1, int x2, int y2, 
                Cairo::RefPtr<Cairo::Context> cr) const;
public:
  XMMazeCell(int x = 0, int y = 0, int sidelen = 0)
    : XMSquare(x, y, sidelen), mAdjusted(false)
  {
    SetWalls(true);
  }
  void SetWall(Wall wall, bool set);
  void SetWalls(bool set);
  bool IsWall(Wall wall) const;
  int Adjust();
  std::vector<Wall> Walls() const;
  void Draw(Cairo::RefPtr<Cairo::Context> cr) const;
};

//
// XMMazeWidget
//

class XMMazeWidget : public Gtk::DrawingArea, public XMMazeLevel 
{
private:
  XMActor mA;
  std::set<XMMazeCell> mCells;
  int mLen, mSideLen;
  int mMoveNum;
  bool mNewMaze;
public:
  XMMazeWidget(Level level = EASY);
  ~XMMazeWidget() { /* */ };
protected:
  // overriding default signal handler
  virtual bool on_expose_event(GdkEventExpose *event);
  virtual bool OnKeyPress(GdkEventKey *event);
private:
  void BuildMaze();
};

#endif /* XMMAZEWIDGET_HH */

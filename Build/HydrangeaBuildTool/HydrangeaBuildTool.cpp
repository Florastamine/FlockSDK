//
// Copyright (c) 2008-2017 Flock SDK developers & contributors. 
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
// 

// g++ -o HydrangeaBuildTool HydrangeaBuildTool.cpp -lfltk -Wall -Ofast -fno-exceptions && ./HydrangeaBuildTool

#include <stdio.h>			// fprintf()
#include <stdlib.h>			// exit()
#include <string.h>			// strcmp()
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Menu_Bar.H>
#include <FL/filename.H>		// fl_open_uri()

// This callback is invoked whenever the user clicks an item in the menu bar
static void MyMenuCallback(Fl_Widget *w, void *) {
  Fl_Menu_Bar *bar = (Fl_Menu_Bar*)w;				// Get the menubar widget
  const Fl_Menu_Item *item = bar->mvalue();			// Get the menu item that was picked

  char ipath[256]; bar->item_pathname(ipath, sizeof(ipath));	// Get full pathname of picked item

  fprintf(stderr, "callback: You picked '%s'", item->label());	// Print item picked
  fprintf(stderr, ", item_pathname() is '%s'", ipath);		// ..and full pathname

  if ( item->flags & (FL_MENU_RADIO|FL_MENU_TOGGLE) ) {		// Toggle or radio item?
    fprintf(stderr, ", value is %s", item->value()?"on":"off");	// Print item's value
  }
  fprintf(stderr, "\n");
  if ( strcmp(item->label(), "Google") == 0 ) { fl_open_uri("http://google.com/"); }
  if ( strcmp(item->label(), "&Quit") == 0 ) { exit(0); }
}

int main() {
  // Fl::scheme("gtk+");
  Fl::scheme("plastic");
  // Fl::scheme("none");
  Fl_Window *win = new Fl_Window(400,200, "menubar-simple");	// Create window
  Fl_Menu_Bar *menu = new Fl_Menu_Bar(0,0,400,25);		// Create menubar, items..
  menu->add("&File/&Open",  "^o", MyMenuCallback);
  menu->add("&File/&Save",  "^s", MyMenuCallback, 0, FL_MENU_DIVIDER);
  menu->add("&File/&Quit",  "^q", MyMenuCallback);
  menu->add("&Edit/&Copy",  "^c", MyMenuCallback);
  menu->add("&Edit/&Paste", "^v", MyMenuCallback, 0, FL_MENU_DIVIDER);
  menu->add("&Edit/Radio 1",   0, MyMenuCallback, 0, FL_MENU_RADIO);
  menu->add("&Edit/Radio 2",   0, MyMenuCallback, 0, FL_MENU_RADIO|FL_MENU_DIVIDER);
  menu->add("&Edit/Toggle 1",  0, MyMenuCallback, 0, FL_MENU_TOGGLE);			// Default: off 
  menu->add("&Edit/Toggle 2",  0, MyMenuCallback, 0, FL_MENU_TOGGLE);			// Default: off
  menu->add("&Edit/Toggle 3",  0, MyMenuCallback, 0, FL_MENU_TOGGLE|FL_MENU_VALUE);	// Default: on
  menu->add("&Help/Google",    0, MyMenuCallback);

  // Example: show how we can dynamically change the state of item Toggle #2 (turn it 'on')
  {
      Fl_Menu_Item *item = (Fl_Menu_Item*)menu->find_item("&Edit/Toggle 2");	// Find item
      if ( item ) item->set();							// Turn it on
      else fprintf(stderr, "'Toggle 2' item not found?!\n");			// (optional) Not found? complain!
  }

  win->end();
  win->show();
  return(Fl::run());
}

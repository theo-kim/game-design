#ifndef MENU_DEF
#define MENU_DEF

#include "standard.h"
#include "Entity.h"

class Menu {
public:
  class MenuItem;
  
  Menu();
  Menu(int numOptions);

  void AddItem(std::string label);
  int GetClicked() const; // Gets what items (by index) were clicked

  class MenuItem {
  public:
    MenuItem(std::string _label, );
    
  }
};

#endif

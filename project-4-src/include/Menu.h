#ifndef MENU_DEF
#define MENU_DEF

#include "standard.h"
#include "Entity.h"
#include "Text.h"

class Menu {
public:
  class MenuItem;
  
  Menu();
  Menu(int numOptions, glm::vec3 pos, glm::vec3 fontSize, Text::Alignment alignment);
  ~Menu();

  void AddItem(std::string label);
  int GetClicked() const; // Gets what items (by index) were clicked

  class MenuItem {
  public:
    enum State { IDLE, CLICKED, HOVER };
    
    MenuItem(std::string _label, Text::Alignment alignment);

    void Update(State newState);
    void Render();
    State GetState() const;
  private:
    Text text;
    int state;
  };

private:
  std::vector<MenuItem *> items;
  glm::vec3 pos;
};

#endif

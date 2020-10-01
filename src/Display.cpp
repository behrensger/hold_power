#include "Display.h"


Display::~Display()
{
  //dtor
}

Display::DisplayMode Display::getdisplayMode() const
{
  return displayMode;
}

void Display::setDisplayMode(const Display::DisplayMode val)
{
  displayMode = val;
}

unsigned long Display::getValue()const {
  return value;
};
void Display::setValue(const unsigned long v) {
  value = v;
};

Display& Display::operator=( const Display& other ) {
  displayMode = other.displayMode;
  value = other.value;
  return *this;
}

bool Display::operator==( const Display& other ) {
  return (displayMode == other.displayMode && value == other.value);
}
bool Display::operator!=( const Display& other ) {
  return (displayMode != other.displayMode || value != other.value);
}

/*
  std::ostream& operator<<(std::ostream& os, const Display& disp)
  {
    unsigned long a;
    unsigned long b;
    switch(disp.getdisplayMode())
    {
    case Display::SHOW_USAGEMODE:
        os << "Disp(SHOW_USAGEMODE): " << disp.getValue();
        break;
    case Display::SHOW_VALUE:
        os << "Disp(SHOW_VALUE): " << disp.getValue();
        break;
    case Display::SHOW_MIN_SEC:
         a = disp.getValue() / 60;
         b = disp.getValue() % 60;
        os << "Disp(SHOW_MIN_SEC): " << a << ":"<<b;
        break;
    case Display::SHOW_SEC_HUN:
        a = disp.getValue() / 1000;
        b = (disp.getValue() % 1000)/10;
        os << "Disp(SHOW_SEC_HUN): " << a << ":"<<b;
        break;
    default:
        os << "Disp(Unknown): " << disp.getValue();
        break;
    }
    return os;
  }
*/

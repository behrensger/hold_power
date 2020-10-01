#ifndef DISPLAY_H
#define DISPLAY_H
//#include <iostream>



class Display
{
  public:
    typedef enum
    {
      SHOW_USAGEMODE = 0,
      SHOW_MIN_SEC = 1,
      SHOW_SEC_HUN = 2,
      SHOW_VALUE = 3,
      SHOW_CLEAR = 4,
      SHOW_SUCCESS_ROUND = 5
    } DisplayMode;

  private:
    DisplayMode displayMode;
    unsigned long value;


  public:
    Display(): displayMode(SHOW_USAGEMODE), value(0) {};
    Display(const Display& other): displayMode(other.displayMode),
      value(other.value) {};
    virtual ~Display();

    DisplayMode getdisplayMode() const;
    void setDisplayMode(const DisplayMode);

    unsigned long getValue()const;
    void setValue(const unsigned long);

    Display& operator=( const Display& other );
    bool operator==( const Display& other );
    bool operator!=( const Display& other );
};
//std::ostream& operator<<(std::ostream&, const Display&);

#endif // DISPLAY_H

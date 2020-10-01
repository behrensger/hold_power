#ifndef LED_H
#define LED_H

class LED
{
  public:


  private:
    bool led_status;
    LED(const LED&) = delete;//no copy constructor


  public:

    //default constructor
    LED(): led_status(false) {};
    LED(bool status): led_status(status) {};

    virtual ~LED() {};

    //Getter / Setter
    bool getStatus(void)const
    {
      return led_status;
    };
    void setStatus(const bool s)
    {
      led_status = s;
    };
};

#endif // LED_H

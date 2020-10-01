#ifndef HELPFULL_H
#define HELPFULL_H


class Helper
{

  private:

    //default constructor
    Helper() {}
    virtual ~Helper() {};

  public:
    static int constraint(const int min, const int val, const int max);
    static int maps(const int value, const int fromLow, const int fromHigh, const int toLow, const int toHigh);
    static int maps(const int value, const int fromLow, const int fromHigh, const int toLow, const int toHigh, const int steps);
    //static double round(double d);
    static int roundUp(int numToRound, int multiple)  ;

};

#endif // HOLDER_H

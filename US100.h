//*********************************************************
//-- US100: Library(header file)
//*********************************************************
#ifndef US100_H
#define US100_H
class US100{
  public:
    void init(int trig,int echo);
    float getDistance();
  private:
    int _trig;
    int _echo;
};
#endif

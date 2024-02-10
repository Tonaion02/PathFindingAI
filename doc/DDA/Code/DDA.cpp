#include <iostream>
#include <math.h>

using namespace std;





int main()
{
  float x;
  float y;
  float x1, y1;
  float x2, y2, dx, dy, step;
  
  cout << "Enter the value of x1 and y1: ";
  cin >> x1 >> y1;
  cout << "Enter the value of x2 and y2: ";
  cin >> x2 >> y2;
  
  dx = (x2 - x1);
  dy = (y2 - y1);
  if (abs(dx) >= abs(dy))
    step = abs(dx);
  else
    step = abs(dy);
  dx = dx / step;
  dy = dy / step;
  x = x1;
  y = y1;
  int i = 0;
  while (i < step) {
    x = x + dx;
    y = y + dy;
    i = i + 1;
    //delay(100);
    cout << "i: " << i << " ( " << x << ", " << y << " )" << endl;
  }
  
  return 0;
}

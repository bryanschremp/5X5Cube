#include "5x5Cube/5x5Cube.h"

Cube cube = Cube();

void setup()
{
  cube.begin();
  cube.background(black);
}

void loop()
{
  cube.setVoxel(3,3,3,red);
  cube.show();
  delay(500);
  cube.background(black);
  cube.show();
  delay(500);
}

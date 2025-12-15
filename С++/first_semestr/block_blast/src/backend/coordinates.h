#ifndef COORDINATES_H
#define COORDINATES_H

struct Coordinates {
  int x;
  int y;
  Coordinates() : x(0), y(0) {} 
  Coordinates(int xx, int yy) : x(xx), y(yy) {}
  Coordinates operator+(const Coordinates& rhs) {
    return Coordinates(x + rhs.x, y + rhs.y);
  }

  bool operator==(const Coordinates& rhs) const {
    return x == rhs.x && y == rhs.y;
  }
};

#endif //COORDINATES_H
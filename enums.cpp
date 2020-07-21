#include "enums.h"

StoneColor opposite_color(StoneColor color) {
   return (StoneColor)(1 - (int)color);
}

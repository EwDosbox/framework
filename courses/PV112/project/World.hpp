#include "headers.cpp";
#include <glm/fwd.hpp>
using namespace glm;

class World {
private:
  vec3 bounds;

public:
  vec3 GetBounds();
};
#ifndef RAYCASTING_H
#define RAYCASTING_H

#include <glm/glm.hpp>

class Raycasting
{
public:
	static bool RaycastAABB(const glm::vec3& rayOrigin, const glm::vec3& rayDirection, const glm::vec3& aabbMin, const glm::vec3& aabbMax, float& tmin);
};
#endif // !RAYCASTING_H


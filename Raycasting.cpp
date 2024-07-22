#include "Raycasting.h"

// Raycast against an AABB
								// Ray Starting Point		// Ray Direction Vector		// Bottom-left AABB corner	// Top-right AABB corner  
bool Raycasting::RaycastAABB(const glm::vec3& rayOrigin, const glm::vec3& rayDirection, const glm::vec3& aabbMin, const glm::vec3& aabbMax, float& t)	//distance from ray origin to the intersection point, if an intersection occurs.
{
	/* Inverse of the ray direction. Used to transform ray direction into a format that's easier to work with for the intersection calculation.*/
	glm::vec3 invDir = 1.0f / rayDirection; 

	/* Intersection Distances to the Quads.*/
	/*Computes the distances to the near and far planes of the AABB. Checks how far along the ray we need to travel to reach the boundaries of the AABB*/
	glm::vec3 t0s = (aabbMin - rayOrigin) * invDir;		
	glm::vec3 t1s = (aabbMax - rayOrigin) * invDir;	    //t0s and t1s are the distances to the near and far planes of the AABB.

	/* Calculate the minimum distances(entry points) and maximum distances (exit points) for each axis. 
		Used to determine which planes of the AABB the ray enters and exits first. */
	glm::vec3 tsmaller = glm::min(t0s, t1s);		// Get the smallest t value
	glm::vec3 tbigger = glm::max(t0s, t1s);			// Get the largest t value

	/* Calculate the overall entry and exit points of the ray */
	float tmin = glm::max(glm::max(tsmaller.x, tsmaller.y), tsmaller.z); // Get the maximum of the smallest t values
	float tmax = glm::min(glm::min(tbigger.x, tbigger.y), tbigger.z);	// Get the minimum of the largest t values

	
	/* Checks for valid Ray intersection */
	if (tmax >= tmin && tmax >= 0.0f)
	{
		t = tmin;
		return true;
	}
	else
	return false;
}
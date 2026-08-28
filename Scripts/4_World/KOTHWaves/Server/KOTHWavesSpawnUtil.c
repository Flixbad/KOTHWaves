class KOTHWavesSpawnUtil
{
	static bool TryGetSpawnPosition(vector center, float minRadius, float maxRadius, bool outdoorOnly, out vector outPosition, int maxAttempts = 16)
	{
		vector bestFallback = center;
		bestFallback[1] = GetGame().SurfaceY(bestFallback[0], bestFallback[2]);
		outPosition = bestFallback;
		
		for (int attempt = 0; attempt < maxAttempts; attempt++)
		{
			float angle = Math.RandomFloatInclusive(0.0, 6.283185);
			float radius = Math.RandomFloatInclusive(minRadius, maxRadius);
			vector candidate = Vector(center[0] + Math.Cos(angle) * radius, center[1], center[2] + Math.Sin(angle) * radius);
			candidate[1] = GetGame().SurfaceY(candidate[0], candidate[2]);
			
			if (!outdoorOnly || IsOutdoorSpawnPosition(candidate))
			{
				outPosition = candidate;
				return true;
			}
		}
		
		return !outdoorOnly;
	}
	
	static bool IsOutdoorSpawnPosition(vector position)
	{
		float surfaceY = GetGame().SurfaceY(position[0], position[2]);
		position[1] = surfaceY;
		
		vector rayStart = position;
		rayStart[1] = surfaceY + 40.0;
		vector rayEnd = position;
		rayEnd[1] = surfaceY + 0.2;
		
		vector hitPos;
		vector hitNormal;
		int contactComponent;
		set<Object> hitObjects = new set<Object>;
		
		if (DayZPhysics.RaycastRV(rayStart, rayEnd, hitPos, hitNormal, contactComponent, hitObjects, null, null, false, true, ObjIntersectGeom, 0.25))
		{
			if (hitPos[1] > surfaceY + 2.5)
			{
				return false;
			}
		}
		
		vector lowStart = position;
		lowStart[1] = surfaceY + 3.0;
		vector lowEnd = position;
		lowEnd[1] = surfaceY + 0.2;
		
		hitObjects.Clear();
		
		if (DayZPhysics.RaycastRV(lowStart, lowEnd, hitPos, hitNormal, contactComponent, hitObjects, null, null, false, true, ObjIntersectGeom, 0.25))
		{
			if (hitPos[1] > surfaceY + 1.8)
			{
				return false;
			}
		}
		
		return true;
	}
}

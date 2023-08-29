#pragma once

#include "CoreMinimal.h"

#define DRAW_DEBUG_SPHERE(Location) if (GetWorld()) DrawDebugSphere(GetWorld(), Location, 15.F, 15, FColor::Red, true)
#define DRAW_DEBUG_LINE(Start, End) if (GetWorld()) DrawDebugLine(GetWorld(), Start, End, FColor::Red, true)
#define DRAW_DEBUG_POINT(Location) if(GetWorld()) DrawDebugPoint(GetWorld(), Location, 10, FColor::Red, true)
#define DRAW_DEBUG_SPHERE_SingleFrame(Location) if (GetWorld()) DrawDebugSphere(GetWorld(), Location, 15.F, 15, FColor::Red, false, -1, 0)
#define DRAW_DEBUG_LINE_SingleFrame(Start, End) if (GetWorld()) DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, -1, 0)
#define DRAW_DEBUG_POINT_SingleFrame(Location) if(GetWorld()) DrawDebugPoint(GetWorld(), Location, 10, FColor::Red, false, -1, 0)
#define DRAW_DEBUG_VECTOR(Start, End) if (GetWorld()) \
	{ \
		DRAW_DEBUG_LINE_SingleFrame(Start, End);\
		DRAW_DEBUG_POINT_SingleFrame(End);\
	}


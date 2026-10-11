//====== Copyright © 1996-2005, Valve Corporation, All rights reserved. =======
//
// Purpose: The CS2 nav mesh area.
//
//=============================================================================

#ifndef NAVAREA_H
#define NAVAREA_H
#ifdef _WIN32
#pragma once
#endif

#include <mathlib/vector.h>

#include <cstdint>
#include <vector>

// CS2 rewrote the nav mesh, so hl2sdk's game/server/nav_area.h -- which is
// Source 1 code and describes a CNavArea starting with m_nwCorner at offset 0
// -- does not describe what the game actually has in memory. Nothing in any SDK
// we build against declares this class, so the layout below is fixed by
// observation, the same offsets CounterStrikeSharp reads and the same
// CSource2Server::GetNavMeshData signature is used to find the mesh.
//
// Nothing here is schema-backed. If the game moves a field, every accessor
// silently returns rubbish -- there is no check that can catch it.
//
//   0x00  vtable
//   0x08  uint32   id
//   0x0c  Vector   center
//   0x18  Vector   surface normal
//   0x24  Vector   one corner
//   0x30  Vector   the opposite corner
//
// The two corners are not sorted, hence Min()/Max() rather than raw accessors.
class CCSNavArea
{
public:
	// A view onto memory the game owns, reached only through a pointer the
	// game handed out. Constructing or copying one would produce something
	// whose accessors read whatever happens to follow it on the stack.
	CCSNavArea() = delete;
	~CCSNavArea() = delete;
	CCSNavArea(const CCSNavArea&) = delete;
	CCSNavArea& operator=(const CCSNavArea&) = delete;

	/// <summary>Nav area identifier.</summary>
	uint32_t Id() const;

	/// <summary>Centre of the area.</summary>
	const Vector& Center() const;

	/// <summary>Surface normal, which is what gives the area its slope.</summary>
	const Vector& Normal() const;

	/// <summary>Corner with the smaller components on every axis.</summary>
	Vector Min() const;

	/// <summary>Corner with the larger components on every axis.</summary>
	Vector Max() const;

	/// <summary>Extent along X.</summary>
	float Width() const;

	/// <summary>Extent along Y.</summary>
	float Height() const;

	/// <summary>Width times height; the area ignoring slope.</summary>
	float Area2D() const;

	/// <summary>Height of the area's surface directly under a world position.</summary>
	float GetHeightAtPosition(float x, float y) const;

	/// <summary>Whether a position sits on this area, within flZTolerance of its surface.</summary>
	bool ContainsPoint(const Vector& vecPosition, float flZTolerance = 32.0f) const;

	/// <summary>Whether a box sits entirely on this area.</summary>
	bool ContainsBox(const Vector& vecMins, const Vector& vecMaxs, float flZTolerance = 32.0f) const;

	/// <summary>Whether a box overlaps this area at all.</summary>
	bool IntersectsBox(const Vector& vecMins, const Vector& vecMaxs, float flZTolerance = 32.0f) const;

	/// <summary>Point of this area nearest a world position.</summary>
	Vector GetClosestPoint(const Vector& vecPosition) const;

	/// <summary>Distance from a world position to this area.</summary>
	float GetDistanceToPoint(const Vector& vecPosition) const;

	/// <summary>Squared distance, for comparing without the square root.</summary>
	float GetDistanceSquaredToPoint(const Vector& vecPosition) const;

	/* =========================
	The mesh
	========================= */

	/// <summary>Every area of the current nav mesh, or empty when there is none.</summary>
	static std::vector<CCSNavArea*> GetAllNavAreas();

	/// <summary>Area nearest a position, or null when the mesh is missing or nothing is in range.</summary>
	/// <param name="flMaximumDistance">Search limit; zero or less means no limit.</param>
	static CCSNavArea* GetClosestNavArea(const Vector& vecPosition, float flMaximumDistance = -1.0f);

	/// <summary>As above, and writes the distance it found to pflDistanceOut.</summary>
	static CCSNavArea* GetClosestNavArea(const Vector& vecPosition, float* pflDistanceOut,
										 float flMaximumDistance = -1.0f);

	/// <summary>Whether the game has a nav mesh loaded.</summary>
	static bool IsValidNavMesh();
};

#endif // NAVAREA_H

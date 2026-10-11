//====== Copyright © 1996-2005, Valve Corporation, All rights reserved. =======
//
// Purpose: CCSNavArea -- see the layout note in schema/navarea.h.
//
//=============================================================================

#include "source2toolkit/schema/navarea.h"

#include "source2toolkit/IToolkitApi.h"
#include "source2toolkit/IToolkitGameConfig.h"
#include "source2toolkit/IToolkitPlugin.h"

#include <algorithm>
#include <cmath>
#include <limits>
TOOLKIT_GLOBALVARS();

// The includes above drag in windows.h, whose min/max macros would eat every
// std::min/std::max and numeric_limits<>::max() below.
#ifdef _WIN32
#undef min
#undef max
#endif

namespace
{
	// Field offsets, from the note in the header.
	constexpr std::size_t kOffsetId = 0x08;
	constexpr std::size_t kOffsetCenter = 0x0c;
	constexpr std::size_t kOffsetNormal = 0x18;
	constexpr std::size_t kOffsetCornerA = 0x24;
	constexpr std::size_t kOffsetCornerB = 0x30;

	// The mesh object: a count and the array of area pointers.
	constexpr std::size_t kOffsetMeshCount = 0x08;
	constexpr std::size_t kOffsetMeshAreas = 0x10;

	template <typename T>
	const T& FieldAt(const void* pBase, std::size_t nOffset)
	{
		return *reinterpret_cast<const T*>(static_cast<const uint8_t*>(pBase) + nOffset);
	}

	/// The nav mesh pointer lives in a global that CSource2Server::GetNavMeshData
	/// reads through RIP-relative addressing. The signature lands on that
	/// instruction: bytes 3..6 are the displacement, and it is relative to the
	/// end of the instruction -- 7 bytes on Linux (lea rax, [rip+disp]), 8 on
	/// Windows (cmp qword ptr [rip+disp], 0).
	///
	/// The address of the global does not change, so it is resolved once; the
	/// value behind it changes with every map, so it is read every time.
	void** ResolveNavMeshGlobal()
	{
		static void** s_ppNavMesh = nullptr;
		static bool s_bResolved = false;

		if (s_bResolved)
			return s_ppNavMesh;

		s_bResolved = true;

		void* pFunction = g_pToolkitGameConfig
			? g_pToolkitGameConfig->ResolveSignature("CSource2Server::GetNavMeshData")
			: nullptr;
		if (!pFunction)
			return nullptr;

		auto* pBytes = static_cast<uint8_t*>(pFunction);
		const int32_t nDisplacement = FieldAt<int32_t>(pBytes, 3);

#ifdef _WIN32
		constexpr int nInstructionLength = 8;
#else
		constexpr int nInstructionLength = 7;
#endif

		s_ppNavMesh = reinterpret_cast<void**>(pBytes + nInstructionLength + nDisplacement);
		return s_ppNavMesh;
	}

	void* GetNavMesh()
	{
		void** ppNavMesh = ResolveNavMeshGlobal();
		return ppNavMesh ? *ppNavMesh : nullptr;
	}
}

uint32_t CCSNavArea::Id() const
{
	return FieldAt<uint32_t>(this, kOffsetId);
}

const Vector& CCSNavArea::Center() const
{
	return FieldAt<Vector>(this, kOffsetCenter);
}

const Vector& CCSNavArea::Normal() const
{
	return FieldAt<Vector>(this, kOffsetNormal);
}

Vector CCSNavArea::Min() const
{
	const Vector& a = FieldAt<Vector>(this, kOffsetCornerA);
	const Vector& b = FieldAt<Vector>(this, kOffsetCornerB);

	return Vector(std::min(a.x, b.x), std::min(a.y, b.y), std::min(a.z, b.z));
}

Vector CCSNavArea::Max() const
{
	const Vector& a = FieldAt<Vector>(this, kOffsetCornerA);
	const Vector& b = FieldAt<Vector>(this, kOffsetCornerB);

	return Vector(std::max(a.x, b.x), std::max(a.y, b.y), std::max(a.z, b.z));
}

float CCSNavArea::Width() const
{
	return std::fabs(Max().x - Min().x);
}

float CCSNavArea::Height() const
{
	return std::fabs(Max().y - Min().y);
}

float CCSNavArea::Area2D() const
{
	return Width() * Height();
}

float CCSNavArea::GetHeightAtPosition(float x, float y) const
{
	const Vector& vecNormal = Normal();
	const Vector& vecCenter = Center();

	// A vertical face has no height to give; fall back to the centre.
	if (std::fabs(vecNormal.z) <= 0.0001f)
		return vecCenter.z;

	return vecCenter.z - ((vecNormal.x * (x - vecCenter.x)) + (vecNormal.y * (y - vecCenter.y))) / vecNormal.z;
}

bool CCSNavArea::ContainsPoint(const Vector& vecPosition, float flZTolerance) const
{
	const Vector vecMin = Min();
	const Vector vecMax = Max();

	return vecPosition.x >= vecMin.x && vecPosition.x <= vecMax.x
		&& vecPosition.y >= vecMin.y && vecPosition.y <= vecMax.y
		&& std::fabs(vecPosition.z - GetHeightAtPosition(vecPosition.x, vecPosition.y)) <= flZTolerance;
}

bool CCSNavArea::ContainsBox(const Vector& vecMins, const Vector& vecMaxs, float flZTolerance) const
{
	const Vector vecMin = Min();
	const Vector vecMax = Max();

	const float flBoxMinX = std::min(vecMins.x, vecMaxs.x);
	const float flBoxMaxX = std::max(vecMins.x, vecMaxs.x);
	const float flBoxMinY = std::min(vecMins.y, vecMaxs.y);
	const float flBoxMaxY = std::max(vecMins.y, vecMaxs.y);
	const float flBoxBottomZ = std::min(vecMins.z, vecMaxs.z);

	// Every corner has to sit on the surface, not just one -- a sloped area can
	// be within tolerance at one corner and far off at the opposite one.
	return flBoxMinX >= vecMin.x && flBoxMaxX <= vecMax.x
		&& flBoxMinY >= vecMin.y && flBoxMaxY <= vecMax.y
		&& std::fabs(flBoxBottomZ - GetHeightAtPosition(flBoxMinX, flBoxMinY)) <= flZTolerance
		&& std::fabs(flBoxBottomZ - GetHeightAtPosition(flBoxMinX, flBoxMaxY)) <= flZTolerance
		&& std::fabs(flBoxBottomZ - GetHeightAtPosition(flBoxMaxX, flBoxMinY)) <= flZTolerance
		&& std::fabs(flBoxBottomZ - GetHeightAtPosition(flBoxMaxX, flBoxMaxY)) <= flZTolerance;
}

bool CCSNavArea::IntersectsBox(const Vector& vecMins, const Vector& vecMaxs, float flZTolerance) const
{
	const Vector vecMin = Min();
	const Vector vecMax = Max();

	const float flBoxMinX = std::min(vecMins.x, vecMaxs.x);
	const float flBoxMaxX = std::max(vecMins.x, vecMaxs.x);
	const float flBoxMinY = std::min(vecMins.y, vecMaxs.y);
	const float flBoxMaxY = std::max(vecMins.y, vecMaxs.y);

	if (flBoxMaxX < vecMin.x || flBoxMinX > vecMax.x || flBoxMaxY < vecMin.y || flBoxMinY > vecMax.y)
		return false;

	const float x = std::clamp((flBoxMinX + flBoxMaxX) * 0.5f, vecMin.x, vecMax.x);
	const float y = std::clamp((flBoxMinY + flBoxMaxY) * 0.5f, vecMin.y, vecMax.y);
	const float z = GetHeightAtPosition(x, y);

	return z >= std::min(vecMins.z, vecMaxs.z) - flZTolerance
		&& z <= std::max(vecMins.z, vecMaxs.z) + flZTolerance;
}

Vector CCSNavArea::GetClosestPoint(const Vector& vecPosition) const
{
	const Vector vecMin = Min();
	const Vector vecMax = Max();

	const float x = std::clamp(vecPosition.x, vecMin.x, vecMax.x);
	const float y = std::clamp(vecPosition.y, vecMin.y, vecMax.y);

	return Vector(x, y, GetHeightAtPosition(x, y));
}

float CCSNavArea::GetDistanceSquaredToPoint(const Vector& vecPosition) const
{
	return (vecPosition - GetClosestPoint(vecPosition)).LengthSqr();
}

float CCSNavArea::GetDistanceToPoint(const Vector& vecPosition) const
{
	return std::sqrt(GetDistanceSquaredToPoint(vecPosition));
}

bool CCSNavArea::IsValidNavMesh()
{
	return GetNavMesh() != nullptr;
}

std::vector<CCSNavArea*> CCSNavArea::GetAllNavAreas()
{
	std::vector<CCSNavArea*> areas;

	void* pNavMesh = GetNavMesh();
	if (!pNavMesh)
		return areas;

	const int32_t nCount = FieldAt<int32_t>(pNavMesh, kOffsetMeshCount);
	auto* ppAreas = FieldAt<CCSNavArea**>(pNavMesh, kOffsetMeshAreas);

	if (nCount <= 0 || !ppAreas)
		return areas;

	areas.reserve(static_cast<std::size_t>(nCount));

	for (int32_t i = 0; i < nCount; ++i)
	{
		if (CCSNavArea* pArea = ppAreas[i])
			areas.push_back(pArea);
	}

	return areas;
}

CCSNavArea* CCSNavArea::GetClosestNavArea(const Vector& vecPosition, float* pflDistanceOut,
										  float flMaximumDistance)
{
	if (pflDistanceOut)
		*pflDistanceOut = std::numeric_limits<float>::max();

	const float flMaximumDistanceSquared = flMaximumDistance > 0.0f
		? flMaximumDistance * flMaximumDistance
		: -1.0f;

	float flClosestDistanceSquared = std::numeric_limits<float>::max();
	CCSNavArea* pClosest = nullptr;

	for (CCSNavArea* pArea : GetAllNavAreas())
	{
		const float flDistanceSquared = pArea->GetDistanceSquaredToPoint(vecPosition);

		if (flMaximumDistanceSquared > 0.0f && flDistanceSquared > flMaximumDistanceSquared)
			continue;

		if (flDistanceSquared < flClosestDistanceSquared)
		{
			flClosestDistanceSquared = flDistanceSquared;
			pClosest = pArea;
		}
	}

	if (pClosest && pflDistanceOut)
		*pflDistanceOut = std::sqrt(flClosestDistanceSquared);

	return pClosest;
}

CCSNavArea* CCSNavArea::GetClosestNavArea(const Vector& vecPosition, float flMaximumDistance)
{
	return GetClosestNavArea(vecPosition, nullptr, flMaximumDistance);
}

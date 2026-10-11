/**
* vim: set ts=4 sw=4 tw=99 noet:
 * =============================================================================
 * Source2Toolkit
 * Copyright (C) 2025-2026 Michal "Slynx (˙·٠● S l y n x ●٠·˙)" Přikryl.
 * All rights reserved.
 * =============================================================================
 *
 * This program is free software; you can redistribute it and/or modify it under
 * the terms of the GNU General Public License, version 3.0, as published by the
 * Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program. If not, see <http://www.gnu.org/licenses/>.
 *
 * As a special exception, Michal "Slynx (˙·٠● S l y n x ●٠·˙)" Přikryl
 * gives you permission to link the code of this program
 * (as well as its derivative works) to "Counter-Strike 2," "Source 2,"
 * "Steam," and any Game MODs or server software running on software by
 * Valve Corporation. You must obey the GNU General Public License in all
 * respects for all other code used.
 *
 * Additionally, this exception applies to all derivative works unless
 * otherwise stated in LICENSE.txt.
 *
 * Authors:
 *   - Michal "Slynx (˙·٠● S l y n x ●٠·˙)" Přikryl
 *
 * Project: Source2Toolkit
 */

/**

* @file IToolkitTrace.h
* @brief Interface and utilities for ray tracing and collision queries.
*
* This module provides functionality for:
* * Raycasting (line traces)
* * Hull traces (box collision)
* * Physics queries
* * Visibility and hit detection
*
* @note Commonly used for shooting, line-of-sight checks and movement.
  */

#ifndef _INCLUDE_ITOOLKIT_TRACE_H
#define _INCLUDE_ITOOLKIT_TRACE_H

#pragma once
#include "IToolkitPlugin.h"
#include "schema/navarea.h"

#include <bspflags.h>
#include <engine2/isource2server.h>
#include <engine2/ivengineserver2.h>
#include <gametrace.h>

/* =========================
Forward declarations
========================= */

/**
 * @brief The filter TraceShapeEx expects when you have an entity to ignore.
 *
 * The toolkit uses this one internally; it is here because a plugin calling
 * TraceShapeEx needs a CTraceFilter of its own and this covers the ordinary
 * case -- ignore an entity, its owner and everything sharing its hierarchy.
 */
class CTraceFilterEx : public CTraceFilter
{
public:
    // Defined out of line: CBaseEntity.h includes IToolkitPlugin.h, which
    // includes this header, so CBaseEntity is still incomplete here.
    explicit CTraceFilterEx(CBaseEntity* pEntityToIgnore);
    CTraceFilterEx();
};
class CBaseEntity;
class CEntityInstance;

/* =========================
Trace options
========================= */

/**

* @brief Options controlling trace behavior.
  */
struct TraceOptions
{
    uint64_t InteractsAs = CONTENTS_EMPTY;
    uint64_t InteractsWith = MASK_SHOT;
    uint64_t InteractsExclude = CONTENTS_EMPTY;
};

/* =========================
Trace result
========================= */

/**

* @brief Result of a trace query.
*
* Contains information about collision, hit entity,
* hit position, surface data, and more.
  */
struct TraceResult
{
    TraceResult()
    {
        m_flFraction = 1.0f;
        m_bStartInSolid = false;
        m_bExactHitPoint = false;
        m_pEnt = nullptr;
        m_pHitbox = nullptr;
        m_pSurfaceProperties = nullptr;
    }

    explicit TraceResult(const CGameTrace* pTrace)
    {
        m_pSurfaceProperties = pTrace->m_pSurfaceProperties;
        m_pEnt = pTrace->m_pEnt;
        m_pHitbox = pTrace->m_pHitbox;
        m_hBody = pTrace->m_hBody;
        m_hShape = pTrace->m_hShape;
        m_nContents = pTrace->m_nContents;
        m_BodyTransform = pTrace->m_BodyTransform;
        m_ShapeAttributes = pTrace->m_ShapeAttributes;
        m_vStartPos = pTrace->m_vStartPos;
        m_vEndPos = pTrace->m_vEndPos;
        m_vHitNormal = pTrace->m_vHitNormal;
        m_vHitPoint = pTrace->m_vHitPoint;
        m_flHitOffset = pTrace->m_flHitOffset;
        m_flFraction = pTrace->m_flFraction;
        m_nTriangle = pTrace->m_nTriangle;
        m_nHitboxBoneIndex = pTrace->m_nHitboxBoneIndex;
        m_eRayType = pTrace->m_eRayType;
        m_bStartInSolid = pTrace->m_bStartInSolid;
        m_bExactHitPoint = pTrace->m_bExactHitPoint;
    }

    TraceResult(const TraceResult&) = delete;
    TraceResult& operator=(const TraceResult&) = delete;

    TraceResult(TraceResult&&) = default;
    TraceResult& operator=(TraceResult&&) = default;

    const Vector& StartPos() const { return m_vStartPos; }
    const Vector& EndPos() const { return m_vEndPos; }
    const Vector& HitPoint() const { return m_vHitPoint; }
    const Vector& Normal() const { return m_vHitNormal; }

    float Fraction() const { return m_flFraction; }
    float HitOffset() const { return m_flHitOffset; }

    bool DidHit() const { return m_flFraction < 1.0f; }
    bool IsAllSolid() const { return m_bStartInSolid; }
    bool HasExactHit() const { return m_bExactHitPoint; }

    CEntityInstance* HitEntity() const { return m_pEnt; }

    int TriangleIndex() const { return m_nTriangle; }
    int HitboxBoneIndex() const { return m_nHitboxBoneIndex; }

    CHitBox* Hitbox() const { return const_cast<CHitBox*>(m_pHitbox); }

    int Contents() const { return m_nContents; }
    RayType_t RayType() const { return m_eRayType; }

    IPhysicsBody* Body() const { return m_hBody; }
    IPhysicsShape* Shape() const { return m_hShape; }

    const CTransform& BodyTransform() const { return m_BodyTransform; }
    const RnCollisionAttr_t& ShapeAttributes() const { return m_ShapeAttributes; }

    CPhysSurfaceProperties* Surface() const { return const_cast<CPhysSurfaceProperties*>(m_pSurfaceProperties); }

private:
    const CPhysSurfaceProperties* m_pSurfaceProperties;
    CEntityInstance* m_pEnt;
    const CHitBox* m_pHitbox;

    HPhysicsBody m_hBody;
    HPhysicsShape m_hShape;

    uint64 m_nContents; // contents on other side of surface hit

    CTransform m_BodyTransform;
    RnCollisionAttr_t m_ShapeAttributes;

    Vector m_vStartPos; // start position
    Vector m_vEndPos; // final position
    Vector m_vHitNormal; // surface normal at impact
    Vector m_vHitPoint; // exact hit point if m_bExactHitPoint is true, otherwise equal to m_vEndPos

    float m_flHitOffset; // surface normal hit offset
    float m_flFraction; // time completed, 1.0 = didn't hit anything

    int32 m_nTriangle; // the index of the triangle that was hit
    int16 m_nHitboxBoneIndex; // the index of the hitbox bone that was hit

    RayType_t m_eRayType;

    bool m_bStartInSolid; // if true, the initial point was in a solid area
    bool m_bExactHitPoint; // if true, then m_vHitPoint is the exact hit point of the query and the shape
};

/* =========================
Core Toolkit Trace
========================= */

/**

* @brief Interface for performing trace operations.
  */
#define TOOLKIT_TRACE_INTERFACE "IToolkitTrace001"

class IToolkitTrace
{
public:
    virtual ~IToolkitTrace() = default;

    /**

    * @brief Performs directional trace using angles.
      */
    virtual TraceResult TraceShape(const Vector& vecStart,
                                   const QAngle& angAngles,
                                   CBaseEntity* pIgnoreEntity,
                                   TraceOptions* pTraceOptions) = 0;

    /**

    * @brief Performs trace between two points.
      */
    virtual TraceResult TraceEndShape(const Vector& vecStart,
                                      const Vector& vecEnd,
                                      CBaseEntity* pIgnoreEntity,
                                      TraceOptions* pTraceOptions) = 0;

    /**

    * @brief Performs hull (box) trace.
      */
    virtual TraceResult TraceHullShape(const Vector& vecStart,
                                       const Vector& vecEnd,
                                       const Vector& vecMins,
                                       const Vector& vecMaxs,
                                       CBaseEntity* pIgnoreEntity,
                                       TraceOptions* pTraceOptions) = 0;

    /**

    * @brief Advanced trace with custom filter.
      */
    virtual TraceResult TraceShapeEx(const Vector& vecStart,
                                     const Vector& vecEnd,
                                     CTraceFilter* pTraceFilter,
                                     Ray_t* pRay) = 0;

    /**

    * @brief Returns the contents bitmask at a world position.
      */
    virtual uint64 PointContents(const Vector* const vTestPos,
                                 uint64 nContentsMask) = 0;

    /**

    * @brief Checks whether a nav area overlaps with an entity.
      */
    virtual bool CheckAreaOverlappingEntity(const CCSNavArea* const rArea,
                                            const CBaseEntity* const rEntity,
                                            bool bExtrudeHullHeight) = 0;

    /**

    * @brief Retrieves the world-space AABB of an entity.
      */
    virtual void GetEntityWorldSpaceAABB(const CBaseEntity* const rEntity,
                                         Vector* pMinsOut,
                                         Vector* pMaxsOut) = 0;
};

#define TRACE_SHAPE(start, angles, ent, opts) \
    g_pToolkitTrace->TraceShape(start, angles, ent, opts)

#define TRACE_END_SHAPE(start, end, ent, opts) \
    g_pToolkitTrace->TraceEndShape(start, end, ent, opts)

#define TRACE_HULL_SHAPE(start, end, mins, maxs, ent, opts) \
    g_pToolkitTrace->TraceHullShape(start, end, mins, maxs, ent, opts)

#define TRACE_SHAPE_EX(start, end, filter, ray) \
    g_pToolkitTrace->TraceShapeEx(start, end, filter, ray)

#define POINT_CONTENTS(pos, mask) \
    g_pToolkitTrace->PointContents(pos, mask)

#define ENTITY_WORLD_AABB(ent, mins, maxs) \
    g_pToolkitTrace->GetEntityWorldSpaceAABB(ent, mins, maxs)

#define CHECK_AREA_OVERLAPPING_ENTITY(area, ent, extrudeHull) \
    g_pToolkitTrace->CheckAreaOverlappingEntity(area, ent, extrudeHull)

#endif //_INCLUDE_ITOOLKIT_TRACE_H

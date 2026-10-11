//====== Copyright © 1996-2005, Valve Corporation, All rights reserved. =======
//
// Purpose: CTraceFilterEx, out of line because of the include cycle noted in
//          IToolkitTrace.h.
//
//=============================================================================

#include "source2toolkit/IToolkitTrace.h"
#include "source2toolkit/schema/entity/classes/CBaseEntity.h"
#include "source2toolkit/schema/entity/classes/CCollisionProperty.h"

CTraceFilterEx::CTraceFilterEx(CBaseEntity* pEntityToIgnore)
    : CTraceFilter(reinterpret_cast<CEntityInstance*>(pEntityToIgnore),
                   pEntityToIgnore ? pEntityToIgnore->m_hOwnerEntity.Get() : nullptr,
                   pEntityToIgnore
                       ? pEntityToIgnore->m_pCollision()->m_collisionAttribute().m_nHierarchyId()
                       : static_cast<uint16>(0xFFFFFFFF),
                   MASK_ALL,
                   COLLISION_GROUP_DEFAULT, true)
{
}

CTraceFilterEx::CTraceFilterEx()
    : CTraceFilter(MASK_ALL, COLLISION_GROUP_DEFAULT, true)
{
}

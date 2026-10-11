/**
* vim: set ts=4 sw=4 tw=99 noet:
 * =============================================================================
 * Source2Toolkit
 * Copyright (C) 2025-2026 Michal "Slynx (˙·٠● S l y n x ●٠·˙)" Přikryl.
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
 * As a special exception, Michal "Slynx (˙·٠● S l y n x ●٠·˙)" Přikryl
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
 *   - Michal "Slynx (˙·٠● S l y n x ●٠·˙)" Přikryl
 *
 * Project: Source2Toolkit
 */

#ifndef _INCLUDE_CPOINTWORLDTEXT_H
#define _INCLUDE_CPOINTWORLDTEXT_H
#ifdef _WIN32
#pragma once
#endif

#include "../enums/PointWorldTextJustifyHorizontal_t.h"
#include "../enums/PointWorldTextJustifyVertical_t.h"
#include "../enums/PointWorldTextReorientMode_t.h"
#include "CBaseEntity.h"
#include "CModelPointEntity.h"
#include "source2toolkit/IToolkitPlugin.h"
#include "source2toolkit/schema/entityio.h"
#include "source2toolkit/schema/schema.h"

#include <ehandle.h>
#include <entityhandle.h>
#include <igameevents.h>
#include <utlbinaryblock.h>
#include <utlstring.h>
#include <utlstringtoken.h>
#include <utlsymbol.h>
#include <utlsymbollarge.h>
#include <vector.h>

#include <cstdint>

class CPointWorldText : public CModelPointEntity
{
public:
    DECLARE_SCHEMA_CLASS(CPointWorldText);

    SCHEMA_FIELD_POINTER(char, m_messageText);
    SCHEMA_FIELD_POINTER(char, m_FontName);
    SCHEMA_FIELD_POINTER(char, m_BackgroundMaterialName);
    SCHEMA_FIELD(bool, m_bEnabled);
    SCHEMA_FIELD(bool, m_bFullbright);
    SCHEMA_FIELD(float, m_flWorldUnitsPerPx);
    SCHEMA_FIELD(float, m_flFontSize);
    SCHEMA_FIELD(float, m_flDepthOffset);
    SCHEMA_FIELD(bool, m_bDrawBackground);
    SCHEMA_FIELD(float, m_flBackgroundBorderWidth);
    SCHEMA_FIELD(float, m_flBackgroundBorderHeight);
    SCHEMA_FIELD(float, m_flBackgroundWorldToUV);
    SCHEMA_FIELD(Color, m_Color);
    SCHEMA_FIELD(PointWorldTextJustifyHorizontal_t, m_nJustifyHorizontal);
    SCHEMA_FIELD(PointWorldTextJustifyVertical_t, m_nJustifyVertical);
    SCHEMA_FIELD(PointWorldTextReorientMode_t, m_nReorientMode);

public:
    static CPointWorldText* New(const char* className)
    {
        return CBaseEntity::New<CPointWorldText>(className);
    }

    static CPointWorldText* FromIndex(int iIndex)
    {
        return CBaseEntity::FromIndex<CPointWorldText>(iIndex);
    }

    static CPointWorldText* FromIndex(CEntityIndex index)
    {
        return FromIndex(index.Get());
    }

    CHandle<CPointWorldText> GetHandle()
    {
        return CBaseEntity::GetHandle<CPointWorldText>();
    }
};

#endif // _INCLUDE_CPOINTWORLDTEXT_H

//====== Copyright � 1996-2005, Valve Corporation, All rights reserved. =======
//
// Purpose: 
//
//=============================================================================

#ifndef PARTICLEPROPERTY_H
#define PARTICLEPROPERTY_H
#ifdef _WIN32
#pragma once
#endif

#include "smartptr.h"
#include "globalvars_base.h"
#include "particles_new.h"
#include "particle_parse.h"

//-----------------------------------------------------------------------------
// Forward declarations
//-----------------------------------------------------------------------------
class CBaseEntity;
class CNewParticleEffect;

struct ParticleControlPoint_t
{
	ParticleControlPoint_t()
	{
		iControlPoint = 0;
		iAttachType = PATTACH_ABSORIGIN_FOLLOW;
		iAttachmentPoint = 0;
		vecOriginOffset = vec3_origin;
	}

	int								iControlPoint;
	ParticleAttachment_t			iAttachType;
	int								iAttachmentPoint;
	Vector							vecOriginOffset;
	EHANDLE							hEntity;
};

struct ParticleEffectList_t
{
	ParticleEffectList_t()
	{
		pParticleEffect = NULL;
	}

	CUtlVector<ParticleControlPoint_t>	pControlPoints;
	CSmartPtr<CNewParticleEffect>		pParticleEffect;
};

extern int GetAttachTypeFromString( const char *pszString );

//-----------------------------------------------------------------------------
// Encapsulates particle handling for an entity
//-----------------------------------------------------------------------------
class CParticleProperty 
{
	DECLARE_CLASS_NOBASE( CParticleProperty );
	DECLARE_EMBEDDED_NETWORKVAR();
	DECLARE_PREDICTABLE();
	DECLARE_DATADESC();

public:
	CParticleProperty();
	~CParticleProperty();

	void				Init( CBaseEntity *pEntity );
	CBaseEntity			*GetOuter( void ) { return m_pOuter; }

	// Effect Creation
	CNewParticleEffect *Create( const char *pszParticleName, ParticleAttachment_t iAttachType, const char *pszAttachmentName );
	CNewParticleEffect *Create( const char *pszParticleName, ParticleAttachment_t iAttachType, int iAttachmentPoint = -1, Vector vecOriginOffset = vec3_origin );
	void				AddControlPoint( CNewParticleEffect *pEffect, int iPoint, C_BaseEntity *pEntity, ParticleAttachment_t iAttachType, const char *pszAttachmentName = NULL, Vector vecOriginOffset = vec3_origin );
	void				AddControlPoint( int iEffectIndex, int iPoint, C_BaseEntity *pEntity, ParticleAttachment_t iAttachType, int iAttachmentPoint = -1, Vector vecOriginOffset = vec3_origin );

	inline void			SetControlPointParent( CNewParticleEffect *pEffect, int whichControlPoint, int parentIdx );
	void				SetControlPointParent( int iEffectIndex, int whichControlPoint, int parentIdx );

	// Commands
	void				StopEmission( CNewParticleEffect *pEffect = NULL, bool bWakeOnStop = false, bool bDestroyAsleepSystems = false );
	void				StopEmissionAndDestroyImmediately( CNewParticleEffect *pEffect = NULL );

	// kill all particle systems involving a given entity for their control points
	void				StopParticlesInvolving( CBaseEntity *pEntity );
	void				StopParticlesNamed( const char *pszEffectName, bool bForceRemoveInstantly = false ); ///< kills all particles using the given definition name

	// Particle System hooks
	void				OnParticleSystemUpdated( CNewParticleEffect *pEffect, float flTimeDelta );
	void				OnParticleSystemDeleted( CNewParticleEffect *pEffect );

#ifdef CLIENT_DLL
	void				OwnerSetDormantTo( bool bDormant );
#endif

	// Used to replace a particle effect with a different one; attaches the control point updating to the new one
	void				ReplaceParticleEffect( CNewParticleEffect *pOldEffect, CNewParticleEffect *pNewEffect );

	// Debugging
	void				DebugPrintEffects( void );

private:
	int					GetParticleAttachment( C_BaseEntity *pEntity, const char *pszAttachmentName, const char *pszParticleName );
	int					FindEffect( CNewParticleEffect *pEffect );
	int					FindEffect( const char *pEffectName );
	void				UpdateParticleEffect( ParticleEffectList_t *pEffect, bool bInitializing = false, int iOnlyThisControlPoint = -1 );
	void				UpdateControlPoint( ParticleEffectList_t *pEffect, int iPoint, bool bInitializing );
	inline CNewParticleEffect *GetParticleEffectFromIdx( int idx );

private:
	CBaseEntity *m_pOuter;
	CUtlVector<ParticleEffectList_t>	m_ParticleEffects;
	int			m_iDormancyChangedAtFrame;

	friend class CBaseEntity;
};

#include "particle_property_inlines.h"

#endif // PARTICLEPROPERTY_H
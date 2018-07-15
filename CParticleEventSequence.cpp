#include "stdafx.h"
#include "CParticleEventSequence.h"

CParticleEventSequence::CParticleEventSequence()
{
	m_pd3dDevice = NULL;
	m_texParticle = NULL;
	m_Particles = NULL;
	Reset();
}

CParticleEventSequence::~CParticleEventSequence()
{
	Reset();
}

void CParticleEventSequence::Reset()
{
	m_iTotalParticleLives = 0;
	m_Loops = -1;

	m_iNumParticles = 100;
	m_fNumNewPartsExcess = 0.0f;
	m_vGravity = CMinMax<D3DXVECTOR3>( D3DXVECTOR3( 0.0f, 0.0f, 0.0f ), D3DXVECTOR3( 0.0f, 0.0f, 0.0f ) );
	m_vEmitRadius = CMinMax<D3DXVECTOR3>( D3DXVECTOR3( 0.0f, 0.0f, 0.0f ), D3DXVECTOR3( 0.0f, 0.0f, 0.0f ) );
	for( std::vector<CParticleEvent *>::iterator i = m_Events.begin(); i != m_Events.end(); i++ )
	{
		// can't use safe delete here, because it will set i = NULL.
		if( *i ) delete (*i);
	}
	m_Events.clear();

	SAFE_DELETE( m_Particles );
	SAFE_RELEASE( m_texParticle );
}

void CParticleEventSequence::SortEvents( void )
{
	// sort the events by the time they occur (initial = 1st, final = last)
	// a testament to the power of STL: one line to accomplish this.  Pretty sweet, huh?
	CParticleEvent_CompareFunc cf;
	std::sort( m_Events.begin(), m_Events.end(), cf );

#ifdef _DEBUG
	// spit out the list of events, for debugging purposes.
	OutputDebugString( "\n\nSorted Events:" );
	for( std::vector<CParticleEvent *>::iterator i = m_Events.begin(); i != m_Events.end(); i++ )
	{
		char buf[256];
		_snprintf( buf, sizeof( buf ), "\nTime: %0.2f Type: %20s Fade: %c",
			(*i)->GetActualTime(), typeid(*(*i)).name(), (*i)->IsFade() ? 'Y' : 'N' );
		OutputDebugString( buf );
	}

#endif
}

void CParticleEventSequence::CreateFadeLists()
{
	// for each event,
	for( std::vector<CParticleEvent *>::iterator i = m_Events.begin(); i != m_Events.end(); i++ )
	{
		// try to find the next fade event of the same type.
		for( std::vector<CParticleEvent *>::iterator j = i; j != m_Events.end(); j++ )
		{
			if( j != i && typeid(**j) == typeid(**i) && (*j)->IsFade() )
			{
				// we've found a fade event further in the future.  make a note that
				// this event needs to be linked to this future fade event (so that we
				// can calculate the deltas later).
				(*i)->m_NextFadeEvent = (*j);
				break;
			}
		}
	}

}
void CParticleEventSequence::NailDownRandomTimes( void )
{
	for( std::vector<CParticleEvent *>::iterator i = m_Events.begin(); i != m_Events.end(); i++ )
	{
		(*i)->SetActualTime( (*i)->GetTimeRange().GetRandomNumInRange() );
	}
}

HRESULT CParticleEventSequence::RestoreDeviceObjects( LPDIRECT3DDEVICE8 pDev )
{
	m_pd3dDevice = pDev;
	m_Particles = new CRecyclingArrayDyn<CParticle>( m_iNumParticles );

	SetTexture( m_strTexFilename.c_str() ); // trick ourselves into loading the texture

											// restore device objects for each event in this sequence
	for( std::vector<CParticleEvent *>::iterator i = m_Events.begin(); i != m_Events.end(); i++ )
	{
		(*i)->RestoreDeviceObjects( pDev );
	}

	return S_OK;
}

HRESULT CParticleEventSequence::InvalidateDeviceObjects()
{
	SAFE_DELETE( m_Particles );
	SAFE_RELEASE( m_texParticle );

	// invalidate device objects for each event in this sequence
	for( std::vector<CParticleEvent *>::iterator i = m_Events.begin(); i != m_Events.end(); i++ )
	{
		(*i)->InvalidateDeviceObjects();
	}

	return S_OK;
}

void CParticleEventSequence::SetTexture( const char *strTexFilename )
{
	m_strTexFilename = strTexFilename;

	if( m_pd3dDevice )
	{
		SAFE_RELEASE( m_texParticle );

		std::string strFilePath = "D:\\Clients\\MonsterPK\\" + m_strTexFilename;

		if( FAILED( D3DXCreateTextureFromFile( m_pd3dDevice, strFilePath.c_str(), &m_texParticle ) ) )
		{
			m_texParticle = NULL;
		}
	}
}

void CParticleEventSequence::CreateNewParticle( D3DXVECTOR3 m_vPartSysPos )
{

	CParticle *part = m_Particles->New();

	part->m_fLifetime = m_Lifetime.GetRandomNumInRange();
	part->m_vPos = m_vPartSysPos + m_vEmitRadius.GetRandomNumInRange();

	std::vector<CParticleEvent *>::iterator i;

	// process any initial events
	for( i = m_Events.begin(); i != m_Events.end() && !(*i)->GetActualTime(); i++ )
	{
		(*i)->DoItToIt( *part );
	}
	part->m_CurEvent = i;
	m_iTotalParticleLives++;
}

void CParticleEventSequence::RunEvents( CParticle &part )
{
	std::vector<CParticleEvent *>::iterator i;

	// apply any other events to this particle
	for( i = part.m_CurEvent;
		 i != m_Events.end() && (*i)->GetActualTime() <= part.m_fEventTimer; i++ )
	{
		float oldeventtimer = part.m_fEventTimer;
		(*i)->DoItToIt( part );
		if( part.m_fEventTimer != oldeventtimer )
		{

			std::vector<CParticleEvent *>::iterator RecalcIter;

			// event timer has changed, we need to recalc m_CurEvent.
			for( RecalcIter = m_Events.begin();
				 RecalcIter != m_Events.end() && (*RecalcIter)->GetActualTime() < part.m_fEventTimer;
				 RecalcIter++ );

			// set our main iterator to the recalculated iterator
			// the -1 just compensates for the i++ in the main for loop
			i = RecalcIter - 1;
		}
	}
	part.m_CurEvent = i;
}
void CParticleEventSequence::Update( float fElapsedTime, float fTimeDelta, D3DXVECTOR3 m_vPartSysPos )
{
	if( !m_Particles ) return;
	// update existing particles
	{
		for( int q = 0; q < m_Particles->GetTotalElements(); q++ )
		{
			if( m_Particles->IsAlive( q ) )
			{
				CParticle &part = m_Particles->GetAt( q );

				if( !part.Update( fTimeDelta ) )
				{
					m_Particles->Delete( &part );
				}
				else
				{
					// apply gravity to this particle.
					part.m_vDir += fTimeDelta * m_vGravity.GetRandomNumInRange();
					// run all the particle's events
					RunEvents( part );
				}
			} // is alive
		} // next particle
	}

	float fEmitRateThisFrame = m_EmitRate.GetRandomNumInRange();
	int iNumNewParts = (int)(fEmitRateThisFrame * fTimeDelta);
	m_fNumNewPartsExcess += (float)(fEmitRateThisFrame * fTimeDelta) - iNumNewParts;

	if( m_fNumNewPartsExcess > 1.0f )
	{
		iNumNewParts += (int)m_fNumNewPartsExcess;
		m_fNumNewPartsExcess -= (int)m_fNumNewPartsExcess;
	}

	if( m_Loops > 0 && m_iTotalParticleLives + iNumNewParts > m_Loops * m_iNumParticles )
	{
		iNumNewParts = (m_Loops*m_iNumParticles) - m_iTotalParticleLives;
		if( iNumNewParts <= 0 )
		{
			iNumNewParts = 0;
		}
	}
	for( int q = 0; q < iNumNewParts && m_Particles->GetNumFreeElements(); q++ )
	{
		try
		{
			CreateNewParticle( m_vPartSysPos );
		}
		catch( ... )
		{
			q = iNumNewParts;
		}
	}


}

void CParticleEventSequence::Render( LPDIRECT3DVERTEXBUFFER8 pVB, int iVBSize )
{
	HRESULT hr;

	// Set the render states for using point sprites
	m_pd3dDevice->SetRenderState( D3DRS_POINTSPRITEENABLE, true );
	m_pd3dDevice->SetRenderState( D3DRS_POINTSCALEENABLE, true );
	m_pd3dDevice->SetRenderState( D3DRS_POINTSIZE_MIN, FtoDW( 0.00f ) );
	m_pd3dDevice->SetRenderState( D3DRS_POINTSIZE_MAX, FtoDW( 500.0f ) );
	m_pd3dDevice->SetRenderState( D3DRS_POINTSCALE_A, FtoDW( 0.00f ) );
	m_pd3dDevice->SetRenderState( D3DRS_POINTSCALE_B, FtoDW( 0.00f ) );
	m_pd3dDevice->SetRenderState( D3DRS_POINTSCALE_C, FtoDW( 1.00f ) );

#if defined(PARTICLESPT)
	EBlendMode eBlendMode = (EBlendMode)GetSrcBlendMode();

	switch( eBlendMode )
	{
		case BLEND_NONE:
			m_pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );
			m_pd3dDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE );
			m_pd3dDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_ONE );
			break;
		case BLEND_ALPHA:
			m_pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
			m_pd3dDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
			m_pd3dDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
			break;
		case BLEND_COLOR:
			m_pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
			m_pd3dDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCCOLOR );
			m_pd3dDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCCOLOR );
			break;
		case BLEND_SHADOW:
			m_pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
			m_pd3dDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_SRCCOLOR );
			m_pd3dDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_ZERO );
			break;
		case BLEND_LAMP:
			m_pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
			m_pd3dDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
			m_pd3dDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE );
			break;
		case BLEND_ADDCOLOR:
			m_pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
			m_pd3dDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE );
			m_pd3dDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCCOLOR );
			break;
		case BLEND_INVSHADOW:
			m_pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
			m_pd3dDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCCOLOR );
			m_pd3dDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_ZERO );
			break;
	}

#else
	m_pd3dDevice->SetRenderState( D3DRS_SRCBLEND, GetSrcBlendMode() );
	m_pd3dDevice->SetRenderState( D3DRS_DESTBLEND, GetDestBlendMode() );
#endif

	// Set up the vertex buffer to be rendered
	m_pd3dDevice->SetStreamSource( 0, pVB, sizeof( VERTEX_PARTICLE ) );
	m_pd3dDevice->SetVertexShader( D3DFVF_PARTICLE );

	m_pd3dDevice->SetTexture( 0, m_texParticle );

	VERTEX_PARTICLE *pVertices;
	DWORD dwNumParticlesToRender = 0;

	// Lock the vertex buffer.  We fill the vertex buffer in small
	// chunks, using D3DLOCK_NOOVERWRITE.  When we are done filling
	// each chunk, we call DrawPrim, and lock the next chunk.  When
	// we run out of space in the vertex buffer, we start over at
	// the beginning, using D3DLOCK_DISCARD.

	int iCurStart = 0;

	if( FAILED( hr = pVB->Lock( iCurStart * sizeof( VERTEX_PARTICLE ), iVBSize * sizeof( VERTEX_PARTICLE ),
		(BYTE **)&pVertices, iCurStart ? D3DLOCK_NOOVERWRITE : D3DLOCK_DISCARD ) ) )
	{
		return;
	}

	// Render each particle
	for( int q = 0; q < m_iNumParticles; q++ )
	{

		// Render each particle a bunch of times to get a blurring effect
		if( m_Particles->IsAlive( q ) )
		{
			CParticle &part = m_Particles->GetAt( q );

			pVertices->position = part.m_vPos;
			pVertices->pointsize = part.m_fSize;
			pVertices->color = (DWORD)part.m_Color;
			pVertices++;

			if( ++dwNumParticlesToRender == iVBSize )
			{
				// Done filling this chunk of the vertex buffer.  Lets unlock and
				// draw this portion so we can begin filling the next chunk.

				pVB->Unlock();

				if( FAILED( hr = m_pd3dDevice->DrawPrimitive(
					D3DPT_POINTLIST, iCurStart, dwNumParticlesToRender ) ) )
					return;

				// Lock the next chunk of the vertex buffer.  If we are at the 
				// end of the vertex buffer, DISCARD the vertex buffer and start
				// at the beginning.  Otherwise, specify NOOVERWRITE, so we can
				// continue filling the VB while the previous chunk is drawing.
				iCurStart = 0;

				if( FAILED( hr = pVB->Lock( 0, iVBSize * sizeof( VERTEX_PARTICLE ),
					(BYTE **)&pVertices, D3DLOCK_DISCARD ) ) )
					return;

				dwNumParticlesToRender = 0;
			}
		}
	}

	// Unlock the vertex buffer
	pVB->Unlock();

	// Render any remaining particles
	if( dwNumParticlesToRender )
	{
		if( FAILED( hr = m_pd3dDevice->DrawPrimitive( D3DPT_POINTLIST, iCurStart, dwNumParticlesToRender ) ) )
			return;
	}

	// Reset render states
	m_pd3dDevice->SetRenderState( D3DRS_POINTSPRITEENABLE, FALSE );
	m_pd3dDevice->SetRenderState( D3DRS_POINTSCALEENABLE, FALSE );


}
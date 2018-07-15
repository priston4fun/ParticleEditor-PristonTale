#ifndef CH18P3_PARTICLE_H_DEFINED
#define CH18P3_PARTICLE_H_DEFINED

typedef struct 
{
    D3DXVECTOR3 position;
    float       pointsize;
    D3DCOLOR    color;
} VERTEX_PARTICLE;

#define D3DFVF_PARTICLE (D3DFVF_XYZ|D3DFVF_DIFFUSE|D3DFVF_PSIZE)

class CParticleEvent;

class CParticle
{
public:
  CParticle() { 
    m_fWeight = 1.0f; m_fSize = 1.0f; m_fSizeStep = 0.0f;
    m_fLifetime = 1.0f; m_fAge = 0.0f; m_fEventTimer = 0.0f; 
    m_Color = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
    m_ColorStep = D3DXCOLOR(0.0f,0.0f,0.0f,0.0f);
    m_vDir = D3DXVECTOR3(0.0f,0.0f,0.0f);
    m_vDirStep = D3DXVECTOR3(0.0f,0.0f,0.0f);
    m_vPos = D3DXVECTOR3(0.0f,0.0f,0.0f);
  }
  virtual ~CParticle() { }

  std::vector<CParticleEvent *>::iterator m_CurEvent;

  float m_fWeight;
  
  float m_fSize;
  float m_fSizeStep;

  float m_fLifetime;
  float m_fAge;
  
  float m_fEventTimer; // different from age
  
  D3DXCOLOR m_Color;
  D3DXCOLOR m_ColorStep;
  
  D3DXVECTOR3 m_vPos; // current position of particle
  
  D3DXVECTOR3 m_vDir;
  D3DXVECTOR3 m_vDirStep;

  // this function is inline not because it's small, but because it's only called
  // in one place (CParticleEmitter.Update()).  This way we can also dodge the
  // performance hit associated with calling a function many times (remember, we're
  // in a for loop in CParticleEmitter.Update()).

  // your needs may vary.  For example, if you can afford the speed hit, it might
  // be cool to make this a virtual function.
  inline bool Update(float fTimeDelta)
  {
    // age the particle
		m_fAge += fTimeDelta;
    m_fEventTimer += fTimeDelta;

    // if this particle's age is greater than it's lifetime, it dies.
    if (m_fAge >= m_fLifetime) {
      return(false); // dead!
    }

		// move particle
		m_vPos += m_vDir * fTimeDelta;

    m_Color += m_ColorStep * fTimeDelta;
    m_vDir += m_vDirStep * fTimeDelta;
    m_fSize += m_fSizeStep * fTimeDelta;

    return(true); // particle stays alive
  }
};

#endif
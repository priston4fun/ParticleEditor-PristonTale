#include "stdafx.h"
#include "CParticleEmitter.h"

#include "CParticleEmitterTokenizer.h"

CParticleEmitter::CParticleEmitter()
{
  // default particle system configuration - boring!
  Init();
 
  // initialize misc. other things
  m_pd3dDevice = NULL;
  m_vbParticles = NULL;
}

void CParticleEmitter::Init()
{
  m_strName = "";
  for (std::vector<CParticleEventSequence *>::iterator i = m_Sequences.begin(); i != m_Sequences.end(); i++) {
    delete (*i);
  }
  
  m_Sequences.clear();

  SetPosRange(CMinMax<D3DXVECTOR3>(D3DXVECTOR3(0.0f,0.0f,0.0f),D3DXVECTOR3(0.0f,0.0f,0.0f)));
  
  SetVBSize(500);
	Stop();
}


CParticleEmitter::~CParticleEmitter()
{
  Init();
}


CParticleEvent *CParticleEmitter::EventFactory(std::string EventName)
{
  CParticleEvent *event = NULL;

  if (EventName.find("SIZE") != std::string::npos) event = new CParticleEvent_Size();
  else if (EventName.find("EVENTTIMER") != std::string::npos) event = new CParticleEvent_EventTimer();
  else if (EventName.find("REDCOLOR") != std::string::npos) event = new CParticleEvent_RedColor();
  else if (EventName.find("GREENCOLOR") != std::string::npos) event = new CParticleEvent_GreenColor();
  else if (EventName.find("BLUECOLOR") != std::string::npos) event = new CParticleEvent_BlueColor();
  else if (EventName.find("ALPHA") != std::string::npos) event = new CParticleEvent_Alpha();
  else if (EventName.find("COLOR") != std::string::npos) event = new CParticleEvent_Color();
  else if (EventName.find("VELOCITYX") != std::string::npos) event = new CParticleEvent_VelocityX();
  else if (EventName.find("VELOCITYY") != std::string::npos) event = new CParticleEvent_VelocityY();
  else if (EventName.find("VELOCITYZ") != std::string::npos) event = new CParticleEvent_VelocityZ();
  else if (EventName.find("VELOCITY") != std::string::npos) event = new CParticleEvent_Velocity();

  return(event);
}

bool CParticleEmitter::ProcessEventSequenceBlock(CParticleEventSequence &seq,
                                                 std::vector<CParticleEmitterToken>::iterator &TokenIter, 
                                                 std::vector<CParticleEmitterToken>::iterator &EndIter)
{
  bool StartedProcessingEvents = false;

  // move past the event sequence keyword...
  CParticleEmitterTokenizer::NextToken(TokenIter, EndIter);

  // next token should be the name of the sequence...
  if (TokenIter->m_Type != Quote) throw("Must name particle sequence block!");
  seq.SetName(TokenIter->m_strValue);
  CParticleEmitterTokenizer::NextToken(TokenIter, EndIter);

  // next token should be opening brace...
  if (TokenIter->m_Type != OpenBrace) throw("Expected opening brace for particle sequence block!");
  CParticleEmitterTokenizer::NextToken(TokenIter, EndIter);

  while (TokenIter->m_Type != CloseBrace) {

    CParticleEmitterToken savedtoken;
    savedtoken = *TokenIter;

    // the first token here can be a SysNumberProperty, SysAlphaBlendModeProperty, SysVectorProperty,
    // or an EventSequence.
    switch (TokenIter->m_Type) {
      case SeqNumericProp:
      {
        if (StartedProcessingEvents) throw("Cannot specify any sequence properties after specifying events.");
        CMinMax<float> number;
        // the next 2 tokens should be an equals, and a number.
        CParticleEmitterTokenizer::NextToken(TokenIter, EndIter); 
        if (TokenIter->m_Type != Equals) throw("Expected equals sign!");
        CParticleEmitterTokenizer::NextToken(TokenIter, EndIter); 
        CParticleEmitterTokenizer::ProcessNumber(number, TokenIter, EndIter);

        if (savedtoken.IsEmitRate()) { seq.SetEmitRate(number); }
        else if (savedtoken.IsLifeTime()) { seq.SetLifetime(number); }
        else if (savedtoken.IsNumParticles()) { seq.SetNumParticles( (int)number.m_Max ); }
				else if (savedtoken.IsLoops()) { seq.SetLoops( (int)number.m_Max); }
        else {
          throw("Unknown sequence numeric property!");
        }
      }
      break;

      case SeqVectorProp:
      {
        if (StartedProcessingEvents) throw("Cannot specify any sequence properties after specifying events.");
        CMinMax<D3DXVECTOR3> v;
        CParticleEmitterTokenizer::NextToken(TokenIter, EndIter); 
        if (TokenIter->m_Type != Equals) throw("Expected equals sign!");
        CParticleEmitterTokenizer::NextToken(TokenIter, EndIter); 
        CParticleEmitterTokenizer::ProcessVector(v, TokenIter, EndIter);

        if (savedtoken.IsSpawnDir())        { seq.SetSpawnDir(v); }
        else if (savedtoken.IsEmitRadius()) { seq.SetEmitRadius(v); }
        else if (savedtoken.IsGravity())    { seq.SetGravity(v); }
        else {
          throw("Unknown sequence vector property!");
        }
      }
      break;

      case SeqAlphaBlendModeProp:
      {
        if (StartedProcessingEvents) throw("Cannot specify any sequence properties after specifying events.");
        int alphablendmode;
        CParticleEmitterTokenizer::NextToken(TokenIter, EndIter); 
        if (TokenIter->m_Type != Equals) throw("Expected equals sign!");
        CParticleEmitterTokenizer::NextToken(TokenIter, EndIter); 
        CParticleEmitterTokenizer::ProcessAlphaBlendMode(alphablendmode, TokenIter, EndIter);

		if( savedtoken.IsSrcBlendMode() )
		{
			seq.SetSrcBlendMode( alphablendmode );
		}
		else if( savedtoken.IsDestBlendMode() )
		{
			seq.SetDestBlendMode( alphablendmode );
		}
		else
		{
			throw("Unknown sequence alpha blending mode property!");
        }
      }
      break;

      case KeywordTexture:
      {
        // TODO
        
        if (StartedProcessingEvents) throw("Cannot specify any sequence properties after specifying events.");
        CMinMax<D3DXVECTOR3> v;
        CParticleEmitterTokenizer::NextToken(TokenIter, EndIter); 
        if (TokenIter->m_Type != Equals) throw("Expected equals sign!");
        CParticleEmitterTokenizer::NextToken(TokenIter, EndIter); 

        if (TokenIter->m_Type != Quote) throw("Expected filename after texture sequence property.");

        seq.SetTexture(RemoveQuotes(TokenIter->m_strValue).c_str());
        CParticleEmitterTokenizer::NextToken(TokenIter, EndIter); 

      }
      break;

      case KeywordFade: case KeywordAt: case KeywordInitial: case KeywordFinal:
      {
        StartedProcessingEvents = true;

        bool IsFade = false;
        CMinMax<float> TimeRange;

        // parse the time range section of the event line
        CParticleEmitterTokenizer::ProcessTime(TimeRange, IsFade, 0, seq.GetLifetime().m_Max, TokenIter, EndIter);

        if (TokenIter->m_Type != ParticleNumericProp &&  TokenIter->m_Type != ParticleVectorProp &&
            TokenIter->m_Type != ParticleColorProp && TokenIter->m_Type != ParticleAngleProp )
          throw("Expecting particle property after time specifier!");

        CParticleEvent * NewEvent = NULL;
        try {
          // create the appropriate event
          NewEvent = EventFactory(TokenIter->m_strValue);

          if (!NewEvent) throw("Unknown event type, or there was an error creating this event.");

          // let the event parse the rest of its properties from the token stream.
          if (IsFade && !NewEvent->FadeAllowed()) throw("Fading is not supported on this event.");
          NewEvent->ProcessTokenStream(TokenIter, EndIter);
          NewEvent->SetTimeRange(TimeRange);
          NewEvent->SetFade(IsFade);
          seq.m_Events.push_back(NewEvent);

        }
        catch(char *e) {
          SAFE_DELETE(NewEvent);
          throw(e);
        }
        catch(...) {
          SAFE_DELETE(NewEvent);
          throw("Unhandled exception creating event!");
        }
      }
      break;

      default: 
      {
        char buf[256]; _snprintf(buf, sizeof(buf), "Unexpected \"%s\" in Sequence Block!", TokenIter->m_strValue.c_str());
        throw(&buf[0]);
      }
      break;
    }
  }
  seq.NailDownRandomTimes();
  seq.SortEvents();
  seq.CreateFadeLists();
  CParticleEmitterTokenizer::NextToken(TokenIter, EndIter); 
  return(true);
}

bool CParticleEmitter::ProcessParticleSystemBlock(std::vector<CParticleEmitterToken>::iterator &TokenIter, 
                                                  std::vector<CParticleEmitterToken>::iterator &EndIter)
{
  bool StartedProcessingSequences = false;
  // move past the opening brace...
  CParticleEmitterTokenizer::NextToken(TokenIter, EndIter);

  while (TokenIter->m_Type != CloseBrace) {
    

    CParticleEmitterToken savedtoken;
    savedtoken = *TokenIter;

    // the first token here can be a SysNumberProperty, SysAlphaBlendModeProperty, SysVectorProperty,
    // or an EventSequence.
    switch (TokenIter->m_Type) {
      
      case SysVectorProp:
      {
        if (StartedProcessingSequences) throw("Cannot specify any particle system properties after specifying sequences.");
        CMinMax<D3DXVECTOR3> v;
        CParticleEmitterTokenizer::NextToken(TokenIter, EndIter); 
        if (TokenIter->m_Type != Equals) throw("Expected equals sign!");
        CParticleEmitterTokenizer::NextToken(TokenIter, EndIter); 
        CParticleEmitterTokenizer::ProcessVector(v, TokenIter, EndIter);
        if (savedtoken.IsPosition()) {
          m_vPosRange = v;
        }
        else {
          throw("Unknown particle system property!");
        }
      }
      break;

      case KeywordEventSequence:
      {
        StartedProcessingSequences = true;
        CParticleEventSequence *newseq = new CParticleEventSequence;
        m_Sequences.push_back(newseq);
        CParticleEventSequence *seq = *(m_Sequences.end()-1);
        seq->Reset();

        if (!ProcessEventSequenceBlock(*seq, TokenIter, EndIter)) {
          delete seq;
          m_Sequences.pop_back();
        }
      }
      break;

      default: 
      {
        char buf[256]; _snprintf(buf, sizeof(buf), "Unexpected \"%s\" in Particle System Block!", TokenIter->m_strValue.c_str());
        throw(&buf[0]);
      }
      break;
    }
  }
  CParticleEmitterTokenizer::NextToken(TokenIter, EndIter);
  return(true);
}

bool CParticleEmitter::Compile(const char *strScript)
{
  m_strLastError = "0 error(s), you're good to go!";

  try {
    CParticleEmitterTokenizer tokenizer;
    InvalidateDeviceObjects();
                             
    Init();

    // parse the character string into tokens.
    tokenizer.Tokenize(strScript);

    std::vector<CParticleEmitterToken>::iterator TokenIter = tokenizer.m_TokenVector.begin();
    std::vector<CParticleEmitterToken>::iterator EndIter = tokenizer.m_TokenVector.end();

    if (tokenizer.m_TokenVector.size() < 2) throw("This script is too small to be valid.");

    // the first three tokens out of the gate should be
    // ParticleSystem, followed by a name and version number, then
    // an open brace.
    if (TokenIter->m_Type != KeywordParticleSystem) { throw("First word must be ParticleSystem"); }
    CParticleEmitterTokenizer::NextToken(TokenIter, EndIter);
    if (TokenIter->m_Type != Quote) { throw("Must name particle system"); }
    m_strName = RemoveQuotes(TokenIter->m_strValue);
    CParticleEmitterTokenizer::NextToken(TokenIter, EndIter);

    if (TokenIter->m_Type != RealNumber) { throw("Must have version number"); }
    CParticleEmitterTokenizer::NextToken(TokenIter, EndIter);
    if (TokenIter->m_Type != OpenBrace) { throw("Missing opening brace for ParticleSystem block"); }

    ProcessParticleSystemBlock(TokenIter, EndIter);

  } catch(char *e) {
    m_strLastError = e; Init(); OutputDebugString(e); return(false);
  } catch(const char *e) {
    m_strLastError = e; Init(); OutputDebugString(e); return(false);
  } catch (...) {
    m_strLastError = "Unhandled exception in CParticleEmitter::Compile.";
    OutputDebugString(m_strLastError.c_str());
    Init();
    return(false);
  }

  // do misc. processing and calcuations
  {
    m_vPos = m_vPosRange.GetRandomNumInRange();
  }

  RestoreDeviceObjects(m_pd3dDevice);
  return(true);
}

HRESULT CParticleEmitter::RestoreDeviceObjects(LPDIRECT3DDEVICE8 pDev)
{
  
  HRESULT hr;
  if (!pDev) return S_OK; // nothing to do!
  m_pd3dDevice = pDev;
    
  // create vertex buffer
  if(FAILED(hr = m_pd3dDevice->CreateVertexBuffer(m_iVBSize * sizeof(VERTEX_PARTICLE), 
    D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY | D3DUSAGE_POINTS, 
    D3DFVF_PARTICLE, D3DPOOL_DEFAULT, &m_vbParticles))) {
    return(hr);
  }

  // restore device objects for each event sequence
  for (std::vector<CParticleEventSequence *>::iterator i = m_Sequences.begin(); i != m_Sequences.end(); i++) {
    (*i)->RestoreDeviceObjects(pDev);
  }
  
  return S_OK;	
}

void CParticleEmitter::InvalidateDeviceObjects()
{
  // invalidate device objects for each event sequence
  for (std::vector<CParticleEventSequence *>::iterator i = m_Sequences.begin(); i != m_Sequences.end(); i++) {
    (*i)->InvalidateDeviceObjects();
  }
  
  SAFE_RELEASE(m_vbParticles);
}

void CParticleEmitter::Update(float fElapsedTime, float fTimeDelta)
{
	if (IsRunning()) {
		for (std::vector<CParticleEventSequence *>::iterator i = m_Sequences.begin(); i != m_Sequences.end(); i++) {
			(*i)->Update(fElapsedTime, fTimeDelta, m_vPos);
		}
	}
}

HRESULT CParticleEmitter::Render()
{									
	if (IsRunning()) {
		// Set the world matrix
		D3DXMATRIX matWorld;
		D3DXMatrixIdentity( &matWorld );
		m_pd3dDevice->SetTransform( D3DTS_WORLD, &matWorld );

		for (std::vector<CParticleEventSequence *>::iterator i = m_Sequences.begin(); i != m_Sequences.end(); i++) {
			(*i)->Render(m_vbParticles, m_iVBSize);
		}
	}
  return S_OK;
}

  
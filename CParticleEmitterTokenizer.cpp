#include "stdafx.h"
#include "CParticleEmitterTokenizer.h"

#if defined(PARTICLESPT)
const int CParticleEmitterTokenizer::NUMBLENDINGMODES = 6;

const BLENDINGMODE CParticleEmitterTokenizer::m_BlendingModes[6] = {
	{ "BLEND_ALPHA"   , BLEND_ALPHA },
	{ "BLEND_COLOR"   , BLEND_COLOR },
	{ "BLEND_ADDCOLOR", BLEND_ADDCOLOR },
	{ "BLEND_SHADOW"  , BLEND_SHADOW },
	{ "BLEND_LAMP"    , BLEND_LAMP },
	{ "BLEND_INVSHADOW", BLEND_INVSHADOW }
};
#else
const BLENDINGMODE CParticleEmitterTokenizer::m_BlendingModes[11] = {
  { "D3DBLEND_ZERO", D3DBLEND_ZERO },
  { "D3DBLEND_ONE", D3DBLEND_ONE },
  { "D3DBLEND_SRCCOLOR", D3DBLEND_SRCCOLOR },
  { "D3DBLEND_INVSRCCOLOR", D3DBLEND_INVSRCCOLOR },
  { "D3DBLEND_SRCALPHA", D3DBLEND_SRCALPHA },
  { "D3DBLEND_INVSRCALPHA", D3DBLEND_INVSRCALPHA },
  { "D3DBLEND_DESTALPHA", D3DBLEND_DESTALPHA },
  { "D3DBLEND_INVDESTALPHA", D3DBLEND_INVDESTALPHA },
  { "D3DBLEND_DESTCOLOR", D3DBLEND_DESTCOLOR },
  { "D3DBLEND_INVDESTCOLOR", D3DBLEND_INVDESTCOLOR },
  { "D3DBLEND_SRCALPHASAT", D3DBLEND_SRCALPHASAT }
};
#endif

bool CParticleEmitterTokenizer::ProcessNumber(CMinMax<float> &number, std::vector<CParticleEmitterToken>::iterator &TokenIter,
                                     std::vector<CParticleEmitterToken>::iterator &EndIter)
{
  // the first token is either the random keyword, or it's an actual number.
  switch(TokenIter->m_Type) {
    case KeywordRandom:
    {
      // parse random number into minmax
      NextToken(TokenIter, EndIter);
      if (TokenIter->m_Type != OpenParen) throw("Expecting opening paren after Random keyword.");
      NextToken(TokenIter, EndIter);
      if (TokenIter->m_Type != RealNumber) throw("Expecting first number within Random(...).");
      number.m_Min = (float)atof(TokenIter->m_strValue.c_str());
      NextToken(TokenIter, EndIter);
      
      if (TokenIter->m_Type != Comma) throw("Expecting comma within Random(...).");
      NextToken(TokenIter, EndIter);
      
      if (TokenIter->m_Type != RealNumber) throw("Expecting second number within Random(...).");
      number.m_Max = (float)atof(TokenIter->m_strValue.c_str());
      NextToken(TokenIter, EndIter);
      if (TokenIter->m_Type != CloseParen) throw("Missing close paren on Random(...).");
      NextToken(TokenIter, EndIter);

    }
    break;

    case RealNumber:
      // min and max both equal realnumber
      if (TokenIter->m_Type != RealNumber) throw("Expecting number.");
      number.m_Max = number.m_Min = (float)atof(TokenIter->m_strValue.c_str());
      NextToken(TokenIter, EndIter);
    break;

    default: throw("Expecting either Random(...) or a number value.");
  }
  return(true);
}

bool CParticleEmitterTokenizer::ProcessVector(CMinMax<D3DXVECTOR3> &v, 
                                     std::vector<CParticleEmitterToken>::iterator &TokenIter,
                                     std::vector<CParticleEmitterToken>::iterator &EndIter)
{
  // this token needs to be a XYZ keyword.
  if (TokenIter->m_Type != KeywordXYZ) throw("Expecting XYZ(...)!");
  NextToken(TokenIter, EndIter);
  
  if (TokenIter->m_Type != OpenParen) throw("Expecting ( after XYZ!");
  
  CMinMax<float> x;
  CMinMax<float> y;
  CMinMax<float> z;

  NextToken(TokenIter, EndIter); ProcessNumber(x, TokenIter, EndIter);
  if (TokenIter->m_Type != Comma) throw("Vector components must be seperated by a comma.");
  NextToken(TokenIter, EndIter); ProcessNumber(y, TokenIter, EndIter);
  if (TokenIter->m_Type != Comma) throw("Vector components must be seperated by a comma.");
  NextToken(TokenIter, EndIter); ProcessNumber(z, TokenIter, EndIter);
  if (TokenIter->m_Type != CloseParen) throw("Expecting ) to close vector.");
  NextToken(TokenIter, EndIter);
  
  v.m_Min = D3DXVECTOR3(x.m_Min, y.m_Min, z.m_Min);
  v.m_Max = D3DXVECTOR3(x.m_Max, y.m_Max, z.m_Max);
  
  return(true);
}

bool CParticleEmitterTokenizer::ProcessColor(CMinMax<D3DXCOLOR> &c, 
                                     std::vector<CParticleEmitterToken>::iterator &TokenIter,
                                     std::vector<CParticleEmitterToken>::iterator &EndIter)
{
  // this token needs to be a RGBA keyword.
  if (TokenIter->m_Type != KeywordColor) 
		throw("Expecting RGBA(...)!");
  NextToken(TokenIter, EndIter);
  
  if (TokenIter->m_Type != OpenParen) throw("Expecting ( after RGBA!");
  
  CMinMax<float> r;
  CMinMax<float> g;
  CMinMax<float> b;
  CMinMax<float> a;

  NextToken(TokenIter, EndIter); ProcessNumber(r, TokenIter, EndIter);
  if (TokenIter->m_Type != Comma) throw("Color components must be seperated by a comma.");
  NextToken(TokenIter, EndIter); ProcessNumber(g, TokenIter, EndIter);
  if (TokenIter->m_Type != Comma) throw("Color components must be seperated by a comma.");
  NextToken(TokenIter, EndIter); ProcessNumber(b, TokenIter, EndIter);
  if (TokenIter->m_Type != Comma) throw("Color components must be seperated by a comma.");
  NextToken(TokenIter, EndIter); ProcessNumber(a, TokenIter, EndIter);
  if (TokenIter->m_Type != CloseParen) throw("Expecting ) to close vector.");
  NextToken(TokenIter, EndIter);
  
  c.m_Min = D3DXCOLOR(r.m_Min, g.m_Min, b.m_Min, a.m_Min);
  c.m_Max = D3DXCOLOR(r.m_Max, g.m_Max, b.m_Max, a.m_Max);
  
  return(true);
}

bool CParticleEmitterTokenizer::ProcessAlphaBlendMode(int &alphablendmode, 
                                     std::vector<CParticleEmitterToken>::iterator &TokenIter,
                                     std::vector<CParticleEmitterToken>::iterator &EndIter)
{
  if (TokenIter->m_Type != AlphaBlendMode) throw("Expecting alpha blend mode (D3DBLEND_ZERO, D3DBLEND_ONE, etc.).");
  for (int q=0; q < NUMBLENDINGMODES; q++) {
    // careful here... must match on both string and size 
    // (i.e., to differentiate between D3DBLEND_SRCALPHA and D3DBLEND_SRCALPHASAT).
    if (TokenIter->m_strValue.find(m_BlendingModes[q].name) != std::string::npos && 
      TokenIter->m_strValue.size() == strlen(m_BlendingModes[q].name)) {
      alphablendmode = m_BlendingModes[q].mode;
      NextToken(TokenIter, EndIter);
      return(true);
    }
  }
  throw("Invalid alpha blending mode!");
  return(false);
}

bool CParticleEmitterTokenizer::ProcessTime(CMinMax<float> &TimeRange, bool &IsFade, const float InitialTime, const float FinalTime,
                                   std::vector<CParticleEmitterToken>::iterator &TokenIter, 
                                   std::vector<CParticleEmitterToken>::iterator &EndIter)
{

  // determine if this time specification is a fade directive
  if (TokenIter->m_Type == KeywordFade) {
    // it is... the next token must be "so"
    NextToken(TokenIter, EndIter); if (TokenIter->m_Type != KeywordSo) throw("Expecting \"so\" after \"fade\".");

    // flip the fade flag on
    IsFade = true;

    // move to next token (so that we land on "at" for the code below)
    NextToken(TokenIter, EndIter);
  }
  else IsFade = false; // just to be safe

  switch(TokenIter->m_Type) {
    case KeywordAt:
    {
      // easy, just grab the time
      NextToken(TokenIter, EndIter);
      ProcessNumber(TimeRange, TokenIter, EndIter);
    }
    break;

    case KeywordInitial:
    {
      // use initial time that was passed in.
      if (IsFade) throw("Cannot use \"fade so\" modifier on \"initial\" times.");
      TimeRange.m_Min = InitialTime;
      TimeRange.m_Max = InitialTime;
      CParticleEmitterTokenizer::NextToken(TokenIter, EndIter);
    }
    break;

    case KeywordFinal:
    {
      // use final time that was passed in.
      TimeRange.m_Min = FinalTime;
      TimeRange.m_Max = FinalTime;
      CParticleEmitterTokenizer::NextToken(TokenIter, EndIter);
    }
    break;

    default: throw("Expected time specifier: \"[fade so] at\", \"initial\", or \"[fade so] final\"");
  }

  return(true);
}

void CParticleEmitterTokenizer::DetermineTokenType(CParticleEmitterToken &token)
{
  token.m_Type = UnknownToken;
  
  // these things are easy to see...
  if (isdigit(token.m_strValue[0]) || token.m_strValue[0] == '-') { token.m_Type = RealNumber; return; }
  if (token.m_strValue[0] == '=')   { token.m_Type = Equals; return; }
  if (token.m_strValue[0] == ',')   { token.m_Type = Comma; return; }
  if (token.m_strValue[0] == '(')   { token.m_Type = OpenParen; return; }
  if (token.m_strValue[0] == ')')   { token.m_Type = CloseParen; return; }
  if (token.m_strValue[0] == '{')   { token.m_Type = OpenBrace; return; }
  if (token.m_strValue[0] == '}')   { token.m_Type = CloseBrace; return; }
  if (token.m_strValue[0] == '\"')  { token.m_Type = Quote; return; }
  
  // if we got here, it's not a quote... so convert it to uppercase.
  MakeUpperCase(token.m_strValue);

  // keywords are easy to figure out, just look for the text...
  if (token.m_strValue.find("PARTICLESYSTEM") != std::string::npos) { token.m_Type = KeywordParticleSystem; return; }
  if (token.m_strValue.find("EVENTSEQUENCE") != std::string::npos) { token.m_Type = KeywordEventSequence; return; }
  if (token.m_strValue.find("RANDOM") != std::string::npos) { token.m_Type = KeywordRandom; return; }
  if (token.m_strValue.find("XYZ") != std::string::npos) { token.m_Type = KeywordXYZ; return; }
  if (token.m_strValue.find("RGBA") != std::string::npos) { token.m_Type = KeywordColor; return; }
  if (token.m_strValue.find("FADE") != std::string::npos) { token.m_Type = KeywordFade; return; }
  
  if (token.m_strValue.find("INITIAL") != std::string::npos) { token.m_Type = KeywordInitial; return; }
  if (token.m_strValue.find("FINAL") != std::string::npos) { token.m_Type = KeywordFinal; return; }
  if (token.m_strValue.find("TEXTURE") != std::string::npos) { token.m_Type = KeywordTexture; return; }
  
  // these two keywords are embedded in other words, so we've got to be careful.
  if (token.m_strValue.find("SO") != std::string::npos && token.m_strValue.size() == 2) { token.m_Type = KeywordSo; return; }
  if (token.m_strValue.find("AT") != std::string::npos && token.m_strValue.size() == 2) { token.m_Type = KeywordAt; return; }

  // now it gets harder... 
  if (token.m_strValue.find("BLEND_") != std::string::npos) { token.m_Type = AlphaBlendMode; return; }
  if (token.m_strValue.find("SOURCEBLENDMODE") != std::string::npos ||
      token.m_strValue.find("DESTBLENDMODE") != std::string::npos) { 
    token.m_Type = SeqAlphaBlendModeProp; return; 
  }

  if (token.m_strValue.find("LIFETIME") != std::string::npos ||
      token.m_strValue.find("EMITRATE") != std::string::npos ||
      token.m_strValue.find("NUMPARTICLES") != std::string::npos ||
	  token.m_strValue.find("LOOPS") != std::string::npos || 
	  token.m_strValue.find("DELAY") != std::string::npos) { 
    token.m_Type = SeqNumericProp; return; 
  }

  if (token.m_strValue.find("SPAWNDIR") != std::string::npos ||
      token.m_strValue.find("GRAVITY") != std::string::npos || 
      token.m_strValue.find("EMITRADIUS") != std::string::npos) {
    token.m_Type = SeqVectorProp; return;
  }

  if (token.m_strValue.find("POSITION") != std::string::npos) {
    token.m_Type = SysVectorProp; return;
  }

  if (token.m_strValue.find("SIZE") != std::string::npos ||
      token.m_strValue.find("EVENTTIMER") != std::string::npos ||
	  token.m_strValue.find( "SIZEEXT" ) != std::string::npos ) {
    token.m_Type = ParticleNumericProp; return;
  }

  
  if (token.m_strValue.find("VELOCITY") != std::string::npos) {
    token.m_Type = ParticleVectorProp; return;
  }

  if (token.m_strValue.find("COLOR") != std::string::npos) {
    token.m_Type = ParticleColorProp; return;
  }

  if( token.m_strValue.find( "PARTANGLE" ) != std::string::npos ||
	  token.m_strValue.find( "LOCALANGLE" ) != std::string::npos )
  {
	  token.m_Type = ParticleAngleProp; 
	  return;
  }

  if( token.m_strValue.find( "OUTLENGTH" ) != std::string::npos )
  {
	  token.m_Type = KeywordOutLength;
	  return;
  }

  if( token.m_strValue.find( "INLENGTH" ) != std::string::npos )
  {
	  token.m_Type = KeywordInLength;
	  return;
  }
}

void CParticleEmitterTokenizer::AddToken(CParticleEmitterToken token)
{
  if (token.m_strValue.size()) {
    DetermineTokenType(token);
    m_TokenVector.push_back(token);
  }
}

void CParticleEmitterTokenizer::Tokenize(const char *str)
{
  enum CurState {
    InWhiteSpace = 1,
    InText,
    InQuote,
    InComment
  };
  
  CurState cs = InWhiteSpace;
  const char *p = str;
  CParticleEmitterToken token;
  
  while ((*p) != 0) {
    switch(cs) {
      case InWhiteSpace:
      {
        // if this letter is not whitespace,
        if (!isspace(*p)) {
          // add it to the running buffer
          token.m_strValue = (*p);
          // switch to appropriate case
          if ((*p) == '/' && *(p+1) == '/') cs = InComment;
          else cs = (*p == '\"') ? InQuote : InText;
        } // end if
      }
      break;

      case InText:
      {
        // if this letter is whitespace
        if (isspace(*p)) {
          // add the completed token to the vector
          AddToken(token);
          // switch to whitespace case
          cs = InWhiteSpace;
        } else { 
          // if this letter is a token terminator
          if ((*p) == '(' || (*p) == ')' || (*p) == ',' || (*p) == '\"' || (*p) == '{' || (*p) == '}' || (*p) == '/') {
            if ((*p) == '/' && *(p+1) == '/') cs = InComment;
            else {
              // add the completed token to the vector
              DetermineTokenType(token);
              AddToken(token);
            
              // if it was a quote, transition to InQuote state
              if ((*p) == '\"') cs = InQuote; 
              // otherwise, process this one char as a token 
              else {
                token.m_strValue = (*p);
                AddToken(token);
                token.m_strValue = "";
              }
            }
          } else {
            // add this letter to the work in progress token
            token.m_strValue += (*p);
          } // end if
        } // end if
      }
      break;

      case InComment:
      {
        // C++ style comments - skip everything until end of line
        if (*p == '\n') { token.m_strValue = ""; cs = InWhiteSpace; }
      }
      break;

      case InQuote:
      {
        // unconditionally add this letter to the token until we hit a close quote
        token.m_strValue += (*p);
        if (*p == '\"') {
          AddToken(token);
          
          // go back to whitespace
          cs = InWhiteSpace;
        }
      }
      break;
    }
    p++;
  }
  AddToken(token);
}
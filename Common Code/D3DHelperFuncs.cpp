/*
#############################################################################

  D3DHelperFuncs.cpp - a set of my (Mason's) helper functions for dealing
  with Direct3D.

#############################################################################
*/

// Include Directives ///////////////////////////////////////////////////////
#include "D3DHelperFuncs.h"
// Local Constants, namespace usage /////////////////////////////////////////

// Local Variables / functions //////////////////////////////////////////////

/****************************************************************************

 SurfFormatToString: converts a D3DFMT_ enumeration into a string.  

 ****************************************************************************/
char *SurfFormatToString(D3DFORMAT srcformat)
{
  switch(srcformat) {
    case D3DFMT_UNKNOWN:      return("D3DFMT_UNKNOWN");      
                             
    case D3DFMT_R8G8B8:       return("D3DFMT_R8G8B8");       
    case D3DFMT_A8R8G8B8:     return("D3DFMT_A8R8G8B8");     
    case D3DFMT_X8R8G8B8:     return("D3DFMT_X8R8G8B8");     
    case D3DFMT_R5G6B5:       return("D3DFMT_R5G6B5");       
    case D3DFMT_X1R5G5B5:     return("D3DFMT_X1R5G5B5");     
    case D3DFMT_A1R5G5B5:     return("D3DFMT_A1R5G5B5");     
    case D3DFMT_A4R4G4B4:     return("D3DFMT_A4R4G4B4");     
    case D3DFMT_R3G3B2:       return("D3DFMT_R3G3B2");       
    case D3DFMT_A8:           return("D3DFMT_A8");           
    case D3DFMT_A8R3G3B2:     return("D3DFMT_A8R3G3B2");     
    case D3DFMT_X4R4G4B4:     return("D3DFMT_X4R4G4B4");     
                             
    case D3DFMT_A8P8:         return("D3DFMT_A8P8");         
    case D3DFMT_P8:           return("D3DFMT_P8");           
                             
    case D3DFMT_L8:           return("D3DFMT_L8");           
    case D3DFMT_A8L8:         return("D3DFMT_A8L8");         
    case D3DFMT_A4L4:         return("D3DFMT_A4L4");         
                             
    case D3DFMT_V8U8:         return("D3DFMT_V8U8");         
    case D3DFMT_L6V5U5:       return("D3DFMT_L6V5U5");       
    case D3DFMT_X8L8V8U8:     return("D3DFMT_X8L8V8U8");     
    case D3DFMT_Q8W8V8U8:     return("D3DFMT_Q8W8V8U8");     
    case D3DFMT_V16U16:       return("D3DFMT_V16U16");       
    case D3DFMT_W11V11U10:    return("D3DFMT_W11V11U10");    
                                                                 
    case D3DFMT_UYVY:         return("D3DFMT_UYVY");         
    case D3DFMT_YUY2:         return("D3DFMT_YUY2");         
    case D3DFMT_DXT1:         return("D3DFMT_DXT1");         
    case D3DFMT_DXT2:         return("D3DFMT_DXT2");         
    case D3DFMT_DXT3:         return("D3DFMT_DXT3");         
    case D3DFMT_DXT4:         return("D3DFMT_DXT4");         
    case D3DFMT_DXT5:         return("D3DFMT_DXT5");         

    case D3DFMT_D16_LOCKABLE: return("D3DFMT_D16_LOCKABLE"); 
    case D3DFMT_D32:          return("D3DFMT_D32");          
    case D3DFMT_D15S1:        return("D3DFMT_D15S1");        
    case D3DFMT_D24S8:        return("D3DFMT_D24S8");        
    case D3DFMT_D16:          return("D3DFMT_D16");          
    case D3DFMT_D24X8:        return("D3DFMT_D24X8");        
    case D3DFMT_D24X4S4:      return("D3DFMT_D24X4S4");      

    case D3DFMT_VERTEXDATA:   return("D3DFMT_VERTEXDATA"); 
    case D3DFMT_INDEX16:      return("D3DFMT_INDEX16"); 
    case D3DFMT_INDEX32:      return("D3DFMT_INDEX32"); 
    default:                  return("**UNKNOWN**"); 
  };
  return("");
}

HRESULT CreateQuad(LPDIRECT3DVERTEXBUFFER8 *pVB, D3DPOOL pool, float fSize, 
                   DWORD dwColor, LPDIRECT3DDEVICE8 pDev,
                   float fTexTileX, float fTexTileY)
{
  HRESULT hr;

  if(FAILED(hr = pDev->CreateVertexBuffer(6*sizeof(VERTEX_XYZ_DIFFUSE_TEX1),
                  0, D3DFVF_XYZ_DIFFUSE_TEX1, pool, pVB)))
    return hr;

  VERTEX_XYZ_DIFFUSE_TEX1 *pVertices;
  float fSizeDiv2 = fSize/2;

  if( FAILED( hr = (*pVB)->Lock( 0, 6*sizeof(VERTEX_XYZ_DIFFUSE_TEX1), (BYTE**)&pVertices, 0 ) ) )
      return hr;

  // first triangle
  pVertices[0].position = D3DXVECTOR3(-fSizeDiv2, fSizeDiv2, 0.0f);
  pVertices[0].color    = dwColor;
  pVertices[0].tu       = 0.0f;
  pVertices[0].tv       = 0.0f;

  pVertices[1].position = D3DXVECTOR3(fSizeDiv2, fSizeDiv2, 0.0f);
  pVertices[1].color    = dwColor;
  pVertices[1].tu       = fTexTileX;
  pVertices[1].tv       = 0.0f;

  pVertices[2].position = D3DXVECTOR3(fSizeDiv2, -fSizeDiv2, 0.0f);
  pVertices[2].color    = dwColor;
  pVertices[2].tu       = fTexTileX;
  pVertices[2].tv       = fTexTileY;

  // second triangle
  pVertices[3].position = D3DXVECTOR3(-fSizeDiv2, fSizeDiv2, 0.0f);
  pVertices[3].color    = dwColor;
  pVertices[3].tu       = 0.0f;
  pVertices[3].tv       = 0.0f;

  pVertices[4].position = D3DXVECTOR3(fSizeDiv2, -fSizeDiv2, 0.0f);
  pVertices[4].color    = dwColor;
  pVertices[4].tu       = fTexTileX;
  pVertices[4].tv       = fTexTileY;

  pVertices[5].position = D3DXVECTOR3(-fSizeDiv2, -fSizeDiv2, 0.0f);
  pVertices[5].color    = dwColor;
  pVertices[5].tu       = 0.0f;
  pVertices[5].tv       = fTexTileY;

  if( FAILED( hr = (*pVB)->Unlock() ) ) return hr;
  return(S_OK);
}

HRESULT CreateVertexGrid(LPDIRECT3DVERTEXBUFFER8 *pVB, LPDIRECT3DINDEXBUFFER8 *pIB,
                         D3DPOOL pool, float fTotalSize, DWORD dwColor, 
                         int iNumVerticesX, int iNumVerticesY, LPDIRECT3DDEVICE8 pDev)
{
  HRESULT hr;

  // create and fill vertex buffer
  {
    if(FAILED(hr = pDev->CreateVertexBuffer(iNumVerticesX*iNumVerticesY*sizeof(VERTEX_XYZ_DIFFUSE_TEX1),
                    0, D3DFVF_XYZ_DIFFUSE_TEX1, pool, pVB)))
      return hr;

    VERTEX_XYZ_DIFFUSE_TEX1 *pVertices;
    float fSizeDiv2 = fTotalSize/2;

    if( FAILED( hr = (*pVB)->Lock( 0, iNumVerticesX*iNumVerticesY*sizeof(VERTEX_XYZ_DIFFUSE_TEX1), (BYTE**)&pVertices, 0 ) ) )
        return hr;

    for (int x=0; x < iNumVerticesX; x++) {
      for (int y=0; y < iNumVerticesY; y++) {
        pVertices[(y*iNumVerticesX)+x].position = D3DXVECTOR3(
          (iNumVerticesX > 1) ? (((float)x/(float)(iNumVerticesX-1))*fTotalSize)-fSizeDiv2 : 0, 
          (iNumVerticesY > 1) ? (((float)(iNumVerticesY-1-y)/(float)(iNumVerticesY-1))*fTotalSize)-fSizeDiv2 : 0, 
          0.0f);

        pVertices[(y*iNumVerticesX)+x].color    = dwColor;
        pVertices[(y*iNumVerticesX)+x].tu       = (float)x/(float)(iNumVerticesX-1);
        pVertices[(y*iNumVerticesX)+x].tv       = (float)y/(float)(iNumVerticesY-1);
      }
    }
  }

  // create index buffer
  {
    if(FAILED(hr = pDev->CreateIndexBuffer(
        iNumVerticesX*iNumVerticesY*2*3*2, // *2 (tris) *3 (indicies per tri) * 2 (bytes per index)
        D3DUSAGE_WRITEONLY, D3DFMT_INDEX16,
        D3DPOOL_MANAGED, pIB)))
      return hr;

    // lock and fill index buffer

    WORD *pIndices;
    if(FAILED(hr = (*pIB)->Lock(0, iNumVerticesX*iNumVerticesY*2*3*2, (unsigned char **)&pIndices, 
      0)))
      return hr;
  
    WORD *pIndex = pIndices;
    for (int x=0; x < iNumVerticesX-1; x++) {
      for (int y=0; y < iNumVerticesY-1; y++) {
        // first triangle
        *(pIndex++) = ((y)*iNumVerticesX)+x;
        *(pIndex++) = ((y)*iNumVerticesX)+x+1;
        *(pIndex++) = ((y+1)*iNumVerticesX)+x+1;

        // second triangle
        *(pIndex++) = ((y)*iNumVerticesX)+x;
        *(pIndex++) = ((y+1)*iNumVerticesX)+x+1;
        *(pIndex++) = ((y+1)*iNumVerticesX)+x;
      }
    }

    if(FAILED(hr = (*pIB)->Unlock())) return hr;
    if(FAILED(hr = (*pVB)->Unlock())) return hr;
  }
  return(S_OK);

}

HRESULT CreateVertexFrame(LPDIRECT3DVERTEXBUFFER8 *pVB, LPDIRECT3DINDEXBUFFER8 *pIB,
                         D3DPOOL pool, float fTotalSize, DWORD dwColor, 
                         int iNumVerticesX, int iNumVerticesY, LPDIRECT3DDEVICE8 pDev)
{
  HRESULT hr;

  int x, y;

  // create and fill vertex buffer
  {
    if(FAILED(hr = pDev->CreateVertexBuffer((((iNumVerticesX-1)*2)+((iNumVerticesY-1)*2)+2)*sizeof(VERTEX_XYZ_DIFFUSE_TEX1),
      0, D3DFVF_XYZ_DIFFUSE_TEX1, pool, pVB)))
      return hr;

    VERTEX_XYZ_DIFFUSE_TEX1 *pVertices;
    float fSizeDiv2 = fTotalSize/2;

    if( FAILED( hr = (*pVB)->Lock( 0, (((iNumVerticesX-1)*2)+((iNumVerticesY-1)*2)+1)*sizeof(VERTEX_XYZ_DIFFUSE_TEX1), (BYTE**)&pVertices, 0 ) ) )
        return hr;

    // top row
    for (x=0; x < iNumVerticesX; x++) {
      pVertices->position = D3DXVECTOR3(
        (((float)x/(float)(iNumVerticesX-1))*fTotalSize)-fSizeDiv2, 
        fSizeDiv2, 
        0.0f);

      pVertices->color    = dwColor;
      pVertices->tu       = (float)x/(float)(iNumVerticesX-1);
      pVertices->tv       = 0.0f;
      pVertices++;
    }
    
    // right row
    for (y=1; y < iNumVerticesY; y++) {
      pVertices->position = D3DXVECTOR3(
        fSizeDiv2, 
        (((float)(iNumVerticesY-1-y)/(float)(iNumVerticesY-1))*fTotalSize)-fSizeDiv2, 
        0.0f);

      pVertices->color    = dwColor;
      pVertices->tu       = 1.0f;
      pVertices->tv       = (float)y/(float)(iNumVerticesY-1);
      pVertices++;
    }

    // bottom row
    for (x=iNumVerticesX-1; x >= 1; x--) {
      pVertices->position = D3DXVECTOR3(
        (((float)x/(float)(iNumVerticesX-1))*fTotalSize)-fSizeDiv2, 
        -fSizeDiv2, 
        0.0f);

      pVertices->color    = dwColor;
      pVertices->tu       = (float)x/(float)(iNumVerticesX-1);
      pVertices->tv       = 1.0f;
      pVertices++;
    }
    
    // left row
    for (y=iNumVerticesY-1; y >= 1; y--) {
      pVertices->position = D3DXVECTOR3(
        -fSizeDiv2, 
        (((float)(iNumVerticesY-1-y)/(float)(iNumVerticesY-1))*fTotalSize)-fSizeDiv2, 
        0.0f);

      pVertices->color    = dwColor;
      pVertices->tu       = 0.0f;
      pVertices->tv       = (float)y/(float)(iNumVerticesY-1);
      pVertices++;
    }

    
    // last point in exact center
    pVertices->position = D3DXVECTOR3(0.0f,0.0f, 0.0f);
    pVertices->color    = dwColor;
    pVertices->tu       = 0.5f;
    pVertices->tv       = 0.5f;
    
  }

  // create index buffer
  {
    if(FAILED(hr = pDev->CreateIndexBuffer(
        (((iNumVerticesX-1)*2)+((iNumVerticesY-1)*2))*3*2, // *3 (indicies per tri) * 2 (bytes per index)
        D3DUSAGE_WRITEONLY, D3DFMT_INDEX16,
        D3DPOOL_MANAGED, pIB)))
      return hr;

    // lock and fill index buffer

    WORD *pIndices;
    if(FAILED(hr = (*pIB)->Lock(0, (((iNumVerticesX-1)*2)+((iNumVerticesY-1)*2))*3*2, (unsigned char **)&pIndices, 
      0)))
      return hr;
  
    // the triangles are arranged in a fan pattern... 
    // probably could have used tri fans here but GeForce class cards like tri lists better
    WORD *pIndex = pIndices;
    for (int q=0; q < (((iNumVerticesX-1)*2)+((iNumVerticesY-1)*2)); q++) {
      // first triangle
      *(pIndex++) = q;
      *(pIndex++) = ((iNumVerticesX-1)*2)+((iNumVerticesY-1)*2);
      *(pIndex++) = ((q+1 == ((iNumVerticesX-1)*2)+((iNumVerticesY-1)*2)) ? 0 : q+1);
    }

    if(FAILED(hr = (*pIB)->Unlock())) return hr;
    if(FAILED(hr = (*pVB)->Unlock())) return hr;
  }
  return(S_OK);

}


HRESULT RenderCopy(LPDIRECT3DTEXTURE8 ptexSource, LPDIRECT3DTEXTURE8 ptexDest,
                   int iDestWidth, int iDestHeight, LPDIRECT3DDEVICE8 pDev,
                   LPDIRECT3DVERTEXBUFFER8 pvbQuad)
{
  HRESULT hr;
  // get the current depth buffer (we have to pass this into SetRenderTarget
  // so we don't inadvertently drop our depth buffer.)
  LPDIRECT3DSURFACE8 pDepthSurf;
  pDev->GetDepthStencilSurface(&pDepthSurf);

  // get the current rendering target (we'll set it back later)
  LPDIRECT3DSURFACE8 pOldRenderTarget;
  pDev->GetRenderTarget(&pOldRenderTarget);

  // get surface interfaces
  LPDIRECT3DSURFACE8 psurfDest;
  ptexDest->GetSurfaceLevel(0, &psurfDest);

  // set new rendering target & clear
  pDev->SetRenderTarget(psurfDest, NULL);
  pDev->Clear( 0L, NULL, D3DCLEAR_TARGET, 0x000000, 1.0f, 0L );
  
  // turn off z buffering
  pDev->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
  
  // set up texture stages for simple texture stage copy
	pDev->SetTextureStageState(0, D3DTSS_COLOROP,   D3DTOP_SELECTARG1);
	pDev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	pDev->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
	pDev->SetTextureStageState(1, D3DTSS_COLOROP,   D3DTOP_DISABLE); 
	pDev->SetTextureStageState(0, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG1);

  SetupOrthoProjForRenderCopy(pDev, iDestWidth, iDestHeight);

  // set the source texture active
  pDev->SetTexture(0, ptexSource);

  // begin rendering the scene
  if (FAILED(hr = pDev->BeginScene())) return hr;
  pDev->SetStreamSource( 0, pvbQuad, sizeof(VERTEX_XYZ_DIFFUSE_TEX1));
  pDev->SetVertexShader( D3DFVF_XYZ_DIFFUSE_TEX1 );

  // this "blits" the texture
  pDev->DrawPrimitive( D3DPT_TRIANGLELIST, 0, 2 );

  // end scene
  pDev->EndScene();
  
  // release the dest surface 
  psurfDest->Release();

  // set the rendering target back to the old target.
  pDev->SetRenderTarget(pOldRenderTarget, pDepthSurf);
  pDev->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
  pOldRenderTarget->Release();

  // release the depth surface interface.
  pDepthSurf->Release();
  return S_OK;
}

HRESULT SmoothRenderCopy(LPDIRECT3DTEXTURE8 ptexSource, LPDIRECT3DTEXTURE8 ptexDest, 
                         int iDestWidth, int iDestHeight, LPDIRECT3DDEVICE8 pDev, 
                         LPDIRECT3DVERTEXBUFFER8 pvbQuad)
{
  // no alpha blending or lighting
  pDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	pDev->SetRenderState(D3DRS_LIGHTING, FALSE);

  // turn on bilinear filtering (this is what "smooths the noise")
  pDev->SetTextureStageState( 0, D3DTSS_MAGFILTER, D3DTEXF_LINEAR);
  pDev->SetTextureStageState( 0, D3DTSS_MINFILTER, D3DTEXF_LINEAR);
  pDev->SetTextureStageState( 0, D3DTSS_MIPFILTER, D3DTEXF_LINEAR);
  
  // render-copy the texture
  return(RenderCopy(ptexSource, ptexDest, iDestWidth, iDestHeight, pDev, pvbQuad));
}

HRESULT SetupOrthoProjForRenderCopy(LPDIRECT3DDEVICE8 pDev, int iWidth, int iHeight)
{
  // set up orthogonal projection matrix, so that one texel = one pixel in the final
  // image, with no perspective correction.
  D3DXMATRIX projmat;
  D3DXMatrixOrthoLH(&projmat, (float)iWidth, (float)iHeight, 0.0, 100.0);
  pDev->SetTransform( D3DTS_PROJECTION, &projmat );

  // set up world matrix so that it rotates the quad slightly.
  D3DXMATRIX worldmattemp, worldmat, transmat;
  D3DXMatrixScaling(&worldmattemp, (float)iWidth/2.0f, (float)iHeight/2.0f, 1.0);

  // move the quad left and up 0.5 units, so that the texels are perfectly
  // centered on the screen pixels.
  D3DXMatrixMultiply(&worldmat, &worldmattemp, D3DXMatrixTranslation(&transmat, -0.5f, -0.5f, 0));

  pDev->SetTransform( D3DTS_WORLD, &worldmat );

  // set up view matrix
  D3DXMATRIX viewmat;
  D3DXMatrixIdentity(&viewmat);
  pDev->SetTransform( D3DTS_VIEW, &viewmat );
  return S_OK;
}

//-----------------------------------------------------------------------------
// Name: DoesRayIntersectTriangle()
// Desc: Given a ray origin (orig) and direction (dir), and three vertices of
//       of a triangle, this function returns TRUE and the interpolated texture
//       coordinates if the ray intersects the triangle
//-----------------------------------------------------------------------------
bool DoesRayIntersectTriangle( const D3DXVECTOR3& orig,
                               const D3DXVECTOR3& dir, D3DXVECTOR3& v0,
                               D3DXVECTOR3& v1, D3DXVECTOR3& v2,
                               FLOAT* t, FLOAT* u, FLOAT* v )
{
    // Find vectors for two edges sharing vert0
    D3DXVECTOR3 edge1 = v1 - v0;
    D3DXVECTOR3 edge2 = v2 - v0;

    // Begin calculating determinant - also used to calculate U parameter
    D3DXVECTOR3 pvec;
    D3DXVec3Cross( &pvec, &dir, &edge2 );

    // If determinant is near zero, ray lies in plane of triangle
    FLOAT det = D3DXVec3Dot( &edge1, &pvec );
    if( det < 0.0001f )
        return false;

    // Calculate distance from vert0 to ray origin
    D3DXVECTOR3 tvec = orig - v0;

    // Calculate U parameter and test bounds
    *u = D3DXVec3Dot( &tvec, &pvec );
    if( *u < 0.0f || *u > det )
        return false;

    // Prepare to test V parameter
    D3DXVECTOR3 qvec;
    D3DXVec3Cross( &qvec, &tvec, &edge1 );

    // Calculate V parameter and test bounds
    *v = D3DXVec3Dot( &dir, &qvec );
    if( *v < 0.0f || *u + *v > det )
        return false;

    // Calculate t, scale parameters, ray intersects triangle
    *t = D3DXVec3Dot( &edge2, &qvec );
    FLOAT fInvDet = 1.0f / det;
    *t *= fInvDet;
    *u *= fInvDet;
    *v *= fInvDet;

    return true;
}

HRESULT CreateShader(LPDIRECT3DDEVICE8 pDev, const char *strFilename, 
                     DWORD *pDecl, DWORD &dwID)
{
  HRESULT hr;
  LPD3DXBUFFER pCode;

  // Assemble the shader from the file
  if( FAILED( hr = D3DXAssembleShaderFromFile( strFilename, 
                                               0, NULL, &pCode, NULL ) ) )
    return hr;

  // if PDecl is not null, we know this is a vertex shader.
  if (pDecl) {
    hr = pDev->CreateVertexShader( pDecl, 
      (DWORD*)pCode->GetBufferPointer(), &dwID, 0 );
  }
  else {
    // it's a pixel shader!
    hr = pDev->CreatePixelShader((DWORD *)pCode->GetBufferPointer(), &dwID);
  }
  
  pCode->Release();
  if( FAILED(hr) ) return hr;
  return(S_OK);
}

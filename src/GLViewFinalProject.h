#pragma once

#include "GLView.h"
#include "PxPhysicsAPI.h"

namespace Aftr
{
   class Camera;

/**
   \class GLViewFinalProject
   \author Scott Nykl 
   \brief A child of an abstract GLView. This class is the top-most manager of the module.

   Read \see GLView for important constructor and init information.

   \see GLView

    \{
*/

class GLViewFinalProject : public GLView
{
public:
   static GLViewFinalProject* New( const std::vector< std::string >& outArgs );
   virtual ~GLViewFinalProject();
   virtual void updateWorld(); ///< Called once per frame
   virtual void loadMap(); ///< Called once at startup to build this module's scene
   virtual void createFinalProjectWayPoints();
   virtual void onResizeWindow( GLsizei width, GLsizei height );
   virtual void onMouseDown( const SDL_MouseButtonEvent& e );
   virtual void onMouseUp( const SDL_MouseButtonEvent& e );
   virtual void onMouseMove( const SDL_MouseMotionEvent& e );
   virtual void onKeyDown( const SDL_KeyboardEvent& key );
   virtual void onKeyUp( const SDL_KeyboardEvent& key );

   WO* skybox;
   WO* table;
   float pitchX = 0.0f;
   float pitchY = 0.0f;
   float pitchZ = 0.0f;

   bool wPressed = false;
   bool aPressed = false;
    bool sPressed = false;
    bool dPressed = false;

    //Physics Stuff
    physx::PxDefaultAllocator a;
    physx::PxDefaultErrorCallback e;
    physx::PxFoundation* f;
    physx::PxPhysics* p;
    physx::PxScene* scene;

protected:
   GLViewFinalProject( const std::vector< std::string >& args );
   virtual void onCreate();   
};

/** \} */

} //namespace Aftr

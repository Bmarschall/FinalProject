#include "GLViewFinalProject.h"

#include "WorldList.h" //This is where we place all of our WOs
#include "ManagerOpenGLState.h" //We can change OpenGL State attributes with this
#include "Axes.h" //We can set Axes to on/off with this
#include "PhysicsEngineODE.h"

//Different WO used by this module
#include "WO.h"
#include "WOStatic.h"
#include "WOStaticPlane.h"
#include "WOStaticTrimesh.h"
#include "WOTrimesh.h"
#include "WOHumanCyborg.h"
#include "WOHumanCal3DPaladin.h"
#include "WOWayPointSpherical.h"
#include "WOLight.h"
#include "WOSkyBox.h"
#include "WOCar1970sBeater.h"
#include "Camera.h"
#include "CameraStandard.h"
#include "CameraChaseActorSmooth.h"
#include "CameraChaseActorAbsNormal.h"
#include "CameraChaseActorRelNormal.h"
#include "Model.h"
#include "ModelDataShared.h"
#include "ModelMesh.h"
#include "ModelMeshDataShared.h"
#include "ModelMeshSkin.h"
#include "WONVStaticPlane.h"
#include "WONVPhysX.h"
#include "WONVDynSphere.h"
#include "WOImGui.h" //GUI Demos also need to #include "AftrImGuiIncludes.h"
#include "AftrImGuiIncludes.h"
#include "AftrGLRendererBase.h"
#include "PxPhysicsAPI.h"
#include "WOphysx.h"
#include "irrKlang.h"
#include "Gooey.h"
#include "Model.h"

using namespace std;

using namespace Aftr;
float MAX_TILT = 15; // max board tilt in degrees
float TILT_SPEED = 1;

GLViewFinalProject* GLViewFinalProject::New( const std::vector< std::string >& args ){
   GLViewFinalProject* glv = new GLViewFinalProject( args );
   glv->init( Aftr::GRAVITY, Vector( 0, 0, -1.0f ), "aftr.conf", PHYSICS_ENGINE_TYPE::petODE );
   glv->onCreate();
   return glv;
}

GLViewFinalProject::GLViewFinalProject( const std::vector< std::string >& args ) : GLView( args ){
   //Initialize any member variables that need to be used inside of LoadMap() here.
   //Note: At this point, the Managers are not yet initialized. The Engine initialization
   //occurs immediately after this method returns (see GLViewFinalProject::New() for
   //reference). Then the engine invoke's GLView::loadMap() for this module.
   //After loadMap() returns, GLView::onCreate is finally invoked.

   //The order of execution of a module startup:
   //GLView::New() is invoked:
   //    calls GLView::init()
   //       calls GLView::loadMap() (as well as initializing the engine's Managers)
   //    calls GLView::onCreate()

   //GLViewFinalProject::onCreate() is invoked after this module's LoadMap() is completed.
   
   //Physics Dec
    {
        f = PxCreateFoundation(PX_PHYSICS_VERSION, a, e);
        p = PxCreatePhysics(PX_PHYSICS_VERSION, *f, PxTolerancesScale(), false, NULL);
        PxSceneDesc sc(p->getTolerancesScale());
        sc.filterShader = physx::PxDefaultSimulationFilterShader;
        sc.cpuDispatcher = physx::PxDefaultCpuDispatcherCreate(2);
        scene = p->createScene(sc);
        scene->setFlag(PxSceneFlag::eENABLE_ACTIVE_ACTORS, true);
        scene->setGravity(PxVec3(0.0, 0.0, -9.81));
    }
    doSound();
}

void GLViewFinalProject::onCreate(){
   //GLViewFinalProject::onCreate() is invoked after this module's LoadMap() is completed.
   //At this point, all the managers are initialized. That is, the engine is fully initialized.

   if( this->pe != NULL ){
      //optionally, change gravity direction and magnitude here
      //The user could load these values from the module's aftr.conf
      this->pe->setGravityNormalizedVector( Vector( 0,0,-1.0f ) );
      this->pe->setGravityScalar( Aftr::GRAVITY );
   }
   this->setActorChaseType( STANDARDEZNAV ); //Default is STANDARDEZNAV mode
   //this->setNumPhysicsStepsPerRender( 0 ); //pause physics engine on start up; will remain paused till set to 1
}

GLViewFinalProject::~GLViewFinalProject() { /* Implicitly calls GLView::~GLView() */ }

void GLViewFinalProject::updateWorld() {
   GLView::updateWorld(); //Just call the parent's update world first.
                          //If you want to add additional functionality, do it after
                          //this call.

   soundUpdate();
   ballUpdate();

   MAX_TILT = gui->MAX_TILT;
   TILT_SPEED = gui->TILT_SPEED;
   scene->setGravity(PxVec3(gui->XGravity, gui->YGravity, gui->Gravity));

   if (this->wPressed) {
       if (this->pitchX > -MAX_TILT) {
           this->pitchX -= TILT_SPEED;
           this->table->rotateAboutRelX(-TILT_SPEED * DEGtoRAD);
           this->skybox->rotateAboutRelX(-TILT_SPEED * DEGtoRAD);
       }
   }

   if (this->aPressed) {
       if (this->pitchY > -MAX_TILT) {
           this->pitchY -= TILT_SPEED;
           this->table->rotateAboutRelY(-TILT_SPEED * DEGtoRAD);
           this->skybox->rotateAboutRelY(-TILT_SPEED * DEGtoRAD);
       }
   }

   if (this->sPressed) {
       if (this->pitchX < MAX_TILT) {
           this->pitchX += TILT_SPEED;
           this->table->rotateAboutRelX(TILT_SPEED * DEGtoRAD);
           this->skybox->rotateAboutRelX(TILT_SPEED * DEGtoRAD);
       }
   }

   if (this->dPressed) {
       if (this->pitchY < MAX_TILT) {
           this->pitchY += TILT_SPEED;
           this->table->rotateAboutRelY(TILT_SPEED * DEGtoRAD);
           this->skybox->rotateAboutRelY(TILT_SPEED * DEGtoRAD);
       }
   }

   if (
       this->wPressed == false &&
       this->aPressed == false &&
       this->sPressed == false &&
       this->dPressed == false
       ) {
       this->pitchX = this->pitchX + (0.0 - this->pitchX) * 0.03;
       this->pitchY = this->pitchY + (0.0 - this->pitchY) * 0.03;
       this->pitchZ = this->pitchZ + (0.0 - this->pitchZ) * 0.03;

       this->table->rotateToIdentity();
       this->skybox->rotateToIdentity();
       this->table->rotateAboutRelX(this->pitchX * DEGtoRAD);
       this->skybox->rotateAboutRelX(this->pitchX * DEGtoRAD);
       this->table->rotateAboutRelY(this->pitchY * DEGtoRAD);
       this->skybox->rotateAboutRelY(this->pitchY * DEGtoRAD);
       this->table->rotateAboutRelY(this->pitchZ * DEGtoRAD);
       this->skybox->rotateAboutRelY(this->pitchZ * DEGtoRAD);
   }

   //Physics Simulation
   {
       scene->simulate(1.0f / 60.0f);

       PxU32 errorState = 0;
       scene->fetchResults(true);

       {
           physx::PxU32 numActors = 0;
           physx::PxActor** actors = scene->getActiveActors(numActors);
           //make sure you set physx::PxSceneFlag::eENABLE_ACTIVE_ACTORS, true in your scene!
           //poses that have changed since the last update (scene->setFlag)
           //std::cout << numActors << std::endl;
           for (physx::PxU32 i = 0; i < numActors; ++i)
           {
               physx::PxActor* actor = actors[i];
               if (actor->userData != NULL)
               {
                   WOphysx* wo = static_cast<WOphysx*>(actor->userData);
                   //at some point earlier the actor?s userdata was set to the corresponding WO?s pointer
                   //std::cout << "calling Physics Update" << std::endl;
                   wo->updatePoseFromPhysicsEngine(actor);//add this function to your inherited class
               }
               //else { std::cout << "Warn:: No Assosiated WO with actor" << i << std::endl; }
               //make sure it has the information it needs, either the actor or just the pose
           }
       }
   }

   //gui->Gravity = this->pitchZ * DEGtoRAD;
   gui->YGravity =  (this->pitchX * DEGtoRAD) * gui->rollMult;
   gui->XGravity = -1*(this->pitchY * DEGtoRAD) * gui->rollMult;//fix on of these

}

void GLViewFinalProject::onResizeWindow( GLsizei width, GLsizei height ){ GLView::onResizeWindow( width, height ); }
void GLViewFinalProject::onMouseDown( const SDL_MouseButtonEvent& e ){ GLView::onMouseDown( e ); }
void GLViewFinalProject::onMouseUp(const SDL_MouseButtonEvent& e) { GLView::onMouseUp(e); }
void GLViewFinalProject::onMouseMove( const SDL_MouseMotionEvent& e ){ GLView::onMouseMove( e ); }
void GLViewFinalProject::onKeyUp(const SDL_KeyboardEvent& key) { 
    GLView::onKeyUp(key); 
    if (key.keysym.sym == SDLK_a) { this->aPressed = false; }
    if (key.keysym.sym == SDLK_d) { this->dPressed = false; }
    if (key.keysym.sym == SDLK_w) { this->wPressed = false; }
    if (key.keysym.sym == SDLK_s) { this->sPressed = false; }

}

void GLViewFinalProject::onKeyDown( const SDL_KeyboardEvent& key ){
   GLView::onKeyDown( key );
   if( key.keysym.sym == SDLK_0 )
      this->setNumPhysicsStepsPerRender( 1 );
   if( key.keysym.sym == SDLK_1 ){}

   // WASD CONTROLS
   if (key.keysym.sym == SDLK_a) { this->aPressed = true; }
   if (key.keysym.sym == SDLK_d) { this->dPressed = true; }
   if (key.keysym.sym == SDLK_w) { this->wPressed = true; }
   if (key.keysym.sym == SDLK_s) { this->sPressed = true; }
}

void Aftr::GLViewFinalProject::loadMap(){
   this->worldLst = new WorldList(); //WorldList is a 'smart' vector that is used to store WO*'s
   this->actorLst = new WorldList();
   this->netLst = new WorldList();

   ManagerOpenGLState::GL_CLIPPING_PLANE = 1000.0;
   ManagerOpenGLState::GL_NEAR_PLANE = 0.1f;
   ManagerOpenGLState::enableFrustumCulling = false;
   Axes::isVisible = true;
   this->glRenderer->isUsingShadowMapping( false ); //set to TRUE to enable shadow mapping, must be using GL 3.2+

   this->cam->setPosition( 0, 75, 120 );
   this->cam->setCameraLookAtPoint({ 0,0,0 });

   std::string shinyRedPlasticCube( ManagerEnvironmentConfiguration::getSMM() + "/models/cube4x4x4redShinyPlastic_pp.wrl" );
   std::string sphere(ManagerEnvironmentConfiguration::getSMM() + "/models/sphereR5Earth.wrl");
   std::string wheeledCar( ManagerEnvironmentConfiguration::getSMM() + "/models/rcx_treads.wrl" );
   std::string grass( ManagerEnvironmentConfiguration::getSMM() + "/models/grassFloor400x400_pp.wrl" );
   std::string human( ManagerEnvironmentConfiguration::getSMM() + "/models/human_chest.wrl" );
   std::string maze(ManagerEnvironmentConfiguration::getLMM() + "/models/maze.stl");

   //std::string tableTexture(ManagerEnvironmentConfiguration::getLMM() + "models/table.jpg");
   
   {
      //Create a light
      float ga = 0.1f; //Global Ambient Light level for this module
      ManagerLight::setGlobalAmbientLight( aftrColor4f( ga, ga, ga, 1.0f ) );
      WOLight* light = WOLight::New();
      light->isDirectionalLight( true );
      light->setPosition( Vector( 0, 0, 100 ) );
      //Set the light's display matrix such that it casts light in a direction parallel to the -z axis (ie, downwards as though it was "high noon")
      //for shadow mapping to work, this->glRenderer->isUsingShadowMapping( true ), must be invoked.
      light->getModel()->setDisplayMatrix( Mat4::rotateIdentityMat( { 0, 1, 0 }, 90.0f * Aftr::DEGtoRAD ) );
      light->setLabel( "Light" );
      worldLst->push_back( light );
   }

   //SkyBox Textures readily available
   std::vector< std::string > skyBoxImageNames; //vector to store texture paths
   skyBoxImageNames.push_back(ManagerEnvironmentConfiguration::getLMM() + "skyboxes/room.jpg");

   {
      //Create the SkyBox
      WO* wo = WOSkyBox::New( skyBoxImageNames.at( 0 ), this->getCameraPtrPtr() );
      wo->setPosition( Vector( 0, 0, 0 ) );
      wo->setLabel( "Sky Box" );
      wo->renderOrderType = RENDER_ORDER_TYPE::roOPAQUE;
      worldLst->push_back( wo );
      this->skybox = wo;
   }

   { 
      ////Create the infinite grass plane (the floor)
      WO* wo = WO::New( grass, Vector( 0.75, 0.75, 0.75 ), MESH_SHADING_TYPE::mstFLAT );
      wo->setPosition( Vector( 0, 0, 0 ) );
      wo->renderOrderType = RENDER_ORDER_TYPE::roOPAQUE;

      wo->upon_async_model_loaded( [wo]()
         {
            /*std::string tableTexturePath = ManagerEnvironmentConfiguration::getLMM() + "models/table.jpg";
            Texture* text = ManagerTexture::loadTexture(tableTexturePath);
            ModelMeshSkin tableSkin(text);*/
            
            ModelMeshSkin& tableSkin = wo->getModel()->getModelDataShared()->getModelMeshes().at(0)->getSkins().at(0);
            tableSkin.getMultiTextureSet().at( 0 )->setTextureRepeats( 5.0f );
            tableSkin.setAmbient( aftrColor4f( 0.4f, 0.4f, 0.4f, 1.0f ) ); //Color of object when it is not in any light
            tableSkin.setDiffuse( aftrColor4f( 1.0f, 1.0f, 1.0f, 1.0f ) ); //Diffuse color components (ie, matte shading color of this object)
            tableSkin.setSpecular( aftrColor4f( 0.4f, 0.4f, 0.4f, 1.0f ) ); //Specular color component (ie, how "shiney" it is)
            tableSkin.setSpecularCoefficient( 10 ); // How "sharp" are the specular highlights (bigger is sharper, 1000 is very sharp, 10 is very dull) 
            tableSkin.setMeshShadingType(MESH_SHADING_TYPE::mstFLAT);
            //wo->getModel()->getSkins().push_back(tableSkin);

            
          } );
      wo->setLabel( "Table" );
      worldLst->push_back( wo );

      this->table = wo;
   }


   {
      // maze object
      WOphysx* wo = WOphysx::New(maze, Vector( 1, 1, 1 ), MESH_SHADING_TYPE::mstFLAT,p,scene,"t",f);

      wo->upon_async_model_loaded([wo]()
          {
              wo->setPosition(Vector(0, 0, 40));
              wo->rotateAboutRelX(-90 * DEGtoRAD);
          });
      
      wo->renderOrderType = RENDER_ORDER_TYPE::roOPAQUE;
      wo->setLabel( "Maze" );
      worldLst->push_back( wo );

      this->maze = wo;
   }

   {
       ball = WOphysx::New(sphere, Vector(0.25, 0.25, 0.25), MESH_SHADING_TYPE::mstFLAT, p, scene,"s",f);
       ball->setPosition(Vector(0, 0.0f, 100));
       ball->renderOrderType = RENDER_ORDER_TYPE::roOPAQUE;
       ball->setLabel("Grass");
       //std::cout << "BALL INFO SIZE = " << ball->getModel()->getModelDataShared()->getCompositeVertexList().size() << std::endl;
       worldLst->push_back(ball);
   }
   {
       ////Create A Physics Plane
       //physx::PxMaterial* gMaterial = p->createMaterial(0.5f, 0.5f, 0.6f);
       //PxRigidStatic* groundPlane = PxCreatePlane(*p, PxPlane(0, 0, 1, 0), *gMaterial);
       //scene->addActor(*groundPlane);
   }
   
   Gooey* goo = Gooey::New(nullptr);
   goo->setLabel("Goo");
   goo->subscribe_drawImGuiWidget(
       [this, goo]() //this is a lambda, the capture clause is in [], the input argument list is in (), and the body is in {}
       {
           //ImGui::ShowDemoWindow(); //Displays the default ImGui demo from C:/repos/aburn/engine/src/imgui_implot/implot_demo.cpp
           Gooey::draw_gui(goo); //Displays a small Aftr Demo from C:/repos/aburn/engine/src/aftr/WOImGui.cpp
           //ImPlot::ShowDemoWindow(); //Displays the ImPlot demo using ImGui from C:/repos/aburn/engine/src/imgui_implot/implot_demo.cpp
       });
   this->worldLst->push_back(goo);

   gui = goo;


   createFinalProjectWayPoints();
}



void GLViewFinalProject::createFinalProjectWayPoints(){
    // Create a waypoint with a radius of 3, a frequency of 5 seconds, activated by GLView's camera, and is visible.
    WayPointParametersBase params(this);
    params.frequency = 5000;
    params.useCamera = true;
    params.visible = true;
    WOWayPointSpherical* wayPt = WOWayPointSpherical::New(params, 3);
    wayPt->setPosition(Vector(50, 0, 3));
    worldLst->push_back(wayPt);
}

void GLViewFinalProject::doSound() {

    twoDim = irrklang::createIrrKlangDevice();
    std::string filename(ManagerEnvironmentConfiguration::getLMM() + "Cool.wav");
    twoDimSoundSource = twoDim->addSoundSourceFromFile(filename.c_str());
    twoDimSound = twoDim->play2D(twoDimSoundSource, true, true, true, false);

    threeDim = irrklang::createIrrKlangDevice();
    std::string filename3(ManagerEnvironmentConfiguration::getLMM() + "ball_roll.mp3");
    threeDimSoundSource = threeDim->addSoundSourceFromFile(filename3.c_str());
    threeDimSound = threeDim->play3D(threeDimSoundSource, vec3df(0, 0, 0), true, true, true, false);

}

void GLViewFinalProject::soundUpdate() {
    twoDim->setSoundVolume(gui->get_2d_volume());

    Vector pos = ball->getPosition();
    vec3df cubePosition = { pos[0], pos[1], pos[2] };

    Vector camPos = cam->getPosition();
    vec3df cameraPosition = { camPos[0], camPos[1], camPos[2] };

    Vector lookDir = cam->getLookDirection();
    vec3df cameraLookDirection = { lookDir[0], lookDir[1], lookDir[2] };

    threeDim->setListenerPosition(cameraPosition, cameraLookDirection);

    threeDimSound->setPosition(cubePosition);

    gui->volume3 = 1 / (ball->getForce());

    if (gui->is3d) {
        threeDimSound->setIsPaused(false);
        gui->is3d = false;
    }
    if (gui->is2d) {
        twoDimSound->setIsPaused(false);
        gui->is2d = false;
    }

    //Pause

    if (gui->pause2d)
    {
        if (twoDimSound->getIsPaused())
        {
            twoDimSound->setIsPaused(false);
        }
        else {
            twoDimSound->setIsPaused(true);
        }
        gui->pause2d = false;
    }

    if (gui->pause3d)
    {
        if (threeDimSound->getIsPaused())
        {
            threeDimSound->setIsPaused(false);
        }
        else {
            threeDimSound->setIsPaused(true);
        }
        gui->pause3d = false;
    }

}

void GLViewFinalProject::ballUpdate() {


    if (gui-> dropBall)
    {
        ball->setPosition(Vector(0, 0.0f, gui->dropBallHeight));
        gui->dropBall = false;
    }

    if (gui-> resetBall)
    {
        ball->stopForce();
        ball->setPosition(Vector(0, 0.0f, 80.0f));//update with maze object implementation
        gui->resetBall = false;
    }
}
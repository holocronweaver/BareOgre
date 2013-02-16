#include "ExampleApplication.h"

ExampleApplication::ExampleApplication()
{
}

ExampleApplication::~ExampleApplication()
{
}

//-------------------------------------------------------------------------------------
void ExampleApplication::createScene()
{
  // Set the scene's ambient light
  mSceneMgr->setAmbientLight(Ogre::ColourValue(0.5f, 0.5f, 0.5f));

  // Create an Entity
  Ogre::Entity* ogreHead = mSceneMgr->createEntity("Head", "ogrehead.mesh");

  // Create a SceneNode and attach the Entity to it
  Ogre::SceneNode* headNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("HeadNode");
  headNode->attachObject(ogreHead);

  // Create a Light and set its position
  Ogre::Light* light = mSceneMgr->createLight("MainLight");
  light->setPosition(20.0f, 80.0f, 50.0f);
}

//==============================================================================
// Cross-platform boilerplate main()
//==============================================================================
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
  INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT )
#else
    int main(int argc, char *argv[]) 
#endif
  {
    ExampleApplication app;

    try {
      app.go();
    } catch( Ogre::Exception& e ) {
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
      MessageBox( NULL, e.getFullDescription().c_str(), 
                  "An exception has occured!", 
                  MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
      std::cerr << "An exception has occured: " <<
        e.getFullDescription().c_str() << std::endl;
#endif
    }

    return 0;
  }

#ifdef __cplusplus
}
#endif

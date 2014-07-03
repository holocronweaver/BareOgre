#include "BareOgre.h"

#include "OgreCamera.h"
#include "OgreOptionsFile.h"
#include "OgreEntity.h"
#include "OgreException.h"
#include "OgreSceneManager.h"
#include "OgreRenderWindow.h"
#include "OgreViewport.h"
#include "OgreWindowEventUtilities.h"

#include "OgreSceneNode.h"

#include "OgreMaterialManager.h"
#include "OgreTextureManager.h"

BareOgre::BareOgre()
    : mRoot(0)
    , mPluginsCfg("")
    , mResourcesCfg("")
    , mShutdown(false)
{
}


BareOgre::~BareOgre()
{
    // Remove ourself as a window listener.
    // Ogre::WindowEventUtilities::removeWindowEventListener(mWindow, this);
    // windowClosed(mWindow);

    SDL_Quit();

    delete mRoot;
}


bool BareOgre::go()
{
    mLog = new Ogre::LogManager;
    //TODO: Come up with a better log name.
    mLog->createLog("ogre.log");

#ifdef _DEBUG
    mResourcesCfg = "resources_d.cfg";
    mPluginsCfg = "plugins_d.cfg";
#else
    mResourcesCfg = "resources.cfg";
    mPluginsCfg = "plugins.cfg";
#endif

    mRoot = new Ogre::Root(mPluginsCfg);

    // Load resource paths from resources config file.
    setupResources();

    // Configure dialog.
    if (!(mRoot->restoreOptions() || mRoot->showOptionsDialog())) {
        return false;
    }

    SDL_Init(0);

    // Create RenderWindow.
    mWindow = mRoot->initialise(true, "BareOgre Render Window");

    // initVideo();
    // mWindow = mRoot->initialise(false);

    // struct SDL_SysWMinfo wmInfo;
    // SDL_VERSION(&wmInfo.version);

    // if (SDL_GetWindowWMInfo(mSDLWindow, &wmInfo) == SDL_FALSE)
    //     throw std::runtime_error("Couldn't get WM Info!");

    // Ogre::String winHandle;

    // switch (wmInfo.subsystem)
    // {
    // case SDL_SYSWM_X11:
    //     winHandle = Ogre::StringConverter::toString((unsigned long)wmInfo.info.x11.window);
    //     break;
    // default:
    //     throw std::runtime_error("Unexpected WM!");
    //     break;
    // }

    // Ogre::NameValuePairList params;
    // params.insert(std::make_pair("title", "OGRE Window"));
    // params.insert(std::make_pair("FSAA", "0"));
    // params.insert(std::make_pair("vsync", "false"));
    // // params.insert(std::make_pair("parentWindowHandle", winHandle));
    // params.insert(std::make_pair("externalWindowHandle", winHandle));

    // mWindow = Ogre::Root::getSingleton().createRenderWindow("OGRE Window", 640, 480, false, &params);
    // mWindow->setVisible(true);
    // mWindow->setActive(true);
    // mWindow->setAutoUpdated(false);

    // Initialize input. (SDL2)
    initInput();

    // Set default mipmap level.
    Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);

    // Initialize all resource groups.
    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

    // Create scene.
    createScene();

    // Set initial mouse clipping size.
    // windowResized(mWindow);

    // Register as a Window listener.
    // Ogre::WindowEventUtilities::addWindowEventListener(mWindow, this);

    // Start render loop.
    mLog->logMessage("*** Begin Rendering ***");
    mRoot->addFrameListener(this);
    mRoot->startRendering();
}


void BareOgre::createScene()
{
    // Create SceneManager.
    mSceneMgr = mRoot->createSceneManager("DefaultSceneManager");

    // Create a camera.
    mCamera = mSceneMgr->createCamera("PlayerCam");
    mCamera->setPosition(Ogre::Vector3(0,0,80));
    mCamera->lookAt(Ogre::Vector3(0,0,-3));
    mCamera->setNearClipDistance(5);
    mCamNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("CamNode");
    mCamNode->attachObject(mCamera);

    // Create one viewport, entire window.
    Ogre::Viewport* vp = mWindow->addViewport(mCamera);
    vp->setBackgroundColour(Ogre::ColourValue(0,0,0));

    // Set camera aspect ratio to match viewport.
    mCamera->setAspectRatio(
                            Ogre::Real(vp->getActualWidth() / Ogre::Real(vp->getActualHeight())));

    // Add lighting.
    //mSceneMgr->setAmbientLight(Ogre::ColourValue(0.5,0.5,0.5));
    mSceneMgr->setAmbientLight(Ogre::ColourValue(1.0,1.0,1.0));


    Ogre::Light* l = mSceneMgr->createLight("MainLight");
    l->setPosition(20,80,50);

    // Add meshes.
    Ogre::Entity* ogreHead = mSceneMgr->createEntity("Head", "ogrehead.mesh");
    Ogre::SceneNode* headNode =
        mSceneMgr->getRootSceneNode()->createChildSceneNode();
    headNode->attachObject(ogreHead);

    // Set player control parameters.
    mRotate = 0.13;
    mMove = 250;
    mDirection = Ogre::Vector3::ZERO;
}


// void BareOgre::initInput()
// {
//     mLog->logMessage("*** Initializing OIS ***");
//     OIS::ParamList pl;
//     size_t windowHnd = 0;
//     std::ostringstream windowHndStr;

//     mWindow->getCustomAttribute("WINDOW", &windowHnd);
//     windowHndStr << windowHnd;
//     pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));

//     //TODO: Remove me or integrate further.
//     bool nograb = true;
//     if (nograb)
//     {
//         pl.insert(std::make_pair("x11_keyboard_grab", "false"));
//         pl.insert(std::make_pair("x11_mouse_grab", "false"));
//         pl.insert(std::make_pair("w32_mouse", "DISCL_FOREGROUND"));
//         pl.insert(std::make_pair("w32_mouse", "DISCL_NONEXCLUSIVE"));
//         pl.insert(std::make_pair("w32_keyboard", "DISCL_FOREGROUND"));
//         pl.insert(std::make_pair("w32_keyboard", "DISCL_NONEXCLUSIVE"));
//     }


//     mInputManager = OIS::InputManager::createInputSystem(pl);
//     //TODO switch to buffered input
//     mKeyboard = static_cast<OIS::Keyboard*>(
//                                             mInputManager->createInputObject(OIS::OISKeyboard, true));
//     mMouse = static_cast<OIS::Mouse*>(
//                                       mInputManager->createInputObject(OIS::OISMouse, true));

//     mKeyboard->setEventCallback(this);
//     mMouse->setEventCallback(this);
// }

// void BareOgre::initVideo()
// {
//     mLog->logMessage("*** Initializing SDL2 Video ***");

//     // if (SDL_InitSubSystem(SDL_INIT_VIDEO) != 0)
//     // {
//     //     mLog->logMessage("error: SDL2 input failed to initialize!");
//     //     //TODO: error handling.
//     // }

//     // Create an application window with the following settings:
//     mSDLWindow = SDL_CreateWindow(
//                                   "An SDL2 window", // window title
//                                   SDL_WINDOWPOS_UNDEFINED, // initial x position
//                                   SDL_WINDOWPOS_UNDEFINED, // initial y position
//                                   640, // width, in pixels
//                                   480, // height, in pixels
//                                   SDL_WINDOW_OPENGL | SDL_WINDOW_INPUT_GRABBED // flags - see below
//                                   );

//     // Check that the window was successfully made
//     if (mSDLWindow == NULL) {
//         // In the event that the window could not be made...
//         std::cout << "Could not create window: " << SDL_GetError() << std::endl;
//     }
// }
    

void BareOgre::initInput()
{
    mLog->logMessage("*** Initializing SDL2 Input ***");

    if (SDL_InitSubSystem(SDL_INIT_EVENTS) != 0)
    {
        mLog->logMessage("error: SDL2 input failed to initialize!");
        //TODO: error handling.
    }
}

    
void BareOgre::setupResources()
{
    Ogre::OptionsFile cf;
    cf.load(mResourcesCfg);

    //TODO convert to C++11
    Ogre::OptionsFile::SectionIterator seci = cf.getSectionIterator();
    Ogre::String secName, typeName, archName;
    while (seci.hasMoreElements()) {
        secName = seci.peekNextKey();
        Ogre::OptionsFile::OptionsMultiMap *settings = seci.getNext();
        Ogre::OptionsFile::OptionsMultiMap::iterator i;
        for (i = settings->begin(); i != settings->end(); i++) {
            typeName = i->first;
            archName = i->second;
            Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
                                                                           archName, typeName, secName);
        }
    }
}

    
bool BareOgre::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
    if (mWindow->isClosed()) return false;
    if (mShutdown) return false;

    // Need to capture/update each device.
    processInput();

    mCamNode->translate(mDirection * evt.timeSinceLastFrame,
                        Ogre::Node::TS_LOCAL);

    return true;
}


// Adjust mouse clipping area.
// void BareOgre::windowResized(Ogre::RenderWindow* rw)
// {
//     unsigned int width, height, depth;
//     int left, top;
//     rw->getMetrics(width, height, depth, left, top);

//     // const OIS::MouseState &ms = mMouse->getMouseState();
//     // ms.width = width;
//     // ms.height = height;
// }


// // Unattach OIS before window shutdown (very important under Linux).
// void BareOgre::windowClosed(Ogre::RenderWindow* rw)
// {
//     SDL_Quit();
// }


//==============================================================================
// User Input (SDL2)
//==============================================================================
void BareOgre::processInput()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_QUIT:
            std::cout << "Quit!" << std::endl;
            mShutdown = true;
            break;
        case SDL_KEYDOWN:
            if (event.key.repeat) break;
            std::cout << "Key down!" << std::endl;
            keyPressed(event.key);
            break;
        case SDL_KEYUP:
            std::cout << "Key up!" << std::endl;
            keyReleased(event.key);
            break;
        case SDL_MOUSEBUTTONDOWN:
            std::cout << "Mouse button down!" << std::endl;
            break;
        case SDL_MOUSEMOTION:
            mouseMoved();
            break;
            // default:
            //     std::cout << "Got somethin!" << std::endl;
        }
    }
}


void BareOgre::mouseMoved()
{
    //if (evt.state.buttonDown(OIS::MB_Right)) {
    int dx, dy;
    SDL_GetRelativeMouseState(&dx, &dy);
    mCamNode->yaw(Ogre::Degree(-mRotate * dx),
                  Ogre::Node::TS_WORLD);
    mCamNode->pitch(Ogre::Degree(-mRotate * dy),
                    Ogre::Node::TS_LOCAL);
    //}
}


void BareOgre::keyPressed(SDL_KeyboardEvent event) {
    SDL_Keysym key = event.keysym;

    switch (key.scancode)
    {
    case SDL_SCANCODE_ESCAPE:
        mShutdown = true;
        break;

    case SDL_SCANCODE_PRINTSCREEN:
        mWindow->writeContentsToTimestampedFile("screenshot_", ".jpg");
        break;

    case SDL_SCANCODE_F5:
        Ogre::TextureManager::getSingleton().reloadAll();
        break;

    case SDL_SCANCODE_UP:
    case SDL_SCANCODE_W:
        mDirection.z -= mMove;
        break;

    case SDL_SCANCODE_DOWN:
    case SDL_SCANCODE_S:
        mDirection.z += mMove;
        break;

    case SDL_SCANCODE_LEFT:
    case SDL_SCANCODE_A:
        mDirection.x -= mMove;
        break;

    case SDL_SCANCODE_RIGHT:
    case SDL_SCANCODE_D:
        mDirection.x += mMove;
        break;

    case SDL_SCANCODE_PAGEDOWN:
    case SDL_SCANCODE_E:
        mDirection.y -= mMove;
        break;

    case SDL_SCANCODE_PAGEUP:
    case SDL_SCANCODE_Q:
        mDirection.y += mMove;
        break;
    }
}


void BareOgre::keyReleased(SDL_KeyboardEvent event) {
    SDL_Keysym key = event.keysym;

    switch (key.scancode) {
    case SDL_SCANCODE_UP:
    case SDL_SCANCODE_W:
        mDirection.z += mMove;
        break;

    case SDL_SCANCODE_DOWN:
    case SDL_SCANCODE_S:
        mDirection.z -= mMove;
        break;

    case SDL_SCANCODE_LEFT:
    case SDL_SCANCODE_A:
        mDirection.x += mMove;
        break;

    case SDL_SCANCODE_RIGHT:
    case SDL_SCANCODE_D:
        mDirection.x -= mMove;
        break;

    case SDL_SCANCODE_PAGEDOWN:
    case SDL_SCANCODE_E:
        mDirection.y += mMove;
        break;

    case SDL_SCANCODE_PAGEUP:
    case SDL_SCANCODE_Q:
        mDirection.y -= mMove;
        break;

    default:
        break;
    }
}

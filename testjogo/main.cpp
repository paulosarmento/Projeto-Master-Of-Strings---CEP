#include <irrlicht.h>
#include <math.h>

using namespace irr;

using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

int main()
{
    IrrlichtDevice *device = createDevice(EDT_OPENGL, dimension2d<u32>(1024, 768), 16, false, false, false, 0);

    IVideoDriver* driver = device->getVideoDriver();
    ISceneManager* smgr = device->getSceneManager();
    IGUIEnvironment* guienv = device->getGUIEnvironment();

    ISceneNode * pivo = smgr->addEmptySceneNode(0,0);
    ICameraSceneNode * camera = smgr->addCameraSceneNode(0, vector3df(100.0f, 0.0f, 100.0f), vector3df(0,0,40));
    camera->setFOV(1.0f);
    camera->setParent(pivo);

    //smgr->addCameraSceneNode(0, vector3df(30,0,-60), vector3df(0.0f)); //adicionando câmera
    smgr->addLightSceneNode(0, vector3df(0,3,-50), SColor(255,255,255,255), 5000.0f, 0); //adicionando luz


    IAnimatedMesh * guitarrista = smgr->getMesh("texturas/guitarrista_1/guitarrista.x");
    IAnimatedMeshSceneNode * guitar = smgr->addAnimatedMeshSceneNode(guitarrista);
    guitar->setPosition(vector3df(2.5, 2, -4.5));
    guitar->setAnimationSpeed(20);
    guitar->setScale(vector3df(0.43f));
    guitar->setRotation(vector3df(0,100,0));

    IAnimatedMesh * vocalista = smgr->getMesh("texturas/vocalista/vocalista.x");
    IAnimatedMeshSceneNode * vocal = smgr->addAnimatedMeshSceneNode(vocalista);
    vocal->setPosition(vector3df(0, 2, -4.5));
    vocal->setAnimationSpeed(20);
    vocal->setScale(vector3df(0.43f));

    IAnimatedMeshSceneNode * guitara = smgr->addAnimatedMeshSceneNode(guitarrista);
    guitara->getMaterial(1).TextureLayer[0].Texture = driver->getTexture("texturas/guitarrista_2/rosto2.jpg");
    guitara->getMaterial(2).TextureLayer[0].Texture = driver->getTexture("texturas/guitarrista_2/perna2.jpg");
    guitara->getMaterial(3).TextureLayer[0].Texture = driver->getTexture("texturas/guitarrista_2/torax2.jpg");
    guitara->getMaterial(14).TextureLayer[0].Texture = driver->getTexture("texturas/guitarrista_2/cima.jpg");
    guitara->setPosition(vector3df(-2.5, 2, -4));
    guitara->setAnimationSpeed(20);
    guitara->setScale(vector3df(0.43f));
    guitara->setRotation(vector3df(0,80,0));
    guitara->setCurrentFrame(150);

    IAnimatedMesh * baterista = smgr->getMesh("texturas/baterista/baterista.x");
    IAnimatedMeshSceneNode * batera = smgr->addAnimatedMeshSceneNode(baterista);
    batera->setPosition(vector3df(0, 2, 3));
    batera->setAnimationSpeed(20);
    batera->setScale(vector3df(0.43f));

    IMeshSceneNode * cenario = smgr->addMeshSceneNode(smgr->getMesh("texturas/cenario/cenario.x"));

    vector3df rotacao(0.0f, 0.0f, 0.0f);
    vector3df posicao(0.0f, 5.0f, -15.0f);

    while(device->run())
    {
        pivo->setRotation(rotacao);
        camera->setPosition(posicao);
        driver->beginScene(true, true, SColor(0,20,20,20));

        smgr->drawAll();
        guienv->drawAll();

        driver->endScene();
    }
    device->drop();

    return 0;
}
//rotacao += 0.1f; //rodando a seta
        //seta->setRotation(vector3df(0.0f,rotacao,0.0f));
      //  CMatrix4<f32> matriz = seta->getAbsoluteTransformation(); //obtendo a MATRIZ DE TRANSFORMAÇÃO da seta
       // matriz.transformVect(bolPosTransf, bolPosicao); //transformando 'bolPosicao' em 'bolPosTransf'


\

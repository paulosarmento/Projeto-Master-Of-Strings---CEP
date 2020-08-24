#include <irrlicht.h>
#include <math.h>

using namespace irr;

using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

//bool cameraAbertura ( ISceneNode * pivo, ICameraSceneNode * camera, vector3df * rotacao, vector3df * posicao)
//{
//    pivo->setRotation(rotacao);
//    camera->setPosition(posicao);
//
//    if( rotacao->Y <= 50 ) {
//        rotacao->Y += 0.5f;
//        rotacao->X += 0.1f;
//        if( rotacao->Y <= 0.0f )
//            posicao->Z += 0.5f;
//        else
//            posicao->Z -= 0.5f;
//    }
//};

int main()
{
    IrrlichtDevice *device = createDevice(EDT_OPENGL, dimension2d<u32>(800, 600), 16, false, false, false, 0);
	device->setWindowCaption(L"Master of Strings");

    IVideoDriver* driver = device->getVideoDriver();
    ISceneManager* smgr = device->getSceneManager();
    IGUIEnvironment* guienv = device->getGUIEnvironment();

    IGUIFont* font = guienv->getBuiltInFont();
    ITimer* timer = device->getTimer();
	ITexture* imagem = driver->getTexture("bolinha.png");

    ISceneNode * pivo = smgr->addEmptySceneNode(0,0);
    ICameraSceneNode * camera = smgr->addCameraSceneNode(0, vector3df(100.0f, 50.0f, 100.0f), vector3df(0.0f));
    camera->setFOV(1.0f);
    camera->setParent(pivo);

    //smgr->addCameraSceneNode(0, vector3df(30,0,-60), vector3df(0.0f)); //adicionando câmera
    smgr->addLightSceneNode(0, vector3df(10,10,5), SColor(255,255,255,255), 20.0f, 0); //adicionando luz

    // Câmera
    IMeshSceneNode * cenario = smgr->addMeshSceneNode(smgr->getMesh("palco_feira.x"));
    cenario->setMaterialFlag(EMF_LIGHTING, false);
    cenario->setMaterialFlag(EMF_WIREFRAME, true);

    vector3df rotacao ( -10.0f, -50.0f, 0.0f );
    vector3df posicao ( 0.0f, 10.0f, -125.0f );
    int estadoCamera = 1; // 0->parada, 1->panoramica, 2->centralizar, 3->aproximar
    bool ready2d = false;
    dimension2d<s32> janela;
    int posicaoBolinha = 50;

    while ( device->run() )
    {

        ///
        /// Interface 3d
        ///

        vector3df rotacao ( -11.0f, 0.0f, 0.0f );
        vector3df posicao ( 0.0f, 18.8f, -70.0f );

        pivo->setRotation ( rotacao );
        camera->setPosition ( posicao );

//        if ( estadoCamera == 1 )
//        {
//            rotacao.Y += 0.5f;
//            rotacao.X += 0.1f;
//            if( rotacao.Y <= 0.0f )
//                posicao.Z += 0.5f;
//            else
//                posicao.Z -= 0.5f;
//
//            if ( rotacao.Y >= 50 )
//                estadoCamera = 2;
//        }
//        if ( estadoCamera == 2 )
//        {
//            rotacao.Y -= 0.5f;
//            rotacao.X -= 0.1f;
//            if ( rotacao.Y <= 0 )
//                estadoCamera = 3;
//        }
//        if ( estadoCamera == 3 )
//        {
//            posicao.Z += 0.5f;
//            posicao.Y += 0.08f;
//            rotacao.X -= 0.1f;
//            if ( posicao.Z >= -70 )
//            {
//                estadoCamera = 0;
                ready2d = true;
//            }
//        }

        driver->beginScene ( true, true, SColor( 0, 20, 20, 20 ) );

        smgr->drawAll();

        ///
        /// Interface 2d
        ///

        if ( ready2d )
        {
            janela = driver->getCurrentRenderTargetSize();
            SColor grad1( 255, 153, 102, 51 );
            SColor grad2( 150, 153, 102, 51 );
            driver->draw2DRectangle ( rect<s32>( 0, 0, janela.Width, 200 ), grad1, grad1, grad2, grad2 );
            driver->draw2DLine( position2d<s32>( 0, 20 ), position2d<s32>( janela.Width, 20 ), SColor( 255, 255, 255, 255 ) );
            driver->draw2DLine( position2d<s32>( 0, 40 ), position2d<s32>( janela.Width, 40 ), SColor( 255, 255, 255, 255 ) );
            driver->draw2DImage(imagem, position2d<s32>(posicaoBolinha++, 50), rect<s32>(0,0,33,33), NULL, SColor(255,255,255,255), true);
        }

        stringw text;
        text += timer->getTime() / 1000.0f;
        font->draw( text, rect<s32>(130,10,300,50), video::SColor(255,255,255,255) );

        guienv->drawAll();

        driver->endScene();

        //device->sleep(1000, false);
    }
    device->drop();

    return 0;
}

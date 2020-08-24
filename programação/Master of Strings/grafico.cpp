#include "grafico.h"

/* Inicializa e prepara o sistema gráfico */
sistemaGrafico::sistemaGrafico()
{
	// Inicializa o sistema gráfico
    device = createDevice(EDT_OPENGL, dimension2d<u32>(1024,768), 16, false, false, false, &receiver);
	driver = device->getVideoDriver();
    smgr = device->getSceneManager();
    guienv = device->getGUIEnvironment();

	//imagemLoading = guienv->addImage( driver->getTexture("texturas/master_loading.png"), vector2d<s32>(0,0) );

    font = guienv->getBuiltInFont();
	fontBig = guienv->getFont("texturas/bigfont.png");
	//fontBotao = guienv->getFont("texturas/fonthaettenschweiler.bmp");
	//skin->setFont(fontBotao, EGDF_BUTTON);

	janela = driver->getCurrentRenderTargetSize();
	botaoClicado = -1;
	receiver.setBotao(&botaoClicado);
	botaoImpresso = false;

	// Define o texto da barra de título
	device->setWindowCaption(L"Master of Strings");

	// Luz
	smgr->addLightSceneNode(0, vector3df(0,3,-50), SColor(255,255,255,255), 5000.0f, 0);
	smgr->addLightSceneNode(0, vector3df(-50,3,-10), SColor(255,255,255,255), 5000.0f, 0);
	smgr->addLightSceneNode(0, vector3df(50,3,-10), SColor(255,255,255,255), 5000.0f, 0);

	// Câmera
    pivo = smgr->addEmptySceneNode(0,0);
    camera = smgr->addCameraSceneNode(0, vector3df(100.0f, 50.0f, 100.0f), vector3df(0.0f));
    camera->setFOV(0.3f);
    camera->setParent(pivo);

	// Carrega as texturas
	carregarTexturas();

	// Posição inicial da câmera
    rotacaoPivo = vector3df(-10.0f,-50.0f,0.0f);
    posicaoCamera = vector3df(0.0f,10.0f,-75.0f);
    estadoCamera = 1; // 0->parada, 1->panoramica, 2->centralizar, 3->aproximar

	estado = ESTADO_ABERTURA;
	frame = 0;
};

// Destrutor
sistemaGrafico::~sistemaGrafico()
{
    //device->drop();
};

// Carrega todas as texturas, objetos e imagens
void sistemaGrafico::carregarTexturas()
{
    // Gráficos 3D
    cenario = smgr->addMeshSceneNode( smgr->getMesh("texturas/cenario/cenario.x") );

    baterista = smgr->addAnimatedMeshSceneNode( smgr->getMesh("texturas/baterista/baterista.x") );
    baterista->setPosition(vector3df(0, 2, 1));
    baterista->setAnimationSpeed(20);
    baterista->setScale(vector3df(0.43f));

    vocalista = smgr->addAnimatedMeshSceneNode( smgr->getMesh("texturas/vocalista/vocalista.x") );
    vocalista->setPosition( vector3df(0, 2, -4.5) );
    vocalista->setAnimationSpeed(20);
    vocalista->setScale( vector3df(0.43f) );

	meshGuitarrista = smgr->getMesh("texturas/guitarrista_1/guitarrista.x");

	guitarrista1 = smgr->addAnimatedMeshSceneNode( meshGuitarrista );
    guitarrista1->setPosition(vector3df(2.5, 2, -4.5));
    guitarrista1->setAnimationSpeed(20);
    guitarrista1->setScale(vector3df(0.43f));
    guitarrista1->setRotation(vector3df(0,100,0));

	guitarrista2 = smgr->addAnimatedMeshSceneNode( meshGuitarrista );
    guitarrista2->getMaterial(1).TextureLayer[0].Texture = driver->getTexture("texturas/guitarrista_2/rosto2.jpg");
    guitarrista2->getMaterial(2).TextureLayer[0].Texture = driver->getTexture("texturas/guitarrista_2/perna2.jpg");
    guitarrista2->getMaterial(3).TextureLayer[0].Texture = driver->getTexture("texturas/guitarrista_2/torax2.jpg");
    guitarrista2->getMaterial(14).TextureLayer[0].Texture = driver->getTexture("texturas/guitarrista_2/guitarra.jpg");
    guitarrista2->setPosition(vector3df(-2.5, 2, -4));
    guitarrista2->setAnimationSpeed(20);
    guitarrista2->setScale(vector3df(0.43f));
    guitarrista2->setRotation(vector3df(0,80,0));
	guitarrista2->setFrameLoop(1, 150);
	guitarrista2->setCurrentFrame(130);

	// Gráficos 2D
	graficoNota = driver->getTexture("texturas/bolinha.png");
	traste = driver->getTexture("texturas/traste.png");
	corda = driver->getTexture("texturas/corda.png");

	numero[0] = driver->getTexture("texturas/1.png");
	numero[1] = driver->getTexture("texturas/2.png");
	numero[2] = driver->getTexture("texturas/3.png");
};

// Controle do movimento da câmera na abertura do jogo
void sistemaGrafico::cameraAbertura()
{
    if( estadoCamera == 1 )
    {
        rotacaoPivo.Y += 0.5f;
        rotacaoPivo.X += 0.1f;
        if( rotacaoPivo.Y <= 0.0f )
            posicaoCamera.Z += 0.5f;
        else
            posicaoCamera.Z -= 0.5f;

        if ( rotacaoPivo.Y >= 50 )
            estadoCamera = 2;
    }
    if ( estadoCamera == 2 )
    {
        rotacaoPivo.Y -= 0.5f;
        rotacaoPivo.X -= 0.1f;
        if ( rotacaoPivo.Y <= 0 )
            estadoCamera = 3;
    }
    if ( estadoCamera == 3 )
    {
        posicaoCamera.Z += 0.5f;
        posicaoCamera.Y += 0.08f;
        rotacaoPivo.X -= 0.1f;
        if ( posicaoCamera.Z >= -70 )
        {
            estadoCamera = 0;
			estado = ESTADO_MENU;
        }
    }
};

// Posiciona a câmera para a hora da música
void sistemaGrafico::cameraGuitarrista()
{
	// Posição padrão da câmera
	rotacaoPivo = vector3df(0.0f, -90.0f, 0.0f);
    posicaoCamera = vector3df(0.0f, 10.0f, -15.0f);
	camera->setTarget(vector3df(0,2,-20));
	guitarrista1->setRotation(vector3df(0,-90,0));
    guitarrista1->setPosition(vector3df(0, 2, -20)); // Posição na frente
	guitarrista2->setVisible(false);
	vocalista->setVisible(false);
	baterista->setVisible(false);
	//rotacaoPivo = vector3df(0.0f, 0.0f, 0.0f);
 //   posicaoCamera = vector3df(0.0f, 10.0f, -50.0f);
    estadoCamera = 0; // 0->parada, 1->panoramica, 2->centralizar, 3->aproximar
};

// Posiciona a câmera
void sistemaGrafico::posicionarCamera()
{
    pivo->setRotation( rotacaoPivo );
    camera->setPosition( posicaoCamera );
};

// Desenha os gráficos 2D
void sistemaGrafico::desenharGraficos()
{
    SColor grad1( 255, 153, 102, 51 );
    SColor grad2( 150, 153, 102, 51 );
	driver->draw2DRectangle( rect<s32>( 0, 0, janela.Width, 200 ), grad1, grad1, grad2, grad2 );
    for(int i = 1; i <= 23; i++)
	{
		driver->draw2DLine( vector2d<s32>((i * TAMANHO_TRASTE),0), vector2d<s32>((i * TAMANHO_TRASTE),200), SColor(255,255,255,255) );
	}
    for(int i = 23; i <= 180; i += 30)
	{
		driver->draw2DImage(corda, position2d<s32>(25,i), rect<s32>(0,0,janela.Width,11), 0, SColor(255,255,255,255), true);
		driver->draw2DImage(traste, position2d<s32>(5,i-15), rect<s32>(0,0,34,34), 0, SColor(255,255,255,255), true);
	}
};

// Chama o device->run
bool sistemaGrafico::run()
{
	return device->run();
};

// Exibe os elementos 3D na tela
void sistemaGrafico::loop3D()
{
	if(estado == ESTADO_ABERTURA)
		cameraAbertura();
	if(estado == ESTADO_JOGO)
		cameraGuitarrista();

	posicionarCamera();
};

// Encerra o loop gráfico 3D
void sistemaGrafico::fechaLoop3D()
{
    driver->beginScene( true, true, SColor( 0, 20, 20, 20 ) );

	smgr->drawAll();
};

// Exibe os elementos 2D na tela
void sistemaGrafico::loop2D()
{
	if(estado == ESTADO_JOGO)
		desenharGraficos();

	stringw textFPS;
	textFPS = "FPS: ";
	textFPS += driver->getFPS();
	font->draw( textFPS, rect<s32>(janela.Width - 40, janela.Height - 20, 50, 50), SColor(255,255,255,255) );
};

// Encerra o loop gráfico 2D
void sistemaGrafico::fechaLoop2D()
{
    guienv->drawAll();
	driver->endScene();

	//if(imagemLoading->isVisible())
	//	imagemLoading->setVisible(false);

	estado = (estadoCamera == 0) ? estado : ESTADO_ABERTURA;
};

// Desenha e habilita o menu das músicas
s32 sistemaGrafico::menu(int numMusicas)
{
	if( !botaoImpresso )
	{
		for( int i = 0; i < numMusicas; i++ )
		{
			guienv->addButton(rect<s32>(10, 240 + 40 * i ,10 + 100, 240 + 40 * i + 32), 0, i, listaNomes[i], listaNomes[i]);
		}
		botaoImpresso = true;
	}
	if(botaoClicado != -1)
		estado = ESTADO_JOGO;
	return botaoClicado;
};

// Faz a contagem regressiva
bool sistemaGrafico::contagemRegressiva()
{
	static int contador = 3;
	driver->draw2DImage( numero[contador-1] , position2d<s32>(janela.Width / 2 - 35, 52), rect<s32>(0,0,71,84), 0, SColor(255,255,255,255), true);
	frame++;
	if( !(frame % FPS) )
	{
		if( contador == 1 )
		{
			frame = 1;
			return true;
		}
		else
			contador--;
	}
	return false;
};

// Exibe as notas na tela
bool sistemaGrafico::exibeNotas(nota* musicNotes, int numNotes, int* notaCerta)
{
	static int contador = 0;
	vector2d<s32> pos;
	bool pausa = false;
	
	if( contador >= numNotes )
		return false;

	nota now = musicNotes[contador];
	nota next = musicNotes[contador+1];

	text = L"Nota: ";
	text += note[now.numero];

	tempo = time2frames( now.duracao );

	text += L" - Duracao: ";
	text += tempo;
	text += L" Frames - ";
	text += (s32) now.duracao;
	text += L" mS";

	intervalo = next.inicio - now.fim;
	if( intervalo > 0 )
	{
		int t = time2frames( intervalo );
		if( tempo <= frame )
		{
			text = L"Pausa";
			text += L" - Duracao: ";
			text += t;
			text += L" Frames";
			pausa = true;
		}
		tempo += t;
	}
	pos.X = now.casa * TAMANHO_TRASTE + 7;
	pos.Y = now.corda * 30 - 19;
	// Exibe ou troca a nota quando dá o tempo
	if( !(frame % tempo) )
	{
		frame = 0;
		contador++;
		pos += 2;
	}
	if( !pausa )
	{
		stringw textNota;
		textNota += now.casa;
		driver->draw2DImage( graficoNota, pos, rect<s32>(0,0,33,33), 0, SColor(255,255,255,255), true);
		fontBig->draw( textNota, rect<s32>(pos.X + 4, pos.Y, 10, 10), SColor(255,255,255,255) );
		*notaCerta = now.numero;
	}
	font->draw( text, rect<s32>(10, janela.Height - 20, 300, 50), SColor(255,255,255,255) );
	frame++;
	return true;
};

// Commpara as notas
bool sistemaGrafico::compararNotas(int notaCerta, int notaDetectada)
{
	stringw textoNota = L"Nota Detectada: ";
	textoNota += note[notaDetectada];
	font->draw( textoNota, rect<s32>(janela.Width / 2 - 20, janela.Height - 20, 50, 50), SColor(255,255,255,255) );
	if( notaDetectada == notaCerta )
	{
		fontBig->draw( L"Nota Certa!!", rect<s32>(20, 210, 100, 50), SColor(255,255,255,255) );
		return true;
	}
	else
	{
		fontBig->draw( L"Nota Errada!", rect<s32>(20, 210, 100, 50), SColor(255,255,255,255) );
		return false;
	}
};

// "Pausa" o sistema por determinado tempo
void sistemaGrafico::sleep(s16 tempo)
{
	device->sleep( tempo, false );
};

// Verifica se o sistema está pronto para jogar
int sistemaGrafico::getEstado()
{
	return estado;
};

void sistemaGrafico::encerrar()
{
	device->drop();
};

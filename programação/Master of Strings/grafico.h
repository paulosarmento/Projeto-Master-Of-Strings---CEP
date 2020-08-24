#ifndef _MASTER_OF_STRINGS_GRAFICO_
#define _MASTER_OF_STRINGS_GRAFICO_

#include "functions.h"
#include "MyEventReceiver.h"

enum
{
	TAMANHO_TRASTE = 44,
	// Estados do jogo
	ESTADO_ABERTURA = 0,
	ESTADO_MENU = 1,
	ESTADO_JOGO = 2
};

class sistemaGrafico
{
private:
    IrrlichtDevice* device;
    IVideoDriver* driver;

	// Ambiente 3D
    ISceneManager* smgr;
    ISceneNode* pivo;
    ICameraSceneNode* camera;

	// Ambiente 2D
	IGUIEnvironment* guienv;
    IGUIFont* font;
	IGUIFont* fontBig;
	IGUIFont* fontBotao;
	IGUISkin* skin;

	// Gráficos 3D
    IMeshSceneNode* cenario;
    IAnimatedMeshSceneNode* baterista;
    IAnimatedMeshSceneNode* vocalista;
    IAnimatedMeshSceneNode* guitarrista1;
    IAnimatedMeshSceneNode* guitarrista2;
	IAnimatedMesh* meshGuitarrista;

	// Gráficos 2D
	ITexture* traste;
	ITexture* corda;
	ITexture* graficoNota;
	ITexture* numero[3];
	//IGUIImage* imagemLoading;

	// Luz
	ILightSceneNode* luz;

	// Controle da câmera
    vector3df rotacaoPivo;
    vector3df posicaoCamera;
    int estadoCamera; // 0->parada, 1->panoramica, 2->centralizar, 3->aproximar

	// Diz se o jogo está pronto
    int estado;

	// Controle do tempo das notas
	int frame;
	int tempo;
	double intervalo;

	stringw text;
    dimension2d<s32> janela;
	MyEventReceiver receiver;
	s32 botaoClicado;
	bool botaoImpresso;

	void carregarTexturas();
	void cameraAbertura();
	void cameraGuitarrista();
	void posicionarCamera();
	void desenharGraficos();

public:
	sistemaGrafico();
	~sistemaGrafico();

	void loop3D();
	void fechaLoop3D();
	void loop2D();
	void fechaLoop2D();
	s32 menu(int numMusicas);
	bool contagemRegressiva();
	bool exibeNotas(nota* musicNotes, int numNotes, int* notaCerta);
	bool compararNotas(int notaCerta, int notaDetectada);

	void sleep(s16 tempo);
	int getEstado();
	bool run();
	void encerrar();
};

#endif

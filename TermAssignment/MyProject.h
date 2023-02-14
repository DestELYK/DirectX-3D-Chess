#ifndef _MyProject_h
#define _MyProject_h

#include "DirectX.h"
#include "Font.h"
#include "TextureType.h"
#include "IndexedPrimitive.h"
#include "LitColourShader.h"
#include "SkyBox.h"
#include "PawnPiece.h"
#include "RookPiece.h"
#include "KnightPiece.h"
#include "BishopPiece.h"
#include "QueenPiece.h"
#include "KingPiece.h"
#include "ChessBoard.h"
#include "AnimatedTransform.h"
#include "Sprite.h"
#include <CommonStates.h>

// forward declare the sprite batch

using namespace DirectX;
using namespace DirectX::SimpleMath;


//----------------------------------------------------------------------------------------------
// Main project class
//	Inherits the directx class to help us initalize directX
//----------------------------------------------------------------------------------------------

class MyProject : public DirectXClass
{
public:
	// constructor
	MyProject(HINSTANCE hInstance);
	~MyProject();

	// Initialize any directX objects
	void InitializeObjects();

	// window message handler
	LRESULT ProcessWindowMessages(UINT msg, WPARAM wParam, LPARAM lParam);

	// Called by the render loop to render a single frame
	void Render(void);

	// Called by directX framework to allow you to update any scene objects
	void Update(float deltaTime);

	// Compute the view and projection matrices
	void ComputeViewProjection();

private:

	SpriteBatch* spriteBatch;

	LitColourShader shader;
	SkyBox skyBox;

	// Models
	PawnPiece pawn;
	RookPiece rook;
	BishopPiece bishop;
	KnightPiece knight;
	QueenPiece queen;
	KingPiece king;

	// Model matrices
	Matrix pawnMatrices[16];
	Matrix rookMatrices[4];
	Matrix bishopMatrices[4];
	Matrix knightMatrices[4];
	Matrix queenMatrices[2];
	Matrix kingMatrices[2];

	// Board
	ChessBoard board;
	Matrix boardMatrix;

	// Animations
	static const int MAX_ANIMATIONS = 7;
	AnimatedTransform anims[MAX_ANIMATIONS];
	int currentAnimation;

	// Locations of where chess pieces need to go
	static const int CHESS_SIZE = 8;
	Vector3 chessLocations[CHESS_SIZE][CHESS_SIZE];

	// Sprite
	TextureType turnTex;
	Sprite turn;

	// Font
	FontType verdana40;

	// Texture for chess pieces
	TextureType diffuseTex;
	TextureType specTex;

	// Texture for board
	TextureType chessTex;

	Matrix viewMatrix;
	Matrix projectionMatrix;

	// mouse variables
	Vector2 mousePos;
	Vector2 mouseDelta;
	bool buttonDown;

	// for camera controls
	Vector3 cameraPos;
	Vector2 cameraRotationSpeed;
	Vector2 cameraRotation;
	float cameraRadius;
	float cameraRadiusSpeed;

	void UpdateCamera(float deltaTime);

	// call when the mouse is released
	void OnMouseDown();
	void OnMouseMove();

	eChessSide currentSide;

	float delayRestart;

};

#endif
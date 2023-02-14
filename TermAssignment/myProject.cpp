#include "MyProject.h"
#include <Windowsx.h> // for GET__LPARAM macros
#include <d3d11_1.h>
#include <SimpleMath.h>
#include <DirectXColors.h>
#include <sstream>
#include <CommonStates.h>


using namespace DirectX;
using namespace DirectX::SimpleMath;

// helper function
//
// returns a random float between 0 & 1
float RandFloat() { return float(rand())/float(RAND_MAX); } 

static const float CAMERA_SPEED = XM_PI * 0.2f;

//----------------------------------------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pCmdLine, int nShowCmd)
{
	MyProject application(hInstance);    // Create the class variable

	if( application.InitWindowsApp(L"DirectX 3D", nShowCmd) == false )    // Initialize the window, if all is well show and update it so it displays
	{
		return 0;                   // Error creating the window, terminate application
	}

	if( application.InitializeDirect3D() )
	{
		application.SetDepthStencil(true);      // Tell DirectX class to create and maintain a depth stencil buffer
		application.InitializeObjects();
		application.MessageLoop();				// Window has been successfully created, start the application message loop
	}

	return 0;
}

//----------------------------------------------------------------------------------------------
// Constructor
//----------------------------------------------------------------------------------------------
MyProject::MyProject(HINSTANCE hInstance)
	: DirectXClass(hInstance)
{
	mousePos = Vector2(clientWidth * 0.5f, clientHeight * 0.5f);
	buttonDown = false;
	
	ClearColor = Color(DirectX::Colors::Black.v);

	cameraPos = Vector3(0, 0, 6);

	cameraRadius = 32;
	cameraRadiusSpeed = 0;

	cameraRotation = Vector2(0, 45 * XM_PI / 180);
	cameraRotationSpeed = Vector2::Zero;

	currentSide = White;

	currentAnimation = 0;
	delayRestart = 0;
}

//----------------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------------
MyProject::~MyProject()
{
}

//----------------------------------------------------------------------------------------------
// Initialize any fonts we need to use here
//----------------------------------------------------------------------------------------------
void MyProject::InitializeObjects()
{
	skyBox.Initialize(D3DDevice, DeviceContext, L"..\\Textures\\envMap.dds", 64 );

	// load the shader
	shader.LoadShader(D3DDevice);

	spriteBatch = new DirectX::SpriteBatch(DeviceContext);

	// load the textures
	diffuseTex.Load(D3DDevice, DeviceContext, L"..\\Textures\\marble8.jpg");
	specTex.Load(D3DDevice, DeviceContext, L"..\\Textures\\marbleSpec.jpg");
	chessTex.Load(D3DDevice, DeviceContext, L"..\\Textures\\chess.jpg");
	turnTex.Load(D3DDevice, DeviceContext, L"..\\Textures\\turn_title.png");

	// Load the sprite
	turn.Initialize(&turnTex, Vector2(clientWidth / 2, 50), 0, 1.0f, Colors::White.v, 0);

	// Load the font
	verdana40.InitializeFont(D3DDevice, DeviceContext, L"..\\Font\\Verdana40.spriteFont");

	// Load all models
	pawn.Initiialize(D3DDevice, &shader);
	pawn.SetTextures(diffuseTex.GetResourceView(), specTex.GetResourceView());

	rook.Initiialize(D3DDevice, &shader);
	rook.SetTextures(diffuseTex.GetResourceView(), specTex.GetResourceView());

	bishop.Initiialize(D3DDevice, &shader);
	bishop.SetTextures(diffuseTex.GetResourceView(), specTex.GetResourceView());

	knight.Initiialize(D3DDevice, &shader);
	knight.SetTextures(diffuseTex.GetResourceView(), specTex.GetResourceView());

	queen.Initiialize(D3DDevice, &shader);
	queen.SetTextures(diffuseTex.GetResourceView(), specTex.GetResourceView());

	king.Initiialize(D3DDevice, &shader);
	king.SetTextures(diffuseTex.GetResourceView(), specTex.GetResourceView());

	board.Initiialize(D3DDevice, &shader);
	board.SetTextures(chessTex.GetResourceView(), chessTex.GetResourceView());

	boardMatrix = Matrix::CreateTranslation(0, 0, 0);

	// Set locations for where chess pieces go
	float distance = 3.8f;
	for (int i = 0, k = -CHESS_SIZE / 2; i < CHESS_SIZE; i++, k++)
	{
		for (int j = 0, l = -CHESS_SIZE / 2; j < CHESS_SIZE; j++, l++)
		{
			chessLocations[i][j] = Vector3(distance * k + (distance / 2.0f), 2.0f, distance * l + (distance / 2.0f));
		}
	}

	// Create start matrices for pawns
	for (int i = 0; i < 8; i++)
	{
		pawnMatrices[i] = Matrix::CreateTranslation(chessLocations[1][i]);
		pawnMatrices[i + 8] = Matrix::CreateTranslation(chessLocations[6][i]);
	}

	// Create start matrices for rooks, knights and bishops
	for (int i = 0; i < 2; i++)
	{
		rookMatrices[i] = Matrix::CreateTranslation(chessLocations[0][i * 7]);
		rookMatrices[i + 2] = Matrix::CreateTranslation(chessLocations[7][i * 7]);
		knightMatrices[i] = Matrix::CreateRotationY(90 * XM_PI / 180) * Matrix::CreateTranslation(chessLocations[0][i * 5 + 1]);
		knightMatrices[i + 2] = Matrix::CreateRotationY(270 * XM_PI / 180) * Matrix::CreateTranslation(chessLocations[7][i * 5 + 1]);
		bishopMatrices[i] = Matrix::CreateTranslation(chessLocations[0][i * 3 + 2]);
		bishopMatrices[i + 2] = Matrix::CreateTranslation(chessLocations[7][i * 3 + 2]);
	}

	// Create start matrices for queens
	queenMatrices[0] = Matrix::CreateTranslation(chessLocations[0][3]);
	queenMatrices[1] = Matrix::CreateTranslation(chessLocations[7][4]);

	// Create start matrices for kings
	kingMatrices[0] = Matrix::CreateRotationY(90 * XM_PI / 180) * Matrix::CreateTranslation(chessLocations[0][4]);
	kingMatrices[1] = Matrix::CreateRotationY(270 * XM_PI / 180) * Matrix::CreateTranslation(chessLocations[7][3]);

	// Create 6 animations
	anims[0].Initialize(pawnMatrices, 8 + 5, Matrix::CreateTranslation(chessLocations[4][5]), 5);
	anims[1].Initialize(pawnMatrices, 3, Matrix::CreateTranslation(chessLocations[2][3]), 2.5f);
	anims[2].Initialize(queenMatrices, 1, Matrix::CreateTranslation(chessLocations[4][7]), 7.5f);
	anims[3].Initialize(bishopMatrices, 0, Matrix::CreateTranslation(chessLocations[3][5]), 7.5f);
	anims[4].Initialize(knightMatrices, 3, Matrix::CreateRotationY(270 * XM_PI / 180) * Matrix::CreateTranslation(chessLocations[5][7]), 5);
	anims[5].Initialize(pawnMatrices, 7, Matrix::CreateTranslation(chessLocations[3][7]), 5);

}

//----------------------------------------------------------------------------------------------
// Window message handler
//----------------------------------------------------------------------------------------------
LRESULT MyProject::ProcessWindowMessages(UINT msg, WPARAM wParam, LPARAM lParam)
{
	// for mouse positions
	Vector2 pos((float)GET_X_LPARAM(lParam), (float)GET_Y_LPARAM(lParam));

	switch (msg )
	{
	case WM_MOUSEMOVE:
		mouseDelta = pos - mousePos;
		mousePos = pos;
		OnMouseMove();
		return 0;
	case WM_LBUTTONUP:
		buttonDown = false;
		mouseDelta = pos - mousePos;
		mousePos = pos;
		break;
	case WM_LBUTTONDOWN:
		buttonDown = true;
		mouseDelta = pos - mousePos;
		mousePos = pos;
		OnMouseDown();
		break;
	case WM_KEYUP:
		if (wParam >= '0' && wParam <= '4')
		{
			PresentInterval = wParam - '0';
		}
		else if (wParam == VK_UP)	{	cameraRotationSpeed.y = 0;	}
		else if (wParam == VK_DOWN) {	cameraRotationSpeed.y = 0; }
		else if (wParam == VK_LEFT) {	cameraRotationSpeed.x = 0; }
		else if (wParam == VK_RIGHT){	cameraRotationSpeed.x = 0; }
		else if (wParam == VK_ADD)  {	cameraRadiusSpeed = 0; }
		else if (wParam == VK_SUBTRACT)  { cameraRadiusSpeed = 0; }
		else if (wParam == VK_SPACE)
		{
			cameraRotation = Vector2::Zero;
			cameraRadius   = 6;
		}

		break;
	case WM_KEYDOWN:
		if (wParam == VK_UP)	{ cameraRotationSpeed.y = CAMERA_SPEED; }
		else if (wParam == VK_DOWN) { cameraRotationSpeed.y = -CAMERA_SPEED; }
		else if (wParam == VK_LEFT) { cameraRotationSpeed.x = -CAMERA_SPEED; }
		else if (wParam == VK_RIGHT){ cameraRotationSpeed.x = CAMERA_SPEED; }
		else if (wParam == VK_ADD)  { cameraRadiusSpeed = -1.0f; }
		else if (wParam == VK_SUBTRACT)  { cameraRadiusSpeed = 1.0f; }
		break;

	}

	// let the base class handle remaining messages
	return DirectXClass::ProcessWindowMessages(msg, wParam, lParam);
}

//----------------------------------------------------------------------------------------------
// Called by the render loop to render a single frame
//----------------------------------------------------------------------------------------------
void MyProject::Render(void)
{
	// calculate camera matrices
	ComputeViewProjection();

	// draw the skybox FIRST
	skyBox.Draw(DeviceContext, viewMatrix, projectionMatrix);

	// Draw all pawns
	for (int i = 0; i < 16; i++)
	{
		// Changes the color based on side
		if (i < 8)
		{
			pawn.SetSide(D3DDevice, Black);
		}
		else
		{
			pawn.SetSide(D3DDevice, White);
		}

		pawn.Draw(DeviceContext, pawnMatrices[i], viewMatrix, projectionMatrix);
	}

	// Draw all rooks, knights and bishops
	for (int i = 0; i < 4; i++)
	{
		// Changes the color based on side
		if (i < 2)
		{
			rook.SetSide(D3DDevice, Black);
			knight.SetSide(D3DDevice, Black);
			bishop.SetSide(D3DDevice, Black);
		}
		else
		{
			rook.SetSide(D3DDevice, White);
			knight.SetSide(D3DDevice, White);
			bishop.SetSide(D3DDevice, White);
		}

		rook.Draw(DeviceContext, rookMatrices[i], viewMatrix, projectionMatrix);
		knight.Draw(DeviceContext, knightMatrices[i], viewMatrix, projectionMatrix);
		bishop.Draw(DeviceContext, bishopMatrices[i], viewMatrix, projectionMatrix);
	}

	// Draw all queens and kings
	for (int i = 0; i < 2; i++)
	{
		// Changes the color based on side
		if (i < 1)
		{
			queen.SetSide(D3DDevice, Black);
			king.SetSide(D3DDevice, Black);
		}
		else
		{
			queen.SetSide(D3DDevice, White);
			king.SetSide(D3DDevice, White);
		}

		queen.Draw(DeviceContext, queenMatrices[i], viewMatrix, projectionMatrix);
		king.Draw(DeviceContext, kingMatrices[i], viewMatrix, projectionMatrix);
	}

	// Draw the board
	shader.SetDirectionalLight3(Colors::White.v, Vector3(0, -1, 0));
	board.Draw(DeviceContext, boardMatrix, viewMatrix, projectionMatrix);

	// Draw the sprite
	CommonStates commonStates = CommonStates(D3DDevice);
	spriteBatch->Begin(SpriteSortMode_BackToFront, commonStates.NonPremultiplied());
	turn.Draw(spriteBatch);
	spriteBatch->End();

	// Draws text to show which side is going
	wstringstream sstream;
	sstream << (currentSide == Black ? "Black" : "White");
	verdana40.PrintMessage(clientWidth / 2 - verdana40.MeasureString(sstream.str().c_str()).x / 2, 33, sstream.str(), Colors::White.v);

	// render the base class
	DirectXClass::Render();
}

//----------------------------------------------------------------------------------------------
// Called every frame to update objects.
//	deltaTime: how much time in seconds has elapsed since the last frame
//----------------------------------------------------------------------------------------------
void MyProject::Update(float deltaTime)
{
	if (currentAnimation < MAX_ANIMATIONS)
	{
		// update the animation
		anims[currentAnimation].Update(deltaTime);

		// if it's done move to the nextone
		if (anims[currentAnimation].isDone())
		{
			currentAnimation++;
			currentSide = currentSide == Black ? White : Black;
		}
	}
	else if (delayRestart < 5) // After all animations play waits 5 secs to restart
		delayRestart += deltaTime;
	else // Resets all animations
	{
		for (int i = 0; i < MAX_ANIMATIONS; i++)
		{
			anims[i].Reset();
		}
		delayRestart = 0;
		currentAnimation = 0;
		currentSide = White;
	}

	// update the camera movement
	UpdateCamera(deltaTime);
}


//----------------------------------------------------------------------------------------------
// Called when the mouse is released
//----------------------------------------------------------------------------------------------
void MyProject::OnMouseDown()
{

	// this is called when the left mouse button is clicked
	// mouse position is stored in mousePos variable
}

//----------------------------------------------------------------------------------------------
// Called when the mouse is moved
//----------------------------------------------------------------------------------------------
void MyProject::OnMouseMove()
{
}

//----------------------------------------------------------------------------------------------
// Computes the view and camera matrix
//----------------------------------------------------------------------------------------------
void MyProject::ComputeViewProjection()
{
	viewMatrix = Matrix::CreateLookAt(cameraPos, Vector3::Zero, Vector3::UnitY);
	projectionMatrix = Matrix::CreatePerspectiveFieldOfView(60.0f * XM_PI / 180.0f, (float)clientWidth / (float)clientHeight, 1, 128);
}

//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
void MyProject::UpdateCamera(float deltaTime)
{
	const float VERT_LIMIT = XM_PI * 0.35f;

	// update the radius
	cameraRadius += cameraRadiusSpeed * deltaTime;
	if (cameraRadius < 1) cameraRadius = 1;


	// update the rotation amounts
	cameraRotation += cameraRotationSpeed * deltaTime;

	// clamp the vertical rotation
	if (cameraRotation.y < -VERT_LIMIT) cameraRotation.y = -VERT_LIMIT;
	else if (cameraRotation.y > VERT_LIMIT) cameraRotation.y = VERT_LIMIT;

	// calculate the height
	cameraPos.y = cameraRadius * sinf(cameraRotation.y);
	float r = cameraRadius * cosf(cameraRotation.y);

	// calculate the orbit
	cameraPos.x = sinf(cameraRotation.x) * r;
	cameraPos.z = cosf(cameraRotation.x) * r;
}
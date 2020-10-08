#include "PlayScene.h"
#include "Game.h"
#include "EventManager.h"
#include "Util.h"
#include "IMGUI_SDL/imgui_sdl.h"
#include "Renderer.h"
#include <iomanip>

PlayScene::PlayScene()
{
	PlayScene::start();
}

PlayScene::~PlayScene()
= default;

void PlayScene::draw()
{
	//Background
	TextureManager::Instance()->draw("Background1", 400, 300, 0, 255, true);
	drawDisplayList();

	ImGui::Render();
	ImGuiSDL::Render(ImGui::GetDrawData());
	SDL_SetRenderDrawColor(Renderer::Instance()->getRenderer(), 255, 255, 255, 255);
}

void PlayScene::update()
{
	m_updateUI();

	updateDisplayList();

	m_pThermalDetonator->setEnemyPosition(m_pEnemy[1]->getTransform()->position);
	
	m_pThermalDetonator->setDistanceToEnemy(m_pThermalDetonator->checkDistance(m_pEnemy[1]));

	//Printing Labels
	m_pDistanceLabel->setText("Distance = " + std::to_string(m_pThermalDetonator->checkDistance(m_pEnemy[1])) +"m");
	if (std::to_string(Util::magnitude(m_pThermalDetonator->getRigidBody()->velocity)) == "-nan(ind)")
	{
		m_pVelocityLabel->setText("Velocity = 0m/s");
		m_pCannotHitTrooper->setText("Cannot hit trooper with current settings.Reset Scene");
	}
	else
	{
		m_pCannotHitTrooper->setText(" ");
		m_pVelocityLabel->setText("Velocity = " + std::to_string(Util::magnitude(m_pThermalDetonator->getRigidBody()->velocity)) + "m/s");
	}
	m_pMassLabel->setText("Weight = " + std::to_string(m_pThermalDetonator->getMass()) + "kg");
	m_pAccelerationLabel->setText("Acceleration = " + std::to_string(m_pThermalDetonator->getRigidBody()->acceleration.y) + "m/s^2");
	m_pForceLabel->setText("Force = " + std::to_string(m_pThermalDetonator->calculateForce()) + "N");

	if (std::to_string(GuiSliderPlaceholders[5]) == ("-nan(ind)"))
	{
		m_pCannotHitTrooper->setText("Cannot hit trooper with current settings.Reset Scene");
		GuiSliderPlaceholders[5] = 0;
		m_pDistanceLabel->setText("Distance = 0m");
	}
	if (std::to_string(GuiSliderPlaceholders[2]) == ("-nan(ind)"))
	{
		m_pCannotHitTrooper->setText("Cannot hit trooper with current settings.Reset Scene");
		GuiSliderPlaceholders[0] = 0;
		m_pDistanceLabel->setText("Distance = 0m");
	}

}

void PlayScene::clean()
{
	removeAllChildren();
}

void PlayScene::handleEvents()
{
	auto& io = ImGui::GetIO();
	auto wheel = 0;

	SDL_Event event;
	if (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			TheGame::Instance()->quit();
			break;
		case SDL_MOUSEMOTION:
			m_mousePosition.x = event.motion.x;
			m_mousePosition.y = event.motion.y;
			break;
		case SDL_MOUSEWHEEL:
			wheel = event.wheel.y;
			break;
		case SDL_TEXTINPUT:
			io.AddInputCharactersUTF8(event.text.text);
			break;
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym)
			{
			case SDLK_ESCAPE:
				TheGame::Instance()->quit();
				break;
			case SDLK_BACKQUOTE:
				m_displayUI = (m_displayUI) ? false : true;
				break;
			}
			{
				const int key = event.key.keysym.scancode;
				IM_ASSERT(key >= 0 && key < IM_ARRAYSIZE(io.KeysDown));
				io.KeysDown[key] = (event.type == SDL_KEYDOWN);
				io.KeyShift = ((SDL_GetModState() & KMOD_SHIFT) != 0);
				io.KeyCtrl = ((SDL_GetModState() & KMOD_CTRL) != 0);
				io.KeyAlt = ((SDL_GetModState() & KMOD_ALT) != 0);
				io.KeySuper = ((SDL_GetModState() & KMOD_GUI) != 0);
			}
			break;
		case SDL_KEYUP:
			switch (event.key.keysym.sym)
			{
			default:
				break;
			}
			{
				const int key = event.key.keysym.scancode;
				IM_ASSERT(key >= 0 && key < IM_ARRAYSIZE(io.KeysDown));
				io.KeysDown[key] = (event.type == SDL_KEYDOWN);
				io.KeyShift = ((SDL_GetModState() & KMOD_SHIFT) != 0);
				io.KeyCtrl = ((SDL_GetModState() & KMOD_CTRL) != 0);
				io.KeyAlt = ((SDL_GetModState() & KMOD_ALT) != 0);
				io.KeySuper = ((SDL_GetModState() & KMOD_GUI) != 0);
			}
			break;
		default:
			break;
		}
	}
	io.DeltaTime = 1.0f / 60.0f;
	int mouseX, mouseY;
	const int buttons = SDL_GetMouseState(&mouseX, &mouseY);
	io.MousePos = ImVec2(static_cast<float>(mouseX), static_cast<float>(mouseY));
	io.MouseDown[0] = buttons & SDL_BUTTON(SDL_BUTTON_LEFT);
	io.MouseDown[1] = buttons & SDL_BUTTON(SDL_BUTTON_RIGHT);
	io.MouseWheel = static_cast<float>(wheel);

	io.DisplaySize.x = 1280;
	io.DisplaySize.y = 720;

	m_ImGuiKeyMap();
	m_ImGuiSetStyle();
}


void PlayScene::start()
{
	m_updateUI();

	//Setting up background
	TextureManager::Instance()->load("../Assets/textures/Background1.jpg", "Background1");

	// Player Sprite
	m_pPlayer = new Player();
	addChild(m_pPlayer);
	
	// Enemy Sprite
	//2nd enemy's position in array is what is used 
	for (int i = 0; i < 3; i++)
	{
		m_pEnemy[i] = new Enemy();
		addChild(m_pEnemy[i]);
	}

	//Target Sprite(Thermal Detonator)
	m_pThermalDetonator = new Target();
	addChild(m_pThermalDetonator);

	// Label
	const SDL_Color white = { 255,255,255, 255 };
	const SDL_Color red = { 255,0,0,255 };

	m_pDistanceLabel = new Label("Distance", "Consolas", 20, white, glm::vec2(596.0f, 40.0f));
	m_pDistanceLabel->setParent(this);
	addChild(m_pDistanceLabel);

	m_pAccelerationLabel = new Label("Acceleration: ", "Consolas", 20, white, glm::vec2(632.0f, 80.0f));
	m_pAccelerationLabel->setParent(this);
	addChild(m_pAccelerationLabel);

	m_pVelocityLabel = new Label("Velocity: ", "Consolas", 20, white, glm::vec2(599.0f, 120.0f));
	m_pVelocityLabel->setParent(this);
	addChild(m_pVelocityLabel);

	m_pMassLabel = new Label("Mass: ", "Consolas", 20, white, glm::vec2(583.0f, 160.0f));
	m_pMassLabel->setParent(this);
	addChild(m_pMassLabel);

	m_pForceLabel = new Label("Force: ", "Consolas", 20, white, glm::vec2(571.0f, 200.0f));
	m_pForceLabel->setParent(this);
	addChild(m_pForceLabel);

	m_pPPM = new Label("Scale: 1 PPM ", "Consolas", 15, white, glm::vec2(741.0f, 580.0f));
	m_pPPM->setParent(this);
	addChild(m_pPPM);

	m_pCannotHitTrooper = new Label(" ", "Consolas", 25, red, glm::vec2(400.0f, 350.0f));
	m_pCannotHitTrooper->setParent(this);
	addChild(m_pCannotHitTrooper);

	m_pThermalDetonator->setDistanceToEnemy(m_pThermalDetonator->checkDistance(m_pEnemy[1]));
	m_pThermalDetonator->recalculateProjectile();

	resetSceneSettings();
}
void PlayScene::setGuidSlidePlaceholders()
{
	GuiSliderPlaceholders[0] = m_pEnemy[1]->getTransform()->position.x;
	GuiSliderPlaceholders[1] = m_pPlayer->getTransform()->position.x;
	GuiSliderPlaceholders[2] = m_pThermalDetonator->getSpeedThrown();
	GuiSliderPlaceholders[3] = m_pThermalDetonator->getGravity();
	GuiSliderPlaceholders[4] = m_pThermalDetonator->getMass();
	GuiSliderPlaceholders[5] = m_pThermalDetonator->getTheta();
}
void PlayScene::resetSceneSettings()
{
	
	m_pPlayer->getTransform()->position.x = 50;
	m_pThermalDetonator->getTransform()->position = m_pPlayer->getTransform()->position;
	m_pThermalDetonator->setGravity(9.8);
	m_pThermalDetonator->setSpeedThrown(95);
	setEnemyPositionInPlayScene();
	m_pThermalDetonator->setUpdateDetenator(false);
	m_pThermalDetonator->setDistanceToEnemy(m_pThermalDetonator->checkDistance(m_pEnemy[1]));
	m_pThermalDetonator->recalculateProjectile();
	m_pThermalDetonator->setMass(2.2);
	setGuidSlidePlaceholders();
	m_pCannotHitTrooper->setText(" ");
	m_pThermalDetonator->getRigidBody()->velocity = glm::vec2(0.0f);

}
void PlayScene::checkGuiChangs()
{
	
	if (ImGui::Checkbox("Always hit stormtrooper", &hitStormtrooper))
	{
		
	}
	if (ImGui::Checkbox("Always use chosen angle", &useAngleChosen))
	{

	}
	if (ImGui::Button("Throw Detenator"))
	{
		m_pThermalDetonator->getTransform()->position = m_pPlayer->getTransform()->position;
		if (hitStormtrooper)
		{
			if (useAngleChosen)
			{
				m_pThermalDetonator->velocityGivenAngle();
				GuiSliderPlaceholders[2] = m_pThermalDetonator->getSpeedThrown();
			}
			else
			{
				m_pThermalDetonator->recalculateProjectile();
				GuiSliderPlaceholders[5] = m_pThermalDetonator->getTheta();
			}
		}
		else
			m_pThermalDetonator->caculateNotHittingStormT();

		m_pThermalDetonator->setUpdateDetenator(true);
	}
	ImGui::SameLine();
	if (ImGui::Button("Reset Scene"))
	{
		resetSceneSettings();
	}
	if (ImGui::SliderFloat("Stormtrooper X Position", &GuiSliderPlaceholders[0], 110, 750))
	{
		m_pEnemy[1]->getTransform()->position.x = GuiSliderPlaceholders[0];
		m_pEnemy[0]->getTransform()->position.x = m_pEnemy[1]->getTransform()->position.x - 30.0f;
		m_pEnemy[2]->getTransform()->position.x = m_pEnemy[1]->getTransform()->position.x + 30.0f;
	}
	if (ImGui::SliderFloat("Wookie X Position", &GuiSliderPlaceholders[1], 50, 650))
	{
		m_pPlayer->getTransform()->position.x = GuiSliderPlaceholders[1];
		m_pThermalDetonator->getTransform()->position.x = m_pPlayer->getTransform()->position.x;

	}
	if (ImGui::SliderFloat("Speed Thrown", &GuiSliderPlaceholders[2], 80, 200))
	{
		m_pThermalDetonator->setSpeedThrown(GuiSliderPlaceholders[2]);
	}
	if (ImGui::SliderFloat("Gravity", &GuiSliderPlaceholders[3], 5, 20))
	{
		m_pThermalDetonator->setGravity(GuiSliderPlaceholders[3]);
	}
	if (ImGui::SliderFloat("Mass", &GuiSliderPlaceholders[4], 0.1, 10))
	{
		m_pThermalDetonator->setMass(4);
	}
	if (ImGui::SliderFloat("Angle (theta)", &GuiSliderPlaceholders[5], 0, 90))
	{
		m_pThermalDetonator->setTheta(GuiSliderPlaceholders[5]);

	}
}
void PlayScene::setEnemyPositionInPlayScene()
{
	m_pEnemy[0]->getTransform()->position = glm::vec2(505.0f, 500.0f);
	m_pEnemy[1]->getTransform()->position = glm::vec2(535.0f, 500.0f);
	m_pEnemy[2]->getTransform()->position = glm::vec2(565.0f, 500.0f);
}
// ImGui functions ***********************************************

void PlayScene::m_ImGuiKeyMap()
{
	auto& io = ImGui::GetIO();

	// Keyboard mapping. ImGui will use those indices to peek into the io.KeysDown[] array.
	io.KeyMap[ImGuiKey_Tab] = SDL_SCANCODE_TAB;
	io.KeyMap[ImGuiKey_LeftArrow] = SDL_SCANCODE_LEFT;
	io.KeyMap[ImGuiKey_RightArrow] = SDL_SCANCODE_RIGHT;
	io.KeyMap[ImGuiKey_UpArrow] = SDL_SCANCODE_UP;
	io.KeyMap[ImGuiKey_DownArrow] = SDL_SCANCODE_DOWN;
	io.KeyMap[ImGuiKey_PageUp] = SDL_SCANCODE_PAGEUP;
	io.KeyMap[ImGuiKey_PageDown] = SDL_SCANCODE_PAGEDOWN;
	io.KeyMap[ImGuiKey_Home] = SDL_SCANCODE_HOME;
	io.KeyMap[ImGuiKey_End] = SDL_SCANCODE_END;
	io.KeyMap[ImGuiKey_Insert] = SDL_SCANCODE_INSERT;
	io.KeyMap[ImGuiKey_Delete] = SDL_SCANCODE_DELETE;
	io.KeyMap[ImGuiKey_Backspace] = SDL_SCANCODE_BACKSPACE;
	io.KeyMap[ImGuiKey_Space] = SDL_SCANCODE_SPACE;
	io.KeyMap[ImGuiKey_Enter] = SDL_SCANCODE_RETURN;
	io.KeyMap[ImGuiKey_Escape] = SDL_SCANCODE_ESCAPE;

	io.KeyMap[ImGuiKey_A] = SDL_SCANCODE_A;
	io.KeyMap[ImGuiKey_C] = SDL_SCANCODE_C;
	io.KeyMap[ImGuiKey_V] = SDL_SCANCODE_V;
	io.KeyMap[ImGuiKey_X] = SDL_SCANCODE_X;
	io.KeyMap[ImGuiKey_Y] = SDL_SCANCODE_Y;
	io.KeyMap[ImGuiKey_Z] = SDL_SCANCODE_Z;
}

void PlayScene::m_ImGuiSetStyle()
{
	auto& style = ImGui::GetStyle();

	style.Alpha = 0.8f;
	style.FrameRounding = 3.0f;
	style.Colors[ImGuiCol_Text] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.94f, 0.94f, 0.94f, 0.94f);
	style.Colors[ImGuiCol_ChildWindowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	style.Colors[ImGuiCol_PopupBg] = ImVec4(1.00f, 1.00f, 1.00f, 0.94f);
	style.Colors[ImGuiCol_Border] = ImVec4(0.00f, 0.00f, 0.00f, 0.39f);
	style.Colors[ImGuiCol_BorderShadow] = ImVec4(1.00f, 1.00f, 1.00f, 0.10f);
	style.Colors[ImGuiCol_FrameBg] = ImVec4(1.00f, 1.00f, 1.00f, 0.94f);
	style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.40f);
	style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
	style.Colors[ImGuiCol_TitleBg] = ImVec4(0.96f, 0.96f, 0.96f, 1.00f);
	style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(1.00f, 1.00f, 1.00f, 0.51f);
	style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.82f, 0.82f, 0.82f, 1.00f);
	style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.86f, 0.86f, 0.86f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.98f, 0.98f, 0.98f, 0.53f);
	style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.69f, 0.69f, 0.69f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.59f, 0.59f, 0.59f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.49f, 0.49f, 0.49f, 1.00f);
	style.Colors[ImGuiCol_CheckMark] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.24f, 0.52f, 0.88f, 1.00f);
	style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	style.Colors[ImGuiCol_Button] = ImVec4(0.26f, 0.59f, 0.98f, 0.40f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.06f, 0.53f, 0.98f, 1.00f);
	style.Colors[ImGuiCol_Header] = ImVec4(0.26f, 0.59f, 0.98f, 0.31f);
	style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
	style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	style.Colors[ImGuiCol_Column] = ImVec4(0.39f, 0.39f, 0.39f, 1.00f);
	style.Colors[ImGuiCol_ColumnHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.78f);
	style.Colors[ImGuiCol_ColumnActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	style.Colors[ImGuiCol_ResizeGrip] = ImVec4(1.00f, 1.00f, 1.00f, 0.50f);
	style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
	style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
	style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
	style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
	style.Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(0.20f, 0.20f, 0.20f, 0.35f);
}

void PlayScene::m_updateUI()
{
	// Prepare Window Frame
	ImGui::NewFrame();

	ImGui::Begin("Settings ", NULL, ImGuiWindowFlags_AlwaysAutoResize  | ImGuiWindowFlags_MenuBar);

	// set window to top getLeft corner
	ImGui::SetWindowPos(ImVec2(0, 0), true);

	/*************************************************************************************************/
	/* MENU                                                                                          */
	/*************************************************************************************************/

	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			ImGui::Separator();
			ImGui::MenuItem("Exit", NULL, &m_exitApp);
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}
	if (m_exitApp)
	{
		TheGame::Instance()->quit();
	}
	if (m_displayAbout)
	{
		ImGui::Begin("Projectile Tragectory", &m_displayAbout, ImGuiWindowFlags_AlwaysAutoResize);
		ImGui::Separator();
		ImGui::Text("Authors:");
		ImGui::Text("Eric Galway & Ian Chapman ");
		ImGui::End();
	}
	//ImGui::SameLine();
	checkGuiChangs();

	// Main Window End
	ImGui::End();
}


#include "lab_m1/lab1/lab1.h"

#include <vector>
#include <iostream>
#include <glm/gtx/matrix_transform_2d.hpp>

#include <windows.h>
#include <shellapi.h>


using namespace std;
using namespace m1;

vector<string> questions = { "Recruitment is based on the manager's capacity to identify the ______  of the product.",
							"The final decision-making stage in the recruitment process is called _______",
							"The two types of ________ evaluation are: individual and team-based.",
							"Which organizational theory states that workers are motivated by a sense of commitment?",
							//"What is the name of the graph that indicates the level of involvement and responsibility of employees?",
							"What percentage of the time does a manager spend communicating?",
							"The group in charge of reviewing the technical specifications and authorizing changes is called:_____ _____",
							"The number of steps necessary to establish a communication plan.",
							"The extended systematic model of risk management includes _ steps.",
							"The _____ represents the various risk categories that can trigger negative events." };

vector<string> answers = {
	"REQUIREMENTS",
	"SELECTION",
	"PERFORMANCE",
	"Z THEORY",
	//"RESPONSIBLE ACCOUNTABLE CONSULTED INFORMED",
	"805",
	"CONTROL BOARD",
	"8",
	"8",
	"RISK SOURCE" };

vector<string> hints = {
	"RE_________",
	"REQU_______",
	"REQUIR_____",
	"REQUIRMEN__",
	"REQUIREMENTS",
	"SE_______",
	"SELE_____",
	"SELECT___",
	"SELECTI__",
	"SELECTION",
	"PE_________",
	"PERF_______",
	"PERFOR_____",
	"PERFORMAN__",
	"PERFORMANCE",
	"Z ______",
	"Z T_____",
	"Z THE___",
	"Z THEO__",
	"Z THEORY",
	/*"RESPONSIBLE __________ ________ ________",
	"RESPONSIBLE ACCOUNTABLE _________ ________",
	"RESPONSIBLE ACCOUNTABLE CONSULTED _______",
	"RESPONSIBLE ACCOUNTABLE CONSULTED INF_____",
	"RESPONSIBLE ACCOUNTABLE CONSULTED INFORMED",*/
	">70%/",
	">70% && <90%",
	"80%",
	"80%",
	"80%",
	"CO______ _____",
	"CONT___ _____",
	"CONTROL _____",
	"CONTROL BOA__",
	"CONTROL BOARD",
	"2^3",
	"8",
	"8",
	"8",
	"8",
	"pi's eleventh decimal",
	"8",
	"8",
	"8",
	"8",
	"RI__ ______",
	"RISK ______",
	"RISK SO____",
	"RISK SOUR__",
	"RISK SOURCE"
};

constexpr int CAPTURED = 50;

Lab1::Lab1()
{
	player = std::make_unique<Player>(glm::vec3(0, -10, 0), glm::vec3(0.99f, 0, 0));
	meshes["player"] = player->generateModel();

	const auto camera = GetSceneCamera();
	camera->SetPosition(glm::vec3(0, 0, 30));
	camera->SetRotation(glm::vec3(0, 0, 0));
	camera->Update();
	GetCameraInput()->SetActive(false);

	show_credits = false;

	corners = { 29, 16 };

	capturedCircles = 0;
	timp_ramas_evadat = NULL;
	evadat = false;
	exit_open = false;

	checkedWord = false;
	checkedWord1 = false;
	checkedWord2 = false;

	hint = false;
	intrebare_curenta = 0;
	timp_limita = 0;
	timp_hint = 0;
	raspuns = "";

	text_renderer = new gfxc::TextRenderer(window->props.selfDir, window->GetResolution().x, window->GetResolution().y);
	text_renderer->Load(RESOURCE_PATH::FONTS + "\\" + "Hack-Bold.ttf", 128);

	magicNum = rand() % 88889 + 11111;

	q1 = rand() % questions.size();

	do
	{
		q2 = rand() % questions.size();
	} while (q1 == q2);

	do
	{
		q3 = rand() % questions.size();
	} while (q3 == q1 || q3 == q2);

	cronometru = 0;
	cronometru_afisare = 0;
	cronometru_limita = 0;
	menu = false;
	title_screen = true;
	max_hint = 0;
}

Lab1::~Lab1()
{
}

void Lab1::Init()
{
	{
		Mesh* mesh = new Mesh("square");

		std::vector<VertexFormat> vertices;
		glm::vec3 color = { 0, 0, 0 };
		vertices.emplace_back(glm::vec3(-1, 1, 0), color);
		vertices.emplace_back(glm::vec3(1, 1, 0), color);
		vertices.emplace_back(glm::vec3(-1, -1, 0), color);
		vertices.emplace_back(glm::vec3(1, -1, 0), color);

		mesh->InitFromData(vertices, { 0, 1, 3, 2, 0, 2, 1, 3 });
		mesh->SetDrawMode(GL_LINES);

		meshes["square"] = mesh;
	}

	{
		Mesh* mesh = new Mesh("square");

		std::vector<VertexFormat> vertices;
		glm::vec3 color = { 1, 0, 0 };
		vertices.emplace_back(glm::vec3(-1, 1, 0), color);
		vertices.emplace_back(glm::vec3(1, 1, 0), color);
		vertices.emplace_back(glm::vec3(-1, -1, 0), color);
		vertices.emplace_back(glm::vec3(1, -1, 0), color);

		mesh->InitFromData(vertices, { 0, 1, 3, 2, 0, 2, 1, 3 });
		mesh->SetDrawMode(GL_LINES);

		meshes["squareRed"] = mesh;
	}

	{
		Mesh* mesh = new Mesh("circle");

		std::vector<VertexFormat> vertices;
		std::vector<unsigned int> indices;

		for (int i = 0; i < 50; ++i)
		{
			float arg = (2.0 * M_PI * i) / 50;
			vertices.emplace_back(
				glm::vec3(cos(arg) * 0.57f, sin(arg) * 0.57f, 0),
				glm::vec3(0.911f, 0.188f, 0.217f));
			indices.push_back(i);
		}

		mesh->InitFromData(vertices, indices);
		meshes["circle"] = mesh;
		meshes["circle"]->SetDrawMode(GL_TRIANGLE_FAN);
	}

	for (int i = 0; i < 3; i++)
	{
		int rnd = rand() % 2;
		int signX = rnd == 1 ? rnd : -1;

		rnd = rand() % 2;
		int signY = rnd == 1 ? rnd : -1;

		int x = rand() % (corners[0] - 1) + 1;
		x *= signX;

		int y = rand() % (corners[1] - 1) + 1;
		y *= signY;

		circlePositions.emplace_back(x, y);
	}

	for (int i = 0; i < 3; i++)
	{
		int rnd = rand() % 2;
		int signX = rnd == 1 ? rnd : -1;

		rnd = rand() % 2;
		int signY = rnd == 1 ? rnd : -1;

		int x = rand() % (corners[0] - 1) + 1;
		x *= signX;

		int y = rand() % (corners[1] - 1) + 1;
		y *= signY;

		targetPositions.emplace_back(x, y);
	}

	hint = false;

	auto str = PATH_JOIN("file:///", window->props.selfDir,"src", "Credits.pdf");
	if(show_credits)
		ShellExecute(0, 0, str.c_str(), 0, 0, SW_SHOW);
}

void Lab1::FrameStart()
{
	glm::ivec2 resolution = window->props.resolution;
	glClearColor(0.5, 0.5, 0.5, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, resolution.x, resolution.y);
}

void Lab1::Update(float deltaTimeSeconds)
{
	//cronometru += deltaTimeSeconds;
	//cronometru_limita += deltaTimeSeconds;
	////cronometru = static_cast<int>(cronometru);
	//cron_int = (int)cronometru;

	//cronometru_afisare = (int)cronometru;

	//cron_int = cron_int / 10;
	//if (cron_int >= 4)
	//    cron_int = 4;

	auto playerPos = player->position + glm::vec2(0.6, 0.6);
	auto playerNeg = player->position - glm::vec2(0.6, 0.6);

	if (title_screen) {
		glm::mat3 model = glm::mat3(1);
		model = glm::translate(model, player->position);
		RenderMesh2D(meshes["player"], shaders["VertexColor"], model);

		if (playerNeg.x < 0 && 0 < playerPos.x && playerNeg.y < -3 && -3 < playerPos.y)  //daca e pe butonul de play
		{
			title_screen = false;
			menu = true;
		}

		if (playerNeg.x < 25 && 23 < playerPos.x && playerNeg.y < -13.5 && -14.5 < playerPos.y) //daca e pe butonul de credits
		{
			text_renderer->RenderText("Escape Room Game", 300, 200, 0.6f, { 0.90, 0.90, 0.10 });
			text_renderer->RenderText("Play", 590, 400, 0.35f, { 0.10, 0.90, 0.10 });

			auto str = PATH_JOIN("file:///", window->props.selfDir, "src", "Credits.pdf");
			if(!show_credits)
				ShellExecute(0, 0, str.c_str(), 0, 0, SW_SHOW);
			show_credits = true;
			
		}

		else {
			text_renderer->RenderText("Escape Room Game", 300, 200, 0.6f, { 0.90, 0.90, 0.10 });
			text_renderer->RenderText("Play", 590, 400, 0.35f, { 0.10, 0.90, 0.10 });
		}

		
		text_renderer->RenderText("Use WASD or arrow-keys to move", 50, 650, 0.15f, { 0.99, 0.99, 0.99 });
		text_renderer->RenderText("Credits", 1100, 650, 0.2f, { 0.90, 0.90, 0.90 });

		

	}
	else if (menu) //daca se afla in meniul de selectare a dificultatii
	{
		glm::mat3 model = glm::mat3(1);
		model = glm::translate(model, player->position);
		RenderMesh2D(meshes["player"], shaders["VertexColor"], model);

		RenderMesh2D(meshes["player"], glm::translate(glm::mat3(1), { -10, 2 }), glm::vec3(0, 1, 0));
		RenderMesh2D(meshes["player"], glm::translate(glm::mat3(1), { 0, 2 }), glm::vec3(0, 1, 1));
		RenderMesh2D(meshes["player"], glm::translate(glm::mat3(1), { 10, 2 }), glm::vec3(0, 0, 0));

		text_renderer->RenderText("Choose the difficulty", 430, 100, 0.29f, { 0.99, 0.99, 0.99 });

		text_renderer->RenderText("Easy", 410, 280, 0.15f, { 0.99, 0.99, 0.99 });
		text_renderer->RenderText("Time: 5 min", 375, 360, 0.12f, { 0.99, 0.99, 0.99 });
		text_renderer->RenderText("20 sec for new hint", 340, 380, 0.12f, { 0.99, 0.99, 0.99 });
		text_renderer->RenderText("Max 5 hints/question", 340, 400, 0.12f, { 0.99, 0.99, 0.99 });



		text_renderer->RenderText("Medium", 605, 280, 0.15f, { 0.99, 0.99, 0.99 });
		text_renderer->RenderText("Time: 4 min", 585, 360, 0.12f, { 0.99, 0.99, 0.99 });
		text_renderer->RenderText("30 sec for new hint", 550, 380, 0.12f, { 0.99, 0.99, 0.99 });
		text_renderer->RenderText("Max 4 hints/question", 550, 400, 0.12f, { 0.99, 0.99, 0.99 });


		text_renderer->RenderText("Hard", 825, 280, 0.15f, { 0.99, 0.99, 0.99 });
		text_renderer->RenderText("Time: 3 min", 800, 360, 0.12f, { 0.99, 0.99, 0.99 });
		text_renderer->RenderText("40 sec for new hint", 775, 380, 0.12f, { 0.99, 0.99, 0.99 });
		text_renderer->RenderText("Max 3 hints/question", 775, 400, 0.12f, { 0.99, 0.99, 0.99 });



		text_renderer->RenderText("Instructions:", 500, 490, 0.29f, { 0.99, 0.99, 0.99 });
		text_renderer->RenderText("Use WASD or arrow-keys to move", 400, 540, 0.15f, { 0.99, 0.99, 0.99 });
		text_renderer->RenderText("Press space to release the balls", 400, 570, 0.15f, { 0.99, 0.99, 0.99 });
		text_renderer->RenderText("Hover over the dif level you want", 400, 600, 0.15f, { 0.99, 0.99, 0.99 });

		text_renderer->RenderText("(Back)", 50, 650, 0.2f, { 0.99, 0.99, 0.99 });


		glm::mat3 circlePos = glm::mat3(1);
		/*auto playerPos = player->position + glm::vec2(0.6, 0.6);
		auto playerNeg = player->position - glm::vec2(0.6, 0.6);*/


		// Easy mode
		if (playerNeg.x < -10 && -10 < playerPos.x && playerNeg.y < 2 && 2 < playerPos.y)
		{
			menu = false;
			timp_limita = 300;
			timp_hint = 20;
			max_hint = 4;

		}
		// Medium mode
		else if (playerNeg.x < 0 && 0 < playerPos.x && playerNeg.y < 2 && 2 < playerPos.y)
		{
			menu = false;
			timp_limita = 240;
			timp_hint = 30;
			max_hint = 3;
		}
		// Hard mode
		else if (playerNeg.x < 10 && 10 < playerPos.x && playerNeg.y < 2 && 2 < playerPos.y)
		{
			menu = false;
			timp_limita = 180;
			timp_hint = 40;
			max_hint = 2;
		}
		// Back
		else if (playerNeg.x < -26 && -26 < playerPos.x && playerNeg.y < -14 && -14 < playerPos.y)
		{
			menu = false;
			title_screen = true;
		}
		//RenderMesh2D(meshes["player"], glm::translate(glm::mat3(1), { -26, -14 }), glm::vec3(0, 0, 0));   pozitia hitboxului vizualizata

	}
	else //daca se afla in joc
	{
		cronometru += deltaTimeSeconds;
		cronometru_limita += deltaTimeSeconds;
		//cronometru = static_cast<int>(cronometru);
		cron_int = (int)cronometru;

		cronometru_afisare = (int)cronometru;

		cron_int = cron_int / timp_hint;
		if (cron_int >= max_hint)
			cron_int = max_hint;

		

		// de schimbat variabila  <-- ce inseamna comentariul asta catalin?
		if (timp_limita - (int)cronometru_limita <= 0)  //ecranul de game over
		{
			text_renderer->RenderText("Game Over! Your time has expired", 250, 300, 0.3f, { 0.99, 0.10, 0.10 });

			if (timp_limita - (int)cronometru_limita <= -5)
			{
				exit(0);
			}
		}
		else if (evadat) {   //ecranul de victorie
			if (timp_ramas_evadat == NULL)
				timp_ramas_evadat = timp_limita - (int)cronometru_limita;

			text_renderer->RenderText("Congratulations! You escaped the room", 250, 300, 0.3f, { 0.10, 0.90, 0.10 });
			text_renderer->RenderText("Remaining time: " + to_string(timp_ramas_evadat) + " sec", 260, 350, 0.22f, { 0.10, 0.70, 0.10 });

			if (timp_limita - (int)cronometru_limita  <= timp_ramas_evadat - 5)
			{
				exit(0);
			}
		}
		else  // daca e inca in timpul jocului
		{

			glm::mat3 model = glm::mat3(1);
			model = glm::translate(model, player->position);
			RenderMesh2D(meshes["player"], shaders["VertexColor"], model);

			RenderMesh2D(meshes["player"], glm::translate(glm::mat3(1), { -25, -15 }), glm::vec3(0, 0, 0));
			glm::mat3 circlePos = glm::mat3(1);
			auto playerPos = player->position + glm::vec2(1.0, 1.0);
			auto playerNeg = player->position - glm::vec2(1.0, 1.0);

			if (playerNeg.x < -25 && -25 < playerPos.x && playerNeg.y < -15 && -15 < playerPos.y)
			{
				hint = true;
			}
			else {
				hint = false;
			}

			switch (intrebare_curenta)
			{
			case 1:
				if (chars_pressed.size() <= answers[q1].size())
				{
					string cuvant = chars_pressed;
					text_renderer->RenderText("Input: " + cuvant, 700, 680, 0.15f, { 0.99, 0.99, 0.99 });
				}
				else
				{
					text_renderer->RenderText("Input: " + raspuns, 700, 680, 0.15f, { 0.99, 0.99, 0.99 });

				}
				break;

			case 2:
				if (chars_pressed.size() <=answers[q2].size())
				{
					string cuvant = chars_pressed;
					text_renderer->RenderText("Input: " + cuvant, 700, 680, 0.15f, { 0.99, 0.99, 0.99 });
				}
				else
				{
					text_renderer->RenderText("Input: " + raspuns, 700, 680, 0.15f, { 0.99, 0.99, 0.99 });

				}
				break;

			case 3:
				if (chars_pressed.size() <= answers[q3].size())
				{
					string cuvant = chars_pressed;
					text_renderer->RenderText("Input: " + cuvant, 700, 680, 0.15f, { 0.99, 0.99, 0.99 });
				}
				else
				{
					text_renderer->RenderText("Input: " + raspuns, 700, 680, 0.15f, { 0.99, 0.99, 0.99 });

				}
				break;

			case 4:
				if (chars_pressed.size() > 5)
				{
					haubau = (int)cronometru_limita;
					chars_pressed.clear();
				}

				cout << "Cron limita: " << cronometru_limita;
				cout << "HauBau: " << haubau;
				cout << "In afara IF ului: " << (int)cronometru_limita - haubau << '\n';


				if ((int)cronometru_limita - haubau < 1)
				{
					text_renderer->RenderText("Input: Try again! ", 700, 680, 0.15f, { 0.99, 0.99, 0.99 });
					cout << (int)cronometru_limita - haubau << '\n';
				}
				else {
					string cuvant = chars_pressed;
					text_renderer->RenderText("Input: " + cuvant, 700, 680, 0.15f, { 0.99, 0.99, 0.99 });
				}

				break;
			}


			for (auto& pos : circlePositions)
			{

				if (playerNeg.x < pos.x && pos.x < playerPos.x)
				{
					if (playerNeg.y < pos.y && pos.y < playerPos.y)
					{
						if (!player->isInDeliveryZone(deliveryPos1, deliveryPos2, deliveryNeg1, deliveryNeg2))
						{
							pos.x = CAPTURED;
							capturedCircles++;
						}
					}
				}

				if (pos.x != CAPTURED)
				{
					RenderMesh2D(meshes["circle"], shaders["VertexNormal"], glm::translate(circlePos, pos));
				}
			}

			glm::mat3 deliveryZone = glm::mat3(1);
			deliveryZone = glm::translate(deliveryZone, { -26, 11 });
			deliveryZone = glm::scale(deliveryZone, { 3, 5 });

			char* text = new char[30];

			text_renderer->RenderText("Time left: " + to_string(timp_limita - (int)cronometru_limita) + " sec", 1010, 50, 0.15f, { 0.99, 0.99, 0.99 });
			


			if (hint) {
				switch (intrebare_curenta)
				{
				case 0:
					text_renderer->RenderText("Press space to release in the red zone.", 50, 600, 0.2f, { 0.99, 0.99, 0.99 });
					break;
				case 1:
					text_renderer->RenderText(hints[q1 * 5 + cron_int], 50, 600, 0.2f, { 0.99, 0.99, 0.99 });
					break;
				case 2:
					text_renderer->RenderText(hints[q2 * 5 + cron_int], 50, 600, 0.2f, { 0.99, 0.99, 0.99 });
					break;
				case 3:
					text_renderer->RenderText(hints[q3 * 5 + cron_int], 50, 600, 0.2f, { 0.99, 0.99, 0.99 });
					break;

				default:
					text_renderer->RenderText("Insert the key. The key is in front of you!", 50, 600, 0.2f, { 0.99, 0.99, 0.99 });
					break;
				}



			}
			else {
				text_renderer->RenderText("Need a hint?", 50, 600, 0.2f, { 0.99, 0.99, 0.99 });
			}

			if (capturedCircles >= 6)
			{
				if (checkedWord2)
				{
					sprintf(text, "Secret code: %d", magicNum);
					if(!exit_open)
						RenderMesh2D(meshes["player"], glm::translate(glm::mat3(1), { 5, 5 }), glm::vec3(0.4f, 0, 0.99f)); //exit door (inchisa)
					else
						RenderMesh2D(meshes["player"], glm::translate(glm::mat3(1), { 5, 5 }), glm::vec3(0.99, 0.99, 0.99)); //exit door (deschisa)

					text_renderer->RenderText("Congratulations! Now you have to exit!", 200, 90, 0.2f, { 0.99, 0.99, 0.99 });
					intrebare_curenta = 4;
				}
				else if (checkedWord1)
				{
					sprintf(text, "Secret code: %d", magicNum % 10000);
					text_renderer->RenderText("You guessed it! Last one, we promise!", 200, 90, 0.2f, { 0.99, 0.99, 0.99 });
					text_renderer->RenderText(questions[q3], 200, 130, 0.13f, { 0.99, 0.99, 0.99 });
					intrebare_curenta = 3;
				}
				else if (checkedWord)
				{
					sprintf(text, "Secret code: %d", magicNum % 1000);
					text_renderer->RenderText("You guessed it! Time for part 2", 200, 90, 0.2f, { 0.99, 0.99, 0.99 });
					text_renderer->RenderText(questions[q2], 200, 130, 0.13f, { 0.99, 0.99, 0.99 });
					intrebare_curenta = 2;
				}
				else
				{
					sprintf(text, "Secret code: %d", magicNum % 100);
					text_renderer->RenderText(questions[q1], 200, 90, 0.13f, { 0.99, 0.99, 0.99 });
					intrebare_curenta = 1;
				}
			}
			else
			{
				sprintf(text, "Secret code: %d", magicNum % 10);
				cronometru = 0;
			}
			text_renderer->RenderText(text, 200, 40, 0.3f, { 0.99, 0.99, 0.99 });
			//delete text;

			RenderMesh2D(meshes["squareRed"], shaders["VertexColor"], deliveryZone);
			RenderMesh2D(meshes["square"], shaders["VertexColor"], glm::scale(glm::mat3(1), { corners[0] + 0.7f, corners[1] + 0.7f }));

			if (playerNeg.x < 5 && 5 < playerPos.x && playerNeg.y < 5 && 5 < playerPos.y && exit_open) //daca e pe usa de iesire deschisa
			{
				evadat = true;
			}

		}
	}
}

void Lab1::FrameEnd()
{
}

void Lab1::OnInputUpdate(float deltaTime, int mods)
{
	if (player->isInWindow(corners[0], corners[1]))
	{
		if (window->KeyHold(GLFW_KEY_A) || window->KeyHold(GLFW_KEY_LEFT))
		{
			player->moveLeft(deltaTime * 15);
		}
		if (window->KeyHold(GLFW_KEY_D) || window->KeyHold(GLFW_KEY_RIGHT))
		{
			player->moveRight(deltaTime * 15);
		}
		if (window->KeyHold(GLFW_KEY_W) || window->KeyHold(GLFW_KEY_UP))
		{
			player->moveUp(deltaTime * 15);
		}
		if (window->KeyHold(GLFW_KEY_S) || window->KeyHold(GLFW_KEY_DOWN))
		{
			player->moveDown(deltaTime * 15);
		}
	}
	else
	{
		player->position = { 0, 0 };
	}
}

void Lab1::OnKeyPress(int key, int mods)
{
	if (player->isInDeliveryZone(deliveryPos1, deliveryPos2, deliveryNeg1, deliveryNeg2))
	{
		if (key == GLFW_KEY_SPACE)
		{
			for (auto& p : circlePositions)
			{
				if (p.x == CAPTURED)
				{
					p = player->position;
					capturedCircles++;
					break;
				}
			}
		}
	}

	if (capturedCircles >= 6)
	{
		static int pressedChars = 0;
		if ((GLFW_KEY_A <= key && key <= GLFW_KEY_Z) || key == 32 || (key >= 48 && key <= 57))
		{
			chars_pressed.push_back(key);
			
		}
		for (int i = 0; i < chars_pressed.size(); i++)
		{
			cout << chars_pressed[i];
		}
		cout << "\n";

		raspuns = "";

		if (GLFW_KEY_BACKSPACE == key && chars_pressed.size() > 0)
		{
			chars_pressed.pop_back();
		}

		switch (intrebare_curenta)
		{
		case 1:

			for (int i = chars_pressed.size() - answers[q1].size(); i < chars_pressed.size(); i++)
			{
				raspuns = raspuns + chars_pressed[i];
			}
			cout << raspuns << "\n";
			if (raspuns == answers[q1])
			{
				chars_pressed.clear();
				checkedWord = true;
				cronometru = 0;
			}
			break;

		case 2:

			for (int i = chars_pressed.size() - answers[q2].size(); i < chars_pressed.size(); i++)
			{
				raspuns = raspuns + chars_pressed[i];
			}
			cout << raspuns << "\n";
			if (raspuns == answers[q2])
			{
				chars_pressed.clear();
				checkedWord1 = true;
				cronometru = 0;
			}
			break;

		case 3:

			for (int i = chars_pressed.size() - answers[q3].size(); i < chars_pressed.size(); i++)
			{
				raspuns = raspuns + chars_pressed[i];
			}

			if (raspuns == answers[q3])
			{
				chars_pressed.clear();
				checkedWord2 = true;
				cronometru = 0;
			}
		}

		if (checkedWord2)
		{
			/*std::vector<int> secretCode = { magicNum % 10, (magicNum / 10) % 10, (magicNum / 100) % 10 };
			std::reverse(secretCode.begin(), secretCode.end());*/

			int numeros = 0;

			if (key >= GLFW_KEY_0 && key <= GLFW_KEY_9)
			{
				nums_pressed.push_back(key - 48);
			}

			if (nums_pressed.size() >= 5)
				for (int i = nums_pressed.size() - 5; i < nums_pressed.size(); i++)
				{
					numeros = numeros * 10 + nums_pressed[i];
				}

			if (numeros == magicNum)
			{
				exit_open = true;
			}
		}
	};
}

void Lab1::OnKeyRelease(int key, int mods)
{
}

void Lab1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
}

void Lab1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
}

void Lab1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
}

void Lab1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void Lab1::OnWindowResize(int width, int height)
{
}

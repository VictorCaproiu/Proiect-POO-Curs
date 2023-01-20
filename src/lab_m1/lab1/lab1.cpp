#include "lab_m1/lab1/lab1.h"

#include <vector>
#include <iostream>
#include <glm/gtx/matrix_transform_2d.hpp>

using namespace std;
using namespace m1;


constexpr int CAPTURED = 50;

Lab1::Lab1()
{
    player = std::make_unique<Player>(glm::vec3(0, 0, 0), glm::vec3(0.99f, 0, 0));
    meshes["player"] = player->generateModel();

    const auto camera = GetSceneCamera();
    camera->SetPosition(glm::vec3(0, 0, 30));
    camera->SetRotation(glm::vec3(0, 0, 0));
    camera->Update();
    GetCameraInput()->SetActive(false);

    corners = { 29, 16 };
    capturedCircles = 0;
    checkedWord = false;

    text_renderer = new gfxc::TextRenderer(window->props.selfDir, window->GetResolution().x, window->GetResolution().y);
    text_renderer->Load(RESOURCE_PATH::FONTS + "\\" + "Hack-Bold.ttf", 128);

    magicNum = rand() % 889 + 111;
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
            vertices.emplace_back
            (
                glm::vec3(cos(arg) * 0.57f, sin(arg) * 0.57f, 0),
                glm::vec3(0.911f, 0.188f, 0.217f)
            );
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
    glm::mat3 model = glm::mat3(1);
    model = glm::translate(model, player->position);
    RenderMesh2D(meshes["player"], shaders["VertexColor"], model);

    glm::mat3 circlePos = glm::mat3(1);
    auto playerPos = player->position + glm::vec2(0.2, 0.2);
    auto playerNeg = player->position - glm::vec2(0.2, 0.2);

    for (auto& pos: circlePositions)
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
    if (capturedCircles == 6)
    {
        if (checkedWord)
        {
            sprintf(text, "Secret code: %d", magicNum % 1000);
            text_renderer->RenderText("You guessed it!", 200, 90, 0.2f, { 0.99, 0.99, 0.99 });
            RenderMesh2D(meshes["player"], glm::translate(glm::mat3(1), { 5, 5 }), glm::vec3(0.4f, 0, 0.99f));
            text_renderer->RenderText("Now enter the secret code, to escape!", 200, 130, 0.2f, {0.99, 0.99, 0.99});
        }
        else
        {
            sprintf(text, "Secret code: %d", magicNum % 100);
            text_renderer->RenderText("Actiunea care joaca un rol crucial in cadrul unui proiect.", 200, 90, 0.2f, { 0.99, 0.99, 0.99 });  // 190
        }
    }
    else
    {
        sprintf(text, "Secret code: %d", magicNum % 10);
    }
    text_renderer->RenderText(text, 200, 40, 0.3f, { 0.99, 0.99, 0.99 });
    delete text;

    RenderMesh2D(meshes["squareRed"], shaders["VertexColor"], deliveryZone);
    RenderMesh2D(meshes["square"], shaders["VertexColor"], glm::scale(glm::mat3(1), {corners[0] + 0.7f, corners[1] + 0.7f}));
}


void Lab1::FrameEnd()
{
}


void Lab1::OnInputUpdate(float deltaTime, int mods)
{
	if (player->isInWindow(corners[0], corners[1]))
	{
        if (window->KeyHold(GLFW_KEY_A))
        {
            player->moveLeft(deltaTime * 10);
        }
        if (window->KeyHold(GLFW_KEY_D))
        {
            player->moveRight(deltaTime * 10);
        }
        if (window->KeyHold(GLFW_KEY_W))
        {
            player->moveUp(deltaTime * 10);
        }
        if (window->KeyHold(GLFW_KEY_S))
        {
            player->moveDown(deltaTime * 10);
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
            for (auto& p: circlePositions)
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

    if (capturedCircles == 6)
    {
        static int pressedChars = 0;
        if (GLFW_KEY_A <= key && key <= GLFW_KEY_Z)
        {
            chars_pressed.push_back(key);
        }

        std::vector<char> question = { 'C', 'O', 'M' ,'U', 'N', 'I', 'C', 'A', 'R', 'E' };  //263
        int num = 0;

        for (auto ch: question)
        {
	        for (auto chr: chars_pressed)
	        {
		        if(ch == chr)
		        {
                    num++;
		        }
	        }
        }

        if (num == question.size())
        {
            checkedWord = true;
        }

        if (pressedChars == question8.size())
        {
            chars_pressed.clear();
        }

        if (checkedWord)
        {
            std::vector<int> secretCode = { magicNum % 10, (magicNum / 10) % 10, (magicNum / 100) % 10 };
            std::reverse(secretCode.begin(), secretCode.end());

            int numeros = 0;

            if (key >= GLFW_KEY_0 && key <= GLFW_KEY_9)
            {
                nums_pressed.push_back(key - 48);
            }

            for (auto magic: secretCode)
            {
	            for (auto num: nums_pressed)
	            {
                    if (magic == num)
                    {
                        numeros++;
                    }
	            }
            }

            if (numeros == secretCode.size())
            {
                exit(0);
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

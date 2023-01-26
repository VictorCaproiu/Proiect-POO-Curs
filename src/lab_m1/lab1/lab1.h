#pragma once

#include "player.h"
#include "components/simple_scene.h"
#include "components/text_renderer.h"
#include <string>
#include <vector>

namespace m1
{
    class Lab1 : public gfxc::SimpleScene
    {
     public:
        Lab1();
        ~Lab1();

        void Init() override;

     private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

        std::unique_ptr<Player> player;
        std::vector<GLuint> corners;
        std::vector<glm::vec2> circlePositions;
        std::vector<glm::vec2> targetPositions;

        gfxc::TextRenderer* text_renderer;

        std::vector<char> chars_pressed;
        std::vector<int> nums_pressed;

        int capturedCircles;
        int magicNum;

        int intrebare_curenta, q1, q2, q3;

        bool checkedWord;
        bool checkedWord1;
        bool checkedWord2;

        glm::vec2 deliveryPos1 = glm::vec2(-28, 15);
        glm::vec2 deliveryPos2 = glm::vec2(-23, 15);
        glm::vec2 deliveryNeg1 = glm::vec2(-28, 6);
        glm::vec2 deliveryNeg2 = glm::vec2(-23, 6);


    };
}   // namespace m1

#pragma once

#include "player.h"
#include "components/simple_scene.h"
#include "components/text_renderer.h"
#include <string>
#include <vector>
#include <time.h>

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

        //std::vector<char> chars_pressed;
        std::string chars_pressed;
        std::string nums_pressed;

        int capturedCircles;
        int magicNum;
        bool show_credits;
        int Timp_secret_code;

        int intrebare_curenta, q1, q2, q3; //Indici pentru intrebarile curente

        bool hint; //Verifica daca player ul este pe hint sau nu

        bool checkedWord;// True daca s a raspuns corect la a prima intrebare
        bool checkedWord1;// True daca s a raspuns corect la a 2a intrebare
        bool checkedWord2; // True daca s a raspuns corect la a 3a intrebare

        bool menu; // True daca suntem in meniu, false daca nu suntem in meniu
        bool title_screen;

        float cronometru;
        int cron_int;
        int cronometru_afisare;
        int timp_limita;
        int timp_hint;
        int max_hint;
        float cronometru_limita;
        bool evadat;
        bool exit_open;
        int timp_ramas_evadat;
        float punctaj_final;

        std::string raspuns;


        glm::vec2 deliveryPos1 = glm::vec2(-29, 16);
        glm::vec2 deliveryPos2 = glm::vec2(-23, 16);
        glm::vec2 deliveryNeg1 = glm::vec2(-29, 6);
        glm::vec2 deliveryNeg2 = glm::vec2(-23, 6);


    };
}   // namespace m1

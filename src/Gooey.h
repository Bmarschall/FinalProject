#pragma once
#include "AftrConfig.h"
#ifdef AFTR_CONFIG_USE_IMGUI

#include "WOImGui.h"
#include "AftrImGuiIncludes.h"
#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"
#include "Mat4.h"
#include <functional>
#include "irrKlang.h"


using namespace irrklang;

namespace Aftr {

    class Gooey : public WOImGuiAbstract {
    public:
        using Callback_OnDrawImGui = std::function< void() >;

        static Gooey* New(WOGUI* parentWOGUI, float width = 1.0f, float height = 1.0f) {
            Gooey* imgui = new Gooey(parentWOGUI);
            imgui->onCreate(width, height);
            return imgui;
        }
        virtual ~Gooey() {}

        virtual void drawImGui_for_this_frame(){

            bool popIsOpen = false;

            if (time < 0)
            {
                gameStarted = false;
                winner = "Maze";
            }

            if (ImGui::BeginPopup("Menu"))
            {

                ImGui::Indent(150);
                ImGui::Text("Xtreme Ball Roll");
                ImGui::Spacing();
                ImGui::Separator();
                ImGui::Indent(25);
                ImGui::Text("Main Menu");
                ImGui::Indent(-175);
                ImGui::Spacing();
                ImGui::Text("Audio");
                ImGui::SliderFloat("Backgroud Volume", &volume, 0.0f, 1.0f);
                ImGui::SliderFloat("Ball Volume", &volume3, 0.0f, 1.0f);
                ImGui::Text("Misc");
                ImGui::SliderFloat("Maximum Table Tilt", &MAX_TILT, 0.0f, 20.0f);
                ImGui::SliderFloat("Table Tilt Speed", &TILT_SPEED, 0.0f, 10.0f);
                ImGui::SliderFloat("Gravity of Ball", &Gravity, -250.0f, 250.0f);
                ImGui::SliderFloat("Ball Mult", &ballMult, 0, 10);

                if (ImGui::Button("START"))
                {
                    ImGui::CloseCurrentPopup();
                    gameStarted = true;
                    is2d = true;
                    is3d = true;
                    resetBall = true;
                    winner = "";
                    time = 60.0f;
                    startTime += ImGui::GetTime();
                }

                ImGui::EndPopup();
            }

            if (!(gameStarted)) {
                if (popIsOpen)
                {
                    popIsOpen = false;
                }
                else {
                    ImGui::OpenPopup("Menu");
                }
            }

            ImGui::BeginMainMenuBar();
            if (ImGui::Button("Main Menu"))
            {
                gameStarted = false;
            }
            ImGui::Text(" Time Left ->");
            if (time != 0.0f) {
                time = 60 - (ImGui::GetTime() - startTime);
            }
           
            if (time > 0) {

                ImGui::ProgressBar(time / 60);

            }
            else {
                char* char_arr;
                std::string str_obj("Game Over. " + winner + " wins!");
                char_arr = &str_obj[0];
                ImGui::Text(char_arr);
            }



            ImGui::EndMainMenuBar();

            if (guiDebug) {
                ImGui::Begin("Tilta-whirl O'Matic 5000");
                if (gameStarted) {

                    ImGui::Separator();
                    ImGui::Text("Audio");
                    if (ImGui::Button("Pause Music")) {
                        pause2d = true;
                    }
                    ImGui::SameLine();
                    if (ImGui::Button("Play Music")) {
                        pause2d = true;
                    }
                    ImGui::SliderFloat("Music Volume", &volume, 0.0f, 1.0f);

                    if (ImGui::Button("Play Sound Effects")) {
                        is3d = true;
                    }

                    ImGui::SliderFloat("Ball Volume", &volume3, 0.0f, 1.0f);
                    ImGui::SameLine();
                    if (ImGui::Button("Pause")) {
                        pause3d = true;
                    }

                    ImGui::Separator();
                    ImGui::SliderFloat("MAX_TILT", &MAX_TILT, 0.0f, 20.0f);
                    ImGui::SliderFloat("TILT_SPEED", &TILT_SPEED, 0.0f, 10.0f);
                    ImGui::SliderFloat("Z Gravity", &Gravity, -250.0f, 250.0f);
                    ImGui::SliderFloat("X Gravity", &XGravity, -5.0f, 5.0f);
                    ImGui::SliderFloat("Y Gravity", &YGravity, -5.0f, 5.0f);

                    ImGui::Separator();
                    ImGui::SliderFloat("rollMult", &rollMult, 0, 500.0f);

                    if (ImGui::Button("Drop Ball")) {
                        dropBall = true;
                    }
                    if (ImGui::Button("Reset Ball")) {
                        resetBall = true;
                    }


                    ImGui::SliderFloat("dropBallHeight", &dropBallHeight, 10.0f, 100.0f);

                    ImGui::Separator();


                    ImGui::Separator();


                    if (ImGui::Button("Game Win")) {
                        pause3d = true;
                    }
                    ImGui::SameLine();
                    if (ImGui::Button("Game Lose")) {
                        pause3d = true;
                    }
                }
                ImGui::End();
            }

            this->fileDialog_update_after_ImGui_end_is_called(); 
        }
        void subscribe_drawImGuiWidget(Callback_OnDrawImGui callback) {
            this->subscribers_OnDrawImGui.push_back(callback);
        }

        static void draw_gui(WOGUI* gui) {
            ImGui::Begin("Menu");
            ImGui::Separator();
            ImGui::End();

        }

        void drawMainMenu() { gameStarted = false; }

        float get_2d_volume() { return volume; }
        bool getIsPaused() { return is2d; }
        void setIsPaused(bool flag) { is2d = flag; }

        bool is2d = false;
        bool is3d = false;
        bool pause2d = false;
        bool pause3d = false;

        bool resetBall = false;
        bool dropBall = false;

        bool gameStarted = false;
        bool guiDebug = false;

        float MAX_TILT = 15;
        float TILT_SPEED = 1;
        float Gravity = -100.81;
        float XGravity = 0;
        float YGravity = 0;
        float time;
        std::string winner = "";

        float ballMult = 1;
        float rollMult = 50;
        float dropBallHeight = 80;
        float volume3 = 1.0f;

    protected:
        virtual void onCreate(float width, float height) override {
            WOImGuiAbstract::onCreate(width, height);
        }
        Gooey(WOGUI* parentWOGUI) : IFace(this), WOImGuiAbstract(parentWOGUI){}
    private:

        float volume = 0.002f;
        double startTime = 0;
        

        std::vector< Callback_OnDrawImGui > subscribers_OnDrawImGui;
    };

}

#endif
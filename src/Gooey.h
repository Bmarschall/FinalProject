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
            ImGui::Begin("Tilta-whirl O'Matic 5000");

            //ImGui::Text("Relative Rotation");
            //ImGui::SliderFloat("Relative X", &r[0], -90.0f, 90.0f);
            //ImGui::SliderFloat("Relative Y", &r[1], -90.0f, 90.0f);
            //ImGui::SliderFloat("Relative Z", &r[2], -90.0f, 90.0f);

            //ImGui::Text("Global Rotation");
            //ImGui::SliderFloat("Global X", &g[0], -90.0f, 90.0f);
            //ImGui::SliderFloat("Global Y", &g[1], -90.0f, 90.0f);
            //ImGui::SliderFloat("Global Z", &g[2], -90.0f, 90.0f);

            //ImGui::Text("Translate");
            //ImGui::SliderFloat("Position X", &t[0], -90.0f, 90.0f);
            //ImGui::SliderFloat("Position Y", &t[1], -90.0f, 90.0f);
            //ImGui::SliderFloat("Position Z", &t[2], -90.0f, 90.0f);

            ImGui::Separator();
            ImGui::Text("Audio");
            if (ImGui::Button("Play 2d Sound")) {
                is2d = true;
            }
            ImGui::SameLine();
            if (ImGui::Button("Pause/ UnPause")) {
                pause2d = true;
            }
            ImGui::SliderFloat("2d Volume", &volume, 0.0f, 1.0f);

            if (ImGui::Button("Play 3d Sound")) {
                is3d = true;
            }

            ImGui::SliderFloat("3d Volume", &volume3, 0.0f, 1.0f);
            ImGui::SameLine();
            if (ImGui::Button("Pause")) {
                pause3d = true;
            }
            ImGui::SliderFloat("3d Volume", &volume3, 0.0f, 2.0f);

            ImGui::Separator();
            ImGui::SliderFloat("MAX_TILT", &MAX_TILT, 0.0f, 20.0f);
            ImGui::SliderFloat("TILT_SPEED", &TILT_SPEED, 0.0f, 10.0f);
            ImGui::SliderFloat("Z Gravity", &Gravity, -250.0f, 250.0f);
            ImGui::SliderFloat("X Gravity", &XGravity, -5.0f, 5.0f);
            ImGui::SliderFloat("Y Gravity", &YGravity, -5.0f, 5.0f);

            ImGui::Separator();
            ImGui::SliderFloat("rollMult", &rollMult, 0, 50.0f);

            if (ImGui::Button("Drop Ball")) {
                dropBall = true;
            }
            if (ImGui::Button("Reset Ball")) {
                resetBall = true;
            }

            if (ImGui::Button("Drop Ball")) {
                dropBall = true;
            }


            ImGui::SliderFloat("dropBallHeight", &dropBallHeight, 10.0f, 100.0f);



            ImGui::End();

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

        float get_2d_volume() { return volume; }

        bool is2d = false;
        bool is3d = false;
        bool pause2d = false;
        bool pause3d = false;

        bool resetBall = false;
        bool dropBall = false;
        float MAX_TILT = 15;
        float TILT_SPEED = 1;
        float Gravity = -9.81;
        float XGravity = 0;
        float YGravity = 0;


        float rollMult = 20;
        float dropBallHeight = 80;
        float volume3 = 1.0f;

    protected:
        virtual void onCreate(float width, float height) override {
            WOImGuiAbstract::onCreate(width, height);
        }
        Gooey(WOGUI* parentWOGUI) : IFace(this), WOImGuiAbstract(parentWOGUI){}
    private:

        float volume = 0.002f;
        

        std::vector< Callback_OnDrawImGui > subscribers_OnDrawImGui;
    };

}

#endif
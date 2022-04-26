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
            ImGui::Begin("Assignment 4 GUI");
            ImGui::Text("Postion Testing");

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
            ImGui::Text("Audio Testing");
            if (ImGui::Button("Play 2d Sound")) {
                is2d = true;
            }
            ImGui::SameLine();
            if (ImGui::Button("Pause/ UnPause")) {
                pause2d = true;
            }
            ImGui::SliderFloat("2d Volume", &volume, 0.0f, 1.0f);

            ImGui::Separator();
            ImGui::SliderFloat("MAX_TILT", &MAX_TILT, 0.0f, 20.0f);
            ImGui::SliderFloat("TILT_SPEED", &TILT_SPEED, 0.0f, 10.0f);
            ImGui::SliderFloat("Gravity", &Gravity, -10.0f, 10.0f);

            ImGui::Separator();

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
        bool pause2d = false;
        bool dropBall = false;
        float MAX_TILT = 15;
        float TILT_SPEED = 1;
        float Gravity = -9.81;
        float dropBallHeight = 80;


    protected:
        virtual void onCreate(float width, float height) override {
            WOImGuiAbstract::onCreate(width, height);
        }
        Gooey(WOGUI* parentWOGUI) : IFace(this), WOImGuiAbstract(parentWOGUI){}
    private:

        float volume = 0.002f;
        float volume3 = 1.0f;

        std::vector< Callback_OnDrawImGui > subscribers_OnDrawImGui;
    };

}

#endif
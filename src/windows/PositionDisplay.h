#pragma once
#include "ImGuiWindow.h"
#include <imgui.h>
#include "objects/Character.h"

class PositionDisplay : public ImGuiWindow
{
    glm::vec3 position;
    glm::mat4 viewMatrix;
    float yaw;
    float pitch;
    Character *character;

public:
    PositionDisplay(Character *character) : ImGuiWindow("Position Display", true)
    {
        this->character = character;
    }

    void update()
    {
        position = character->get_position();
        viewMatrix = character->get_view();
        yaw = character->get_look_angles().y;
        pitch = character->get_look_angles().x;
    }

    void render() override
    {
        ImGui::Begin(get_name().c_str(), nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove);
        ImGui::Text("Position: (%.2f, %.2f, %.2f)", position.x, position.y, position.z);
        ImGui::BeginTable("view_matrix", 4, ImGuiTableFlags_NoClip);
        for (int i = 0; i < 4; i++)
        {
            ImGui::TableNextRow();
            for (int j = 0; j < 4; j++)
            {
                ImGui::TableSetColumnIndex(j);
                ImGui::Text("%.2f", viewMatrix[j][i]);
            }
        }
        ImGui::EndTable();
        ImGui::Text("Yaw: %.2f", yaw);
        ImGui::Text("Pitch: %.2f", pitch);
        ImGui::End();
    }
};
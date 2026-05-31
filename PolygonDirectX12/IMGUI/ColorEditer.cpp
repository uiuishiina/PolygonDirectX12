#if defined(_DEBUG)

#include"ColorEditer.h"
#include"ImguiManager.h"

//---------------------------------------------------------------------------------
void ColorEditor::draw(colorDate& data) noexcept {
    // ウィンドウ : ブラー設定
    ImGui::Begin("カラー設定");
    {
        int count{};
        for (auto& p : data.color_date) {
            ImGui::PushID(static_cast<int>(count++));
            // 色を変更する項目を追加する
            ImGui::ColorEdit4("色", &p.R);
            ImGui::PopID();
        }
    }
    ImGui::End();
}
#endif
#include <iostream>
#include <SFML/Graphics.hpp>

#include "Cursor.h"
#include "imgui-SFML.h"
#include "imgui.h"

int main() {
    auto window = sf::RenderWindow(sf::VideoMode({1920u, 1080u}), "SF Text Editor");
    window.setFramerateLimit(144);

    if (!ImGui::SFML::Init(window)) {
        std::cerr << "Failed to initialize ImGui!" << std::endl;
    }

    sf::Font font;
    if (!font.openFromFile(R"(C:\Windows\Fonts\CascadiaCode.ttf)")) {
        std::cerr << "Failed to load font!" << std::endl;
    }

    const sf::Glyph exampleGlyph { font.getGlyph('A', 30, false) };
    const sf::Glyph tabGlyph { font.getGlyph('\t', 30, false) };
    const float line_height{exampleGlyph.bounds.size.y + 10.f};
    std::vector<sf::String> text_lines;
    text_lines.emplace_back("");

    sft::Cursor cursor {0 , 0};
    sf::RectangleShape cursor_rect {sf::Vector2f{5, exampleGlyph.bounds.size.y + 10.f}};
    char32_t char_typed;


    sf::Clock delta_clock;
    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event.has_value()) {
                ImGui::SFML::ProcessEvent(window, event.value());
            }
            if (event->is<sf::Event::Closed>()) {
                window.close();
            } else if (const auto* key_pressed = event->getIf<sf::Event::KeyPressed>()) {
                if (key_pressed->scancode == sf::Keyboard::Scancode::Backspace) {
                    if (cursor.col == 0) {
                        if (cursor.row > 0) {
                            cursor.row--;
                            text_lines.pop_back();
                            cursor.col = static_cast<int>(text_lines.back().getSize());
                        }
                    } else {
                        sf::String str { text_lines.at(cursor.row) };
                        text_lines.at(cursor.row) = str.substring(0, str.getSize() - 1);
                        cursor.col--;
                    }
                } else if (key_pressed->scancode == sf::Keyboard::Scancode::Enter) {
                    if (cursor.row == text_lines.size() - 1) {
                        if (cursor.col == text_lines.at(cursor.col).getSize() - 1) {
                            text_lines.emplace_back("");
                        } else {
                            text_lines.emplace_back(text_lines.at(cursor.col).substring(cursor.col));
                        }
                    } else {
                        sf::String str { text_lines.at(cursor.row).substring(cursor.col) };
                        text_lines.insert(cursor.row, str);
                    }
                    text_lines.emplace_back("");
                    cursor.row++;
                    cursor.col = 0;
                } else if (key_pressed->scancode == sf::Keyboard::Scancode::Left) {
                    if (cursor.col > 0) {
                        cursor.col--;
                    }
                } else if (key_pressed->scancode == sf::Keyboard::Scancode::Right) {
                    if (cursor.col < text_lines.at(cursor.row).getSize()) {
                        cursor.col++;
                    }
                } else if (key_pressed->scancode == sf::Keyboard::Scancode::Up) {
                    if (cursor.row > 0) {
                        cursor.row--;
                    }
                } else if (key_pressed->scancode == sf::Keyboard::Scancode::Down) {
                    if (cursor.row < text_lines.size()) {
                        cursor.row++;
                    }
                }
            } else if (const auto* text_entered = event->getIf<sf::Event::TextEntered>()) {
                char_typed = text_entered->unicode;
                // 8 -> Backspace, 13 -> Enter key
                if (char_typed != 8 && char_typed != 13) {
                    if (cursor.col == text_lines.at(cursor.row).getSize()) {
                        text_lines.at(cursor.row) += char_typed;
                    } else {
                        text_lines.at(cursor.row).insert(cursor.col, char_typed);
                    }
                    cursor.col++;
                }
            }
        }

        cursor_rect.setPosition({cursor.col * exampleGlyph.advance, cursor.row * line_height});
        ImGui::SFML::Update(window, delta_clock.restart());

        ImGui::Begin("Test");
        ImGui::Text("UNICODE: " + char_typed);
        ImGui::Text(("ROW: " + std::to_string(cursor.row)).c_str());
        ImGui::Text(("COLUMN: " + std::to_string(cursor.col)).c_str());
        ImGui::End();

        window.clear();
        for (int i = 0; i < text_lines.size(); i++) {
            sf::Text text{font, text_lines.at(i), 30};
            text.setPosition({0, static_cast<float>(i) * line_height});
            window.draw(text);
        }
        window.draw(cursor_rect);
        ImGui::SFML::Render(window);
        ImGui::EndFrame();
        window.display();
    }

    ImGui::SFML::Shutdown();
    return 0;
}

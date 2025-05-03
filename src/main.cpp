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
    const float line_height{exampleGlyph.bounds.size.y + 5.f};

    std::vector<sf::String> text_lines;
    text_lines.emplace_back("");

    sft::Cursor cursor {0 , 0};
    sf::RectangleShape cursor_rect {sf::Vector2f{5, exampleGlyph.bounds.size.y + 10.f}};

    sf::Clock delta_clock;
    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            } else if (const auto* key_pressed = event->getIf<sf::Event::KeyPressed>()) {
                if (key_pressed->scancode == sf::Keyboard::Scancode::Backspace) {
                    if (cursor.col == 0) {
                        if (cursor.row > 0) {
                            cursor.row--;
                            text_lines.pop_back();
                            cursor.col = static_cast<int>(text_lines.back().getSize()) - 1;
                        }
                    } else {
                        sf::String str = text_lines.at(cursor.row);
                        text_lines.at(cursor.row) = str.substring(0, str.getSize() - 1);
                        cursor.col--;
                    }
                } else if (key_pressed->scancode == sf::Keyboard::Scancode::Enter) {
                    text_lines.emplace_back("");
                    cursor.row++;
                    cursor.col = 0;
                } else if (key_pressed->scancode == sf::Keyboard::Scancode::Left) {

                } else if (key_pressed->scancode == sf::Keyboard::Scancode::Right) {

                } else if (key_pressed->scancode == sf::Keyboard::Scancode::Up) {

                } else if (key_pressed->scancode == sf::Keyboard::Scancode::Down) {

                }
            } else if (const auto* text_entered = event->getIf<sf::Event::TextEntered>()) {
                if (text_entered->unicode != 8) {
                    text_lines.at(cursor.row) += text_entered->unicode;
                    cursor.col++;
                    cursor_rect.setPosition(
                        {
                            cursor_rect.getPosition().x + exampleGlyph.bounds.size.x + exampleGlyph.advance,
                            cursor_rect.getPosition().y\
                        });
                }
            }
        }

        ImGui::SFML::Update(window, delta_clock.restart());

        ImGui::Text(("ROW: " + std::to_string(cursor.row)).c_str());
        ImGui::Text(("COLUMN: " + std::to_string(cursor.col)).c_str());

        window.clear();
        for (int i = 0; i < text_lines.size(); i++) {
            sf::Text text{font, text_lines.at(i), 30};
            text.setPosition({0, static_cast<float>(i) * line_height});
            window.draw(text);
        }
        window.draw(cursor_rect);
        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();
    return 0;
}

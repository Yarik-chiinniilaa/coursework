#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <sstream>
#include <cmath>
#include <vector>
#include <fstream>

using namespace std;

string input = "0", expression = "";
float num1 = 0, num2 = 0;
char operation = ' ';
bool newInput = true;
vector<string> history;
bool isEngineerMode = false;

void saveThemeToFile(bool isDark) {
    ofstream out("config.txt");
    if (out.is_open()) {
        out << (isDark ? "dark" : "light");
        out.close();
    }
}

bool loadThemeFromFile() {
    ifstream in("config.txt");
    string theme;
    if (in.is_open()) {
        getline(in, theme);
        in.close();
        return theme == "dark";
    }
    return true;
}

void applyTheme(sf::RectangleShape buttons[], sf::Text labels[], sf::Text& expressionText, sf::Text& display, bool isDark) {
    sf::Color textColor = isDark ? sf::Color::White : sf::Color::Black;
    for (int i = 0; i < 24; i++) {
        labels[i].setFillColor(textColor);
    }
    expressionText.setFillColor(textColor);
    display.setFillColor(textColor);
}

void calculate() {
    if (operation != ' ') {
        stringstream ss(input);
        ss >> num2;
        if (operation == '+') num1 += num2;
        else if (operation == '-') num1 -= num2;
        else if (operation == '*') num1 *= num2;
        else if (operation == '/') num1 = (num2 != 0) ? num1 / num2 : NAN;
        else if (operation == '^') num1 = pow(num1, num2);

        expression += input + " = ";
        input = to_string(num1);
        input = input.substr(0, input.find('.') + 3);
        history.push_back(expression + input);
        if (history.size() > 10) history.erase(history.begin());

        operation = ' ';
        newInput = true;
    }
}

void applyFunction(string func) {
    float value = stof(input);
    if (func == "x^2") value = pow(value, 2);
    else if (func == "1/x") value = (value != 0) ? 1 / value : NAN;
    else if (func == "%") value /= 100;
    else if (func == "/x") value = (value >= 0) ? sqrt(value) : NAN;
    else if (func == "sin") value = sin(value);
    else if (func == "cos") value = cos(value);
    else if (func == "tan") value = tan(value);
    else if (func == "log") value = log10(value);

    expression = func + "(" + input + ") =";
    input = to_string(value);
    input = input.substr(0, input.find('.') + 3);
    newInput = true;
}

void updateEngineerButtons(sf::Text labels[]) {
    if (isEngineerMode) {
        labels[0].setString("sin");
        labels[4].setString("cos");
        labels[5].setString("tan");
        labels[6].setString("log");
    }
    else {
        labels[0].setString("%");
        labels[4].setString("1/x");
        labels[5].setString("x^2");
        labels[6].setString("/x");
    }
}
float easeInOut(float start, float end, float t) {
    t = (t < 0.5f) ? 2 * t * t : -1 + (4 - 2 * t) * t;
    return start + (end - start) * t;
}
int main() {
    sf::RenderWindow window(sf::VideoMode(400, 750), "Calculator");

    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) {
        cerr << "Cannot load font!" << endl;
        return -1;
    }

    bool isDarkTheme = loadThemeFromFile();
    bool showMenu = false;

    sf::Text expressionText(expression, font, 30);
    expressionText.setPosition(20, 20);
    expressionText.setStyle(sf::Text::Bold);

    sf::Text display(input, font, 55);
    display.setPosition(20, 70);
    display.setStyle(sf::Text::Bold);

    sf::RectangleShape buttons[24];
    sf::Text labels[24];
    string btnTexts[] = { "%", "CE", "C", "<", "1/x", "x^2", "/x", "/", "7", "8", "9", "*",
                         "4", "5", "6", "-", "1", "2", "3", "+", "+/-", "0", ".", "=" };

    sf::Color baseColor = isDarkTheme ? sf::Color(50, 50, 50) : sf::Color(240, 240, 240);
    sf::Color highlightColor = isDarkTheme ? sf::Color(100, 100, 100) : sf::Color(200, 200, 200);
    vector<sf::Color> currentColors(24, baseColor);
    int pressedIndex = -1;
    int hoveredIndex = -1;

    for (int i = 0; i < 24; i++) {
        buttons[i].setSize(sf::Vector2f(90, 90));
        buttons[i].setPosition(10 + (i % 4) * 95, 150 + (i / 4) * 95);
        buttons[i].move(3, -8);
        buttons[i].setOutlineThickness(2);
        buttons[i].setOutlineColor(sf::Color::Black);

        labels[i].setFont(font);
        labels[i].setString(btnTexts[i]);
        labels[i].setCharacterSize(32);
        labels[i].setPosition(buttons[i].getPosition().x + 25, buttons[i].getPosition().y + 25);
    }

    applyTheme(buttons, labels, expressionText, display, isDarkTheme);

    sf::RectangleShape menuButton(sf::Vector2f(50, 50));
    menuButton.setPosition(350, 10);
    menuButton.setFillColor(sf::Color(0, 0, 0, 0));

    sf::Text menuButtonLabel("=", font, 30);
    menuButtonLabel.setPosition(355, 10);
    menuButtonLabel.setFillColor(sf::Color::White);

    sf::RectangleShape sideMenu(sf::Vector2f(200, 750));
    sideMenu.setFillColor(sf::Color(60, 60, 60));
    sideMenu.setPosition(-200, 0);
    float menuTargetX = -200;

    sf::Text menuItems[4];
    string menuTexts[] = { "History", "About", "Change theme", "Engineer" };
    for (int i = 0; i < 4; i++) {
        menuItems[i].setFont(font);
        menuItems[i].setString(menuTexts[i]);
        menuItems[i].setCharacterSize(24);
        menuItems[i].setFillColor(sf::Color::White);
        menuItems[i].setPosition(20, 50 + i * 50);
    }

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::MouseButtonPressed) {
                for (int i = 0; i < 24; i++) {
                    if (buttons[i].getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                        pressedIndex = i;
                        string btn = labels[i].getString();
                        if (btn == "C") {
                            input = "0"; expression = "";
                            num1 = num2 = 0;
                            operation = ' ';
                        }
                        else if (btn == "CE") input = "0";
                        else if (btn == "<") input = (input.size() > 1) ? input.substr(0, input.size() - 1) : "0";
                        else if (btn == "=") calculate();
                        else if (btn == "+" || btn == "-" || btn == "*" || btn == "/") {
                            num1 = stof(input);
                            operation = btn[0];
                            expression = input + " " + btn + " ";
                            input = "0";
                        }
                        else if (btn == "x^2" || btn == "1/x" || btn == "%" || btn == "/x" || btn == "sin" || btn == "cos" || btn == "tan" || btn == "log") {
                            applyFunction(btn);
                        }
                        else if (btn == "+/-") {
                            if (input != "0") {
                                float value = stof(input);
                                value = -value;
                                input = to_string(value);
                                input = input.substr(0, input.find('.') + 3);
                            }
                        }
                        else {
                            if (newInput) {
                                input = "";
                                newInput = false;
                            }
                            input = (input == "0") ? btn : input + btn;
                        }
                    }
                }

                if (showMenu) {
                    for (int i = 0; i < 4; i++) {
                        sf::FloatRect itemBounds(menuItems[i].getGlobalBounds());
                        itemBounds.left += sideMenu.getPosition().x + 10;
                        if (itemBounds.contains(event.mouseButton.x, event.mouseButton.y)) {
                            if (menuTexts[i] == "Change theme") {
                                isDarkTheme = !isDarkTheme;
                                baseColor = isDarkTheme ? sf::Color(50, 50, 50) : sf::Color(240, 240, 240);
                                highlightColor = isDarkTheme ? sf::Color(100, 100, 100) : sf::Color(200, 200, 200);
                                applyTheme(buttons, labels, expressionText, display, isDarkTheme);
                                saveThemeToFile(isDarkTheme);
                            }
                            else if (menuTexts[i] == "Engineer") {
                                isEngineerMode = !isEngineerMode;
                                updateEngineerButtons(labels);
                            }
                        }
                    }
                }

                if (event.mouseButton.x > 350 && event.mouseButton.y < 50) {
                    showMenu = !showMenu;
                    menuTargetX = showMenu ? 0 : -200;
                }
            }
        }

        hoveredIndex = -1;
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        for (int i = 0; i < 24; i++) {
            if (buttons[i].getGlobalBounds().contains((float)mousePos.x, (float)mousePos.y)) {
                hoveredIndex = i;
                break;
            }
        }

        float currentX = sideMenu.getPosition().x;
        sideMenu.setPosition(easeInOut(currentX, menuTargetX, 0.1f), 0);

        expressionText.setString(expression);
        display.setString(input);

        window.clear(isDarkTheme ? sf::Color(30, 30, 30) : sf::Color(220, 220, 220));
        window.draw(expressionText);
        window.draw(display);

        for (int i = 0; i < 24; i++) {
            sf::Color targetColor = baseColor;
            if (i == hoveredIndex && i != pressedIndex)
                targetColor = highlightColor;

            sf::Color current = currentColors[i];
            int r = current.r + (targetColor.r - current.r) * 0.1;
            int g = current.g + (targetColor.g - current.g) * 0.1;
            int b = current.b + (targetColor.b - current.b) * 0.1;
            currentColors[i] = sf::Color(r, g, b);
            buttons[i].setFillColor(currentColors[i]);

            if (i == hoveredIndex && i != pressedIndex)
                buttons[i].setScale(1.05f, 1.05f);
            else
                buttons[i].setScale(1.0f, 1.0f);

            window.draw(buttons[i]);
            window.draw(labels[i]);
        }

        if (sideMenu.getPosition().x > -190) {
            window.draw(sideMenu);
            for (int i = 0; i < 4; i++) {
                sf::Text temp = menuItems[i];
                temp.move(sideMenu.getPosition().x + 10, 0);
                window.draw(temp);
            }

            if (!history.empty()) {
                sf::Text historyTitle("Calculation History:", font, 20);
                historyTitle.setFillColor(sf::Color::White);
                historyTitle.setPosition(sideMenu.getPosition().x + 10, 270);
                window.draw(historyTitle);

                for (size_t i = 0; i < history.size(); i++) {
                    sf::Text entry(history[i], font, 16);
                    entry.setFillColor(sf::Color::White);
                    entry.setPosition(sideMenu.getPosition().x + 10, 300 + i * 25);
                    window.draw(entry);
                }
            }
        }

        window.draw(menuButton);
        window.draw(menuButtonLabel);

        window.display();
    }

    return 0;
}










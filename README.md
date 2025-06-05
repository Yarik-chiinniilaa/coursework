# 🧮 Графічний калькулятор на C++ з використанням SFML

## Опис проєкту

Цей репозиторій містить курсову роботу — **Графічний калькулятор** на мові C++ з використанням бібліотеки [SFML](https://www.sfml-dev.org/).  
Мета проєкту — створити зручний графічний калькулятор для виконання базових та додаткових математичних операцій у віконному режимі.

---

## Основні можливості 🚀

- ➕➖✖️➗ Арифметичні операції (додавання, віднімання, множення, ділення)
- √ Квадратний корінь, піднесення до степеня, відсотки
- ⌨️ Введення з клавіатури та інтерактивні кнопки
- 🖱️ Підтримка роботи миші
- 🧹 Очищення поля (кнопка "C")
- 🔢 Десяткові дроби
- 💬 Повідомлення про помилки (наприклад, ділення на нуль)
- 🧾 (Опційно) Історія обчислень

---

## Вимоги до запуску 🛠️

- Операційна система: **Windows**
- Компілятор C++ (GCC, MSVC)
- **SFML** версії 2.5 або новіша

---

## Інструкція з встановлення та запуску 📋

1. **Встановіть SFML**  
   [Офіційний гайд зі встановлення SFML](https://www.sfml-dev.org/tutorials/2.5/)

2. **Склонуйте цей репозиторій**  
   ```bash
   git clone https://github.com/Yarik-chiinniilaa/coursework.git
   cd coursework
   ```

3. **Скомпілюйте проєкт**  
   Для Windows (MinGW):
   ```bash
   g++ main.cpp -o calculator.exe -lsfml-graphics -lsfml-window -lsfml-system
   ```

4. **Запуск**  
   ```bash
   ./calculator.exe
   ```

---

## Особливості реалізації ℹ️

- Графічний інтерфейс створений за допомогою `sf::RenderWindow`, `sf::Text`, `sf::RectangleShape`.
- Вся логіка калькулятора винесена в окремий клас для зручності підтримки та розширення.
- Додані власні шрифти для кращої візуалізації.
- Можливості розширення: нові математичні функції, зміна стилю, підтримка темної теми, локалізація.

---

## Приклад інтерфейсу 🖼️

_Приклад вигляду графічного калькулятора:_  
![image1](image1)

---

## Вихідний код компонента (Calculator.h) 👇

```cpp
#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <string>
#include <stack>
#include <stdexcept>
#include <cmath>

class Calculator {
public:
    Calculator() : currentValue("0"), lastOperator(' '), isNewEntry(true) {}

    // Обробка введення символу (цифри, точки або операції)
    void input(char c);

    // Отримати поточне значення для відображення
    std::string getDisplay() const { return currentValue; }

    // Скинути калькулятор
    void clear() {
        currentValue = "0";
        lastOperator = ' ';
        isNewEntry = true;
        operands = std::stack<double>();
        operators = std::stack<char>();
    }

private:
    std::string currentValue;
    char lastOperator;
    bool isNewEntry;
    std::stack<double> operands;
    std::stack<char> operators;

    void calculate();
    int precedence(char op);
    double applyOp(double a, double b, char op);
};

#endif // CALCULATOR_H
```

> Повний вихідний код та інші компоненти доступні у публічному репозиторії:  
> [https://github.com/Yarik-chiinniilaa/coursework](https://github.com/Yarik-chiinniilaa/coursework)

---

## Чому саме SFML? 🤔

SFML — це сучасна кросплатформена бібліотека, яка спрощує розробку графічних програм на C++. Вона проста у використанні, підтримує роботу з графікою, подіями, аудіо, що робить її ідеальним вибором для студентських проєктів.

---

## Автор ✍️

Полиба Ярослав, студент групи 24

---

## Ліцензія 📄

Проєкт поширюється під ліцензією **MIT**.

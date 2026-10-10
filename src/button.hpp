#pragma once
#include <raylib.h>

class Button {
    public:
        Button(const char* imagePath, Vector2 imagePosition, float scale);
        ~Button();
        void Draw();
        bool IsPressed(Vector2 mousepos, bool mousePressed);
    private:
        Texture2D texture;
        Vector2 position;
};
#pragma once
#include "IScreen.h"
#include <vector>

namespace ngiv
{

class IScreenManager
{
public:
    IScreenManager() {}
    ~IScreenManager() {}

    void init(ngiv::Window* window, ColorRGBA8 backgroundColor)
    {
        _window = window;
        _backgroundColor = backgroundColor;
    }

    void addScreen(IScreen* screen)
    {
        _screens.push_back(screen);
    }
    void setActiveScreen(int index)
    {
        static bool first = true;
        if (first)
        {
            active_screen = _screens[index];
            active_screen->onEntry();
            return;
        }
        active_screen->onExit();
        active_screen = _screens[index];
        active_screen->onEntry();
        return;
    }
    void setActiveScreen(IScreen* screen)
    {
        static bool first = true;
        if (first)
        {
            active_screen = screen;
            active_screen->onEntry();
            return;
        }
        active_screen->onExit();
        active_screen = screen;
        active_screen->onEntry();
        return;
    }

    void putMouseMiddleofScreen()
    {
        if (_window->getState() == ngiv::Window_State::FocusOn)
        {
            SDL_WarpMouseInWindow(_window->getwindow(), _window->getWidth() / 2, _window->getHeight() / 2);
        }
    }



    bool do_screen_loop()
    {
        if (active_screen->switchscreen != nullptr)
        {
            IScreen* ns = active_screen->switchscreen;
            active_screen->switchscreen = nullptr;
            active_screen->onExit();
            active_screen = ns;
            active_screen->onEntry();
        }

        bool exit = active_screen->internalupdate();
        if (exit)
        {
            quitlogic();
            return true;
        }

        exit = active_screen->update(0);
        if (exit)
        {
            quitlogic();
            return true;
        }


        active_screen->draw();
        active_screen->render();

        _window->swapWindow();
        return false;
    }



private:
    void quitlogic()
    {
        for (int i = 0; i < _screens.size(); i++)
        {
            _screens[i]->disposeScreen();
        }
        _screens.clear();
    }


    ngiv::Window* _window;
    ColorRGBA8 _backgroundColor;
    std::vector<IScreen*> _screens;
    IScreen* active_screen = nullptr;

};


}

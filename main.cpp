//mishaaskarov@gmail.com
#include <iostream>
#include "HomeScreen.hpp"


int main()
{

    //init Game
    HomeScreen hs;
    //HomeScreen
    while(hs.running())
    {
        //Update
        hs.update();
        //Render
        hs.render();

    }

    return 0;
}
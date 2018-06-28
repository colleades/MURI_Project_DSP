#include <TouchScreenMenu.h>
#include <SeeedTouchScreen.h>
#include <TFTv2.h>
#include <SPI.h>
#include <stdint.h>

//create array for Main Menu
TouchScreenMenuItem mainItems [] =
{
  TouchScreenMenuItem("Effect 1"),
  TouchScreenMenuItem("Effect 2"),
  TouchScreenMenuItem("Effect 3"),
  TouchScreenMenuItem("ENDOFMENU")
};

//create array for Effect 1
TouchScreenMenuItem effectOneItems [] =
{
  //TouchScreenMenuItem("Button 1"),
  //TouchScreenMenuItem("ENDOFMENU")
};

//create array for Effect 2
TouchScreenMenuItem effectTwoItems [] =
{
 // TouchScreenMenuItem("Button 1"),
 // TouchScreenMenuItem("ENDOFMENU")
};

//create array for Effect 3
TouchScreenMenuItem effectThreeItems [] =
{
 // TouchScreenMenuItem("Button 1"),
  //TouchScreenMenuItem("ENDOFMENU")
};

//Menu settings - Item; Font Size; Space; Padding; Justification; Title
TouchScreenMenu mainMenu = TouchScreenMenu(mainItems, 2, 10, 5, CENTERJ, "Main Menu");
TouchScreenMenu effectOne = TouchScreenMenu(effectOneItems, 2, 10, 5, CENTERJ, "Effect 1");
TouchScreenMenu effectTwo = TouchScreenMenu(effectTwoItems, 2, 10, 5, CENTERJ, "Effect 2");
TouchScreenMenu effectThree = TouchScreenMenu(effectThreeItems, 2, 10, 5, CENTERJ, "Effect 3");

//keep track of which menu is active -> *curMenu
TouchScreenMenu *curMenu = &mainMenu;

//Button settings - Title; Font color; Button Background Color; Where on page; Height of screen
TouchScreenArea backEffectOne = TouchScreenButton("Back", TSC.createColor(0, 0, 0), TSC.createColor(255, 255, 255), 80, TSC.getScreenHeight() -50, 2, 10);
TouchScreenArea backEffectTwo = TouchScreenButton("Back", TSC.createColor(0, 0, 0), TSC.createColor(255, 255, 255), 80, TSC.getScreenHeight() -50, 2, 10);
TouchScreenArea backEffectThree = TouchScreenButton("Back", TSC.createColor(0, 0, 0), TSC.createColor(255, 255, 255), 80, TSC.getScreenHeight() -50, 2, 10);

//inside color; background color; shift on screen (higher = more to the right); screen height - up & down, length of slider, witdth
TouchScreenSlider slider1 = TouchScreenSlider("slider1",TSC.createColor(50, 50, 255), TSC.createColor(255, 255, 255), 10, TSC.getScreenHeight() - 125, 225, 40, HORIZONTAL);

void setup(void) 
{
   //Main Menu Settings
  TSC.setBackColor(TSC.createColor(0, 0, 0));
  TSC.init(); //everything gets initalized
  slider1.setValue(.5);
  slider1.setPadding(5);
  curMenu->draw(); //main menu 

}

void loop(void) 
{
 //handle current menu
 if(curMenu!=NULL)
  {
    //process
    TouchScreenMenuItem *item = curMenu->process(false);
    //check to see which is pressed
    checkMenuSelection(item);
  }
  else
  {
    //if current menu not displayed, check all buttons to see if pressed
    checkButtons();
  }
}
  
void checkButtons()
{
  if(backEffectOne.process()) //return from effect one
    {
      curMenu = &mainMenu;
      TSC.clearScreen();
      curMenu->draw();
    }
    else if(backEffectTwo.process()) //return from effect two
    {
      curMenu = &mainMenu;
      TSC.clearScreen();
      curMenu->draw();
    }
    else if(backEffectThree.process()) //return from effect three
    {
      curMenu = &mainMenu;
      TSC.clearScreen();
      curMenu->draw();
    }
    else if(slider1.process())
    {
      //leave blank
    }
    }


void checkMenuSelection(TouchScreenMenuItem *item)
{
  boolean handled = false; //initialized as nothing pressed
    if(item !=NULL)
      { //main menu items
       if(curMenu == &mainMenu)
        {
          slider1.draw();
          if(!strcmp(item->getText(),"Effect 1"))
            {
              curMenu = NULL;
              TSC.clearScreen();
              backEffectOne.draw();
              slider1.draw();
              handled = true;   
            }
           else if(!strcmp(item->getText(),"Effect 2"))
            {
              curMenu = NULL;
              TSC.clearScreen();
              backEffectTwo.draw();
              handled = true; 
            }
          else if(!strcmp(item->getText(),"Effect 3"))
            {
              curMenu = NULL;
              TSC.clearScreen();
              backEffectThree.draw();
              handled = true; 
            }
        }
        if(handled==false)
          curMenu->drawItem(item,false);
        
      }
}


//                                                                                                                                                                                                                                                                                                            
//                                                                                                                                                                                                                                                                                                            
//        SDL FRAMEWORK FOR GRAPHICS OPERATIONS                                                                                                                                                                                                                                                                                       
//                                                                                                                                                                                                                                                                                                       
//                                                                                                                                                                                                                                                                                                       
//                                                                                                                                                                                                                                                                                                       
//                                                                                                                                                                                                                                                                                                       
// TARGET: WINDOW *SCREEN pointer. This is where all functions read from and draw to.                                                                                                                                                                                                                                                                                                 
//=============================================================================================================                                                                                                                                                                                                                                                                                                              


#include"cell.h"
#include<iostream>
#include "brain.h"



int KEYBOARD_HANDLER(SDL_Keycode sym){
    return 0;
}


#define NUBER_OF_NEURONS    10

void Draw_Edges(int Xx,int Yy, Cell Parent)
{
         for(Edge &Child: Parent.edges){
         
            float 
            X = Parent.Offset.X + Xx,
            Y = Parent.Offset.Y + Yy;

            SET_DRAW_COLOR(Child.Color);                 // SETS DRAW COLOR TO THAT OF THE CELL
            LINE2(X,Y, Child.Angle, Child.Distance *.5); // DRAWS EACH EDGE
            FILLED_CIRCLE(X,Y,5);                        // DRAWS EACH NODE
     }
}




void main()
{
    srand(1500);
    WINDOW main(0,0,1280,960,"Multicelled automatons");
    SET_ACTIVE_WINDOW(&main);

    Organism Creature(30);


    Cell *Selected = nullptr;

    while(LOOP_GAME()){

       _CLS;
     Print(SCREEN->FPS) ;
   //    Print("Geneva anne Long");
//----------------------------- DRAW EACH CREATURE ------------------------------------------------------------
 
 for(Cell &Parent:Creature.cells){

    // Parent.Force.X = Parent.Force.Y = 0;

     double 
     Old_X = Parent.Offset.X,
     Old_Y = Parent.Offset.Y;
    
     Draw_Edges(Creature.Position.X, Creature.Position.Y , Parent);
 }

//------------------------- CHECK TO SEE IF NODE IS CLICKED ---------------------------------------------------

        if(Selected == nullptr && SCREEN->MOUSE_BUTTON.LEFT == true){
            for(Cell &cell: Creature.cells){

                if(Is_CLICK(cell.Offset.X + Creature.Position.X, cell.Offset.Y + Creature.Position.Y))Selected = &cell;

            }
        }
        

        if (Selected != nullptr){ 
       
           /// Selected->Offset.X = SCREEN->MOUSE_POSITION.x - Creature.Position.X;
            //Selected->Offset.Y = SCREEN->MOUSE_POSITION.y - Creature.Position.Y;
             
            float MASS = 1;

            Selected->Force.X = (MASS * SCREEN->MOUSE_VELOCITY.x) ; // .03, TIME STEP? 
            Selected->Force.Y = (MASS * SCREEN->MOUSE_VELOCITY.y);//(rand()%3-1) * 1;

            SET_DRAW_COLOR(RGB(255,255,255));
            FILLED_CIRCLE(Selected->Offset.X + Creature.Position.X,Selected->Offset.Y + Creature.Position.Y,7);
       }
        if(SCREEN->MOUSE_BUTTON.LEFT == false){Selected = nullptr;}

//-------------------------------------------------------------------------------------------------------------
        float Length = abs(SCREEN->MOUSE_VELOCITY.x) + abs(SCREEN->MOUSE_VELOCITY.y);

           if(Length > 400) Length  = 400;                                              // DRAW MOUSE INFORMATION
        LINE2(200,200,SCREEN->MOUSE_ANGLE, Length);
//-------------------------------------------------------------------------------------------------------------
#if 0
        for(Cell &c:Creature.cells){
           c.Force.X += 50*((RANDOM(2) - 1)); // c.Brain.Layers[2].Neurons[0].Value;//
           c.Force.Y += 50*((RANDOM(2) - 1)); //c.Brain.Layers[2].Neurons[0].Value;;//
          // c.Angle = RANDOM(360);
           //Print(c.Brain.Layers[1].Neurons[1].Value);
       }
#endif     
#if 1
         //  Creature.cells[1].Force.X += 50*((RANDOM(2) - 1)); // c.Brain.Layers[2].Neurons[0].Value;//
        //  Creature.cells[1].Force.Y += 50*((RANDOM(2) - 1)); //c.Brain.Layers[2].Neurons[0].Value;;//
          Creature.cells[1].Angle += RANDOM(20) - 10;
#endif
        Creature.Update();
       _SYNC;
    }

}

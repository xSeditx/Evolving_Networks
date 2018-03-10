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
#include"world.h" 
#include <string>     // std::string, std::stoi
World WORLD(960, 1280);
Neuron *GLOBAL_NEURON = nullptr;
Neuron NARRAY[100];

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
            WORLD.SetSpace(X,Y,Child.Color);
     }
}




void main()
{
    srand(1500);
    WINDOW main(0,0,1280,960,"Multicelled automatons");
    SET_ACTIVE_WINDOW(&main);


    Organism *Creature;
    //vector<Organism> *CRE = nullptr;


    Organism *C[20];
    int number_of_creatures = 20;
    
    FOR_LOOP(count,number_of_creatures){
        C[count] = new Organism(10); //1 + rand()%90);
        C[count]->cells[1].Brain.Layers[0].Neurons[1].Value = RANDOM(1); // <- to Trigger Motion in Dormant cells;
    }

    FOR_LOOP(count,20){
        C[count] = Make_Copy(*C[19]); //1 + rand()%90);
       // C[count]->Position.X = rand()%500;
       // C[count]->Position.Y = rand()%500;
       // C[count]->Distance_moved = 0;
       C[count]->cells[1].Brain.Layers[0].Neurons[1].Value = RANDOM(1); // <- to Trigger Motion in Dormant cells;
    }


//
//   FOR_LOOP(count,10){
//     Organism *temp =  new Organism(10);  // (C[13]);  
//     temp->Position.X = rand()%500;
//     temp->Position.Y = rand()%500;
//      *C[count] = *temp;
//   }
//

    Cell *Selected = nullptr;
    _CLS;

    while(LOOP_GAME()){

      _CLS;
       Print(SCREEN->FPS);

       FOR_LOOP(count,number_of_creatures){
                     
                 Creature = C[count];
//----------------------------- DRAW EACH CREATURE ------------------------------------------------------------
                 
                 for(Cell &Parent:Creature->cells){
                     
                         double 
                         Old_X = Parent.Offset.X,
                         Old_Y = Parent.Offset.Y;
                        
                         Draw_Edges(Creature->Position.X, Creature->Position.Y , Parent);
                 }
                
//------------------------- CHECK TO SEE IF NODE IS CLICKED ---------------------------------------------------
                
                        if(Selected == nullptr && SCREEN->MOUSE_BUTTON.LEFT == true){
                            for(Cell &cell: Creature->cells){
                
                                if(Is_CLICK(cell.Offset.X + Creature->Position.X, cell.Offset.Y + Creature->Position.Y))Selected = &cell;
                
                            }
                         
                        }
                        
                
                        if (Selected != nullptr){ 
                                
                                float MASS = 5;
                               
                                Selected->Force.X = (MASS * SCREEN->MOUSE_VELOCITY.x); // .03, TIME STEP? 
                                Selected->Force.Y = (MASS * SCREEN->MOUSE_VELOCITY.y); //(rand()%3-1) * 1;
                               
                                SET_DRAW_COLOR(RGB(255,255,255));
                                FILLED_CIRCLE(Selected->Offset.X + Creature->Position.X,
                                              Selected->Offset.Y + Creature->Position.Y,7);
                       }
                        if(SCREEN->MOUSE_BUTTON.LEFT == false){Selected = nullptr;}
                
//-------------------------------------------------------------------------------------------------------------
                       Creature->Update();
                      
        }

    _SYNC;
    }

// CLEAN UP
    delete (*C);
}




#if 0
                         //Creature.cells[1].Force.X += 50*((RANDOM(2) - 1)); // c.Brain.Layers[2].Neurons[0].Value;//
                         //Creature.cells[1].Force.Y += 50*((RANDOM(2) - 1)); //c.Brain.Layers[2].Neurons[0].Value;;//
                         //Creature.cells[2].Brain.Layers[0].Neurons[0].Value = RANDOM(1);// - 40;
#endif
                
#if 0   // For drawing the screen buffer directly to the screen for debugging uses.
       FOR_LOOP(x,WORLD.Size.Width){
                  FOR_LOOP(y,WORLD.Size.Height){
                        if(WORLD.CheckSpace( x, y)!=0) SET_PIXEL(x, y,WORLD.CheckSpace( x, y));
                  }
       } 
#endif
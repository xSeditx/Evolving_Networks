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

const   int number_of_creatures = 55;


using namespace std;


int KEYBOARD_HANDLER(SDL_Keycode sym)
{
    return 0;
}



void main()
{

    srand(1000);
    WINDOW main(0, 0, 1280, 960, "Multicelled automatons");
    SET_ACTIVE_WINDOW(&main);

    Organism *Creature;
    Organism *C[number_of_creatures];

    FOR_LOOP(count, number_of_creatures)
    {
        C[count] = new Organism(10, 100 + (rand() % SCREENWIDTH - 100), 100 + (rand() % SCREENHEIGHT - 100)); //1 + rand()%90);
    }


    int x     = 0,
        y     = 10,
        xjump = 100,
        yjump = 100;

    FOR_LOOP(count, number_of_creatures)
    {
        x+=xjump;
        if (x > SCREENWIDTH - 100)
        {
            x=xjump; y+=yjump;
        }
        C[count]->Set_Position(x, y);//rand()%SCREENWIDTH;


        for (Cell &p : C[count]->cells)
        {
            p.Brain.Layers[0].Neurons[0].Value = RANDOM(1) - .5;
            p.Brain.Layers[0].Neurons[1].Value = RANDOM(1) - .5;
            p.Brain.Layers[0].Neurons[2].Value = RANDOM(1) - .5;

        }


        // C[count]->cells[2].Brain.Layers[0].Neurons[1].Value = .5;//RANDOM(1); // <- to Trigger Motion in Dormant cells;
    }


    Cell *Selected = nullptr;
    _CLS;
    int Epoch = 1;
    int Generation = 0;

    float AverageAvg = 0;
    Vector2D TEST;
    int FrameSkip = 2;
    while (LOOP_GAME())
    {



        if (Epoch % 300 == 0)
        {
            Organism   Parent;
            int   Best    = 0,
                Worst   = 0;
            float   Average = 0;

            FOR_LOOP(count, number_of_creatures - 1)
            {
                Average+=C[count]->Distance_moved;
                if (C[count]->Distance_moved < C[Worst]->Distance_moved) Worst = count;
                if (C[count]->Distance_moved > C[Best]->Distance_moved)  Best = count;

            }

            Average+=C[number_of_creatures - 1]->Distance_moved / number_of_creatures;
            AverageAvg +=Average;
            Print("  ");
            cout << " FPS: " << SCREEN->FPS << "  CyclesPerSecond:" << SCREEN->CyclePerSecond << " Generation:" << Generation << endl;
            cout << " Best:" << C[Best]->Distance_moved << "  " << " Worst: " << C[Worst]->Distance_moved << endl;
            cout << " Average:" << Average / number_of_creatures << "  Overall Avg :" << AverageAvg / Epoch << " " << endl;

            Generation++;

            Parent.Copy(C[Best]);

            FOR_LOOP(count, number_of_creatures)
            {

                C[count]->Mutate(Parent);

                for (Cell &p : C[count]->cells)
                {
                    p.Brain.Layers[0].Neurons[0].Value =  RANDOM(1) - .5;
                    p.Brain.Layers[0].Neurons[1].Value =  RANDOM(1) - .5;
                    p.Brain.Layers[0].Neurons[2].Value =  RANDOM(1) - .5;

                }
            }
        }
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////


        if ((Epoch++) % FrameSkip == 0)
        {
            _CLS;
        }

        FOR_LOOP(count, number_of_creatures)
        {

            Creature = C[count];

            if (Selected == nullptr && SCREEN->MOUSE_BUTTON.LEFT == true)
            {
                for (Cell &cell : Creature->cells)
                {

                    if (Is_CLICK(cell.Offset.X + Creature->Potential.X, cell.Offset.Y + Creature->Potential.Y))Selected = &cell;

                }

            }


            if (Selected != nullptr)
            {

                float MASS = 5;

                Selected->Force.X = (MASS * SCREEN->MOUSE_VELOCITY.x); // .03, TIME STEP? 
                Selected->Force.Y = (MASS * SCREEN->MOUSE_VELOCITY.y); //(rand()%3-1) * 1;

                SET_DRAW_COLOR(RGB(255, 255, 255));
                FILLED_CIRCLE(Selected->Offset.X + Selected->Parent->Potential.X,
                    Selected->Offset.Y + Selected->Parent->Potential.Y, 7);
            }
            if (SCREEN->MOUSE_BUTTON.LEFT == false)
            {
                Selected = nullptr;
            }



            Creature->Update(10);
            // Creature->Collision(&C[0]);

            if (Epoch%FrameSkip == 0)
            {
                Creature->Draw();
            }

        }

        int yy = 0;
        FOR_LOOP(count, number_of_creatures)
        {
            yy +=5;
            LINE2(5, yy, 0, C[count]->Distance_moved);

        }

        if (Epoch%FrameSkip == 0)
        {
            _SYNC;
        }

    }

    // CLEAN UP
    delete (*C);
}




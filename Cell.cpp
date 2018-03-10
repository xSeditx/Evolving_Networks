//                                                                                                                                                                                                                                                                                                                                       
//                                                                                                                                                                                                                                                                                   
//   Cell class for building and manipulating organisms                                                                                                                                                                                                                                                                    
//                                                                                                                                                                                                                                                                                   
//     _____      ______       __           __                                                                                                                                                                                                                                                
//    /  _  \    |  ____}     |  |         |  |                                                                                                                                                                                                                                              
//   | |  \ _|   | |___       |  |         |  |                                                                                                                                                                                                                                         
//   | |         |  ___}      |  |         |  |                                                                                                                                                                                                                                            
//   | |    _    | |          |  |         |  |
//   | | _/  |   | |____      |  |______   |  |______                                                                                                                                                                                                                                            
//    \_____/    |______}     |_________|  |_________|                                                                                                                                                                                                                                                    
//                                                                                                                                                                                                                                                                                   
//                                                                                                                                                                                                                                                                                   
// TODO: Impliment better friction so that cells lose Velocity proportional to their mass and friction Coefficient                                                                                                                                                                                                                                                                             
//=============================================================================================================================================================                                                                                                                                                                                                                                                                                  

#include"cell.h"
#include"window.h"
#include"Vertex2D.h"
#include"world.h"

using namespace std;

    Cell::~Cell(){}
    Edge::~Edge(){}
Organism::~Organism(){}


    Cell::Cell()      : Number_of_edges(0),Angle(0),Speed(0){}
    Edge::Edge()     {}
Organism::Organism() {}


void Cell::See(){
    float Xx = 0,
          Yy = 0;

    float X = Offset.X + Parent->Position.X,
          Y = Offset.Y + Parent->Position.Y;
      int Ang = Angle;
       //   for(int Ang = (Angle - 30);Ang < (Angle + 30); Ang +=10){
                  for(float dist = 0;dist < 8; dist++){
                          Xx = X + dist * cos(RADIANS(Ang));
                          Yy = Y + dist * sin(RADIANS(Ang));
                          SET_PIXEL(Xx,Yy,RGB(255,255,255));
                  }
         // }
}




//=============================================================================================================================================================
//                                                          CONSTRUCTORS FUNCTIONS                                                                                     
//=============================================================================================================================================================
Cell::Cell(Organism *parent){

  //  parent->Number_of_Cells++;
    ID =  parent->Number_of_Cells++;
    Offset.X = Potential.X  = RANDOM(300);
    Offset.Y = Potential.Y  = RANDOM(300);

    Velocity  = 0;
    Force     = 0;

    Speed = 0;
    Angle = RANDOM(0);
    Friction  = RANDOM(1);

    Color = RGB(RANDOM(255),
                    RANDOM(255),
                    RANDOM(255));

    if (Color <= RGB(55,55,55))Color = RGB(255,255,255);
    Mass = 10;
    Parent = parent;

}
Edge::Edge(Cell *parent, Cell &other, unsigned char tension){
        parent->Number_of_edges++;
        Parent_ID = parent->ID;
        Child_ID = other.ID;
        RestDistance = Get_Displacement( other.Offset, parent->Offset);
        second   = &other;
        Distance =  Get_Distance(*parent, other); 
        Tension  =  tension;
        Angle    =  0;
        Color    =  parent->Color; 
        Displacement = 0;
}
Organism::Organism(unsigned char numcells)
        :Number_of_Cells(0) ,  Distance_moved(0.0)
{
    Position.X = Potential.X =  (rand()%SCREENWIDTH  / 2) + 100;  
    Position.Y = Potential.Y =  (rand()%SCREENHEIGHT / 2) + 100 ;
    Velocity.X = Velocity.Y = 0;

    Starting.X = Position.X;
    Starting.Y = Position.Y;
  

    FOR_LOOP(count, numcells){
        cells.push_back(Cell(this));
    }

    float     X =  0, Y =  0, 
          angle =  0, Theta =  360 / Number_of_Cells,
          dist =  25;

    for(Cell &c : cells){
        angle+=Theta;
        X += dist * cos(RADIANS(angle));    Y += dist * sin(RADIANS(angle));
        c.Offset.X =  X;                    c.Offset.Y =  Y;                        // rand()%(int)dist;//   // rand()%(int)dist;//
    c.Number_of_edges = 0;
    }

   FOR_LOOP(cellcount, numcells){   // FOR EACH CELL.....
       FOR_LOOP(edgecount, rand()%numcells){  // MAKE EDGES CONNECTING THE OTHER CELLS
             if(edgecount != cellcount){
                  int cnum  = rand()%numcells;
                  int cnum2 = rand()%numcells;

                      cells[cnum ].edges.push_back(Edge(&cells[cnum ], cells[cnum2], RANDOM(1)));  
                      cells[cnum2].edges.push_back(Edge(&cells[cnum2], cells[cnum ], RANDOM(1)));  
                      cells[cnum].Number_of_edges++;
                      cells[cnum2].Number_of_edges++;
             }
       }
   }

   for(Cell &C : cells){
      int  I = 3, 
           H = 5, 
           O = 2; 
           Net *temp = new  Net(I,H,O); 
           C.Brain   = *temp; 
           C.Number_of_edges /= 2;
   }
}



//=============================================================================================================================================================
//                                                      CLASS METHODS                                                                                          
//=============================================================================================================================================================

void Organism::Update()
{
    
      float 
      DELTA_TIME     = 0, 
      DELTA_VELOCITY = 0;
    
      DELTA_TIME = SDL_GetTicks()- SCREEN->TIME;  SCREEN->TIME = SDL_GetTicks();


      float Xmove = 0, Ymove = 0;

        for(Cell &Parent:cells){ // Cycle Every Cell
                WORLD.SetSpace(Position.X + Parent.Offset.X,Position.Y + Parent.Offset.Y,0);                                                                   
                Parent.Acceleration = (Parent.Force) / Parent.Mass;  
                Parent.Velocity += Parent.Acceleration; // Change in Velocity equals Acceleration    
                Parent.Offset   += Parent.Velocity;     // Change in Position over time equals Velocity   
                Parent.Force.X =0;
                Parent.Force.Y =0;
               WORLD.SetSpace(Position.X + Parent.Offset.X,Position.Y + Parent.Offset.Y,Parent.Color);  
               Xmove += Position.X + Parent.Offset.X;
               Ymove += Position.Y + Parent.Offset.Y;
        }
        Xmove /= Number_of_Cells;
        Ymove /= Number_of_Cells;
        FILLED_CIRCLE(Xmove,Ymove, 7);
       Distance_moved =  sqrt(Squared(Xmove - Starting.X)  + Squared(Ymove - Starting.Y)); 
      // Print(Distance_moved);
       LINE2((10),(Starting.Y / 10),0, Distance_moved);

       //Parent.Brain.Layers[0].Neurons[0].Value = RANDOM(1);
       // Parent.Brain.Layers[0].Neurons[1].Value = RANDOM(1);
//-------------------------------------------------------------------------------------------------------------------------------------------------------------

        for(Cell &Parent:cells){ // Cycle Every Cell
            Parent.Velocity *= .5; // APPLY A CRUDE "FRICTION" SO THAT VELOCITY IS LOST OVER TIME
           
            Parent.See();
            Parent.Brain.Think();

            for(Edge &Child: Parent.edges){ // For Every Cell Get and Set Information about all Connecting Cells.
          
                  Child.Angle = GetAngle(Parent.Offset,  Child.second->Offset);
                  
                  Child.Displacement =  Get_Displacement(Child.second->Offset,Parent.Offset ) - Child.RestDistance; 
                  Child.Distance     =  Get_Distance(Parent, *Child.second);
                  float K = .1;
                  Child.second->Force.X += -K * (Child.Displacement.X);   
                  Child.second->Force.Y += -K * (Child.Displacement.Y);

                // Child.second->Brain.Layers[0].Neurons[2].Value = (Child.second->Angle) ; //Parent.Brain.Layers[2].Neurons[1].Value;
                 Child.second->Brain.Layers[0].Neurons[1].Value = Child.second->Force.X; //Parent.Brain.Layers[2].Neurons[1].Value;
                 Child.second->Brain.Layers[0].Neurons[0].Value = Child.second->Force.Y; //.Brain.Layers[2].Neurons[0].Value;
            }
        

        if (Parent.Angle <    0) Parent.Angle   += 360;
        if (Parent.Angle >  360) Parent.Angle   -=   0;

      }


for(Cell &C: cells){
C.Speed =   C.Brain.Layers[2].Neurons[0].Value * 30;
C.Angle += (C.Brain.Layers[2].Neurons[1].Value * 5); // rand()%180;//
C.Force.X += C.Speed * cos(RADIANS(C.Angle));
C.Force.Y += C.Speed * sin(RADIANS(C.Angle));

}
//Print(cells[1].Brain.Layers[2].Neurons[0].Value);
//Print(cells[1].Brain.Layers[2].Neurons[1].Value);
//_____________________________________________________________________________________________________________________________________________________________
//=============================================================================================================================================================

}
void Organism::Draw(){             /// WAIT WHY IS THIS NOT BEING CALLED AT ALL... DID I SHUT IT OFF????

   for(Cell &Parent:cells){
           for(Edge &Child: Parent.edges){

            float x1 =  Parent.Offset.X + Position.X,
                  y1 =  Parent.Offset.Y + Position.Y,
                  x2 =  Child.second->Offset.X + Position.X,
                  y2 =  Child.second->Offset.Y + Position.Y;
            SET_DRAW_COLOR(Parent.Color);
            LINE(x1,y1,x2,y2);
           }
    }

    for(Cell &cell: cells)
    {
        float x = Position.X+ cell.Offset.X,
              y = Position.Y+ cell.Offset.Y;
      
              SET_PIXEL(x,y,RGB(255,255,255));
    }
}


float Get_Distance(const Cell &parent,const Cell &child){
    float ret =  sqrt(Squared(parent.Offset.X - child.Offset.X)  + Squared(parent.Offset.Y - child.Offset.Y));
    if(ret < 0) ret = 0; // HMMMMMMM
    return ret;
}




Organism *Make_Copy(Organism Parent)
{
    int numcells =  Parent.Number_of_Cells;

    Organism *ret = new Organism(numcells);
    
    
    ret->Number_of_Cells =(0);
    ret->Distance_moved =(0.0);

    ret->Position.X = ret->Potential.X =  (rand()%SCREENWIDTH  / 2) + 100; // Parent.Position.X; //  
    ret->Position.Y = ret->Potential.Y = (rand()%SCREENHEIGHT / 2) + 100 ; //  Parent.Position.Y; //
    ret->Velocity.X = ret->Velocity.Y = 0;

    ret->Starting.X = ret->Position.X;
    ret->Starting.Y = ret->Position.Y;



    FOR_LOOP(Cellcount, Parent.Number_of_Cells){
        Cell *temp = new Cell(ret);
        temp->Color = Parent.cells[Cellcount].Color;
        temp->Offset = Parent.cells[Cellcount].Offset;
        ret->cells[Cellcount] = *temp;
    }

   FOR_LOOP(cellcount, Parent.Number_of_Cells){ // FOR EACH CELL....
      FOR_LOOP(edgecount, Parent.cells[cellcount].Number_of_edges){  // MAKE EDGES CONNECTING THE OTHER CELLS
             if(edgecount != cellcount){
                  int cnum  = Parent.cells[cellcount].edges[edgecount].Parent_ID;//rand()%numcells;
                  int cnum2 = Parent.cells[cellcount].edges[edgecount].Child_ID;//rand()%numcells;

                      ret->cells[cnum ].edges.push_back(Edge(&ret->cells[cnum ], ret->cells[cnum2], RANDOM(1)));  
                      ret->cells[cnum2].edges.push_back(Edge(&ret->cells[cnum2], ret->cells[cnum ], RANDOM(1)));  
             }
       }
   }
      
   for(Cell &C : ret->cells){
      int  I = 3, 
           H = 5, 
           O = 2; 
           Net *temp = new  Net(I,H,O); 
           C.Brain   = *temp; 
   }


    FOR_LOOP(cellcount, Parent.Number_of_Cells){ // FOR EACH CELL....
        ret->cells[cellcount].Brain.Layers[0] = Parent.cells[cellcount].Brain.Layers[0];
    }
    FOR_LOOP(cellcount, Parent.Number_of_Cells){ // FOR EACH CELL....
        ret->cells[cellcount].Brain.Layers[1] = Parent.cells[cellcount].Brain.Layers[1];
    }
    FOR_LOOP(cellcount, Parent.Number_of_Cells){ // FOR EACH CELL....
        ret->cells[cellcount].Brain.Layers[2] = Parent.cells[cellcount].Brain.Layers[2];
    }
    return ret;
}

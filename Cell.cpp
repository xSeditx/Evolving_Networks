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
using namespace std;


int EDGECOUNT= 0,CELLCOUNT=0;
int EDGECOPY = 0, CELLCOPY =0;

Cell::Cell():Number_of_edges(0),Angle(0){
}

Cell::~Cell(){}
Edge::Edge(){ID = rand()%100 + 1000;}
Edge::~Edge(){}


//=============================================================================================================================================================
//                                                          CONSTRUCTORS FUNCTIONS                                                                                     
//=============================================================================================================================================================

Organism::Organism(unsigned char numcells):Number_of_Cells(0)
{
    Position.X = Potential.X = SCREENWIDTH  / 2;   Position.Y = Potential.Y = SCREENHEIGHT / 2;
    Velocity.X = Velocity.Y = 0;
    Number_of_Cells=0;

    FOR_LOOP(count, numcells){
        cells.push_back( MakeCell(this));
    }

    float     X = -100,
              Y = -200, 
          angle =  0,
           dist =  200,
          Theta =  360 / Number_of_Cells;

    for(Cell &c : cells){
       angle+=Theta;
        X+= dist * cos(RADIANS(angle));
        Y+= dist * sin(RADIANS(angle));
        c.Offset.X =rand()%(int)dist;// X; // 
        c.Offset.Y =rand()%(int)dist;//Y; //
    }

   FOR_LOOP(cellcount, numcells){   // FOR EACH CELL.....
       int e = 0;
       FOR_LOOP(edgecount, rand()%numcells){  // MAKE EDGES CONNECTING THE OTHER CELLS
             if(edgecount != cellcount){
                  int cnum  = rand()%numcells;
                  int cnum2 = rand()%numcells;

                      cells[cnum ].edges.push_back(MakeEdge(&cells[cnum ], cells[cnum2], RANDOM(1)));  
                      cells[cnum2].edges.push_back(MakeEdge(&cells[cnum2], cells[cnum ], RANDOM(1)));  
                    
                      e++;
             }
       }
   }


   for(Cell &C : cells){
           int  I = 3,//C.Number_of_edges * 2,
                H = 5,//rand()%(I * 2),
                O = 2;//2;

                C.Brain =  Net(I,H,O);

   }

FOR_LOOP(CellCount, Number_of_Cells){
   FOR_LOOP(Hcount, cells[CellCount].Brain.Layers[1].Number_of_Neurons){
       FOR_LOOP(Icount,  cells[CellCount].Brain.Layers[0].Number_of_Neurons){
             cells[CellCount].Brain.Layers[1].Neurons[Hcount].Synapses.push_back(
                MakeSynapse(RANDOM(1), &cells[CellCount].Brain.Layers[1].Neurons[Hcount],
                                       &cells[CellCount].Brain.Layers[0].Neurons[Icount]));    
       }
   }
   
   FOR_LOOP(Ocount, cells[CellCount].Brain.Layers[2].Number_of_Neurons){
       FOR_LOOP(Hcount, cells[CellCount].Brain.Layers[1].Number_of_Neurons){
            cells[CellCount].Brain.Layers[2].Neurons[Ocount].Synapses.push_back(
                MakeSynapse(RANDOM(1), &cells[CellCount].Brain.Layers[2].Neurons[Ocount],
                                       &cells[CellCount].Brain.Layers[1].Neurons[Hcount]));    
       }
   }
}





}

//=============================================================================================================================================================
//                                                        CREATION FUNCTIONS                                                                                     
//=============================================================================================================================================================


//_______________________________________________________________
//||  int ID;
//||  Vector2D Offset, Potential, Velocity, Acceleration, Force;
//||  double   Friction;
//||  unsigned char Number_of_edges;
//||  std::vector<Edge> edges;

Cell MakeCell(Organism *parent){
    Cell ret; 
    parent->Number_of_Cells++;

    ret.Offset.X = ret.Potential.X  = RANDOM(300);
    ret.Offset.Y = ret.Potential.Y  = RANDOM(300);

    ret.Velocity  = 0;
    ret.Force     = 0;

    ret.Angle = RANDOM(0);
    ret.Friction  = RANDOM(1);

    ret.Color = RGB(RANDOM(255),
                    RANDOM(255),
                    RANDOM(255));

    if (ret.Color <= RGB(55,55,55))ret.Color = RGB(255,255,255);
    ret.Mass = 10;
    ret.Parent = parent;
    return ret;
}

//_______________________________________________________________
//|    Edge(Cell *parent, Cell *other, double Tension);
//|    Cell *Parent, *second;
//|    float Distance , Tension;

Edge MakeEdge(Cell *parent, Cell &other, unsigned char tension){
    parent->Number_of_edges++;

    Edge ret;
        ret.RestDistance = Get_Displacement( other.Offset, parent->Offset);
        ret.second   = &other;
        ret.Distance =  Get_Distance(*parent, other); 
        ret.Tension  =  tension;
        ret.Angle    =  0;
        ret.Color    =  parent->Color; 
        ret.Displacement = 0;
    return ret;

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

        for(Cell &Parent:cells){ // Cycle Every Cell
                                                                                  
                Parent.Acceleration = (Parent.Force) / Parent.Mass;  
                Parent.Velocity += Parent.Acceleration; // Change in Velocity equals Acceleration    
                Parent.Offset   += Parent.Velocity;     // Change in Position over time equals Velocity   
                Parent.Force.X =0;
                Parent.Force.Y =0;
        }

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
                 Child.second->Brain.Layers[0].Neurons[1].Value = Child.second->Force.X / 29; //Parent.Brain.Layers[2].Neurons[1].Value;
                 Child.second->Brain.Layers[0].Neurons[0].Value = Child.second->Force.Y / 29; //.Brain.Layers[2].Neurons[0].Value;
            }
        
        if (Parent.Angle <    0) Parent.Angle   += 360;
        if (Parent.Angle >  360) Parent.Angle   -= 0;

      }


for(Cell &C: cells){
C.Speed =  C.Brain.Layers[2].Neurons[0].Value * 30;
C.Angle = (C.Brain.Layers[2].Neurons[1].Value * 180); // rand()%180;//
C.Force.X += C.Speed * cos(RADIANS(C.Angle));
C.Force.Y += C.Speed * sin(RADIANS(C.Angle));
}
Print(cells[1].Brain.Layers[2].Neurons[0].Value);
Print(cells[1].Brain.Layers[2].Neurons[1].Value);
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

void Cell::See(){
    float Xx = 0,
          Yy = 0;

    float X = Offset.X + Parent->Position.X,
          Y = Offset.Y + Parent->Position.Y;
    
     for(float dist = 0;dist < 30; dist++){
             Xx = X + dist * cos(RADIANS(Angle));
             Yy = Y + dist * sin(RADIANS(Angle));
             SET_PIXEL(Xx,Yy,RGB(255,255,255));
     }

  //   Brain.Layers[0].Neurons[0].Value = RANDOM(2)-1;
  //   Brain.Layers[0].Neurons[1].Value = RANDOM(2)-1;
}



float Get_Distance(const Cell &parent,const Cell &child){
    float ret =  sqrt(Squared(parent.Offset.X - child.Offset.X)  + Squared(parent.Offset.Y - child.Offset.Y));
    if(ret < 0) ret = 0; // HMMMMMMM
    return ret;
}
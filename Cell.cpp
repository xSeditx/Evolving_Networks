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

Cell::Cell(){
    ID = rand()%100 + 1000;
    Angle = 180; // RANDOM(360);
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

    float X = -100,
          Y = -200, 
          angle=0,
          dist = 200,
          Theta = 360 / Number_of_Cells;

    for(Cell &c : cells){
       angle+=Theta;
        X+= dist * cos(RADIANS(angle));
        Y+= dist * sin(RADIANS(angle));
        c.Offset.X = rand()%(int)dist;//X;
        c.Offset.Y = rand()%(int)dist;//Y;
    }

   FOR_LOOP(cellcount, numcells){                                                // FOR EACH CELL.....
       int e = 0;
       FOR_LOOP(edgecount, rand()%numcells){                                            // MAKE EDGES CONNECTING THE OTHER CELLS
             if(edgecount != cellcount){
                 int cnum = rand()%numcells;
                  int cnum2 = rand()%numcells;

                      cells[cnum].edges.push_back(MakeEdge(&cells[cnum], cells[cnum2], RANDOM(1)));  
                      cells[cnum2].edges.push_back(MakeEdge(&cells[cnum2], cells[cnum], RANDOM(1)));  
                    
                      e++;
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
    ret.Brain =  Net(3,5,2);
    ret.Friction  = RANDOM(1);

    ret.Color = RGB(RANDOM(255),RANDOM(255),RANDOM(255));

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
        ret.ID       =  parent->ID - other.ID;
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
//-------------------------------------------------------------------------------------------------------------------------------------------------------------
      for(Cell &Parent:cells){ // Cycle Every Cell
            Parent.Velocity *= .9; // APPLY A CRUDE "FRICTION" SO THAT VELOCITY IS LOST OVER TIME

            for(Edge &Child: Parent.edges){ // For Every Cell Get and Set Information about all Connecting Cells.
          
                  Child.Angle = GetAngle(Parent.Offset,  Child.second->Offset);
                  
                  Child.Displacement =  Get_Displacement(Child.second->Offset,Parent.Offset ) - Child.RestDistance; 
                  Child.Distance = Get_Distance(Parent, *Child.second);
                  float K = .1;
                  Child.second->Force.X += -K * (Child.Displacement.X);  // ((Child.second->Force * 1) / Number_of_Cells);
                  Child.second->Force.Y += -K * (Child.Displacement.Y);
            
            }
            
      Parent.See();

      Parent.Brain.Think();
      Parent.Angle = Parent.Brain.Layers[0].Neurons[0].Value;
      }

//_____________________________________________________________________________________________________________________________________________________________
//=============================================================================================================================================================
    
}


float Get_Distance(const Cell &parent,const Cell &child){
    float ret =  sqrt(Squared(parent.Offset.X - child.Offset.X)  + Squared(parent.Offset.Y - child.Offset.Y));
    if(ret < 0) ret = 0; // HMMMMMMM
    return ret;
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
    
     for(float dist = 0;dist < 100; dist++){
             Xx = X + dist * cos(RADIANS(Angle));
             Yy = Y + dist * sin(RADIANS(Angle));
             SET_PIXEL(Xx,Yy,RGB(255,255,255));
 //FILLED_CIRCLE(100,100,100);
     }
  Brain.Layers[0].Neurons[0].Value = Angle;
}

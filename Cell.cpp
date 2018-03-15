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

//=============================================================================================================================================================
//                                                         CELL CLASS                                                                                
//=============================================================================================================================================================

Cell::Cell(Organism *parent)
    : Offset(0, 0)
    , Starting(Offset)
    , Potential(Offset)
    , Velocity(0, 0)
    , Acceleration(0, 0)
    , Force(0, 0)
    , ID(parent->Number_of_Cells++)
    , Friction(RANDOM(1))
    , Angle(0)
    , Speed(0)
    , Size(10)
    , Mass(20)
    , Color(color_from_rgb(
        (55 + RANDOM(200)),
        (55 + RANDOM(200)),
        (55 + RANDOM(200))
    ))
    , Parent(parent)
    , Brain(Net(3, 5, 2))
{ }

void Cell::See() const
{
    const float
        X = Offset.X + Parent->Potential.X,
        Y = Offset.Y + Parent->Potential.Y;

    for (int dist = 0; dist < 8; dist++)
    {
        const auto Xx = X + dist * _COS(Angle);
        const auto Yy = Y + dist * _SIN(Angle);
        SET_PIXELII(Xx, Yy, 0xffffff);
    }
}

//=============================================================================================================================================================
//                                                         ORGANISM CLASS                                                                              
//=============================================================================================================================================================


Organism::Organism(unsigned char numcells, int x, int y)
    : Number_of_Cells(0)
    , Distance_moved(0.0)
    , Position(x, y)
    , Starting(x, y)
    , Potential(x, y)
    , X(x), Y(y)
    , Velocity(0, 0)
    , Radius(20)
    , Angle(0)

{
    FOR_LOOP(count, numcells)
        cells.emplace_back(this);

    const float Theta = 360.f / Number_of_Cells;

    for (Cell &c : cells)
    {
        Angle += Theta;
        while (Angle < 0) Angle += 360;
        while (Angle >= 360) Angle -= 360;

        const auto Xx = X + Radius * _COS(Angle);
        const auto Yy = Y + Radius * _SIN(Angle);
        c.Offset.X = Xx - Position.X;
        c.Offset.Y = Yy - Position.Y;  // rand()%(int)dist;//   // rand()%(int)dist;//
        c.Starting = c.Offset;
        c.Number_of_edges = 0;
    }

    FOR_LOOP(cellcount, numcells)
    {
        FOR_LOOP(edgecount, rand() % numcells)
        {
            // MAKE EDGES CONNECTING THE OTHER CELLS
            if (edgecount != cellcount)
            {
                int cnum = rand() % numcells;
                int cnum2 = rand() % numcells;

                cells[cnum].edges.emplace_back(&cells[cnum], &cells[cnum2], RANDOM(1));
                cells[cnum2].edges.emplace_back(&cells[cnum2], &cells[cnum], RANDOM(1));
                cells[cnum].Number_of_edges++;
                cells[cnum2].Number_of_edges++;
            }
        }
    }

}
Organism* Organism::Copy(Organism *Parent)
{
    *this = *Parent;
    return this;
}

Organism* Organism::Mutate(Organism Parent)
{
    this->Number_of_Cells = Parent.Number_of_Cells;
    this->cells = Parent.cells;


    this->Velocity.X = 0;
    this->Velocity.Y = 0;
    this->Distance_moved = 0;


    FOR_LOOP(cellcount, Parent.Number_of_Cells)
    {
        this->cells[cellcount].Offset = this->cells[cellcount].Starting;

        this->cells[cellcount].Velocity = 0;
        this->cells[cellcount].Force = 0;
        this->cells[cellcount].Acceleration = 0;

        this->cells[cellcount].Speed = 0;
        this->cells[cellcount].Angle = 0;

        this->cells[cellcount].Parent = this;

        this->cells[cellcount].Brain.Layers[0].Neurons[0].Value = 0;
        this->cells[cellcount].Brain.Layers[0].Neurons[1].Value = 0;
        this->cells[cellcount].Brain.Layers[0].Neurons[2].Value = 0;


        this->cells[cellcount].Brain.Layers[1].Neurons[0].Value = 0;
        this->cells[cellcount].Brain.Layers[1].Neurons[1].Value = 0;
        this->cells[cellcount].Brain.Layers[1].Neurons[2].Value = 0;
        this->cells[cellcount].Brain.Layers[1].Neurons[3].Value = 0;
        this->cells[cellcount].Brain.Layers[1].Neurons[4].Value = 0;

        FOR_LOOP(HNeuron, Parent.cells[cellcount].Brain.Layers[1].Number_of_Neurons)
        {
            FOR_LOOP(HSynapses, Parent.cells[cellcount].Brain.Layers[0].Number_of_Neurons)
            {
                this->cells[cellcount].Brain.Layers[1].Neurons[HNeuron].Synapses[HSynapses].Weight += ((RANDOM(2) - 1) / 10);
            }
        }

        this->cells[cellcount].Brain.Layers[2].Neurons[0].Value = 0;
        this->cells[cellcount].Brain.Layers[2].Neurons[1].Value = 0;

        FOR_LOOP(ONeuron, Parent.cells[cellcount].Brain.Layers[2].Number_of_Neurons)
        {
            FOR_LOOP(OSynapses, Parent.cells[cellcount].Brain.Layers[1].Number_of_Neurons)
            {
                this->cells[cellcount].Brain.Layers[2].Neurons[ONeuron].Synapses[OSynapses].Weight += ((RANDOM(2) - 1) / 10);
            }
        }
    }

    return this;
}


void Organism::Update(float Time_Step)
{
    const auto DELTA_TIME = (SDL_GetTicks() - SCREEN->TIME) / 10;
    SCREEN->TIME = SDL_GetTicks();

    float Xmove = 0, Ymove = 0;

    for (Cell &Parent : cells)
    {
        Parent.Acceleration = Parent.Force / Parent.Mass;
        Parent.Velocity += Parent.Acceleration;
        Parent.Offset += Parent.Velocity;

        Parent.Force = 0;

        Xmove += Parent.Offset.X;
        Ymove += Parent.Offset.Y;
    }

    Xmove = Xmove / Number_of_Cells;
    Ymove = Ymove / Number_of_Cells;
    X = Xmove + Potential.X;
    Y = Ymove + Potential.Y;

    Potential.X = Xmove + Starting.X;
    Potential.Y = Ymove + Starting.Y;
    Distance_moved = sqrt(Squared(X - Starting.X) + Squared(Y - Starting.Y));


    for (Cell &Parent : cells)
    {
        Parent.Velocity *= .5f; // APPLY A CRUDE "FRICTION" SO THAT VELOCITY IS LOST OVER TIME

        Parent.See();
        Parent.Brain.Think();

        for (Edge &Child : Parent.edges)
        {
            // For Every Cell Get and Set Information about all Connecting Cells.
            const int off = Child.Child_ID;

            Child.Angle = GetAngle(Parent.Offset, cells[off].Offset);

            Child.Displacement = Get_Displacement(cells[off].Offset, Parent.Offset) - Child.RestDistance;
            Child.Distance = Child.Get_Distance(cells[off]);

            constexpr float K = .1f;
            cells[off].Force.X += -K * Child.Displacement.X;
            cells[off].Force.Y += -K * Child.Displacement.Y;


            // acceleration = (−SpringStiffness ⁄ mass) * Position 


            cells[off].Brain.Layers[0].Neurons[1].Value = cells[off].Force.X; //Parent.Brain.Layers[2].Neurons[1].Value;
            cells[off].Brain.Layers[0].Neurons[0].Value = cells[off].Force.Y; //.Brain.Layers[2].Neurons[0].Value;
        }

        while (Parent.Angle < 0) Parent.Angle += 360;
        while (Parent.Angle >= 360) Parent.Angle -= 360;
    }


    for (Cell &C : cells)
    {
        C.Speed = C.Brain.Layers[2].Neurons[0].Value * 30;
        C.Angle += (C.Brain.Layers[2].Neurons[1].Value * 5);

        while (C.Angle < 0) C.Angle += 360;
        while (C.Angle >= 360) C.Angle -= 360;

        C.Force.X += C.Speed * _COS(C.Angle);
        C.Force.Y += C.Speed * _SIN(C.Angle);
    }
}

void Organism::Draw() const
{
    FOR_LOOP(cellcount, Number_of_Cells)
    {
        FOR_LOOP(edgecount, cells[cellcount].Number_of_edges)
        {
            const int Child = cells[cellcount].edges[edgecount].Child_ID;

            const float
                x1 = cells[cellcount].Offset.X + Potential.X,
                y1 = cells[cellcount].Offset.Y + Potential.Y,

                x2 = (x1 + cells[Child].Offset.X + Potential.X) / 2,
                y2 = (y1 + cells[Child].Offset.Y + Potential.Y) / 2;

            SET_DRAW_COLOR(cells[cellcount].Color);
            LINE(x1, y1, x2, y2);
        }
    }
}

int Organism::Collision(Organism *List []) const
{
    CIRCLE(X, Y, 50);
    FOR_LOOP(OrganismCount, 55)
    {
        if (List[OrganismCount] != this)
        {
            if (sqrt(Squared(List[OrganismCount]->Y - this->Y) + Squared(List[OrganismCount]->X - this->X)) < 50)
            {
                List[OrganismCount]->Mutate(Organism(*this));
                FILLED_CIRCLE(X, Y, 30);
                FILLED_CIRCLE(List[OrganismCount]->X, List[OrganismCount]->Y, 30);
            }
        }
    }

    return 1;
}



//=============================================================================================================================================================
//                                                      EDGE CLASS                                                                                          
//=============================================================================================================================================================


Edge::Edge(Cell *parent, Cell *other, unsigned char tension)
    : Parent_ID(parent->ID),
    Child_ID(other->ID),
    Tension(tension),
    Angle(0),
    Parent_ptr(parent),
    Child_ptr(other),
    Distance(Get_Distance(*other)),
    RestDistance(Get_Displacement(other->Offset, parent->Offset)),
    Displacement(0, 0)
{ }

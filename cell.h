#pragma once

#include<vector>
#include<iostream>
#include"window.h"
#include"Vertex2D.h"
#include"brain.h"

#define MUTATION_RATE ((rand()%101)-50)

#define MAZE_SIZE          100

#define MAX_AGE            960
#define MAX_HEALTH         2000
#define MAX_CREATURES      1000
#define MAX_CELLS          30

#define WORLD_WIDTH        SCREENWIDTH  
#define WORLD_HEIGHT       SCREENHEIGHT

#define NUM_NEURONS        5
#define NUM_RESPONSES      3

#define NORTHING           0
#define ENEMY              1
#define MATE               2
#define WALL               3
#define FOOD               4
#define NUMBER_OF_NEURONS  2
#define AMOUNT_FOOD        2200


class Organism;
struct Edge;


class Cell final
{
public:
    Cell() = default;
    ~Cell() = default;
    Cell(Organism *Parent);


    Vector2D
        Offset,
        Starting,
        Potential,
        Velocity,
        Acceleration,
        Force;

    int           ID;
    float         Friction;

    float         Angle,
        Speed,
        Size,
        Mass;

    unsigned char Number_of_edges;
    unsigned long Color;

    Organism     *Parent;

    std::vector<Edge> edges;

    Net Brain;

    void See() const;
    void Set_Position(int x, int y)
    {
        Offset.X = x;
        Offset.Y = y;
        Starting.X = x;
        Starting.Y = y;
    }

    int Collision();
};


struct Edge final
{
    Edge() = default;
    ~Edge() = default;

    Edge(Cell *parent, Cell *other, unsigned char tension);

    // Cell *second;
    Cell *Parent_ptr,
        *Child_ptr;

    int   Parent_ID,
        Child_ID;


    Vector2D  Displacement,
        RestDistance;

    float         Distance,
        Tension,
        Angle;

    float     Get_Distance(const Cell &child)
    {
        float ret = sqrt(Squared(Parent_ptr->Offset.X - child.Offset.X) + Squared(Parent_ptr->Offset.Y - child.Offset.Y));
        //  if(ret < 0) ret = 0; // HMMMMMMM
        return ret;
    }
    //    unsigned long Color;
};


class Organism final
{
public:
    Organism() = default;
    ~Organism() = default;

    Organism(unsigned char numcells, int x, int y);

    unsigned char Number_of_Cells;

    int ID;
    int X, Y;
    Vector2D Position,
        Potential,
        Velocity,
        Starting;

    float    Distance_moved,
        Radius,
        Angle;

    std::vector<Cell> cells;

public:

    void  Update(float Time_Step);
    void  Draw() const;
    void  Set_Position(int x, int y)
    {
        Starting.X = x;
        Starting.Y = y;
        Position.X = x;
        Position.Y = y;
        Potential.X = x;
        Potential.Y = y;
    }

    Organism*   Copy(Organism *Parent);
    Organism*   Mutate(Organism  Parent);
    int         Collision(Organism *List []) const;
};

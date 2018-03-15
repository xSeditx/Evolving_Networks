
#include <stdio.h>  
#include <stdlib.h> 

class World final
{
public:
    World(int x, int y)
    {
        Size.Height = x;
        Size.Width = y;
        Buffer = new int[x * y];
        memset(Buffer, 0, x * y * sizeof(*Buffer));
    }
    struct size
    {
        int Height,
            Width;
    }Size;

    struct camera
    {
        struct pos
        {
            int X,
                Y;
        } Position;

        struct view
        {
            int Height,
                Width;
        } ViewPort;
    } Camera;

    int CheckSpace(int x, int y) const
    {
        return Buffer[x + Size.Width * y];
    }

    void SetSpace(int x, int y, int value)
    {
        if (y < 0) return;
        Buffer[x + Size.Width * y] = value;
    }

    std::vector<Organism>  Creatures;
    int *Buffer;
};

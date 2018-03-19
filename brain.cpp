#include<math.h>
#include"brain.h"
#include"window.h"

//---------------------------------------------------------------------------------------------------
//                             LAYER CONSTRUCTOR AND METHODS                                         
//___________________________________________________________________________________________________

Layer::Layer(int number_of_neurons)
    :Number_of_Neurons(number_of_neurons)
{
    Neurons.resize(number_of_neurons);
}


//---------------------------------------------------------------------------------------------------
//                             NET CONSTRUCTOR AND METHODS                                           
//___________________________________________________________________________________________________

Net::Net(int inputs, int hidden, int outputs)
{
    Layers.emplace_back(inputs);
    Layers.emplace_back(hidden);
    Layers.emplace_back(outputs);


    FOR_LOOP(Hcount, hidden)
    {
        FOR_LOOP(Icount, inputs)
        {
            Layers[1].Neurons[Hcount].Synapses.emplace_back(RANDOM(1));
        }
    }

    FOR_LOOP(Ocount, outputs)
    {
        FOR_LOOP(Hcount, hidden)
        {
            Layers[2].Neurons[Ocount].Synapses.emplace_back(RANDOM(1));

        }
    }
}

void Net::Think()
{

    float Sum = 0.0;

    FOR_LOOP(HiddenCount, Layers[1].Number_of_Neurons)
    {
        FOR_LOOP(InputCount, Layers[0].Number_of_Neurons)
        {
            Sum += (Layers[0].Neurons[InputCount].Value *
                Layers[1].Neurons[HiddenCount].Synapses[InputCount].Weight);
        }
        Layers[1].Neurons[HiddenCount].Value = Activation(Sum);
    }

    Sum = 0.0;

    FOR_LOOP(OutputCount, Layers[2].Number_of_Neurons)
    {
        FOR_LOOP(HiddenCount, Layers[1].Number_of_Neurons)
        {
            Sum += (Layers[1].Neurons[HiddenCount].Value *
                Layers[2].Neurons[OutputCount].Synapses[HiddenCount].Weight);
        }
        Layers[2].Neurons[OutputCount].Value = Activation(Sum);
    }
}



void Net::Draw(int x,int y)
{
   int SizeX = 20, 
       SizeY = 15;

   int X1 = x,
       Y1 = y, 
       X2 = 0,
       Y2 = 0;
    
   float WeightValue = 0;
    
   for(int Layercount = 1; Layercount < Number_of_Layers; Layercount++)
    {
        X1 = (Layercount * SizeX) + x;
       for(int NeuronCount = 0; NeuronCount < Layers[Layercount].Number_of_Neurons; NeuronCount++)
       {
           Y1 = (NeuronCount * SizeY) + y;
           for(int SynapseCount = 0; SynapseCount <  Layers[Layercount - 1].Number_of_Neurons; SynapseCount++)
           {
               Y2 = (SynapseCount * SizeY) + y;
               X2 = ((Layercount - 1) * SizeX) + x;
               WeightValue = Layers[Layercount].Neurons[NeuronCount].Synapses[SynapseCount].Weight;
               
               if(WeightValue > 0)
               { 
                  int Color = WeightValue * 255;
                      SET_DRAW_COLOR(RGB(0, Color, 0));
               } 
               else
               {
                  int Color = abs(WeightValue * 255);
                      SET_DRAW_COLOR(RGB(Color,0,0));
               }
               LINE(X1,Y1,X2,Y2);
               float Mag = 128 * (1+(Layers[Layercount-1].Neurons[SynapseCount].Value));
               SET_PIXELII(X2,Y2,RGB((int)Mag,(int)Mag,(int)Mag));
           }

              float Mag = 128 * (1+(Layers[Layercount].Neurons[NeuronCount].Value));
              SET_PIXELII(X1,Y1,RGB((int)Mag,(int)Mag,(int)Mag));
       }
    }
}



Synapse::Synapse(float value)
    : Weight(value)
{ }

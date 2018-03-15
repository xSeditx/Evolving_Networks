#include<math.h>
#include"brain.h"
#include"window.h"


////Synapse MakeSynapse(float value, Neuron *parent, Neuron *other){
//        Synapse ret;
//                ret.Other = other; 
//                ret.Weight = value; 
//                ret.CREATOR = 3;
// return ret;
//}
//


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
void Net::Draw()
{ }


Synapse::Synapse(float value)
    : Weight(value)
{ }

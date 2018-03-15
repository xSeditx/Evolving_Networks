#include<math.h>
#include"brain.h"
#include"window.h"

extern Neuron *GLOBAL_NEURON; // DEBUG SHIT
extern Neuron  NARRAY[100];

Synapse::Synapse()
{ }

Synapse::~Synapse()
{ }



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
Layer::Layer()
{ }

Layer::Layer(int number_of_neurons)
    :Number_of_Neurons(number_of_neurons)
{
    Neurons.reserve(number_of_neurons);

    FOR_LOOP(N, number_of_neurons)
    {
        Neurons.emplace_back(Neuron());
    }
}

Layer::~Layer()
{ }



//---------------------------------------------------------------------------------------------------
//                             NEURON CONSTRUCTOR AND METHODS                                        
//___________________________________________________________________________________________________
Neuron::~Neuron()
{ }
Neuron::Neuron()
    : Value(0)
{ }



//---------------------------------------------------------------------------------------------------
//                             NET CONSTRUCTOR AND METHODS                                           
//___________________________________________________________________________________________________
Net::Net()
{ }
Net::~Net()
{ }
Net::Net(int inputs, int hidden, int outputs)
{


    Layers.emplace_back(Layer(inputs));
    Layers.emplace_back(Layer(hidden));
    Layers.emplace_back(Layer(outputs));


    FOR_LOOP(Hcount, hidden)
    {
        FOR_LOOP(Icount, inputs)
        {
            Layers[1].Neurons[Hcount].Synapses.emplace_back(
                Synapse(RANDOM(1), &Layers[0].Neurons[Icount]));
        }
    }

    FOR_LOOP(Ocount, outputs)
    {
        FOR_LOOP(Hcount, hidden)
        {
            Layers[2].Neurons[Ocount].Synapses.emplace_back(
                Synapse(RANDOM(1), &Layers[1].Neurons[Hcount]));

        }
    }
}



void Net::Think()
{

    float
        Sum = 0.0;

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





Synapse::Synapse(float value, Neuron *other)
    :Other(other),
    Weight(value)
{ }





inline float Sigmoid(float x)
{
    return 1.0 / (1.0 + exp(-x));
}
inline float Activation(float x)
{
    float y = (Sigmoid(x) * 2) - 1;
    return y;
}


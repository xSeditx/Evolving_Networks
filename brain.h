#pragma once

#include<iostream>
#include<vector>

class Neuron;

class Synapse
{
public:
    Synapse() = default;
    ~Synapse() = default;

    Synapse(float value, Neuron *other);

    int CREATOR;
    float     Weight;
    Neuron   *Other;


};
extern Synapse MakeSynapse(float value, Neuron *parent, Neuron *other);


class Neuron
{
public:
    Neuron() = default;
    ~Neuron() = default;

    float Value;

    std::vector<Synapse> Synapses;
};

class Layer
{
public:
    Layer() = default;
    ~Layer() = default;

    Layer(int number_of_neurons);

    enum LType
    {
        Input,
        Hidden,
        Output
    };   LType LayerType;

    int Number_of_Neurons;
    std::vector<Neuron> Neurons;

};


class Net
{
public:
    Net() = default;
    ~Net() = default;
    Net(int inputs, int hidden, int outputs);

    int Number_of_Layers;
    std::vector<Layer> Layers;

    void Think();
    void Draw();
};


inline static float Sigmoid(float x)
{
    return 1.f / (1.f + exp(-x));
}

inline static float Activation(float x)
{
    return Sigmoid(x) * 2 - 1;
}

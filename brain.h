#pragma once

#include<iostream>
#include<vector>

using namespace std;

class Neuron;

class Synapse{
public:
    Synapse();
   ~Synapse();
    Synapse(float value, Neuron *other);

    float     Weight;
    float     Delta_Weight;
    Neuron   *Other;
public:

};           


class Neuron{
public:
    Neuron();~Neuron();


    float Value;
    float ActivatedValue;
    float DerivativeValue; // f(x) * (1 - f(x))
    vector<Synapse> Synapses;
};


class Layer{
public:
    Layer();~Layer();

    enum LType{
        Input,
        Hidden,
        Output
    };
    LType LayerType;

    int Number_of_Neurons;
    vector<Neuron> Neurons;

};


class Net{
public:
    Net();~Net();
    Net(int inputs, int hidden, int outputs);
    int Number_of_Layers;
    vector<Layer> Layers;
    void FeedForward();

    void MakeSynapse(Neuron parent, Neuron child);

    void Think();
};

extern inline float Sigmoid(float x);
extern inline float Activation(float x);

#include<math.h>
#include"brain.h"
#include"window.h"



Synapse::Synapse(){
}
Synapse::~Synapse(){}
Synapse::Synapse(float value , Neuron *other)
{
     Other = other; 
    Weight = value; 
    Delta_Weight = 0;

}



//---------------------------------------------------------------------------------------------------------------------------------------
//                             LAYER CONSTRUCTOR AND METHODS                                                                                              
//_____________________________________________________________________________________________________________________

Layer::Layer(){}
Layer::~Layer(){}


//---------------------------------------------------------------------------------------------------------------------------------------
//                             NEURON CONSTRUCTOR AND METHODS                                                                                              
//_____________________________________________________________________________________________________________________

Neuron::~Neuron(){
Value = 200;}

Neuron::Neuron(){
    Value = 999; //RANDOM(10);//0.0;
    ActivatedValue = 0.0;
    DerivativeValue = 0.0;
}

//---------------------------------------------------------------------------------------------------------------------------------------
//                             NET CONSTRUCTOR AND METHODS                                                                                              
//_____________________________________________________________________________________________________________________

Net::Net(int inputs, int hidden, int outputs){

//_____________________________Input________________________________________________________________

    Layer *l = new Layer();
    l->LayerType = Layer::Input; 
    l->Number_of_Neurons = inputs;

    FOR_LOOP(N, inputs){
        Neuron *nur = new Neuron(); 
         l->Neurons.push_back(*nur);
      
    }
    Layers.push_back(*l);
    
//___________________________Hidden__________________________________________________________________

    Layer *h = new Layer();
    h->LayerType = Layer::Hidden; 
    h->Number_of_Neurons = hidden;

    FOR_LOOP(H, hidden){
        Neuron *hnur = new Neuron(); 
        h->Neurons.push_back(*hnur);
    }
    Layers.push_back(*h);

//___________________________Output____________________________________________________________________

    Layer *o = new Layer();
    o->LayerType = Layer::Output; 
    o->Number_of_Neurons = outputs;

    FOR_LOOP(O, outputs){
        Neuron *onur = new Neuron(); 
        o->Neurons.push_back(*onur);
    }
   
    Layers.push_back(*o);
//_______________________________________________________________________________________________________
    Number_of_Layers = 3;

 //
 //for(Neuron &H: Layers[1].Neurons){
 //    for(Neuron &I: Layers[0].Neurons){
 //         Synapse *Syn =  new Synapse(RANDOM(1), I);  
 //        // Syn.Other = &I; // SHOULD NOT NEED THIS AS IT SHOULD BE ASSIGNEDD IN THE ABOVE
 //         Layers[1].Neurons.Synapses.push_back(Syn);
 //    }
 //}
 //
     // Layers[1].Neurons[I].Synapses.reserve(Layers[1].Number_of_Neurons);
   FOR_LOOP(H, Layers[1].Number_of_Neurons){
        FOR_LOOP(I , Layers[0].Number_of_Neurons){
            Synapse *Syn =  new Synapse(RANDOM(1),  &Layers[0].Neurons[I]);  
            Layers[1].Neurons[H].Synapses.push_back(*Syn);   // Synapses.push_back(&Syn);
        }
    }
   FOR_LOOP(O, Layers[2].Number_of_Neurons){
        FOR_LOOP(H , Layers[1].Number_of_Neurons){
            Synapse Syn =  Synapse(RANDOM(1),  &Layers[1].Neurons[H]);  
            Layers[2].Neurons[O].Synapses.push_back(Syn);   // Synapses.push_back(&Syn);
        }
    }




//
//  for(Neuron &O: Layers[2].Neurons){
//       for(Neuron &H: Layers[1].Neurons){
//            Synapse *Syn =  new Synapse(RANDOM(1), H);  
//           // Syn.Other = &H; // SHOULD NOT NEED THIS AS IT SHOULD BE ASSIGNEDD IN THE ABOVE
//            Layers[2].Neurons.Synapses.push_back(&Syn);
//       }
//  }
//
//

}

Net::Net(){} Net::~Net(){}

void Net::Think(){
    float Sum = 0.0;

//
//    for(Neuron &H: Layers[1].Neurons){
//        for(Synapse &S: H.Synapses){
//          Sum += S.Weight * S.Other->Value;
//        }
//        H.Value = Activation(Sum);
//    }
//    Sum = 0.0;

 //   for(Neuron &O: Layers[2].Neurons){
 //       for(Synapse &S: O.Synapses){
 //         Sum += S.Weight * S.Other->Value;
 //       }
 //       O.Value = Activation(Sum);
 //   }
 //

}




inline float Sigmoid      (float x)
{
    return 1.0 / (1.0 + exp(-x));
}
inline float Activation   (float x)
{
    float y = (Sigmoid(x) * 2) - 1;
    return y;
}
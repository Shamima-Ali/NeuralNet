#include <iostream>
#include <vector>
#include <string>
#include "neuron.h"

using namespace std;

void create_neurons(vector<Neuron> &neu, int num) {
    for(int i = 0; i < num; i++) {
        Neuron n;
        double rand_weight = ((double) rand() / (RAND_MAX)) + 1;;
        n.setWeight((rand_weight + 1) / 10);
        // cout << n.getWeight() << endl;

        double rand_bias = ((rand() % 10) + 1) / 10;
        n.setBias((rand_bias + 1) / 10);
        
        neu.push_back(n);
    }
}

void create_neurons(int in, int mi, int ou) {
    cout << "this id test " << endl;
    // Input Neuronsssss
    int inputs_num = in;
    vector<Neuron> inputs;
    create_neurons(inputs, inputs_num);
   

    // Hidden Neuronsssss
    int mids_num = mi;
    vector<Neuron> mid1;
    // vector<Neuron> mid2(mids_num);
    create_neurons(mid1, mids_num);
    // create_neurons(mid2, mids_num);


    // Output Neuronsssss
    int outputs_num = ou;
    vector<Neuron> outputs;
    create_neurons(outputs, outputs_num);
}

    // cout << "done creating neurons..." << endl;
    // for (Neuron o : outputs) {
    //     cout << o.getWeight() << endl;
    // }
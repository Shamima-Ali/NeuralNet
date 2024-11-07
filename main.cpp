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

int main()
{
    // create_neurons(784, 15, 10);

    int inputs_num = 784;
    vector<Neuron> inputs;
    create_neurons(inputs, inputs_num);

    int mids_num = 15;
    vector<Neuron> mid1;
    create_neurons(mid1, mids_num);

    int outputs_num = 10;
    vector<Neuron> outputs;
    create_neurons(outputs, outputs_num);

    // prepare input




}


// g++ -o dee main.cpp
// ./dee
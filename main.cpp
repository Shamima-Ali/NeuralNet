#include <iostream>
#include <vector>
#include <string>
#include "neuron.h"
#include <fstream>
#include <sstream>

using namespace std;

void run_network(vector<Neuron> &inputs, vector<Neuron> &middle, vector<Neuron> &outputs) {
    // cout << "geting = " <<  inputs[782].getInput() << endl;
    for (Neuron in : inputs) {
        
        // cout << in.getOutput() << endl;
    }


}

void create_neurons(vector<Neuron> &neu, int num) {
    for(int i = 0; i < num; i++) {
        Neuron n;
        double rand_weight = ((double) rand() / (RAND_MAX)) + 1;;
        n.setWeight((rand_weight + 1) / 10);

        double rand_bias = ((rand() % 10) + 1) / 10;
        n.setBias((rand_bias + 1) / 10);
        
        neu.push_back(n);
    }
}

int main() {
    // create_neurons(784, 15, 10);

    // TODO: create vector of neurons to remove repitions
    int inputs_num = 784;
    vector<Neuron> inputs;
    create_neurons(inputs, inputs_num);

    int mids_num = 15;
    vector<Neuron> middle;
    create_neurons(middle, mids_num);

    int outputs_num = 10;
    vector<Neuron> outputs;
    create_neurons(outputs, outputs_num);

    // prepare input - using py here heehee

    // get pic
    fstream fin;
    fin.open("img.csv", ios::in);
    string temp, word;
    int line = 0;
    int position = 0;


    // assign each img to input neurons
    while (getline(fin, word)) {
        stringstream ss (word);
        string w;
        line++;
        
        // assign to neuron
        while (getline(ss, w, ','))
        {
            inputs[position].setInput(stoi(w));
            position++;
        }

        // end of an img - start learning
        if ((line % 28) == 0) {
            position = 0;
            run_network(inputs, middle, outputs);
        }

    }

   
}


// g++ -o dee main.cpp
// ./dee
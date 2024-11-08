#include <iostream>
#include <vector>
#include <string>
#include "neuron.h"
#include <fstream>
#include <sstream>

using namespace std;

void run_input_middle_layers(vector<Neuron> &inputs, vector<Neuron> &middle, vector< vector<float> > &inp_mid_weights, vector<float> &bias) {
    // cout << "geting = " <<  inputs[782].getInput() << endl;
    // int size = inp_mid_weights.size();
    int mid_neuron_position = 0;
    double mid_neuron_input = 0;
  
    


}

// x = input, y = out
void create_neuron_properties(vector< vector<float> > &prop, int x, int y) {
    for(int i = 0; i < x; i++) {
        vector<float> temp;
        for (int j = 0; j < y; j++) {
            double rand_weight = ((double) rand() / (RAND_MAX)) + 1;
            temp.push_back((rand_weight + 1) / 10);
        }
        prop.push_back(temp);
    }

    // cout << prop.size() << endl;
    // cout << prop[0].size() << endl;
}

int main() {
    // create_neurons(784, 15, 10);

    // TODO: remove repitions
    vector< vector<float> > inp_mid_weights;
    int inputs_num = 784;
    vector<Neuron> inputs(inputs_num);

    vector< vector<float> > mid_out_weights;
    vector<float> bias;
    int mid_num = 15;
    vector<Neuron> middle(mid_num);
    create_neuron_properties(inp_mid_weights, inputs_num, mid_num);
    // create_neuron_properties(mid_num, bias);

    int outputs_num = 10;
    vector<Neuron> outputs(outputs_num);
    create_neuron_properties(mid_out_weights, mid_num, outputs_num);
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
            // run_network(inputs, middle, outputs, inp_mid_weights, mid_out_weights, bias);
        }

    }

   
}


// g++ -o dee main.cpp
// ./dee


/*
 // double rand_weight_in = ((double) rand() / (RAND_MAX)) + 1;;
    // inp_midd.push_back((rand_weight_in + 1) / 10);

*/
#include <iostream>
#include <vector>
#include <string>
#include "neuron.h"
#include <fstream>
#include <sstream>

using namespace std;

void run_input_middle_layers(vector<Neuron> &inputs, vector<Neuron> &middle, vector< vector<float> > &inp_mid_weights, vector<float> &bias) {

    double summed_weights = 0;
    int mid_neuron = 0;

    while (mid_neuron < middle.size()) {
        for (int input_neuron = 0; input_neuron < inputs.size(); input_neuron++) {
            summed_weights += inputs[input_neuron].getInput() * inp_mid_weights[input_neuron][mid_neuron];
        }

        double z = summed_weights + bias[mid_neuron];
        middle[mid_neuron].setInput(z);
        middle[mid_neuron].applySigmoid(z);

        mid_neuron++;
    }
}

void run_middle_output_layers(vector<Neuron> &middle, vector<Neuron> &outputs, vector< vector<float> > &mid_out_weights, vector<float> &mid_out_bias) {

    double summed_weights = 0;
    int out_neuron = 0;

    while (out_neuron < outputs.size()) {
        for (int mid_neuron = 0; mid_neuron < middle.size(); mid_neuron++) {
            summed_weights += middle[mid_neuron].getOutput() * mid_out_weights[mid_neuron][out_neuron];
        }

        double z = summed_weights + mid_out_bias[out_neuron];
        outputs[out_neuron].setInput(z);
        outputs[out_neuron].applySigmoid(z);

        out_neuron++;
    }
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
    vector<int> guess;

    // TODO: remove repitions
    vector< vector<float> > inp_mid_weights;
    int inputs_num = 784;
    vector<Neuron> inputs(inputs_num);
    cout << "1. Number of inputs neurons = " << inputs.size() << endl;

    vector< vector<float> > mid_out_weights;
    int mid_num = 15;
    vector<Neuron> middle(mid_num);
    cout << "2. Number of middle neurons = " << middle.size() << endl;
    create_neuron_properties(inp_mid_weights, inputs_num, mid_num);
    cout << "3. Creating matrix for inp_mid " << middle.size() << endl;

    vector<float> bias;
    for (int i = 0; i < mid_num; i++) {
        double rand_weight = ((double) rand() / (RAND_MAX)) + 1;
        bias.push_back((rand_weight + 1) / 10);
    }

    vector<float> mid_out_bias;
    for (int i = 0; i < mid_num; i++) {
        double rand_weight = ((double) rand() / (RAND_MAX)) + 1;
        mid_out_bias.push_back((rand_weight + 1) / 10);
    }
    

    int outputs_num = 10;
    vector<Neuron> outputs(outputs_num);
    cout << "Number of outputs neurons = " << outputs.size() << endl;
    create_neuron_properties(mid_out_weights, mid_num, outputs_num);
    // prepare input - using py here heehee

    // get pic
    fstream fin;
    fin.open("img.csv", ios::in);
    string temp, word;
    int line = 0;
    int position = 0;
int res = 0;
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
            run_input_middle_layers(inputs, middle, inp_mid_weights, bias);
            run_middle_output_layers(middle, outputs, mid_out_weights, mid_out_bias);
            
            
            double maxVal = -1;
            
            for (int i = 0; i < outputs.size(); i++) {
                // cout << i << " " << outputs[i].getOutput() << endl;
                if (outputs[i].getOutput() > maxVal) {
                    maxVal = outputs[i].getOutput();
                    res = i;
                }
            }
        }

        guess.push_back(res);
    }

    
    // cout << res;
}


// g++ -o dee main.cpp
// ./dee


/*
 // double rand_weight_in = ((double) rand() / (RAND_MAX)) + 1;;
    // inp_midd.push_back((rand_weight_in + 1) / 10);

*/
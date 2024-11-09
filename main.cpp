#include <iostream>
#include <vector>
#include <string>
#include "neuron.h"
#include <fstream>
#include <sstream>
#include <random>

using namespace std;

ofstream wfile;

// TODO: combine the run functions cuz they are like basically the same and that's like sooooo not good cus like like....
void run_input_middle_layers(vector<Neuron> &inputs, vector<Neuron> &middle, vector< vector<double> > &inp_mid_weights, vector<double> &bias) {
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

    cout << "9. the middle layer after " << endl;
}

void run_middle_output_layers(vector<Neuron> &middle, vector<Neuron> &outputs, vector< vector<double> > &mid_out_weights, vector<double> &mid_out_bias) {

    double summed_weights = 0;
    int out_neuron = 0;

    while (out_neuron < outputs.size()) {
        for (int mid_neuron = 0; mid_neuron < middle.size(); mid_neuron++) {
            summed_weights += middle[mid_neuron].getActivation() * mid_out_weights[mid_neuron][out_neuron];
        }

        double z = summed_weights + mid_out_bias[out_neuron];
        outputs[out_neuron].setInput(z);
        outputs[out_neuron].applySigmoid(z);

        out_neuron++;
    }

    cout << "11. the output layer after " << endl;
    // for (auto o : outputs) {
    //     cout << o.getActivation() << endl;
    // }
}

vector<double> calculate_cost (vector<int> &guess, vector<int> &labelVect) {
    fstream fin;
    fin.open("label.csv", ios::in);
    vector<double> cost_functions;

    string temp, word;
    int line = 0;
    int position = 0;
    int res = 0;
    int labels = 0;

    int max_label = guess.size();

    // get the each label from the file
    while (getline(fin, word, ',')) {
        
        labelVect.push_back(stoi(word));
        double val = pow(stoi(word) - guess[labels], 2);
        cost_functions.push_back(val);

        // TODO: Take this out when testing 60k
        labels++;
        if (labels == max_label) {
            break;
        }
        
    }
    
    return cost_functions;
}

/*The calculation:

*/
vector<vector<double> > get_mid_out_weight_gradient(vector<vector<double> > & mid_out_weights, vector<Neuron> &outputs, vector<Neuron> &middle, vector<int> true_label) {
    int size = mid_out_weights.size();
    int sz = mid_out_weights[0].size();
    vector<vector<double> > weights_grad(size, vector<double>(sz, 0));

    for (int mid_neuron = 0; mid_neuron < size; mid_neuron++) { // 15 mid
        for (int out_neuron = 0; out_neuron < sz; out_neuron++) { // 10 out
            double cost_wrt_act =  outputs[out_neuron].getActivation() - true_label[out_neuron];
            double act_wrt_out = outputs[out_neuron].getActivation()* (1 - outputs[out_neuron].getActivation());
            double out_wrt_weight =  middle[mid_neuron].getActivation();

            weights_grad[mid_neuron][out_neuron] = cost_wrt_act * act_wrt_out * out_wrt_weight;
        }
    }
    return weights_grad;
}

vector<double> get_mid_out_bias_gradient(vector<Neuron> &outputs, vector<int> true_label) {
    vector<double> bias_grad;

    for (int out_neuron = 0; out_neuron < outputs.size(); out_neuron++) { // 10 out
        double cost_wrt_act =  outputs[out_neuron].getActivation() - true_label[out_neuron];
        double act_wrt_out = outputs[out_neuron].getActivation()* (1 - outputs[out_neuron].getActivation());
        double out_wrt_bias =  1;

        bias_grad.push_back(cost_wrt_act * act_wrt_out * out_wrt_bias);
    }
    return bias_grad;
}

vector<double> get_mid_out_error_signal(vector<Neuron> &outputs, vector<int> true_label) {
    vector<double> err_sign;

    for (int out_neuron = 0; out_neuron < outputs.size(); out_neuron++) { // 10 out
        double cost_wrt_act =  outputs[out_neuron].getActivation() - true_label[out_neuron];
        double act_wrt_out = outputs[out_neuron].getActivation()* (1 - outputs[out_neuron].getActivation());

        err_sign.push_back(cost_wrt_act * act_wrt_out);
    }
    return err_sign;
}


// x = input, y = out
void create_neuron_properties(vector< vector<double> > &prop, int x, int y) {    
    srand(time(0));
    for(int i = 0; i < x; i++) {
        vector<double> temp;
        for (int j = 0; j < y; j++) {

            double rand_weight = ((double) rand() / (RAND_MAX));
            int rand_sign = ((int) rand() / (RAND_MAX/2));
            if (rand_sign == 0) {
                temp.push_back(rand_weight*-1);
            } else {
                temp.push_back(rand_weight);
            }            
        }
        prop.push_back(temp);
    }

    
    for (int i = 0; i < prop.size(); i++) {
        for (int j = 0; j < prop[0].size(); j++) {
            wfile << prop[i][i] << " ";
        }
        wfile << endl;
    }
    
    wfile << "end of weights " << endl;
    // wfile.close();
}

int main() {
    vector<int> guess;

    // TODO: remove repitions
    vector< vector<double> > inp_mid_weights;
    int inputs_num = 784;
    vector<Neuron> inputs(inputs_num);
    cout << "1. Number of inputs neurons = " << inputs.size() << endl;

    vector< vector<double> > mid_out_weights;
    int mid_num = 15;
    vector<Neuron> middle(mid_num);
    cout << "2. Number of middle neurons = " << middle.size() << endl;
    cout << "3. Matrix for inp_mid of " << inputs_num << " and " << mid_num << endl;

    wfile.open ("weights-in-mid.txt");
    create_neuron_properties(inp_mid_weights, inputs_num, mid_num);

    vector<double> bias;
    for (int i = 0; i < mid_num; i++) {
        double rand_weight = ((double) rand() / (RAND_MAX)) + 1;
        bias.push_back((rand_weight + 1) / 100);
    }
    cout << "4. Vector for bias (inp_mid) of mid_num size" << mid_num << endl;
    // for (auto f : bias) {
    //     cout << f << " " << endl;
    // }
    // cout << bias[0] << endl;


    vector<double> mid_out_bias;
    for (int i = 0; i < mid_num; i++) {
        double rand_weight = ((double) rand() / (RAND_MAX)) + 1;
        mid_out_bias.push_back((rand_weight + 1) / 100);
    }
    cout << "5. Vector for bias (mid_num) of mid_num size" << mid_num << endl;
    // for (auto f : mid_out_bias) {
    //     cout << f << " " << endl;
    // }
    

    int outputs_num = 10;
    vector<Neuron> outputs(outputs_num);
    cout << "Number of outputs neurons = " << outputs.size() << endl;
    cout << "6. Matrix for mid_out_weights of " << mid_num << " and " << outputs_num << endl;
    create_neuron_properties(mid_out_weights, mid_num, outputs_num);
    // prepare input - using py here heehee

    // get pic
    fstream fin;
    fin.open("img.csv", ios::in);
    string temp, word;
    int line = 0;
    int position = 0;
    int res = 0;
    ofstream valfile;
    valfile.open ("val.txt");

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

        // end of an img - start learning - line % 28
        if ((line % 28) == 0) {

            cout << "7. reached the end of an img. Input neurons until line " << line << endl;
            valfile << "reached the end of an img. Input neurons until line " << line << endl;
            for (int i = 0; i < inputs.size(); i++) {
                valfile << inputs[i].getInput() << endl;
            }
            
            position = 0;

            cout << "8. run input middle layer " << endl;
            run_input_middle_layers(inputs, middle, inp_mid_weights, bias);

            cout << "10. run middle output layer " << endl;
            run_middle_output_layers(middle, outputs, mid_out_weights, mid_out_bias);
            
            double maxVal = -1;
            
            for (int i = 0; i < outputs.size(); i++) {
                if (outputs[i].getActivation() > maxVal) {            
                    maxVal = outputs[i].getActivation();
                    res = i;
                }
            }
            guess.push_back(res);
        }        
    }

    

    for (auto i : guess) {
        cout << "guess = " << i << endl;
    }    
    cout << "____________________________________________" << endl;

    vector<int> labelVect;
    vector<double> cost_functions = calculate_cost(guess, labelVect);
    // cout << endl;
    // for (auto i : cost_functions) {
    //     cout << "cost_functions = " << i << " ";
    // }
    // cout << endl;
    
    vector<vector<double> > mo_weights = get_mid_out_weight_gradient(mid_out_weights, outputs, middle, labelVect);
    vector<double> mo_bias = get_mid_out_bias_gradient(outputs, labelVect);
    vector<double> err_sig = get_mid_out_error_signal(outputs, labelVect);

    // cout << mo_weights[0].size();
    valfile.close();
}


// g++ -o dee main.cpp
// ./dee


/*
 // double rand_weight_in = ((double) rand() / (RAND_MAX)) + 1;;
    // inp_midd.push_back((rand_weight_in + 1) / 10);
*/
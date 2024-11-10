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
        summed_weights = 0;
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
        summed_weights = 0;
    }

    cout << "11. the output layer after " << endl;
    // for (auto o : outputs) {
    //     cout << o.getActivation() << endl;
    // }
}

double calculate_cost (vector<int> &guess, vector<int> &labelVect) {
    fstream fin;
    fin.open("label.csv", ios::in);
    vector<double> cost_functions;

    string temp, word;
    int line = 0;
    int position = 0;
    int res = 0;
    int labels = 0;
    double total_err = 0;

    int max_label = guess.size();

    // get the each label from the file
    while (getline(fin, word, ',')) {
        
        labelVect.push_back(stoi(word));
        total_err += pow(guess[labels] - stoi(word), 2);

        // TODO: Take this out when testing 60k
        labels++;
        if (labels == max_label) {
            break;
        }
        
    }
    
    return total_err/2;
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

            weights_grad[mid_neuron][out_neuron] = 2* cost_wrt_act * act_wrt_out * out_wrt_weight;
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

void update_weights_biases(vector<vector<double> > & weights, vector<double> &biases, double learning_rate, vector<vector<double> >& grad_weights, vector<double>& grad_biases){

    for (int i = 0; i < weights.size(); i++) {
        for (int j = 0; j < weights[0].size(); j++) {
            weights[i][j] = weights[i][j] - (learning_rate * grad_weights[i][j]); 
        }
    }

    for (int i = 0; i < biases.size(); i++) {
        biases[i] = biases[i] - (learning_rate * grad_biases[i]);
    }
}

// article that saved me - https://mattmazur.com/2015/03/17/a-step-by-step-backpropagation-example/
            /*
            double cost_wrt_act = cost_output1_wrt_act_mid1 + cost_output2_wrt_act_mid1 + cost_output3_wrt_act_mid1;
                // do the next 3 steps for all output neurons - 10 times and then add up like above
            cost_output1_wrt_act_mid1 = cost_output1_output_output1 * output_output1_wrt_act_mid1
            cost_output1_output_output1 = cost_output1_wrt_act_output1 * act_output1_wrt_output_output1
            output_output1_wrt_act_mid1 = mid_out_weight[mid1][out1]

            double act_wrt_out = act * (1 - act)
            double out_wrt_weight = input
            */

vector<double> get_inp_mid_error_signal(vector<vector<double> > & mid_out_weights, vector<double> &err_signals) {
    vector<double> err_sign;
    int size = mid_out_weights.size();
    int sz = mid_out_weights[0].size();

    for (int mid = 0; mid < size; mid++) { // 15 mid
        double val = 0;
        for (int out = 0; out < sz; out++) { // 10 out
            val += err_signals[out] * mid_out_weights[mid][out]; 
        }
        err_sign.push_back(val);
    }

    return err_sign;
}

vector<vector<double> > get_inp_mid_weight_gradient(vector<vector<double> > & mid_out_weights, vector<Neuron> &inputs, vector<double> &inp_mid_err_sig) {
    int size = mid_out_weights.size();
    vector<vector<double> > weights_grad(size, vector<double>(inputs.size(), 0));

    for (int mid = 0; mid < size; mid++) {
        for (int in = 0; in < inputs.size(); in++) {
            weights_grad[mid][in] = inp_mid_err_sig[mid] * inputs[in].getInput();        
        }
    }
    return weights_grad;
}


/*
   double cost_wrt_act =  inp_mid_weights[in_neuron][mid_neuron];
            double act_wrt_out = middle[mid_neuron].getActivation()* (1 - middle[mid_neuron].getActivation());
            double out_wrt_weight =  inputs[in_neuron].getActivation();

*/


vector<double> get_inp_mid_bias_gradient(vector<vector<double> > & mid_out_weights, vector<Neuron> &middle, vector<double> &inp_mid_err_sig) {
    int size = mid_out_weights.size();
    vector<double> bias_grad(size, 0);


    for (int mid = 0; mid < size; mid++) {
        bias_grad[mid] = inp_mid_err_sig[mid] * (middle[mid].getActivation() * (1 - middle[mid].getActivation()));    
    }

    return bias_grad;
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
    double total_error = calculate_cost(guess, labelVect);
    // cout << endl;
    // for (auto i : cost_functions) {
    //     cout << "cost_functions = " << i << " ";
    // }
    // cout << endl;

    // TODO: make global vars
    double learning_rate = 0.01; //b/w 0.001 to 0.1

    // Between the middle and output layers
    vector<vector<double> > mo_grad_weights = get_mid_out_weight_gradient(mid_out_weights, outputs, middle, labelVect);
    vector<double> mo_grad_bias = get_mid_out_bias_gradient(outputs, labelVect);
    vector<double> err_sig = get_mid_out_error_signal(outputs, labelVect);
    update_weights_biases(mid_out_weights, mid_out_bias, learning_rate, mo_grad_weights, mo_grad_bias);

    // Between the input and middle layers
    vector<double> inp_mid_err_sig = get_inp_mid_error_signal(mid_out_weights, err_sig);
    vector<vector<double> > im_grad_weights = get_inp_mid_weight_gradient(mid_out_weights, inputs, inp_mid_err_sig);
    vector<double> im_grad_bias = get_inp_mid_bias_gradient(mid_out_weights, middle, inp_mid_err_sig);
    update_weights_biases(inp_mid_weights, bias, learning_rate, im_grad_weights, im_grad_bias);


    valfile.close();
}


// g++ -o dee main.cpp
// ./dee


/*
 // double rand_weight_in = ((double) rand() / (RAND_MAX)) + 1;;
    // inp_midd.push_back((rand_weight_in + 1) / 10);
*/
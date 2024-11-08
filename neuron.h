#include <iostream>
#include <vector>
#include <string>

using namespace std;


class Neuron {
    double weight, input, output, bias, activationValue; // activate
    vector<double> weights;
    
    public:
    double getWeight() {
        return weight;
    }

    double getInput() {
        return input;
    }

    double getOutput() {
        return activationValue;
    }

    double getBias() {
        return bias;
    }

    void setInput(double in) {
        input = in;
    }

    void setBias(double bi) {
        bias = bi;
    }

    // void setOutput(double out) {
    //     output = out;
    // }

    void setWeight(double wei) {
        weight = wei;
    }

    void applySigmoid() {
        double z = (weight*input) + bias;
        activationValue = z / (1 + exp(z));
    }

} ;
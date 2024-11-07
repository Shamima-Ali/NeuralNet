#include <iostream>
#include <vector>
#include <string>

using namespace std;


class Neuron {
    double weight, input, output, bias, actValue; // activate

    public:
    double getWeight() {
        return weight;
    }

    double getInput() {
        return input;
    }

    double getOutput() {
        return output;
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

    void setOutput(double out) {
        output = out;
    }

    void setWeight(double wei) {
        weight = wei;
    }

    double applySigmoid() {
        double z = (weight*input) + bias;
        actValue = z / (1 + exp(z));
        return actValue;
    }

} ;
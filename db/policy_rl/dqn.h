//
// Created by Navneet Madhu Kumar on 2019-07-10.
//

#pragma once


#include <torch/torch.h>

struct DQN : torch::nn::Module{
    DQN(int64_t input_channels, int64_t num_actions)
            :
            conv1(torch::nn::Conv2dOptions(input_channels, 32, 2)
                          .stride(1)
                          ),
            conv2(torch::nn::Conv2dOptions(32, 64, 2)
                          .stride(1)
                          ),
//            conv3(torch::nn::Conv2dOptions(64, 64, 3)
//                          .stride(1)
//                          ),

            linear1(torch::nn::Linear(64*2*7998, 512)),
            output(torch::nn::Linear(512, num_actions)){}

    torch::Tensor forward(torch::Tensor input) {
        input = torch::relu(conv1(input));
        input = torch::relu(conv2(input));
//        input = torch::relu(conv3(input));
//                std::cout << "4 " << std::endl;
        // Flatten the output
        input = input.view({input.size(0), -1});
        input = torch::relu(linear1(input));
        input = output(input);
        return input;
    }

    torch::Tensor act(torch::Tensor state){
        torch::Tensor q_value = forward(state);
        torch::Tensor action = std::get<1>(q_value.max(1));
        return action;
    }
 //conv3
    torch::nn::Conv2d conv1, conv2;
    torch::nn::Linear linear1, output;
};

"""
Simple demo with multiple subplots.
"""
import numpy as np
import matplotlib.pyplot as plt

def PlotModel(inputVars, outputVars, learnedModel):

    plt.figure(1)
    for i in range(0,len(inputVars[0])):
        for j in range(0,len(outputVars[0])):

           # print('Current Subplot index', i*len(outputVars[0])+j+1)
           # print('Current x-axis values', [k[i] for k in inputVars])
           # print('Current y-axis values', [k[j] for k in outputVars])
            print('Current learned values', [k[j] for k in learnedModel])
            plt.subplot(len(inputVars[0]), len(outputVars[0]),i*len(outputVars[0])+j+1)
            plt.scatter([k[i] for k in inputVars], [k[j] for k in outputVars])
            plt.hold()
            plt.scatter([k[i] for k in inputVars], [k[j] for k in learnedModel], 'r')
            #plt.title('A tale of 2 subplots')
            #plt.ylabel('Damped oscillation')
            #plt.ylabel('Undamped')

    plt.show()


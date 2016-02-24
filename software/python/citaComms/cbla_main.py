from cbla_learner import Learner
import simpleTeensyComs
import Plots


#Initialize Comms
# Initialize Comms and setup the teensy
destination = simpleTeensyComs.teensy_sernum
origin = simpleTeensyComs.cbla_pc_id
Grasshopper = simpleTeensyComs.udp_node_id

teensyComms = simpleTeensyComs.initializeComms(simpleTeensyComs.teensy_comport)


# Find out what the Teensy has
numDevices = simpleTeensyComs.QueryNumDevices(teensyComms, destination, origin)
print('The teensy has', numDevices, 'devices')
devList = simpleTeensyComs.QueryIDs(teensyComms, destination, origin)
numActs = 0
ActsList = []
numSens = 0
SensList = []
sensValues = []
actValues = []

for i in range(0,len(devList)):
    print(devList[i].pr())
    if devList[i].type%2 == 0:
        numSens += 1
        SensList.append(devList[i])
        sensValues.append(0)
    else:
        numActs += 1
        ActsList.append(devList[i])
        actValues.append(0)

def normalize_sens(sensValues,SensList):
    normValues = []
    for i in range(0,len(SensList)):
        #for now, scale everything the same, this function can be extended to scale
        #separately based on the device type in the sensors list
        normValues.append(sensValues[i]/1023)

    return normValues




lrnr = Learner(tuple([0]*numSens),tuple([0]*numActs))



iterNum = 0
actionValHist = []
while 1:


    # Act:  Update all the active actuators (do not act on the very first iteration,
    # until the sensors have been read
    if iterNum > 0:
        for i in range(0,len(ActsList)):
            simpleTeensyComs.Fade(teensyComms, destination, origin,ActsList[i].genByteStr(),
                                  int(actValues[i]),0)
            print('Command Actuator ', i, 'to Value ', int(actValues[i]))

    #Sense:  Read all the sensors
    for i in range(0,len(SensList)):
        sensValues[i] = simpleTeensyComs.Read(teensyComms, destination,
                                              origin,SensList[i].genByteStr(), 0)
        print('Sensor ', i, 'Reads a Value of ',sensValues[i])

    #Learn:
    lrnr.learn(tuple(normalize_sens(sensValues,SensList)),tuple(actValues))

    #Select Next action to perform
    actValues = lrnr.select_action()

    numExperts = lrnr.expert.get_num_experts()

    #if iterNum == 10:
     #  Plots.PlotModel(list(lrnr.expert.training_data),
     #                  list(lrnr.expert.training_label),
      #                 lrnr.expert.predict_model.predict(list(lrnr.expert.training_data)))

    if numExperts > 1:
        print('Increased number of experts')
    #Report the action value and the number of experts currently in the system
    print('Current max action value is ', lrnr.expert.get_largest_action_value())
    print('Current number of experts is', lrnr.expert.get_num_experts())

    simpleTeensyComs.CBLAStatusReport(origin,Grasshopper, 0,
                                      lrnr.expert.get_num_experts(),
                                      lrnr.expert.get_largest_action_value())

    iterNum += 1
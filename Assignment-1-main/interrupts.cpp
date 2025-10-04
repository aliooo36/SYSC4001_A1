/**
 *
 * @file interrupts.cpp
 * @author Muhammad Ali & Gregory Horvat
 *
 */

#include<interrupts.hpp>

int main(int argc, char** argv) {

    //vectors is a C++ std::vector of strings that contain the address of the ISR
    //delays  is a C++ std::vector of ints that contain the delays of each device
    //the index of these elemens is the device number, starting from 0
    auto [vectors, delays] = parse_args(argc, argv);
    std::ifstream input_file(argv[1]);

    std::string trace;      //!< string to store single line of trace file
    std::string execution;  //!< string to accumulate the execution output

    /******************ADD YOUR VARIABLES HERE*************************/

    int currentTime = 0;
    const int contextSRTime = 10;
    const int ISRActivityTime = 40;

    /******************************************************************/

    //parse each line of the input trace file
    while(std::getline(input_file, trace)) {
        auto [activity, duration_intr] = parse_trace(trace);

        /******************ADD YOUR SIMULATION CODE HERE*************************/

        if (activity == "CPU") {
            execution += std::to_string(currentTime) + ", " + std::to_string(duration_intr) + ", CPU\n";
            currentTime += duration_intr;
        }

        else if (activity == "SYSCALL") {
            int device_num = duration_intr;

            // checking to see if ISR is in vector table
            if (duration_intr >= vectors.size()) { 
                execution += std::to_string(currentTime) + ", Error: Invalid ISR. Does not exist in vector table.";
                currentTime += 1;
                continue;
            }
        
            auto [intrpExec, updateTime] = intr_boilerplate(currentTime, device_num, contextSRTime, vectors); // helper function execution and then storing in variables
            execution += intrpExec;
            currentTime += updateTime;

            execution += std::to_string(currentTime) + " ," + std::to_string(ISRActivityTime) + " , Call device driver";
            currentTime += ISRActivityTime;

            execution += std::to_string(currentTime) + " , 1" + ", IRET";

        }
        else if (activity == "END_IO") {
            continue;
        }

        /************************************************************************/

    }

    input_file.close();

    write_output(execution);

    return 0;
}

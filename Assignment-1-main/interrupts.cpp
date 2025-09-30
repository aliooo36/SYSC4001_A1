/**
 *
 * @file interrupts.cpp
 * @author Sasisekhar Govind
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

    int current_time = 0;
    const int context_SR_time = 10;
    std::vector<int> deviceTimers = delays;

    /******************************************************************/

    //parse each line of the input trace file
    while(std::getline(input_file, trace)) {
        auto [activity, duration_intr] = parse_trace(trace);

        /******************ADD YOUR SIMULATION CODE HERE*************************/

        if (activity == "CPU") {
            execution += std::to_string(current_time) + ", " + std::to_string(duration_intr) + ", CPU\n";
            current_time += duration_intr;
        }
        else if (activity == "SYSCALL") {
            int device_num = duration_intr;
            execution += std::to_string(current_time) + ", " + std::to_string(1) + ", SYSCALL " + std::to_string(device_num) + "\n";
            current_time += 1;
        }
        else if (activity == "END_IO") {
            int device_num = duration_intr;  
            
            auto [intr_execution, updated_time] = intr_boilerplate(current_time, device_num, context_SR_time, vectors);
            
            execution += intr_execution;
            current_time = updated_time;
            
            int device_io_time = delays[device_num];
            execution += std::to_string(current_time) + ", " + std::to_string(device_io_time) + ", call device driver\n";
            current_time += device_io_time;
        
            execution += std::to_string(current_time) + ", " + std::to_string(1) + ", IRET\n";
            current_time += 1;
        }

        /************************************************************************/

    }

    input_file.close();

    write_output(execution);

    return 0;
}

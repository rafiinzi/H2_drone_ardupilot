#include "Copter.h"

int serial_port_TELEM2          = 2;
char flow_sensor_1_data[]       = "00000";
float flow_sensor_1_actual_data = 0.0;

float sensor_data_2 = 2.59;
float sensor_data_3 = 3.78;

#ifdef USERHOOK_INIT
void Copter::userhook_init()
{
    // put your initialisation code here
    // this will be called once at start-up
    hal.serial(serial_port_TELEM2)->begin(115200);         // TELEM2 - port Pixhawk CubeOrangePlus

    // SERIAL1_ - TELEM1
    // SERIAL2_ - TELEM2
    // SERIAL3_ - GPS1
    // SERIAL4_ - GPS2

}
#endif

#ifdef USERHOOK_FASTLOOP
void Copter::userhook_FastLoop()
{
    // put your 100Hz code here
    // hal.console->printf("Hello World from Mahesh \n");


        bool receiving_data = false;
        int index           = 0;
        char startChar      = '#';
        char endChar        = '/';
        bool new_data       = false;

        while (hal.serial(serial_port_TELEM2)->available()>0 && new_data == false)
        {
            char temp = hal.serial(serial_port_TELEM2)->read();
            // hal.console->printf("serial data -> %c\n",temp);
            if (receiving_data == true)
            {
                if (temp != endChar)
                {
                    flow_sensor_1_data[index] = temp;
                    index++;
                }
                else
                {
                    // hal.console->printf("Index number -> %d\n",index);
                    flow_sensor_1_data[index] = '\0';
                    receiving_data = false;
                    new_data = false;
                    index = 0;
                }
            }
            else if (temp == startChar)
            {
                receiving_data = true;
                index = 0; 
            }
        }

        // hal.console->printf("Flow sensor 1 data -> %s\n",flow_sensor_1_data);

        int flow_sensor_1_data_int       = atoi(flow_sensor_1_data);        // To convert array in integer
        flow_sensor_1_actual_data         =  (float)((flow_sensor_1_data_int  - 50000.0) / 100.0);         // 

        hal.console->printf("%f\n",flow_sensor_1_actual_data);


        Custom_data_logger_example();
}
#endif

#ifdef USERHOOK_50HZLOOP
void Copter::userhook_50Hz()
{
    // put your 50Hz code here
}
#endif

#ifdef USERHOOK_MEDIUMLOOP
void Copter::userhook_MediumLoop()
{
    // put your 10Hz code here
}
#endif

#ifdef USERHOOK_SLOWLOOP
void Copter::userhook_SlowLoop()
{
    // put your 3.3Hz code here
}
#endif

#ifdef USERHOOK_SUPERSLOWLOOP
void Copter::userhook_SuperSlowLoop()
{
    // put your 1Hz code here


}
#endif

#ifdef USERHOOK_AUXSWITCH
void Copter::userhook_auxSwitch1(const RC_Channel::AuxSwitchPos ch_flag)
{
    // put your aux switch #1 handler here (CHx_OPT = 47)
}

void Copter::userhook_auxSwitch2(const RC_Channel::AuxSwitchPos ch_flag)
{
    // put your aux switch #2 handler here (CHx_OPT = 48)
}

void Copter::userhook_auxSwitch3(const RC_Channel::AuxSwitchPos ch_flag)
{
    // put your aux switch #3 handler here (CHx_OPT = 49)
}
#endif


void Copter::Custom_data_logger_example()
{
    struct log_custom_func pkt = {
    LOG_PACKET_HEADER_INIT(LOG_CUSTOM_DATA_MSG),
    time_us  : AP_HAL::micros64(),
    data1    : flow_sensor_1_actual_data,
    data2    : sensor_data_2,
    data3    : sensor_data_3,
    };
    logger.WriteBlock(&pkt, sizeof(pkt));
}


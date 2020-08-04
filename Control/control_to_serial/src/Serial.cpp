#include <ros/ros.h>

#include <serial/serial.h>

#include <std_msgs/String.h>

#include <std_msgs/Empty.h>

#include <std_msgs/Float32.h>

serial::Serial ser;

int i;

int j;

uint8_t receive_data[1000];

int receive_data_cnt = 0;

// recevie_data_value

uint8_t control_mode_r;

uint8_t e_stop_r;

uint8_t gear_r;

uint16_t speed_r;

int16_t steer_r;

uint8_t brake_r;

int enc_r;

/////////////////////////

// transmit_data_value

uint8_t control_mode_t = 1;

uint8_t e_stop_t = 0;

uint8_t gear_t = 0;

uint16_t speed_t = 0;

int16_t steer_t = 0;

uint8_t brake_t = 1;

uint8_t alive_t = 0;

int16_t del_speed = 0;       // test
int16_t delta = 0;       // test
int16_t speed = 0;       // test

// steer value subscribe callback function

void steer_callback(const std_msgs::Float32::ConstPtr& float_msgs)

{

        del_speed = (float_msgs->data);

        if(del_speed > 100)
        {
                delta = del_speed - 3000;
                speed = (del_speed - delta)/1000;
        }
        else
        {
                delta = del_speed;
                speed = 0;
        }

        steer_t = delta * 71;
        speed_t = speed * 10;

        //  (oneborn)  steer_t = (float_msgs->data * 71);

        //if angle is reverse use under command instead of up command

        // steer_t = -(float_msgs -> data * 71);

}

int main(int argc, char** argv) {

        ros::init(argc, argv, "Serial");

        ros::NodeHandle n;

        // steer value subscribe

        ros::Subscriber steering_sub = n.subscribe<std_msgs::Float32>("delta", 1, steer_callback);

        try

        {

                ser.setPort("/dev/ttyUSB1");  //"/dev/ttyUSB0"

                ser.setBaudrate(115200);

                serial::Timeout to = serial::Timeout::simpleTimeout(1000);

                ser.setTimeout(to);

                ser.open();

        }

        catch (serial::IOException& e)

        {

                ROS_ERROR_STREAM("Unable to open port ");

                return -1;

        }

        if (ser.isOpen()) {

                ROS_INFO_STREAM("Serial Port initialized");

        }
        else {

                return -1;

        }

        ros::Rate loop_rate(50);

        while (ros::ok()) {

                ros::spinOnce();



                if (ser.available())

                {

                        // ERP42_data_receive_start-----------------------------------------------------------------------------------------------------

                        std_msgs::String result;

                        result.data = ser.read(ser.available());

                        for (i = 0; i < sizeof(result.data); i++)

                        {

                                receive_data[i + receive_data_cnt] = uint8_t(result.data[i]);

                        }

                        receive_data_cnt += i;

                        if (receive_data_cnt >= 18)

                        {

                                for (i = 0; i < receive_data_cnt - 3; i++)

                                {

                                        if (receive_data[i] == 0x53 & receive_data[i + 1] == 0x54 & receive_data[i + 2] == 0x58)

                                        {

                                                if (receive_data_cnt > i + 17)

                                                {

                                                        if (receive_data[i + 16] == 0x0D & receive_data[i + 17] == 0x0A)

                                                        {

                                                                // system("clear");

                                                                // ROS_INFO_STREAM("DATA_RECEIVED~!!!!!!!!!!!!!!!!!");

                                                                receive_data_cnt = 0;



                                                                control_mode_r = receive_data[i + 3];

                                                                e_stop_r = receive_data[i + 4];

                                                                gear_r = receive_data[i + 5];

                                                                speed_r = receive_data[i + 7] * 256 + receive_data[i + 6];

                                                                steer_r = receive_data[i + 9] * 256 + receive_data[i + 8];

                                                                brake_r = receive_data[i + 10];

                                                                enc_r = (receive_data[i + 14] << 24) + (receive_data[i + 13] << 16) + (receive_data[i + 12] << 8) + receive_data[i + 11];

                                                                // printf("control_mode : %d | e_stop : %d | gear_r : %d | speed_r : %d | steer_r : %d |brake_r : %d | enc_r : %d", control_mode_r, e_stop_r, gear_r, speed_r, steer_r, brake_r, enc_r);

                                                                for (i = 0; i < receive_data_cnt; i++)

                                                                {

                                                                        receive_data[i] = 0;

                                                                }

                                                                break;

                                                        }

                                                }

                                        }

                                }

                        }

                }

                // ERP42_data_receive_end-----------------------------------------------------------------------------------------------------

                control_mode_t = 1;

                e_stop_t = 0;

                gear_t = 0;

                // speed_value ex)3km/h = 30,  5km/h = 50

                speed_t = speed_t;

                steer_t = steer_t;

                brake_t = 1;



                uint8_t transmit[14];

                transmit[0] = 'S';

                transmit[1] = 'T';

                transmit[2] = 'X';

                transmit[3] = control_mode_t;

                transmit[4] = e_stop_t;

                transmit[5] = gear_t;

                transmit[6] = speed_t / 256;

                transmit[7] = speed_t % 256;

                transmit[8] = steer_t / 256;

                transmit[9] = steer_t % 256;

                transmit[10] = brake_t;

                transmit[11] = alive_t;

                transmit[12] = 0x0D;

                transmit[13] = 0x0A;

                ser.write(transmit, 14);

                alive_t++;

                alive_t %= 256;



                for (i = 0; i < 14; i++)

                {

                        printf("| %x |", transmit[i]);

                }

                printf("\n");

        }

        loop_rate.sleep();

}

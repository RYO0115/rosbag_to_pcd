#include <ros/ros.h>
#include <rosbag/bag.h>
#include <rosbag/view.h>
#include <sensor_msgs/PointCloud2.h>

#include <pcl/io/pcd_io.h>
#include <pcl/point_types.h>
#include <pcl_conversions/pcl_conversions.h>

#include <dirent.h>
#include <string.h>
#include <iostream>

int GetInputFileLists(std::string input_dir, std::vector<std::string> &filenames)
{
    std::cout<<"GetInputFileLists from :"<<input_dir<<std::endl;
    int file_num = 0;
    std::string filename;

    DIR *dp;
    dp = opendir(input_dir.c_str());

    dirent* entry = readdir(dp);    // .
    std::cout<<"Entry:"<<entry->d_name<<std::endl;
    entry = readdir(dp);            // ..
    std::cout<<"Entry:"<<entry->d_name<<std::endl;
    entry = readdir(dp);

    while(entry != NULL)
    {
        std::cout<<"Entry:"<<entry->d_name<<std::endl;
        filename = entry->d_name;
        std::cout<<filename<<std::endl;
        filename = input_dir + filename;
        filenames.push_back(filename);
        file_num++;
        entry = readdir(dp);
    }

    std::cout<<"Input File Num:"<<file_num<<std::endl;
    return(file_num);
}

int main(int argc, char** argv)
{
    std::cout<<"start"<<std::endl;
    //std::vector<std::string> topics;
    std::string target_topic;

    std::string input_dir = "./input/";
    std::string output_dir = "./output/";

    if(argc < 2)
    {
        printf("No topic selected !! rosrun rosbag_to_pcd rosbag_to_ascii_pcd {topic_name}\n");
        return(-1);
    }

    else if(argc > 3)
    {
        printf("Too much topics!! (Only 1 topic can be selected)\n");
        return(-2);
    }

    target_topic = argv[1];


    std::cout<<target_topic<<std::endl;

    std::vector<std::string> bagfile_names;

    GetInputFileLists( input_dir, bagfile_names);

    std::string bagfile_name;
    std::string output_filename;
    rosbag::Bag bag;
    std::string msg_topic;
    ros::Time msg_timestamp;
    std::string msg_timestamp_sec, msg_timestamp_nsec;
    pcl::PointCloud<pcl::PointXYZINormal> cloud;

    for(int i=0; i<bagfile_names.size(); i++)
    {
        bagfile_name = bagfile_names[i];
        std::cout<<"Bagfile:"<<bagfile_name<<std::endl;
        bag.open(bagfile_name);
        for(rosbag::MessageInstance const m: rosbag::View(bag))
        {
            msg_topic = m.getTopic();
            msg_timestamp = m.getTime();
            msg_timestamp_sec = std::to_string(msg_timestamp.sec);
            msg_timestamp_nsec = std::string(std::max(0,10-(int)msg_timestamp_sec.size()), '0')
                                 + msg_timestamp_sec;
            msg_timestamp_nsec = std::to_string(msg_timestamp.nsec);
            msg_timestamp_nsec = msg_timestamp_nsec + std::string(std::max(0,9-(int)msg_timestamp_nsec.size()), '0');


            sensor_msgs::PointCloud2::ConstPtr p_pc = m.instantiate<sensor_msgs::PointCloud2>();
            if(p_pc!=NULL)
            {
                if(msg_topic == target_topic)
                {
                    pcl::fromROSMsg( *p_pc, cloud);
                    output_filename = output_dir
                                 + msg_timestamp_sec
                                 + "_"
                                 + msg_timestamp_nsec
                                 + ".pcd";
                    pcl::io::savePCDFileASCII(output_filename, cloud);
                }
            }
        }

        bag.close();

    }


    

    return(0);
}
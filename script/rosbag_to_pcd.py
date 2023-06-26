import rosbag
import glob, os
import sys

def SearchBagFiles(input_dir):
    file_list = []

    file_list = glob.glob(input_dir + "/*.bag")

    return file_list

def CreateOutputDir(file_name):
    output_dirname = "./output/" + os.path.basename(file_name)
    os.makedirs(output_dirname)





if __name__ == '__main__':

    target_topic = sys.argv[0]

    print("Target Topic:", target_topic )

    try:
        input_bagfiles = SearchBagFiles("./input")

        for bagfilename in input_bagfiles:
            CreateOutputDir(bagfilename)
            bag = rosbag.Bag(bagfilename, 'r')

            for topic, msg, ts in bag.read_messages():
                if topic == target_topic:
                    print(msg)





    except Exception as e:
        print("Service call faile: %s" % str(e) )

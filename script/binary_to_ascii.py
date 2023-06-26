import subprocess
import glob, os
import sys

def SearchPcdFiles(input_dir):
    file_list = []

    file_list = glob.glob(input_dir + "/*.pcd")

    return file_list


if __name__ == '__main__':
    pcd_files = SearchPcdFiles('./input')

    #for pcd_filename in pcd_files:
    #    subprocess.run(["pcl_convert_pcd_ascii_binary", pcd_filename, "./output/"+pcd_filename, "0"])
    print(pcd_files[0])
    subprocess.run(["pcl_convert_pcd_ascii_binary", pcd_files[0], "./output/"+pcd_files[0], "0"])
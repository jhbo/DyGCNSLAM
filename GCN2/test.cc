#include <torch/script.h> // One-stop header.
#include <torch/torch.h>

#include <iostream>
#include <algorithm>
#include <fstream>
#include <chrono>
#include <unistd.h>
#include <opencv2/core/core.hpp>


#include "MaskNet.h"
#include "Geometry.h"
#include <System.h>

using namespace std;


void LoadImages(const string &strAssociationFilename, vector<string> &vstrImageFilenamesRGB,
                vector<string> &vstrImageFilenamesD, vector<double> &vTimestamps);

int main(int argc, char **argv)
{
    if(argc != 5 && argc != 6 && argc != 7)
    {
        cerr << endl << "Usage: ./rgbd_gcn path_to_vocabulary path_to_settings path_to_sequence path_to_association (path_to_masks) (path_to_output)" << endl;
        return 1;
    }

    // Retrieve paths to images
    vector<string> vstrImageFilenamesRGB;
    vector<string> vstrImageFilenamesD;
    vector<double> vTimestamps;
    string strAssociationFilename = string(argv[4]);
    LoadImages(strAssociationFilename, vstrImageFilenamesRGB, vstrImageFilenamesD, vTimestamps);

    // Check consistency in the number of images and depthmaps
    int nImages = vstrImageFilenamesRGB.size();
    std::cout << "nImages: " << nImages << std::endl;

    if(vstrImageFilenamesRGB.empty())
    {
        cerr << endl << "No images found in provided path." << endl;
        return 1;
    }
    else if(vstrImageFilenamesD.size()!=vstrImageFilenamesRGB.size())
    {
        cerr << endl << "Different number of images for rgb and depth." << endl;
        return 1;
    }

    // Initialize Mask R-CNN
    DynaSLAM::SegmentDynObject *MaskNet;
    if (argc==6 || argc==7)
    {
        cout << "Loading Mask R-CNN. This could take a while..." << endl;
        MaskNet = new DynaSLAM::SegmentDynObject();
        cout << "Mask R-CNN loaded!" << endl;
    }

    cout << "successful......" << endl;


}

void LoadImages(const string &strAssociationFilename, vector<string> &vstrImageFilenamesRGB,
                vector<string> &vstrImageFilenamesD, vector<double> &vTimestamps)
{
    ifstream fAssociation;
    fAssociation.open(strAssociationFilename.c_str());
    while(!fAssociation.eof())
    {
        string s;
        getline(fAssociation,s);
        if(!s.empty())
        {
            stringstream ss;
            ss << s;
            double t;
            string sRGB, sD;
            ss >> t;
            vTimestamps.push_back(t);
            ss >> sRGB;
            vstrImageFilenamesRGB.push_back(sRGB);
            ss >> t;
            ss >> sD;
            vstrImageFilenamesD.push_back(sD);

        }
    }
}
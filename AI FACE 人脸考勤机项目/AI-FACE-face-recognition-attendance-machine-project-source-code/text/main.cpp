#include <iostream>
#include "opencv2/opencv.hpp"
#include "face.h"

using namespace std;
using namespace cv;
using namespace aip;

int main()
{
    VideoCapture cap(0); // open the default camera
    if(!cap.isOpened())  // check if we succeeded
    {
        cout << "Camera open failed!" << endl;
        return -1;
    }
    cout << "Camera open success!" << endl;

    Mat img;
    Mat grayImag;
    Mat equalizeImag;
    
    CascadeClassifier classifier("/usr/share/opencv/haarcascades/haarcascade_frontalface_alt2.xml");
    
    vector<Rect> allFace;
    Mat faceImag;
    vector<uchar> jpgBuf;
    
    std::string app_id = "45521198";
    std::string api_key = "a8mlqV8sCQovr5y91RQnA26L";
    std::string secret_key = "bhfubB34VnkFM7s46qwX5bRKaAhgGEt8";
    
    aip::Face client(app_id, api_key, secret_key);
    
    std::string base64Imag;
    
    Json::Value result;
    
    time_t sec;
    
    for(;;)
    {
        
        cap >> img; // get a new frame(img) from camera
        cvtColor(img,grayImag,CV_BGR2GRAY);
        equalizeHist(grayImag,equalizeImag);
        classifier.detectMultiScale(equalizeImag, allFace);
         
        if(allFace.size())
        {
            rectangle(equalizeImag,allFace[0],Scalar(255,255,255)); // Scalar(R,G,B);allFace[0]:first Imag;
            faceImag = equalizeImag(allFace[0]);
            imencode(".jpg",faceImag,jpgBuf);
            
            base64Imag = base64_encode((char *)jpgBuf.data(), jpgBuf.size());
            
            result = client.search(base64Imag, "BASE64", "Teacher", aip::null);
            
            if( !result["result"].isNull() )
            {
                if(result["result"]["user_list"][0]["score"].asInt() > 80)
                {
                    cout << result["result"]["user_list"][0]["user_id"] << endl;
                    sec = time(NULL);  // time from 1970.1.1 0:0:0 to now
                    //ctime(&sec);     // change time's more
                    cout << ctime(&sec) << endl;
                    putText(equalizeImag, result["result"]["user_list"][0]["user_id"].asString(), Point(0,50),
                     FONT_HERSHEY_SIMPLEX, 1.0, Scalar(255,255,255));
                    putText(equalizeImag, ctime(&sec), Point(0,100),FONT_HERSHEY_SIMPLEX, 1.0, Scalar(255,255,255));

                }
                /* else
                {
                    cout << "score < 80" << endl;
                } */
            }
            /* else
            {
                cout << "result is null" << endl;
            } */
            
            //cout << result << endl;
        }
        
        imshow("video",equalizeImag);
        waitKey(40); 
    }
    return 0;

}

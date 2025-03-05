#include<iostream>
#include"opencv2/opencv.hpp"
#include"face.h"
using namespace cv;
using namespace std;
using namespace aip;
int main()
{
VideoCapture cap(0);//open default camera
	if(!cap.isOpened())//检查摄像头是否打开
		{cout<<"Camera open failed!"<<endl;
		return -1;
		}
	cout<<"Camera open success!"<<endl;
	CascadeClassifier object("/usr/share/opencv4/haarcascades/haarcascade_frontalface_alt2.xml");
	Mat frame;
	Mat grayimag;
	vector <Rect> AllFace;
	Mat matface;
	vector <uchar> jpgface;
	string base64face;
	Json::Value result;
	  // 设置APPID/AK/SK(这个需要你自己在百度智能云注册并且创建需要对比的库并且导入文件)
    std::string app_id = "";
    std::string api_key = "";
    std::string secret_key = "";
    time_t sec;//时间

    aip::Face client(app_id, api_key, secret_key);
while(1)
{

	cap>>frame;//从摄像头获取图像
	if(frame.empty())
        break;
	cvtColor(frame,grayimag,COLOR_BGR2GRAY);//转换为灰度降低图片信息	   
	equalizeHist(grayimag,grayimag);//均衡化
	object.detectMultiScale(grayimag,AllFace);
	if (AllFace.size())
	{
		rectangle(grayimag,AllFace[0],Scalar(255,255,255));
		matface=grayimag(AllFace[0]);
		imencode(".jpg",matface,jpgface);
		base64face=base64_encode((char *)jpgface.data(),jpgface.size());//把截图转化为base64格式
		result = client.face_search_v3(base64face, "BASE64", "", aip::json_null);//“”中需要输入创建的组的名称
		if(!result["result"].isNull())
		{
		
		if(result["result"]["user_list"][0]["score"].asInt() > 80)//匹配度大于百分之八十才输出
		{
		cout<<result["result"]["user_list"][0]["user_id"]<<endl;
		sec=time(NULL);
		cout << ctime(&sec) <<endl;
		putText(grayimag,result["result"]["user_list"][0]["user_id"].asString(),Point(0,50), FONT_HERSHEY_COMPLEX,3,Scalar(0,0,255));
		putText(grayimag,ctime(&sec),Point(0,150),FONT_HERSHEY_SCRIPT_SIMPLEX,1,Scalar(0,0,255));//图像上输出信息
		}
		
		
		
		}
		
		
		
		
		
		
	}
	imshow("video",grayimag);
	if(waitKey(30)==27)
	break;
}

return 0;




}

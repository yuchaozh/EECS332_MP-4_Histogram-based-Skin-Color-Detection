#include <iostream>
#include <fstream>
#include "cv.h"
#include "highgui.h"
using namespace std;
using namespace cv;
 
//色相饱和度直方图
void CalcHistHs() 
{
		IplImage* img_source;
		IplImage* HImg= NULL;
		IplImage* SImg= NULL;
		IplImage* VImg= NULL;
		IplImage* result= NULL;
		IplImage* result2= NULL;
		
 		//ofstream outfile;
		//outfile.open("output.txt");
		int sum = 0;
 
     if (img_source = cvLoadImage("images.jpeg",1))
	//if (img_source = cvLoadImage("gun1.bmp",1))
      //if (img_source = cvLoadImage("pointer1.bmp",1))
		//if (img_source = cvLoadImage("joy1.bmp",1))
		{
        IplImage* hsv = cvCreateImage(cvGetSize(img_source),8,3);
		CvSize SrcSize = cvGetSize(img_source);
		HImg= cvCreateImage(SrcSize,8,1);  
		SImg= cvCreateImage(SrcSize,8,1);  
		VImg= cvCreateImage(SrcSize,8,1);  
		result= cvCreateImage(SrcSize,8,3);
		result2= cvCreateImage(SrcSize,8,3);
		cout<<"height:  "<<SrcSize.height<<"width:  "<<SrcSize.width<<endl;
        //rgb色彩空间转换到hsv色彩空间
        cvCvtColor(img_source,hsv,CV_BGR2HSV);
		cvNamedWindow( "hsv", 1 );
        cvShowImage( "hsv", hsv );
		//cvCvtColor(hsv,hsv,CV_HSV2RGB);

        //cvNamedWindow( "hsvBACK", 1 );
        //cvShowImage( "hsvBACK", hsv );
		cvSplit(hsv,HImg,SImg,VImg,NULL);
		//img_source->imageData

		int h;
		for (int i = 0; i< img_source->height-1; i++)
		{
			for (int a = 0; a< img_source->width-1; a++)
			{
				h=cvGetReal2D(HImg,i,a); // get the (j,i) pixel value
				//outfile<<"h:  "<<h<<endl;
				if (h >=0 && h <= 25)   
                  {     //*(HImg->imageData+i*HImg->widthStep+a) = 255;    
				}    
                else    {     *(HImg->imageData+i*HImg->widthStep+a) = 0;    }   
			}
		}

		int s;
		for (int i = 0; i< img_source->height-1; i++)
		{
			for (int a = 0; a< img_source->width-1; a++)
			{
				s=cvGetReal2D(SImg,i,a); // get the (j,i) pixel value
				//outfile<<"s:  "<<s<<endl;
				if (s >=26 && s <= 200)   
                  {    // *(SImg->imageData+i*SImg->widthStep+a) = 255;    
				}    
                else    {     *(SImg->imageData+i*SImg->widthStep+a) = 0;    }   
			}
		}

		int v;
		for (int i = 0; i< img_source->height-1; i++)
		{
			for (int a = 0; a< img_source->width-1; a++)
			{
				v=cvGetReal2D(VImg,i,a); // get the (j,i) pixel value
				//outfile<<"v:  "<<v<<endl;
				if (v >=20 && v <= 200)   
                  {    // *(VImg->imageData+i*VImg->widthStep+a) = 255;    
				}    
                else    {     //*(VImg->imageData+i*VImg->widthStep+a) = 0;    
				}   
			}
		}
		 //cvAnd(HImg,SImg,result,0);  
		//cvAnd(VImg,result,result,0);
		cvMerge(HImg,SImg,VImg,NULL, result);
		cvNamedWindow( "segementation", 1 );
		cvShowImage( "segementation", result );

        IplImage* h_plane = cvCreateImage(cvGetSize(img_source),8,1);
        IplImage* s_plane = cvCreateImage(cvGetSize(img_source),8,1);
        IplImage* v_plane = cvCreateImage(cvGetSize(img_source),8,1);
  
        IplImage* planes[] ={h_plane,s_plane};
        //分割为单通道图像
        cvCvtPixToPlane(hsv,h_plane,s_plane,v_plane,0);
        cvNamedWindow( "h_plane", 1 );
        cvShowImage( "h_plane", h_plane );
        cvNamedWindow( "s_plane", 1 );
        cvShowImage( "s_plane", s_plane );
        cvNamedWindow( "v_plane", 1 );
        cvShowImage( "v_plane", v_plane );
         cvNamedWindow( "Source", 1 );
       cvShowImage( "Source", img_source );
  
        //build the histogram and compute its contents
  
        /** H 分量划分为30个等级，S分量划分为32个等级 */
        int h_bins =32, s_bins = 30;
  
        CvHistogram* hist;
  
        {
            int hist_size[] = {h_bins,s_bins};
            float h_ranges[] = {0,180}; /* hue varies from 0 (~0°red) to 180 (~360°red again) */
  
            float s_ranges[] = {0,255};  /* saturation varies from 0 (black-gray-white) to 255 (pure spectrum color) */
  
            float* ranges[] = {h_ranges,s_ranges};
  
            /** 创建直方图，二维, 每个维度上均分 */
            hist = cvCreateHist(
                2, // int dims 
                hist_size, // int* sizes 
                CV_HIST_ARRAY, // int type
               ranges, // float** ranges
                1       //uniform 
                );
        }
		cout<<hist->bins<<endl;
		cout<<hist->thresh<<endl;
		cout<<hist->thresh2<<endl;
		cout<<hist->type<<endl;
        //  根据H,S两个平面数据统计直方图 
        cvCalcHist( planes, hist, 0, 0 );
        //归一化处理 
        //cvNormalizeHist(hist,1.0);
        /** 获取直方图统计的最大值，用于动态显示直方图 */
        float max_value;
		float min_value;
		int maxindex;
		int minindex;
        cvGetMinMaxHistValue( hist, &min_value, &max_value, 0, 0);
        int scale=10;
        //创建直方图图像
        /** 设置直方图显示图像 */
        int height = 300;
        int width = (h_bins*s_bins);
        IplImage* hist_img = cvCreateImage( cvSize(width,height), 8, 3 );
        cvZero( hist_img );
  

        /** 用来进行HSV到RGB颜色转换的临时单位图像 */
        IplImage * hsv_color = cvCreateImage(cvSize(1,1),8,3);
        IplImage * rgb_color = cvCreateImage(cvSize(1,1),8,3);
        int bin_w = width / (h_bins * s_bins);
        for(int h = 0; h < h_bins; h++)
        {
            for(int s = 0; s < s_bins; s++)
            {
                int i = h*s_bins + s;
               /** 获得直方图中的统计次数，计算显示在图像中的高度 */
                float bin_val = cvQueryHistValue_2D( hist, h, s );
				//outfile<<"h:   "<<h<<"   s:   "<<s<<"   bin_value:   "<<bin_val<<endl;
                int intensity = cvRound(bin_val*height/max_value);
  
                /** 获得当前直方图代表的颜色，转换成RGB用于绘制 */
                cvSet2D(hsv_color,0,0,cvScalar(h*180.f / h_bins,s*255.f/s_bins,255,0));
                cvCvtColor(hsv_color,rgb_color,CV_HSV2BGR);
               CvScalar color = cvGet2D(rgb_color,0,0);
 
               cvRectangle( hist_img, cvPoint(i*bin_w,height),
                   cvPoint((i+1)*bin_w,height - intensity),
                   color, -1, 8, 0 );
           }
			
		}
		CvScalar x;
		CvScalar z;
		for (int i = 0; i< img_source->height-1; i++)
		{
			for (int a = 0; a< img_source->width-1; a++)
			{
				x=cvGet2D(hsv,i,a); // get the (j,i) pixel value
				z = cvGet2D(img_source,i,a);
				//printf("B=%f, G=%f, R=%f\n",s.val[0],s.val[1],s.val[2]);
				//outfile<<"before:  "<<endl;
				//outfile<<"H:   "<<x.val[0]<<"  S:  "<<x.val[1]<<"  V:  "<<x.val[2]<<endl;
				if ((0<=x.val[0])&&(x.val[0]<=25))
				{
				}
					else
					{
						z.val[0]=0;
						z.val[1]=0;
						z.val[2]=0;
						//outfile<<"after"<<endl;
						//outfile<<"H:   "<<x.val[0]<<"  S:  "<<x.val[1]<<"  V:  "<<x.val[2]<<endl;
						cvSet2D(img_source,i,a,z);
				}
				
			}
       }
		//outfile.close();
		cvNamedWindow( "afterchange", 1 );
       cvShowImage( "afterchange", img_source );
        cvNamedWindow( "H-S Histogram", 1 );
       cvShowImage( "H-S Histogram", hist_img );
 
     }
  }
 

int main()
{
    CalcHistHs();
    cvWaitKey(0);
}

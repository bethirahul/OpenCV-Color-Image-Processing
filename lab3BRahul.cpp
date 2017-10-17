// Computer Vision and Image Processing - Assignment 3
// Rahul Bethi
// A03972041
// rbethi@islander.tamucc.edu
// OpenCV 2.4.9 | Putty 0.65 | WinSCP 575 | Xming 6.9.0.13 | Notepad++ 6.8.3 | Windows 10 64bit

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#define THRESHOLD 16	// Threshold for the grey intensity value of the river.

using namespace cv;
using namespace std;

int pseudoColorIP();				// This is the 1st task of the assignment
int histogramIP( int );				// This is the second task of the assignment
int showHistogram( Mat[3], int );	// This function calculates and draws histograph

int exit_flag = 0;					// This flag is used to retain in the program after the execution of a task.

int main()	// Main Program
{
	int choice = 0;
	
	while(exit_flag == 0)
	{
		choice = 0;
		cout << "\n<<     1 - Pseudo-Color Image Processing\n";
		cout << "<< ** Color Image Enhancement by Histogram Processing :-\n";
		cout << "<<     2 - Show Histogram of RGB channels of original image\n";
		cout << "<<     3 - Show Blue  channel of Histogram equalized image\n";
		cout << "<<     4 - Show Green channel of Histogram equalized image\n";
		cout << "<<     5 - Show Red   channel of Histogram equalized image\n";
		cout << "<<     6 - Show Histogram equalization of the image\n";
		cout << "<<     7 - Show Histogram of RGB channels of the image after equalization\n";
		cout << "<< ** Any other option to exit\n\n";
		cout << "<< Enter your choice: ";
		cin >> choice;									// Input saved inot interger variable choice
		
		if	   (choice == 1)				pseudoColorIP();			// functions are called according to the choice chosen.
		else if(choice >= 2 && choice <= 7) histogramIP( choice );
		else
		{
			cout << "\n>> Error: Invalid input - Defalut action - Exit\n\n";
			exit_flag = 1;		// Exit flag is set to 1 to exit the program if anyone other input is given other than the options given.
		}
	}
	return 0;
}

int pseudoColorIP()
{
	Mat in_img, out_img;
	int r, c, i[8], j;
	
	in_img = imread("Fig0627(d)(WashingtonDC Band4).TIF", CV_LOAD_IMAGE_COLOR);		// Tast 1 - Washington DC river
	if(in_img.empty())
	{
		cout << "\n>> Error : Image cannot be loaded..!!\n";
		return -1;
	}
	
	out_img = in_img.clone();	// output is cloned because, only the river color is changed to blue, rest all are the same.
	
	for(c = 0; c<in_img.cols; c++)
	{
		for(r = 0; r<in_img.rows; r++)
		{
			if(in_img.at<Vec3b>(r,c)[0] < THRESHOLD)
			{
				if( r == 0 || r == (in_img.rows - 1) || c == 0 || c == (in_img.cols - 1) )		// pixels which are in the border of the image and are black.
				{
					out_img.at<Vec3b>(r,c) = Vec3b(255,0,0);
				}
				else
				{
					for(j=0; j<8; j++) i[j] = 0;							// All the pixels around the desired pixel are taken and their intensities are checked.
					if(in_img.at<Vec3b>(r-1,c-1)[0] < THRESHOLD) i[0]++;	// If the pixel is surrounded by more than 3 pixels side by side, those pixels are chosen.
					if(in_img.at<Vec3b>(r-1,c  )[0] < THRESHOLD) i[1]++;	// because it forms a group of 4 pixels all together.
					if(in_img.at<Vec3b>(r-1,c+1)[0] < THRESHOLD) i[2]++;	// here we are counting how many oixels are dark around our desired pixel.
					if(in_img.at<Vec3b>(r  ,c-1)[0] < THRESHOLD) i[7]++;
					if(in_img.at<Vec3b>(r  ,c+1)[0] < THRESHOLD) i[3]++;
					if(in_img.at<Vec3b>(r+1,c-1)[0] < THRESHOLD) i[6]++;
					if(in_img.at<Vec3b>(r+1,c  )[0] < THRESHOLD) i[5]++;
					if(in_img.at<Vec3b>(r+1,c+1)[0] < THRESHOLD) i[4]++;
					
					if(i[0] == 1 && i[7] == 1 && i[6] == 1)      out_img.at<Vec3b>(r,c) = Vec3b(255,0,0); 
					else if(i[1] == 1 && i[0] == 1 && i[7] == 1) out_img.at<Vec3b>(r,c) = Vec3b(255,0,0);
					else
					{
						for(j=2; j<8; j++)
						{
							if(i[j] == 1 && i[j+1] == 1 && i[j+2] == 1)
							{
								out_img.at<Vec3b>(r,c) = Vec3b(255,0,0);	// here we are assigning the blue color to the water.
								j = 8;
							}
						}
					}
				}
			}
		}
	}
	
	imwrite("washington_river.png", out_img);  // Output of this section is stored in this file.
	
	namedWindow("OutputImage",CV_WINDOW_AUTOSIZE);
	startWindowThread();
	imshow("OutputImage", out_img);		// Output of this section is shown in this window.
	waitKey(0);
	destroyWindow("OutputImage");
	
	return 0;
}

int histogramIP( int p_choice )		// 2nd Task
{
	Mat in_img, out_img, rgb_channel[3], histImage, tmp_img[3];
	int i, r, c;
	
	in_img = imread("Fig0635(bottom_left_stream).tif", CV_LOAD_IMAGE_COLOR);	// Task two - dark image
	if(in_img.empty())
	{
		cout << "\n>> Error : Image cannot be loaded..!!\n";
		return -1;
	}
	
	out_img = in_img.clone();
	
	split(in_img, rgb_channel);		// The given image is split into RGB channels and stored in rgb_channel Array.
	
	if(p_choice == 2) showHistogram( rgb_channel, p_choice );		// If option 2 is chosen, then Histogram of original image is shown.
	else
	{
		for(i = 0; i<3; i++) equalizeHist(rgb_channel[i], rgb_channel[i]);		// If option 3 is chosen, then the 3 channels are individually eualized and merged back into single image
		if(p_choice == 7) showHistogram( rgb_channel, p_choice );		// If option 4 is chosen, then Histogram of eualized image is calculated and shown.
		else
		{
			for(i = 0; i<3; i++)
					tmp_img[i] = rgb_channel[i].clone();
				
			if(p_choice == 3)
			{
				/* BLUE */
				for(i = 0; i<3; i++)
					for(r = 0; r<tmp_img[i].rows; r++)
						for(c = 0; c<tmp_img[i].cols; c++)
							tmp_img[i].at<uchar>(r,c) = 0;
				tmp_img[0] = rgb_channel[0].clone();			// To get Blue channel of the Histograph equalised image.
				merge(tmp_img, 3, out_img);
				imwrite("blue_color.png",  out_img);
			}
			else if(p_choice == 4)	
			{
				/* GREEN */
				for(i = 0; i<3; i++)
					for(r = 0; r<tmp_img[i].rows; r++)
						for(c = 0; c<tmp_img[i].cols; c++)
							tmp_img[i].at<uchar>(r,c) = 0;
				tmp_img[1] = rgb_channel[1].clone();			// To get Green channel of the Histograph equalised image.
				merge(tmp_img, 3, out_img);
				imwrite("green_color.png",  out_img);
			}
			else if(p_choice == 5)
			{
				/* RED */
				for(i = 0; i<3; i++)
					for(r = 0; r<tmp_img[i].rows; r++)
						for(c = 0; c<tmp_img[i].cols; c++)
							tmp_img[i].at<uchar>(r,c) = 0;
				tmp_img[2] = rgb_channel[2].clone();			// To get Red channel of the Histograph equalised image.
				merge(tmp_img, 3, out_img);
				imwrite("red_color.png",  out_img);		
			}
			else
			{
				merge(rgb_channel, 3, out_img);			// Merging takes place here
				
				imwrite("histEqual.png", out_img);		// Merged file output is stored with this file name.
			}
			namedWindow("OutputImage",CV_WINDOW_AUTOSIZE);
			startWindowThread();
			imshow("OutputImage", out_img);
			waitKey(0);
			destroyWindow("OutputImage");
		}
	}
	
	return 0;
}

int showHistogram( Mat channel[3], int p2_choice )
{
	int i, j, r, c, iVal[3][256], highVal;
	Point2f start_pt, end_pt;
	
	for(i = 0; i<3; i++)
	{
		for(j = 0; j<256; j++)
			iVal[i][j] = 0;
			
		for(r = 0; r<channel[i].rows; r++)
			for(c = 0; c<channel[i].cols; c++)
				iVal[i][channel[i].at<uchar>(r,c)]++;		// Calculated how many pixels are having that particular intensity
	}
		
	highVal = 0;
	for(i = 0; i<3; i++)
		for(j = 0; j<256; j++)
			if(iVal[i][j] > highVal) highVal = iVal[i][j];		// Highest number of pixels having the same intensity is calculated to know the height of the Histogram window.
	
	Mat out_img(highVal/100, 256*2, CV_8UC3);	// Histogram is scaled down 100 times to fit in the college monitors and gap between levels in doubled to clearly understand.
	
	for(r = 0; r<out_img.rows; r++)
		for(c = 0; c<out_img.cols; c++)
			out_img.at<Vec3b>(r,c) = Vec3b(0,0,0);		// Empty image is created and black color is choosen as background to draw histograms
	
	for(i = 0; i<3; i++)
		for(j = 0; j<255; j++)
		{
			start_pt.x = j*2;									// Starting point and end points are determined for every pixel to draw the graph.
			start_pt.y = ( highVal - iVal[i][j]   ) / 100;
			end_pt.x   = (j+1)*2;
			end_pt.y   = ( highVal - iVal[i][j+1] ) / 100;
			if     (i == 0) line( out_img, start_pt, end_pt, Scalar(255,   0,   0), 1, CV_AA, 0 );	// Respective colors are applied while drawing the histograph for respective color channels.
			else if(i == 1) line( out_img, start_pt, end_pt, Scalar(  0, 255,   0), 1, CV_AA, 0 );
			else		    line( out_img, start_pt, end_pt, Scalar(  0,   0, 255), 1, CV_AA, 0 );
		}
	
	if(p2_choice == 2) imwrite("OrigHist.png", out_img);	// Save the Histographs in the respective files (Original and Equalized).
	else			   imwrite("EqualHist.png",  out_img);
	
	namedWindow("OutputImage",CV_WINDOW_AUTOSIZE);
	startWindowThread();
	imshow("OutputImage", out_img);		// Here the Histographs are shown from the output window.
	waitKey(0);
	destroyWindow("OutputImage");
}
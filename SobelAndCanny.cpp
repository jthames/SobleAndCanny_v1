/*===========================================================================*\
						S O B E L and C A N N Y
\*============================================================================/
							SobelAndCanny.cpp

	Joshua Thames
	8/22/2013
	
	Built in Visual Studio Express 2012
		Warning level = 2

	Description:
	
	Part A
	Modify Sobel to output three images. First output image is a gradient
	image from sobel function. Second output image is sobel with a low 
	threshold. Third image is sobel with a high threshold. 
		
		Status: Done and working properly

	Part B
	This section is the canny algorithm which outputs 3 files. First output
	is the gradient image, the second is the peak pixel values which have 
	been calculated and the final output is result of the whole canny edge
	detection algorithm. 

		Status: Complete but working on automatic HIGH and LOW threshold

	Instructions:
	Simply build/run the program making sure that the file chess.pgm is in the
	same directory as your project. Follow the menu prompts. Sample output 
	images are included.

/*===========================================================================*\
\*===========================================================================*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#define BUFFER_SIZE  70
#define TRUE          1
#define FALSE         0
#define PICSIZE		256
#define MAXMASK		100
#define LOW_THRESH	 35
#define HIGH_THRESH	 110

/*-------------------------[ Global Variables ]------------------------------*/

int    numRows;
int    numCols;
int    maxVal;
int    bright, choice, Value1=0;

/*-------------------------[ Function Declarations ]-------------------------*/

void addtopixels(int** imgtemp, int value);
void  writeoutpic(char* fileName, int** imgtemp);
int** readpic(char* fileName);
void  readHeader(FILE* imgFin);
int   isComment(char* line);
void  readImgID(char* line);
void  readImgSize(char* line);
void  readMaxVal(char* line);
int** setImage();
void  readBinaryData(FILE* imgFin, int** imgtemp);

/*===========================================================================*\
						IMAGE SUPPORT
\*===========================================================================*/

void addtopixels(int** imgtemp, int value)
{
        int i,j;

        for (i=0;i<numRows;i++)
        { for (j=0;j<numCols;j++)
                {
                  imgtemp[i][j] += value;
                }
        }
}

void writeoutpic(char* fileName, int** imgtemp)
{
        int i,j;
        char ci;
        FILE* fo1;

        if((fo1 = fopen(fileName, "wb")) == NULL)
        {
			printf("Unable to open out image file '%s'\n", fileName);
			exit(EXIT_FAILURE);
        }

        fprintf(fo1,"P5\n");
        fprintf(fo1,"%d %d\n", numRows, numCols);
        fprintf(fo1,"255\n");

        for (i=0;i<numRows;i++)
        { for (j=0;j<numCols;j++)
                {
                  ci   =  (char) (imgtemp[i][j]);
                  fprintf(fo1,"%c", ci);
                }
        }
}

void writeoutpic(char* fileName, double** imgtemp)
{
        int i,j;
        char ci;
        FILE* fo1;

        if((fo1 = fopen(fileName, "wb")) == NULL)
        {
			printf("Unable to open out image file '%s'\n", fileName);
			exit(EXIT_FAILURE);
        }

        fprintf(fo1,"P5\n");
        fprintf(fo1,"%d %d\n", numRows, numCols);
        fprintf(fo1,"255\n");

        for (i=0;i<numRows;i++)
        { for (j=0;j<numCols;j++)
                {
                  ci   =  (char) (imgtemp[i][j]);
                  fprintf(fo1,"%c", ci);
                }
        }
}

int** readpic(char* fileName)
{
        FILE* imgFin;
        int** imgtemp;

        if((imgFin = fopen(fileName, "rb")) == NULL)
        {
                printf("Unable to open image file '%s'\n", fileName);
                exit(EXIT_FAILURE);
        }

        readHeader(imgFin);


        imgtemp  = setImage();

        readBinaryData(imgFin, imgtemp);

        fclose(imgFin);

        return  imgtemp;

}

void readHeader(FILE* imgFin)
{
        int  haveReadImgID   = FALSE;
        int  haveReadImgSize = FALSE;
        int  haveReadMaxVal  = FALSE;
        char line[BUFFER_SIZE];

        while(!(haveReadImgID && haveReadImgSize && haveReadMaxVal))
        {
                fgets(line, BUFFER_SIZE, imgFin);

                if((strlen(line) == 0) || (strlen(line) == 1))
                        continue;

                if(isComment(line))
                        continue;

                if(!(haveReadImgID))
                {
                        readImgID(line);
                        haveReadImgID = TRUE;
                }
                else if(!(haveReadImgSize))
                {
                        readImgSize(line);
                        haveReadImgSize = TRUE;
                }
                else if(!(haveReadMaxVal))
                {
                        readMaxVal(line);
                        haveReadMaxVal = TRUE;
                }
        }

}

int isComment(char *line)
{
        if(line[0] == '#')
                return(TRUE);

        return(FALSE);
}

void readImgID(char* line)
{
        if(strcmp(line, "P5\n") != 0)
        {
                printf("Invalid image ID\n");
                exit(EXIT_FAILURE);
        }
}

void readImgSize(char* line)
{
        unsigned i;

        for(i = 0; i < strlen(line); ++i)
        {
                if(!((isdigit((int) line[i])) || (isspace((int) line[i]))))
                {
                        printf("Invalid image size\n");
                        exit(EXIT_FAILURE);
                }
        }

        sscanf(line, "%d %d", &numRows, &numCols);
}

void readMaxVal(char* line)
{
        unsigned i;

        for(i = 0; i < strlen(line); ++i)
        {
                if(!((isdigit((int) line[i])) || (isspace((int) line[i]))))
                {
                        printf("Invalid image max value\n");
                        exit(EXIT_FAILURE);
                }
        }

        maxVal = atoi(line);
}

int** setImage()
{
        int** imgTemp;
        unsigned i;

        imgTemp = (int**) calloc(numRows, sizeof(int*));

        for(i = 0; i < numRows; ++i)
        {
                imgTemp[i] = (int*) calloc(numCols, sizeof(int));
        }

		return imgTemp;
}

double** setDouble()
{
        double** imgTemp;
        unsigned i;

        imgTemp = (double**) calloc(numRows, sizeof(double*));

        for(i = 0; i < numRows; ++i)
        {
                imgTemp[i] = (double*) calloc(numCols, sizeof(double));
        }

		return imgTemp;
}

void readBinaryData(FILE* imgFin, int** imgtemp)
{
        unsigned  i;
        unsigned  j;
        for(i = 0; i < numRows; ++i)
        {
                for(j = 0; j < numCols; ++j)
                {
                            imgtemp[i][j] =
                            fgetc(imgFin);
                }
        }
}

/*===========================================================================*\
						Menu Op 1 - Sobel
\*===========================================================================*/

void sobelThresh(int** pic, int** grad, int** threshLow, int** threshHigh){ 
/*
	Inputs:
		Pic			Input image.
		edges		Container for output.
		tempx		Used to compute the x direction convolution.
		tempy 		Used to compute the y direction convolution.
		
	Outputs:
		grad		At end of function this contains the resultant image.
		thershLow	Output image with low thresholding.
		threshHigh	Output image with high thresholding.
		
*/        
        char ci;

		// These two arrays are the result of convo from 2x2 in x and y direction
        int maskx[3][3] = {{-1,0,1},{-2,0,2},{-1,0,1}};
        int masky[3][3] = {{1,2,1},{0,0,0},{-1,-2,-1}};
        
		int maxival;
        int i,j,p,q,mr,sum1,sum2;
        
        int **tempx = setImage();
        int **tempy = setImage();
             
		mr = 1;

        for (i=mr;i<numRows-mr;i++)
        { for (j=mr;j<numCols-mr;j++)
          {
             sum1 = 0;
             sum2 = 0;
			// Do convo on 3x3 regions.
             for (p=-mr;p<=mr;p++)
             {
                for (q=-mr;q<=mr;q++)
                {
                   sum1 += pic[i+p][j+q] * maskx[p+mr][q+mr];
                   sum2 += pic[i+p][j+q] * masky[p+mr][q+mr];
                }
             }
			 // Save pixel-wise convo in temp images.
             tempx[i][j] = sum1;
             tempy[i][j] = sum2;
          }
        }

		// Calc magnitude from temps and threshold on gradient image.
        maxival = 0;
        for (i=mr;i<numRows-mr;i++)
        { for (j=mr;j<numCols-mr;j++)
          {
			  // Calc the smoothened image
             grad[i][j] = (int) (sqrt((double)((tempx[i][j]*tempx[i][j]) 
						 + (tempy[i][j]*tempy[i][j]))));

			// No threshold.......?
             if (grad[i][j] > maxival)
                maxival = grad[i][j];

           }
        }

		// Smooth(average) gradient image and threshold(make binary) 
		// threshLow and threshHigh output images.
       for (i=0;i<numRows;i++)
          { for (j=0;j<numCols;j++)
            {
				// What is this?
	            grad[i][j] = ((grad[i][j]*1.0) / maxival) * 255;

				if( grad[i][j] > LOW_THRESH )
					threshLow[i][j] = 255;

				if( grad[i][j] > HIGH_THRESH)
					threshHigh[i][j] = 255;
					
            }
          }

}

/*===========================================================================*\
						Menu Op 2 - Canny
\*===========================================================================*/

void computePeaks(double **mag, double **xconv, double **yconv, double **peaks, int MR){
/*
	The function is used to copute non-maxima suppression from vector gradient
	image. 

	Inputs:
		mag		Image which contains the vector gradients

	Output:
		peaks	Resultant image which contains the peaks
*/

	double slope = 0;
	int i, j;

	for(i=MR;i<256-MR;i++){
		for(j=MR;j<256-MR;j++){
    
			if((xconv[i][j]) == 0.0) 
				xconv[i][j] = .00001;
       
			slope = yconv[i][j]/xconv[i][j];

			if( (slope <= .4142)&&(slope > -.4142)){
				if((mag[i][j] > mag[i][j-1])&&(mag[i][j] > mag[i][j+1]))
					peaks[i][j] = 255;
			}
			else if( (slope <= 2.4142)&&(slope > .4142)){
				if((mag[i][j] > mag[i-1][j-1])&&(mag[i][j] > mag[i+1][j+1])){
					peaks[i][j] = 255;
				}
			}
			else if( (slope <= -.4142)&&(slope > -2.4142)){
				if((mag[i][j] > mag[i+1][j-1])&&(mag[i][j] > mag[i-1][j+1])){
					peaks[i][j] = 255;
				}
			}else{
				if((mag[i][j] > mag[i-1][j])&&(mag[i][j] > mag[i+1][j])){
					peaks[i][j] = 255;
				}
			}
		}
	}

}

void Hysteresis(double** peaks, double** mag, int** out, int Hi, int Low){
/*
	This function is called from canny to do the Hysteresis thresholding.
	If value is greater than high thresh hold then pixel goes into output. 
	If value is greater than low threshold AND connected to a high threshold
	pixel then pixel goes into output image. 

	Inputs:
		peaks	Image send in from canny that contains gradient values.

	Output:
		  out	Resultant image that has Hysteresis threshold applied. 

*/
	double **peaksCopy = setDouble();

	bool moretodo = true;

	for(int r=0; r<numRows; r++)
		for(int c=0; c<numCols; c++)
			peaksCopy[r][c] = peaks[r][c];

	for(int r=0; r<numRows; r++){
		for(int c=0; c<numCols; c++){
     
			if( peaksCopy[r][c] ){
				if( mag[r][c] >= Hi){
					peaksCopy[r][c] = 0;
					out[r][c] = 255;
				}
				else if (mag[r][c] < Low){
					peaksCopy[r][c] = 0;
					out[r][c] = 0;
				}
			}
		}
	}

	while (moretodo){

		moretodo = false;

		for(int r=0; r<numRows; r++)
			for(int c=0; c<numCols; c++){

				if( peaksCopy[r][c] ){

					for(int p=-1; p<=1; p++)
						for(int q=-1; q<=1; q++)
							
							if (out[r+p][c+q] ){
								peaksCopy[r][c] = 0;
								out[r][c] = 255;
								moretodo = true;
							}
				}
			}

	}

}

void canny(int **inImg, double **peaks, double **mag, int **cannyOut, double sig, int percent){

	int     i, j, p, q, s, t, mr, centx, centy;
	double  maskval, sum1, sum2, maxival, minival, maxval, ZEROTOL;
	
	double** xmask = setDouble();
	double** ymask = setDouble();
	double** xconv = setDouble();
    double** yconv = setDouble();

	mr = (int)(sig * 3);

	centx = (MAXMASK / 2);
	centy = (MAXMASK / 2);

	// First Derivative 
    for (p=-mr;p<=mr;p++)
    {  for (q=-mr;q<=mr;q++)
        {
		
			maskval = -q*exp(-.5*( (p*p + q*q)/(sig*sig)));
            (xmask[p+centy][q+centx]) = maskval;

			maskval = -p*exp(-.5*( (p*p + q*q)/(sig*sig)));
            (ymask[p+centy][q+centx]) = maskval;
        }
    }

	// Convolution
	for (i=mr; i<=255-mr; i++)
    { for (j=mr;j<=255-mr;j++)
        {
            sum1 = 0;
			sum2 = 0;

            for (p=-mr;p<=mr;p++)
            {
				for (q=-mr;q<=mr;q++)
				{
					sum1 += inImg[i+p][j+q] * xmask[p+centy][q+centx];
					sum2 += inImg[i+p][j+q] * ymask[p+centy][q+centx];
				}
            }

           xconv[i][j] = sum1;
		   yconv[i][j] = sum2;

        }
    }

	// From Sobel
	// Calc magnitude from temps and threshold on gradient image.
    maxival = 0;
    for (i=mr;i<numRows-mr;i++)
    { for (j=mr;j<numCols-mr;j++)
        {
			// Calc the smoothened image
            mag[i][j] = (int) (sqrt((double)((xconv[i][j]*xconv[i][j]) 
						+ (yconv[i][j]*yconv[i][j]))));

			// find largest pixel value
            if (mag[i][j] > maxival)
				maxival = mag[i][j];
        }
    }

	// Scale output
	for(i=0; i<256; i++)
	{ for (j=0; j<256; j++)
		{
			mag[i][j] = (mag[i][j] / maxival) * 255.0;
		}
	}

	// At this point, "mag" contains the gradient vectors
	
	// Calc peaks which will have nice edges with snowy looking noise
	// in the output.
	computePeaks(mag, xconv, yconv, peaks, mr);

	// Compute Hi and Low
	int val, Hi, Low, numPix, sum=0;
	
	numPix = percent * numRows * numCols;
	/*
	for(int r=0; r<numRows; r++)
		for(int c=0; c<numCols; c++){


		}
	*/

	Hi = HIGH_THRESH;
	Low = LOW_THRESH;
	Hysteresis(peaks, mag, cannyOut, Hi, Low);

}

/*===========================================================================*\
							MAIN
\*===========================================================================*/

int main()
{

	char inName[BUFFER_SIZE];
	char gradFileName[BUFFER_SIZE];
	char threshLowFileName[BUFFER_SIZE];
	char threshHigFileName[BUFFER_SIZE];
			
	strcpy(           inName,         "chess.pgm");
	strcpy(     gradFileName,      "grad_OUT.pgm");
	strcpy(threshLowFileName, "threshLow_OUT.pgm");
	strcpy(threshHigFileName, "threshHig_OUT.pgm");

    printf("\tENTER YOUR SELECTION\n"
		    "1. Modified Sobel With Threshold\n"
			"2. Canny Algorithm\n"
			"9. EXIT\n");

    scanf("%d", &choice);

	while (choice !=9)
	{

        if( choice == 1){
			
			int **img, **sobelout, **threshLow, **threshHigh;
			
			img = readpic(inName);

			sobelout = setImage();
			threshLow = setImage();
			threshHigh = setImage();

			printf("Successfully read image file '%s'\n", inName);

			sobelThresh(img, sobelout, threshLow, threshHigh);

			writeoutpic(threshLowFileName,  threshLow);
			writeoutpic(threshHigFileName, threshHigh);
			writeoutpic(      gradFileName,  sobelout);
			
			printf("COMPLETE!\n");

       }

		if ( choice==2){

			int **inImg, **cannyOut;
			double **mag, **peaks;
			double sig = 0;

			// Enter Sigma value.
			enterSigma:
			printf("Enter sigma. Must be 1 though 9\n");

			scanf("%lf", &sig);
			
			if(sig>9 || sig<1)
				goto enterSigma;	// jump back to "enterSigma" label

			// Enter Percent value.
			int percent;
			printf("Enter percent to calc Hi and Low\n");
			scanf("%d", &percent);

			inImg = readpic(inName);

			mag		 = setDouble();
			peaks	 = setDouble();
			cannyOut = setImage();

			canny(inImg, peaks, mag, cannyOut, sig, percent);

			writeoutpic(         "canny_OUT.pgm", cannyOut);
			writeoutpic(   "canny_peaks_OUT.pgm",    peaks);
			writeoutpic("canny_gradient_OUT.pgm",      mag);

			printf("COMPLETE!\n");

		}
       
    	printf("Enter your menu selection again\n");
        
		scanf("%d", &choice);

    }
        
    printf("Program Complete - safe to open images.\n");

}

/*===========================================================================*\
						End of SobelAndCanny.cpp
\*===========================================================================*/
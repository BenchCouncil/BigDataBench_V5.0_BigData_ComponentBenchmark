/*
siftfeat_mpi.c is a MPI version altered on the basis of siftfeat.c, which has implemented a process to all images in any specified directory.
Copyright (C) 2014 modified by ACS ICT

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.
This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

When using this program encounter any problems, please contact us by email address "gaowanling@ict.ac.cn" or "wanglei@ict.ac.cn".
*/
#include "mpi.h"
#include <stdio.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include "sift.h"
#include "imgfeatures.h"
#include "utils.h"

#include <highgui.h>

#include <unistd.h>


#define OPTIONS ":o:m:i:s:c:r:n:b:dxh"
#define MAX_LINE 200

/*************************** Function Prototypes *****************************/

static void usage( char* );
static void arg_parse( int, char** );

/******************************** Globals ************************************/

char* pname;
char* img_file_name;
const char* out_file_name = NULL;
const char* out_img_name = NULL;
int intvls = SIFT_INTVLS;
double sigma = SIFT_SIGMA;
double contr_thr = SIFT_CONTR_THR;
int curv_thr = SIFT_CURV_THR;
int img_dbl = SIFT_IMG_DBL;
int descr_width = SIFT_DESCR_WIDTH;
int descr_hist_bins = SIFT_DESCR_HIST_BINS;
int NonDisplay = 1;


/********************************** Main *************************************/

int main( int argc, char** argv )
{
  FILE *in;
  IplImage* img;
  struct feature* features;
  int n = 0;
  char buf[MAX_LINE];
  int len;

  MPI_Init(&argc,&argv);
  int myrank;
  int size;
  MPI_Comm_rank(MPI_COMM_WORLD,&myrank);
  MPI_Comm_size(MPI_COMM_WORLD,&size);

  arg_parse( argc, argv );
  in=fopen(img_file_name, "r");
  if(NULL==in)
      return -1;

  if(NULL!= out_file_name && NULL==opendir(out_file_name))
      mkdir(out_file_name,0775);

  if(NULL!= out_img_name && NULL==opendir(out_img_name))
      mkdir(out_img_name,0775);
  if(myrank==0)
      printf("Loading file and sift begin:\n");
  int fileNum=0;
  while(fgets(buf, MAX_LINE, in)!=NULL)
  {
      fileNum++;
      len = strlen(buf);
      buf[len-1]='\0';
      if(myrank==(fileNum-1)%size)
      {
          img = cvLoadImage( buf, 1 );
          if( ! img )
              fatal_error( "unable to load image from %s", img_file_name );
          n = _sift_features( img, &features, intvls, sigma, contr_thr, curv_thr,
                              img_dbl, descr_width, descr_hist_bins );
	  if(out_file_name)
	  {
	      char outfilename[MAX_LINE];
	      strcpy(outfilename,out_file_name);
	      if(outfilename[strlen(outfilename)-1]!='/')
		      strcat(outfilename,"/");
	      strncat(outfilename,strrchr(buf,'/')+1,strlen(strrchr(buf,'/')+1)-5);
	      strcat(outfilename,"_out.sift");
	      export_features( outfilename, features, n );
	  }
	  if(out_img_name)
	  {
	      char outfilename[MAX_LINE];
              strcpy(outfilename,out_img_name);
	      if(outfilename[strlen(outfilename)-1]!='/')
		      strcat(outfilename,"/");
              strncat(outfilename,strrchr(buf,'/')+1,strlen(strrchr(buf,'/')+1)-5);
              strcat(outfilename,"_out.jpeg");
              cvSaveImage( outfilename, img, NULL );
	  }
	  cvReleaseImage(&img);
	  free(features);
      }
  }
  fclose(in);

  
  if( !NonDisplay )
    {
      draw_features( img, features, n );
      display_big_img( img, img_file_name );
      cvWaitKey( 0 );
    }
  cvReleaseImage(&img);
  if(myrank==0)
      printf("Processing %d images, Complete!\n",fileNum);
  MPI_Finalize();
  return 0;
}


/************************** Function Definitions *****************************/

// print usage for this program
static void usage( char* name )
{
  fprintf(stderr, "%s: detect SIFT keypoints in an image\n\n", name);
  fprintf(stderr, "Usage: %s [options] <img_file>\n", name);
  fprintf(stderr, "Options:\n");
  fprintf(stderr, "  -h               Display this message and exit\n");
  fprintf(stderr, "  -o <out_file>    Output keypoints to text file directory\n");
  fprintf(stderr, "  -m <out_img>     Output keypoint image file directory\n");
  fprintf(stderr, "  -i <intervals>   Set number of sampled intervals per" \
	  " octave in scale space\n");
  fprintf(stderr, "                   pyramid (default %d)\n",
	  SIFT_INTVLS);
  fprintf(stderr, "  -s <sigma>       Set sigma for initial gaussian"	\
	  " smoothing at each octave\n");
  fprintf(stderr, "                   (default %06.4f)\n", SIFT_SIGMA);
  fprintf(stderr, "  -c <thresh>      Set threshold on keypoint contrast" \
	  " |D(x)| based on [0,1]\n");
  fprintf(stderr, "                   pixel values (default %06.4f)\n",
	  SIFT_CONTR_THR);
  fprintf(stderr, "  -r <thresh>      Set threshold on keypoint ratio of" \
	  " principle curvatures\n");
  fprintf(stderr, "                   (default %d)\n", SIFT_CURV_THR);
  fprintf(stderr, "  -n <width>       Set width of descriptor histogram" \
	  " array (default %d)\n", SIFT_DESCR_WIDTH);
  fprintf(stderr, "  -b <bins>        Set number of bins per histogram" \
	  " in descriptor array\n");
  fprintf(stderr, "                   (default %d)\n", SIFT_DESCR_HIST_BINS);
  fprintf(stderr, "  -d               Toggle image doubling (default %s)\n",
	  SIFT_IMG_DBL == 0 ? "off" : "on");
  fprintf(stderr, "  -x               Turn on keypoint display\n");
}



/*
  arg_parse() parses the command line arguments, setting appropriate globals.
  
  argc and argv should be passed directly from the command line
*/
static void arg_parse( int argc, char* argv[] )
{
  //extract program name from command line (remove path, if present)
  pname = basename( argv[0] );

  //parse commandline options
  while( 1 )
    {
      char* arg_check;
      int arg = getopt( argc, argv, OPTIONS );
      if( arg == -1 )
	break;

      switch( arg )
	{
	  // catch unsupplied required arguments and exit
	case ':':
	  fatal_error( "-%c option requires an argument\n"		\
		       "Try '%s -h' for help.", optopt, pname );
	  break;

	  // read out_file_name
	case 'o':
	  if( ! optarg )
	    fatal_error( "error parsing arguments at -%c\n"	\
			 "Try '%s -h' for help.", arg, pname );
	  out_file_name = optarg;
	  break;

	  // read out_img_name
	case 'm':
	  if( ! optarg )
	    fatal_error( "error parsing arguments at -%c\n"	\
			 "Try '%s -h' for help.", arg, pname );
	  out_img_name = optarg;
	  break;
	  
	  // read intervals
	case 'i':
	  // ensure argument provided
	  if( ! optarg )
	    fatal_error( "error parsing arguments at -%c\n"	\
			 "Try '%s -h' for help.", arg, pname );
	  
	  // parse argument and ensure it is an integer
	  intvls = strtol( optarg, &arg_check, 10 );
	  if( arg_check == optarg  ||  *arg_check != '\0' )
	    fatal_error( "-%c option requires an integer argument\n"	\
			 "Try '%s -h' for help.", arg, pname );
	  break;
	  
	  // read sigma
	case 's' :
	  // ensure argument provided
	  if( ! optarg )
	    fatal_error( "error parsing arguments at -%c\n"	\
			 "Try '%s -h' for help.", arg, pname );
	  
	  // parse argument and ensure it is a floating point number
	  sigma = strtod( optarg, &arg_check );
	  if( arg_check == optarg  ||  *arg_check != '\0' )
	    fatal_error( "-%c option requires a floating point argument\n" \
			 "Try '%s -h' for help.", arg, pname );
	  break;
	  
	  // read contrast_thresh
	case 'c' :
	  // ensure argument provided
	  if( ! optarg )
	    fatal_error( "error parsing arguments at -%c\n"	\
			 "Try '%s -h' for help.", arg, pname );
	  
	  // parse argument and ensure it is a floating point number
	  contr_thr = strtod( optarg, &arg_check );
	  if( arg_check == optarg  ||  *arg_check != '\0' )
	    fatal_error( "-%c option requires a floating point argument\n" \
			 "Try '%s -h' for help.", arg, pname );
	  break;
	  
	  // read curvature_thresh
	case 'r' :
	  // ensure argument provided
	  if( ! optarg )
	    fatal_error( "error parsing arguments at -%c\n"	\
			 "Try '%s -h' for help.", arg, pname );
	  
	  // parse argument and ensure it is a floating point number
	  curv_thr = strtol( optarg, &arg_check, 10 );
	  if( arg_check == optarg  ||  *arg_check != '\0' )
	    fatal_error( "-%c option requires an integer argument\n"	\
			 "Try '%s -h' for help.", arg, pname );
	  break;
	  
	  // read descr_width
	case 'n' :
	  // ensure argument provided
	  if( ! optarg )
	    fatal_error( "error parsing arguments at -%c\n"	\
			 "Try '%s -h' for help.", arg, pname );
	  
	  // parse argument and ensure it is a floating point number
	  descr_width = strtol( optarg, &arg_check, 10 );
	  if( arg_check == optarg  ||  *arg_check != '\0' )
	    fatal_error( "-%c option requires an integer argument\n"	\
			 "Try '%s -h' for help.", arg, pname );
	  break;
	  
	  // read descr_histo_bins
	case 'b' :
	  // ensure argument provided
	  if( ! optarg )
	    fatal_error( "error parsing arguments at -%c\n"	\
			 "Try '%s -h' for help.", arg, pname );
	  
	  // parse argument and ensure it is a floating point number
	  descr_hist_bins = strtol( optarg, &arg_check, 10 );
	  if( arg_check == optarg  ||  *arg_check != '\0' )
	    fatal_error( "-%c option requires an integer argument\n"	\
			 "Try '%s -h' for help.", arg, pname );
	  break;
	  
	  // read double_image
	case 'd' :
	  img_dbl = ( img_dbl == 1 )? 0 : 1;
	  break;

	  // read display
	case 'x' :
	  NonDisplay = 0;
	  break;

	  // user asked for help
	case 'h':
	  usage( pname );
	  exit(0);
	  break;

	  // catch invalid arguments
	default:
	  fatal_error( "-%c: invalid option.\nTry '%s -h' for help.",
		       optopt, pname );
	}
    }

  // make sure an input file is specified
  if( argc - optind < 1 )
    fatal_error( "no input file specified.\nTry '%s -h' for help.", pname );

  // make sure there aren't too many arguments
  if( argc - optind > 1 )
    fatal_error( "too many arguments.\nTry '%s -h' for help.", pname );

  // copy image file name from command line argument
  img_file_name = argv[optind];
}

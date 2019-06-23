package org.hipi.tools.sift;

import static org.bytedeco.javacpp.opencv_imgproc.CV_RGB2GRAY;

import org.hipi.opencv.OpenCVUtils;
import org.hipi.image.FloatImage;  
import org.hipi.image.HipiImageHeader;  
import org.hipi.image.HipiImageHeader.HipiColorSpace;
//import org.hipi.imagebundle.mapreduce.ImageBundleInputFormat;
import org.hipi.imagebundle.HipiImageBundle;
import org.hipi.opencv.OpenCVMatWritable;  
import org.hipi.mapreduce.BinaryOutputFormat;
import org.hipi.imagebundle.mapreduce.HibInputFormat;
  
import org.apache.hadoop.conf.Configured;  
import org.apache.hadoop.util.Tool;  
import org.apache.hadoop.util.ToolRunner;  
import org.apache.hadoop.fs.Path;  
import org.apache.hadoop.io.Text;  
import org.apache.hadoop.mapreduce.Job;  
import org.apache.hadoop.mapreduce.Mapper;  
import org.apache.hadoop.mapreduce.Reducer;  
import org.apache.hadoop.mapreduce.lib.input.FileInputFormat;  
import org.apache.hadoop.mapreduce.lib.output.FileOutputFormat;  

import org.bytedeco.javacpp.opencv_core;
import org.bytedeco.javacpp.opencv_core.Mat;
import org.bytedeco.javacpp.opencv_core.Rect;
import org.bytedeco.javacpp.opencv_core.Scalar;

import org.bytedeco.javacpp.opencv_core.DMatchVector;
import org.bytedeco.javacpp.opencv_core.KeyPoint;
import org.bytedeco.javacpp.opencv_core.KeyPointVector;
import org.bytedeco.javacpp.opencv_features2d;
import org.bytedeco.javacpp.opencv_xfeatures2d.SIFT;
import org.bytedeco.javacpp.opencv_features2d.BFMatcher;
import org.bytedeco.javacpp.opencv_features2d.DrawMatchesFlags;
import org.bytedeco.javacpp.opencv_imgproc;
//import org.opencv.core.Mat;
//import org.opencv.core.MatOfKeyPoint;
//import org.opencv.features2d.*;
import java.io.IOException;  
  
public class HadoopSift extends Configured implements Tool {  
    
  public static class SIFTMapper extends Mapper<HipiImageHeader, FloatImage, Text, OpenCVMatWritable> {  
    public void map(HipiImageHeader key, FloatImage value, Context context)   
      throws IOException, InterruptedException {  

    //
    //Perform conversion to OpenCV
    //
     String meta = key.getMetaData("filename");
     Mat cvImage = new Mat(value.getHeight(), value.getWidth(), opencv_core.CV_8U);
/*     HipiColorSpace colorSpace = value.getColorSpace();
     switch(colorSpace) {
         case RGB:
            Mat cvImageRGB = OpenCVUtils.convertRasterImageToMat(value);
            opencv_imgproc.cvtColor(cvImageRGB, cvImage, CV_RGB2GRAY);
         case LUM:
             cvImage = OpenCVUtils.convertRasterImageToMat(value);
         default:
             System.out.println("HipiColorSpace [" + colorSpace + "] not supported. ");
     }
*/
     SIFT sift=SIFT.create(0, 3, 0.03, 10, 1.6);
     KeyPointVector kp1 = new KeyPointVector();
     sift.detect(cvImage, kp1);
     Mat desc = new Mat();
     opencv_features2d.drawKeypoints(cvImage, kp1, desc,new Scalar(255, 255, 255, 0), DrawMatchesFlags.DRAW_RICH_KEYPOINTS);
     context.write(new Text(meta), new OpenCVMatWritable(desc));
/*     Mat desc = new Mat();
     FeatureDetector fd = FeatureDetector.create(FeatureDetector.SIFT);
     MatOfKeyPoint mkp =new MatOfKeyPoint();
     fd.detect(cvImage, mkp);
     DescriptorExtractor de = DescriptorExtractor.create(DescriptorExtractor.SIFT);
     de.compute(cvImage,mkp,desc );
     context.write(new IntWritable(1), new OpenCVMatWritable(desc));
*/

/*
     if (value != null && value.getWidth() > 1 && value.getHeight() > 1 && value.getBands() == 3) {  
  
        int w = value.getWidth();  
        int h = value.getHeight();  
  
        float[] valData = value.getData();  
  
        float[] avgData = {0,0,0};  
  
        for (int j = 0; j < h; j++) {  
          for (int i = 0; i < w; i++) {  
            avgData[0] += valData[(j*w+i)*3+0]; // R  
            avgData[1] += valData[(j*w+i)*3+1]; // G  
            avgData[2] += valData[(j*w+i)*3+2]; // B  
          }  
        }  
  
        FloatImage avg = new FloatImage(1, 1, 3, avgData);  
        avg.scale(1.0f/(float)(w*h));  
        context.write(new IntWritable(1), avg);  
      }
*/    
    }  
  }  
/*    
  public static class SIFTReducer extends Reducer<IntWritable, FloatImage, IntWritable, Text> {  
    public void reduce(IntWritable key, Iterable<FloatImage> values, Context context)   
      throws IOException, InterruptedException {  
       FloatImage avg = new FloatImage(1, 1, 3);  
      int total = 0;  
      for (FloatImage val : values) {  
        avg.add(val);  
        total++;  
      }  
      if (total > 0) {  
        avg.scale(1.0f / total);  
        float[] avgData = avg.getData();  
        String result = String.format("Average pixel value: %f %f %f", avgData[0], avgData[1], avgData[2]);  
        context.write(key, new Text(result));  
      }  
    }  
  }  
  */  
  public int run(String[] args) throws Exception {  
    // Check input arguments  
    if (args.length != 2) {  
      System.out.println("Usage: firstprog <input HIB> <output directory>");  
      System.exit(0);  
    }  
      
    // Initialize and configure MapReduce job  
    Job job = Job.getInstance();  
    // Set input format class which parses the input HIB and spawns map tasks  
    job.setInputFormatClass(HibInputFormat.class);  
    // Set the driver, mapper, and reducer classes which express the computation  
    job.setJarByClass(HadoopSift.class); 
    job.setNumReduceTasks(0); 
    job.setMapperClass(SIFTMapper.class);  
    //job.setReducerClass(SIFTReducer.class);  
    // Set the types for the key/value pairs passed to/from map and reduce layers  
//    job.setMapOutputKeyClass(IntWritable.class);  
//    job.setMapOutputValueClass(FloatImage.class);  
//    job.setOutputKeyClass(IntWritable.class);  
//    job.setOutputValueClass(Text.class);  
  
//    job.setMapOutputKeyClass(IntWritable.class);
//    job.setMapOutputValueClass(OpenCVMatWritable.class);

    job.setOutputKeyClass(Text.class);
    job.setOutputValueClass(OpenCVMatWritable.class);
   
    job.setOutputFormatClass(BinaryOutputFormat.class);
 
    // Set the input and output paths on the HDFS  
    FileInputFormat.setInputPaths(job, new Path(args[0]));  
    FileOutputFormat.setOutputPath(job, new Path(args[1]));  
  
    // Execute the MapReduce job and block until it complets  
    boolean success = job.waitForCompletion(true);  
      
    // Return success or failure  
    return success ? 0 : 1;  
  }  
    
  public static void main(String[] args) throws Exception {  
    ToolRunner.run(new HadoopSift(), args);  
    System.exit(0);  
  }  
    
}  

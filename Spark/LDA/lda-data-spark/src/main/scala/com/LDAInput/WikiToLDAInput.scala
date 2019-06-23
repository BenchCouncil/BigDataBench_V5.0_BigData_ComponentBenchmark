package com.LDAInput

import org.apache.spark.mllib.feature.{HashingTF, IDF}
import org.apache.spark.mllib.linalg.SparseVector
import org.apache.spark.{SparkConf, SparkContext}

import scala.io.Source

object WikiToLDAInput {
  def main(args: Array[String]) {

    val conf = new SparkConf().setAppName("Prepare_LDA_Data")
    val sc = new SparkContext(conf)

    val textData = sc.textFile(args(0)).map(_.split(" ").toSeq).zipWithIndex().filter(_._2 > 7).map{case (x,y)=>(y,x)}
    
    val dictionary = textData.flatMap(line=>line._2).distinct().sortBy(x => x, true).zipWithIndex()
    dictionary.cache()
    dictionary.map(x=>x._1).coalesce(1,true).saveAsTextFile(args(1))

    var corpus = textData.flatMapValues(line=>line.map(word => (word, 1))).map{case (num,seq)=>((num,seq._1),seq._2)}.reduceByKey(_+_).map{case(seq,num)=>(seq._2,(seq._1,num))}

    val ldaIn=dictionary.join(corpus).map{case(x,y)=>(y._1,y._2._1,y._2._2)}
    ldaIn.sortBy(x=>x._2,true).map(x=>x._1+"\t"+x._2+"\t"+x._3).saveAsTextFile(args(2))

    sc.stop()
    }

}

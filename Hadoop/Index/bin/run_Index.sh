#!/bin/bash
# Licensed to the Apache Software Foundation (ASF) under one or more
# contributor license agreements.  See the NOTICE file distributed with
# this work for additional information regarding copyright ownership.
# The ASF licenses this file to You under the Apache License, Version 2.0
# (the "License"); you may not use this file except in compliance with
# the License.  You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

bin=`dirname "$0"`
bin=`cd "$bin"; pwd`

echo "========== running nutchindex data =========="
# configure
DIR=`cd $bin/../; pwd`
. "${DIR}/conf/hibench-config.sh"
. "${DIR}/conf/configure.sh"

export NUTCH_HOME=$BigdataBench_Home/SearchEngine/Index/nutch-1.2-hadoop1
cd $NUTCH_HOME
export NUTCH_CONF_DIR=$HADOOP_CONF_DIR:$NUTCH_HOME/conf

hadoop dfs -rmr /Nutch/Output
hadoop dfs -rmr $INPUT_HDFS/indexes
# run bench
../nutch-1.2-hadoop1/bin/nutch index $COMPRESS_OPTS $OUTPUT_HDFS $INPUT_HDFS/crawldb $INPUT_HDFS/linkdb $INPUT_HDFS/segments/*



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

echo "========== preparing nutchindex data =========="

# configure
DIR=`cd $bin/../; pwd`
. "${DIR}/conf/hibench-config.sh"
. "${DIR}/conf/configure.sh"

# compress
if [ $COMPRESS -eq 1 ]; then
    COMPRESS_OPT="-c ${COMPRESS_CODEC}"
fi

rm -rf $TMPLOGFILE
hadoop dfs -rmr /Nutch
hadoop dfs -mkdir /Nutch
# generate data
OPTION="-t nutch \
        -b ${NUTCH_BASE_HDFS} \
        -n ${NUTCH_INPUT} \
        -m ${NUM_MAPS} \
        -r ${NUM_REDS} \
        -p ${PAGES} \
        -o sequence"

$HADOOP_EXECUTABLE jar  ../conf/datatools.jar HiBench.DataGen ${OPTION} ${COMPRESS_OPT} 

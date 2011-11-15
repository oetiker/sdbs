#!/bin/bash

. `dirname $0`/sdbs.inc



simplebuild http://www.rsyslog.com/files/download/rsyslog/ \
 rsyslog-5.8.6.tar.gz \
  --disable-klog \
  --disable-testbench





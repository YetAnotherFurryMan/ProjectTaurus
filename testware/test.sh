#!/bin/bash

../build/testware/trsap a b c -a -bv -b=v -b:v -c -cv -c=v -c:v -dv1:v2 -dv1=v2 -d:v1:v2 -d:v1=v2 -d=v1:v2 -d=v1=v2 -ev1:v2 -ev1=v2 -e:v1:v2 -e:v1=v2 -e=v1:v2 -e=v1=v2 -ev -e:v -e=v -- --aa --bb=v --bb:v --cc --cc=v --cc:v --dd:v1:v2 --dd:v1=v2 --dd=v1:v2 --dd=v1=v2 --ee:v1:v2 --ee:v1=v2 --ee=v1:v2 --ee=v1=v2 --ee:v --ee=v
../build/testware/trsap++ a b c -a -bv -b=v -b:v -c -cv -c=v -c:v -dv1:v2 -dv1=v2 -d:v1:v2 -d:v1=v2 -d=v1:v2 -d=v1=v2 -ev1:v2 -ev1=v2 -e:v1:v2 -e:v1=v2 -e=v1:v2 -e=v1=v2 -ev -e:v -e=v -- --aa --bb=v --bb:v --cc --cc=v --cc:v --dd:v1:v2 --dd:v1=v2 --dd=v1:v2 --dd=v1=v2 --ee:v1:v2 --ee:v1=v2 --ee=v1:v2 --ee=v1=v2 --ee:v --ee=v


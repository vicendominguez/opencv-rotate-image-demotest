opencv-rotate-image-demotest
============================

Description
-----------

My tests with opencv rotate image.

```<x>``` and ```<y>``` and ```<z>``` and ```<step>``` must be degrees.

Compile
-------

You will need opencv libs installed.

```
g++ -o rotate rotate.cpp `pkg-config opencv --cflags --libs`
```

Usage
-----

```
./rotate testingtesting.jpg 0 0 -360 360 0 0 10
```


Notes
-----

* In my tests i wanna chroma color (0,255,0) as background.
* It is possible to save the output. Check the commented line in the source.

Errors
------

Yes, i have seen the thin line arround the picture :(     

It was a test right?

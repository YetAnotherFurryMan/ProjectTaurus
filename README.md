# Taurus
The Taurus Project is a collection of tools and libraries made for the Taurus Programming Language (TRS) and the Taurus Scripting Language (TRSS). All tools strive to be as independent as my knowledge will allow.

## Fundamentals
<ul>
  <li><b>No syntax sugar</b> - will make the code more readable</li>
  <li><b>No compiler's magic</b> - will allow the programmer to have more control on what is really going on and what the compiler is doing</li>
  <li><b>Uniform syntax</b> - one operator always means the same</li>
  <li><b>Trust</b> - TRS and TRSS are not made for beginners! They can give you good results or a headache</li>
</ul>

## Everything is subject to change
I am open to suggestions and objective criticism. This project is created for me and it has to meet my requirements in the first place, but I also create it because I want to learn more, so my approach, assumptions and solutions may change. Any feedback is welcome.

## Building form source
Building Taurus form source is preatty easy. All you need is C/C++ compiler like gcc/g++ or clang and GNU Make or Ninja.

``` bash
 make RELEASE=1
```

``` bash
 ninja
```

Both Makefile and build.ninja are auto-generated, if you want you can use bs.cpp to re-generate them.

``` bash
 g++ buildsyatem.cpp -o buildsyatem -std=c++17
 ./bs
```

``` bash
 clang buildsyatem.cpp -o buildsyatem -std=c++17
 ./bs
```

Also there is testware folder with all the tests for the project. You can use the bs executable to automatically perform the tests.

``` bash
 ./bs test
```


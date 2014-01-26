K8055/VM110N
=====

Code base to control a Velleman K8055/VM110N from my Mac

After spending some time looking at the various (old) projects available on the net, I realized that none of them is doing what I need: be able to control my Velleman K8055/VM110N from my Mac. Thus I decided to write my own interface based on several findings.

* First things you'll need is the kext file: your Mac needs to know what is the USB device which is connected. I used the one from [piha](https://github.com/piha/k8055-mac-codeless-kext). Clone the project and run the command, this will do everything for you.
* Next, you need to know how to talk to the device. This is done by a library, easiest on a Mac is to write it in CPP. I used the one from [Richard Plumridge](http://sourceforge.net/projects/k8055usbosx/). Richard replicated the Velleman DLL and the demo interface to run on Mac. You do not need to download it to run "my" solution, I packaged the necessary files inside the [cpp] area. The sample application is usefull to know if you Mac talks to the device! (do not for to set the appropriate SK combination to identify your device - by default SK5 and SK6 must be checked).
* Once you have a working library (.dylib on Mac), you can start play with JNI to have the bridge between your Java code and the CPP code. Do not forget to use the library path parameter so that the library loader can find your lib (`java -Djava.library.path="./lib"`).

If you do not want to take care of the CPP part, I commited [libk8055.dylib](java/lib/libk8055.dylib) file. Thus once the project is cloned, just complile the java part and run `java -Djava.library.path="../lib" com.alex.k8055.Tester`. You should be ready to implement you own Java code on top of the library.

Note: the code is a really first shot, I will clean it up when I will write the web part of my project (control my K8055/VM110N from a web page). Expect some changes but when... ? ;)


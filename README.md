# FSMonitor



#### Description

---

A simple file system monitor written in C++.



#### Usage

---

``` bash
./scripts/build.sh
./build/bin/fsmonitor
```



### Day 1

---

###### Approch

Divide the objective into three main topics. Each topic will be delegated seperatly.

- Logger Thread
- inotify() Abstraction
- Threads Syncornazation



###### TODO

- [x] Setup Cmake Project

- [x] Parse input directories list

  - Launch thread per dir

- [ ] Filter nested paths.
- [ ] Logger

  - Basic API
  - Test with dummy monitor instances.



###### Questions

1. Should we limit the number of watchable directories?

   This point is crucial for the efficent logging syncornization.

   If we don't constraint the number of watchable directories, then some syncronozation mechanisem may not scale well.

2. If we recieved a directory to monitor, and at some point this directory is deleted.

   Should we end the thread, or keep waiting for a it to be recreated?

3. How strong should be the ordering?

   Idealy, we probably wish to take a timestamp on each notified event, and log them chronologically.

   

###### Concurency & Communication

---

Regarding the log. There are two constraints I am pondering about:

- Should the log be real-time?
- Should the log be unifed?

If the log must not be unifed, then each thread can log its own events, and we don't need threads syncornaization.

If the log must not be real-time, then each thread log its own events, and we can unify the logs on termination.



Assuming our log is real-time and unifed, then we have the isssue of multiple threads syncornization.

Here are the basic approches that came to my mind:

- Each thread writes its own events to the log, while taking a lock before each write.

  The obvious issue here is scalabilty.

- Spanning a logger thread.

  Now we only have to synchoronaize in a single reader multiple writers fashion.

  We potentialy can take one of these approches:

  - Unix Domain Sockets.

    Windows support begins with **Windows 10 Version 1803**.

  - Shared data variable for each monitoring thread with the logger thread.

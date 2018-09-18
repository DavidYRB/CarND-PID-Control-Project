# CarND-Controls-PID
Self-Driving Car Engineer Nanodegree Program

## Discussion

### Review of PID controller

In this project, we create a PID controller to control the steering angle of the car in the simulator. The mean idea of the controller is to use the Cross Track Error (CTE) to generate a value that will decrease/eliminate the CTE so that the actual steering angle will be the desired steering angle.

PID controller is a simple controller that just use three parameters to generate control command.

* P parameter
  
  P parameter means proportional, it will generate the command proportional to CTE, for example, if we use `c` to represent CTE, and `Kp` represent P parameter, the controller out put will be:
  $$Kp * c$$. 
  This parameter makes the controller sensitive to the error, if CTE is large, the controller output is large, and vice versa.

* I parameter
  
  I parameter means integral. It means it has the ability to remember all the error in the past. Actually, it will accumulate CTE of every time step in the past. If we use `c` to represent CTE, and `Ki` represent I parameter, the output of I controller is:
  $$ Ki * \sum{c}$$
  It means the controller will take accumulative error into account, as time goes by, if the sum of CTE be come smaller, the control output will be not that fast. It can be useful for systems that has internal bias. For example, the car itself cannot go straight eventhough we fix the wheel.

* D parameter
  
  D parameter means differential. It takes the change rate into account, which means it regulate how fast CTE changes. When CTE in this time step changes a lot in next time step, the output of D controller is great. So D controller can be used to reduce oscillation. D controller is always used with P controller to reduce overshoot and make the change smoothly.
  $$ Kd * (c_t - c_{t-1})$$

* PID controller in math
  
  Based on the description above, the controller can be represented as
  $$ Kp * c + Ki * \sum{c} + Kd * (c_t - c_{t-1}) $$

### Tuning PID paramters

* Start points

  From class we learned a way to tune PID paramters called Twiddle, but we always need to have a start for tuning so that we can quickly approach to the converge point. To find the start point, I first used only P parameter to find a value that can keep the car in the track eventhough there was some oscillation. Then find D value that can reduce the oscillation a little. For I parameter, since it will take all past CTE into account, it should not be too large. I just set a value a little greater than zero. After I determined the parameter that can make the car run in the track, we start to use twiddle.
  The beginning parameter of PID is 
  $$Kp = 0.2 \text{   } Ki = 0.002\text{ }Kd = 2$$

* Twiddle to converge
  
  To make the parameter more precise, I update parameter in order every 100 time steps. The first 50 steps will just make it run, and record CTE of next 50 steps. Then send the average CTE for each step to twiddle algoritm. The final paramter is: 
  $$Kp = 0.211288 \text{   } Ki = 0.0022\text{ }Kd = 2.90795$$

---

## Dependencies

* cmake >= 3.5
 * All OSes: [click here for installation instructions](https://cmake.org/install/)
* make >= 4.1(mac, linux), 3.81(Windows)
  * Linux: make is installed by default on most Linux distros
  * Mac: [install Xcode command line tools to get make](https://developer.apple.com/xcode/features/)
  * Windows: [Click here for installation instructions](http://gnuwin32.sourceforge.net/packages/make.htm)
* gcc/g++ >= 5.4
  * Linux: gcc / g++ is installed by default on most Linux distros
  * Mac: same deal as make - [install Xcode command line tools]((https://developer.apple.com/xcode/features/)
  * Windows: recommend using [MinGW](http://www.mingw.org/)
* [uWebSockets](https://github.com/uWebSockets/uWebSockets)
  * Run either `./install-mac.sh` or `./install-ubuntu.sh`.
  * If you install from source, checkout to commit `e94b6e1`, i.e.
    ```
    git clone https://github.com/uWebSockets/uWebSockets 
    cd uWebSockets
    git checkout e94b6e1
    ```
    Some function signatures have changed in v0.14.x. See [this PR](https://github.com/udacity/CarND-MPC-Project/pull/3) for more details.
* Simulator. You can download these from the [project intro page](https://github.com/udacity/self-driving-car-sim/releases) in the classroom.

There's an experimental patch for windows in this [PR](https://github.com/udacity/CarND-PID-Control-Project/pull/3)

## Basic Build Instructions

1. Clone this repo.
2. Make a build directory: `mkdir build && cd build`
3. Compile: `cmake .. && make`
4. Run it: `./pid`. 

Tips for setting up your environment can be found [here](https://classroom.udacity.com/nanodegrees/nd013/parts/40f38239-66b6-46ec-ae68-03afd8a601c8/modules/0949fca6-b379-42af-a919-ee50aa304e6a/lessons/f758c44c-5e40-4e01-93b5-1a82aa4e044f/concepts/23d376c7-0195-4276-bdf0-e02f1f3c665d)

## Editor Settings

We've purposefully kept editor configuration files out of this repo in order to
keep it as simple and environment agnostic as possible. However, we recommend
using the following settings:

* indent using spaces
* set tab width to 2 spaces (keeps the matrices in source code aligned)

## Code Style

Please (do your best to) stick to [Google's C++ style guide](https://google.github.io/styleguide/cppguide.html).

## Project Instructions and Rubric

Note: regardless of the changes you make, your project must be buildable using
cmake and make!

More information is only accessible by people who are already enrolled in Term 2
of CarND. If you are enrolled, see [the project page](https://classroom.udacity.com/nanodegrees/nd013/parts/40f38239-66b6-46ec-ae68-03afd8a601c8/modules/f1820894-8322-4bb3-81aa-b26b3c6dcbaf/lessons/e8235395-22dd-4b87-88e0-d108c5e5bbf4/concepts/6a4d8d42-6a04-4aa6-b284-1697c0fd6562)
for instructions and the project rubric.

## Hints!

* You don't have to follow this directory structure, but if you do, your work
  will span all of the .cpp files here. Keep an eye out for TODOs.

## Call for IDE Profiles Pull Requests

Help your fellow students!

We decided to create Makefiles with cmake to keep this project as platform
agnostic as possible. Similarly, we omitted IDE profiles in order to we ensure
that students don't feel pressured to use one IDE or another.

However! I'd love to help people get up and running with their IDEs of choice.
If you've created a profile for an IDE that you think other students would
appreciate, we'd love to have you add the requisite profile files and
instructions to ide_profiles/. For example if you wanted to add a VS Code
profile, you'd add:

* /ide_profiles/vscode/.vscode
* /ide_profiles/vscode/README.md

The README should explain what the profile does, how to take advantage of it,
and how to install it.

Frankly, I've never been involved in a project with multiple IDE profiles
before. I believe the best way to handle this would be to keep them out of the
repo root to avoid clutter. My expectation is that most profiles will include
instructions to copy files to a new location to get picked up by the IDE, but
that's just a guess.

One last note here: regardless of the IDE used, every submitted project must
still be compilable with cmake and make./

## How to write a README
A well written README file can enhance your project and portfolio.  Develop your abilities to create professional README files by completing [this free course](https://www.udacity.com/course/writing-readmes--ud777).


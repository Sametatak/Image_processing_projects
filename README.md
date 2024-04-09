# kivi_task
Kivi task solutians

# Task 1
-This repository contains an implementation of the double three-step phase-shifting algorithm for fringe pattern analysis, reproducing the methodology outlined in the reference [HHC02]. It's designed to operate in an ideal setting, with patterns and computations that adhere strictly to the described theoretical models.

-The primary objective is to faithfully implement the double three-step phase-shifting algorithm, generate synthetic sinusoidal patterns, compute phase maps, and average these maps to obtain a single, unwrapped phase map free from discontinuities.

-The code in my /task1/main.cpp file generate 6 synthetic sinusoidal patterns then using the formula in figure 7 computes two phase maps of each three pattern.

![image](https://github.com/Sametatak/kivi_task/assets/50046275/62dbb1f4-5aae-4d79-aa87-c5a30a1b82cc)

-Then we Obtain the averaged single phase maps according to the paper by simply taking arithmetic mean of two maps.

![image](https://github.com/Sametatak/kivi_task/assets/50046275/3cd36da2-2f2b-4ce1-a8c4-5deaa826d237)
 _avaraged phase map_

 1.3 Phase Unwrapping
 -
This step utilizes a phase unwrapping technique leveraging wrapped phase maps constrained between −π and π and fringe order patterns encoded in Gray code. The procedure involves decoding seven Gray-coded binary images into a decimal matrix, indicative of fringe orders, which corresponds to unique wave periods and increments every 10 pixels. This matrix assists in unwrapping the phase map by adding 2πK per pixel, with K being the fringe order, to yield a continuous phase profile across a single row, evidenced by a smoothly varying plot. But I could not achieve a successful result, I had some mistakes unwrapping the phase map and instead of a smoothline I got this pattern in figure 1

![image](https://github.com/Sametatak/kivi_task/assets/50046275/7af92d67-c557-4654-944f-c22a45ca80b7)
_figure 1. resulting plot_
# Task 2
-The goal of task 2 is implementing e 3-D bilateral filter to provided .ply file. I wrote the filter as described in Bilateral Mesh Denoising article. The formula of the filter is in figure 2 

![image](https://github.com/Sametatak/kivi_task/assets/50046275/4527ec5c-b8db-4745-b879-651b8b735725)

_figure 2_

-After implementing the filter to provided point cloud(figure 1) we can see that in the result (figure 2) the noise is increased

![Ekran görüntüsü 2024-04-08 190621](https://github.com/Sametatak/kivi_task/assets/50046275/14954651-f951-4ab2-9a2c-ecfb11a77e17)

_figure 1 provided point cloud_

![Ekran görüntüsü 2024-04-08 190435](https://github.com/Sametatak/kivi_task/assets/50046275/8f05071d-aa79-4d1f-977c-3d4a52ef90c9)

_figure 2 filtered point cloud_



# Task 3 
-In task 3 we have  two distinct unwrapped phase map images that represent the phase maps captured from the left and right camera setups. The goal of the task is develop an algorithm that effectively matches pixels between two unwrapped distorted phase map images obtained from left and right cameras.

- I first computes disparity by finding the minimum difference in intensity for each pixel in the left image against all pixels in the same row in the right image.
The resulting map shows horizontal displacement (disparity) values. Then I used a filter to clear the residual noise the resulting disparity map is very close to the zero matrix but it has very small noise. I shared the resulting images and final disparity csv file. 

![image](https://github.com/Sametatak/kivi_task/assets/50046275/283f7b5d-41ea-4740-8609-a7aafd9ab72f)
![image](https://github.com/Sametatak/kivi_task/assets/50046275/589dfb1f-614c-4db8-b36c-dd96ad26e97f)



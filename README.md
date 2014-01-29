CV-Lab-4
========

ICP3038 Computer Vision Laboratory 4: Detection and recognition
---------------------------------------------------------------

Task:

1. Reads a movie file whose name is passed as argv[1] argument. The movie will be played in a loop continuously. If no argument is passed, the application captures life video from the camera.

2. For each frame:
	(a) Plots the result of the Canny edge detector. This will help you finding the best parameters for the circle detection (next step).
	(b) Detects circles using the Hough transform and plots them in the current frame. Note that the OpenCV function for detecting circles using the Hough transform calls Canny edge detector internally. You will need to call Canny edge detector in addition to that to visualize the edge detection results.
	(c) If the mouse button is clicked on top of one of the circles, it is selected for recognition. The application extracts the descriptor for the selected circle and uses it to recognize the selected sphere in the subsequent frames. Once the sphere is recognized, it is plotted using a different colour than the rest of the detected circles.

3. The application should include the following sliders:
	(a) “Canny low” controlling the low threshold of the Canny edge detector (0–255, used for the Hough transform).
	(b) “Canny high” controlling the high threshold of the Canny edge detector (0–255).
	(c) “Detection threshold” controlling the threshold used for recognizing the sphere (nor- malise it to the range from 0 to 1).

	Use the sliders to find the best set of parameters and then use them as the default parameter values.

4. The video pauses when the space bar is pressed. The application exits if any other key is pressed.

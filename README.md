ICP3038 Computer Vision Lab 4
=============================
Detection and recognition
-------------------------

<b>Task:<b>
<ol><li>
	Reads a movie file whose name is passed as argv[1] argument. The movie will be played in a loop continuously. If no argument is passed, the application captures life video from the camera.
<br /><br /></li><li>
	For each frame:
	<ul><li>
		Plots the result of the Canny edge detector. This will help you finding the best parameters for the circle detection (next step).
	</li><li>
		Detects circles using the Hough transform and plots them in the current frame. Note that the OpenCV function for detecting circles using the Hough transform calls Canny edge detector internally. You will need to call Canny edge detector in addition to that to visualize the edge detection results.
	</li><li>
		If the mouse button is clicked on top of one of the circles, it is selected for recognition. The application extracts the descriptor for the selected circle and uses it to recognize the selected sphere in the subsequent frames. Once the sphere is recognized, it is plotted using a different colour than the rest of the detected circles.
	</li></ul>
<br /></li><li>
	The application should include the following sliders:
	<ul><li>
		“Canny low” controlling the low threshold of the Canny edge detector (0–255, used for the Hough transform).
	</li><li>
		“Canny high” controlling the high threshold of the Canny edge detector (0–255).
	</li><li>
		“Detection threshold” controlling the threshold used for recognizing the sphere (nor- malise it to the range from 0 to 1).
	</li></ul>
<br /></li><li>
	Use the sliders to find the best set of parameters and then use them as the default parameter values.
<br /><br /></li><li>
	The video pauses when the space bar is pressed. The application exits if any other key is pressed.
</li></ol>

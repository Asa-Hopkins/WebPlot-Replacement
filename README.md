# WebPlot-Replacement
An offline tool similar to WebPlotDigitizer, useful for extracting data points from a graph.

Webplot relies heavily on the user placing points in order to calculate values. This is time consuming and can also be inaccurate. Under some assumptions on how a plot is drawn, the graph can easily give sub-pixel accuracy, even to as much as 1/100 of a pixel.
When these assumptions break down, the methods here are still as accurate as a human manually placing points.

The assumption used currently is that lines are drawn in a solid colour, and that changes in this colour at the edges are due to the line only filling part of those pixels. A linear interpolation between the line colour and the background colour then gives what proportion of those pixels the line fills, giving the edge position to sub-pixel accuracy. 

## Compilation
Depends on CImg 2.9.2 or higher

To compile under Linux, clone the repository and set it to the current working directory. Then run
```sh
g++ webplot.cpp -lX11 -pthread -o webplot
```
to compile. Add `-O3` if you require optimisations. If compiling for windows, use MinGW and replace `-lX11` with `-lgdi32`.

## Usage
The program takes three command lines arguments, the filename of the graph, the lower axis label and the upper axis label.
Two windows open up, the first contains the graph being examined, and the second contains a zoomed section of the graph centered on the mouse. First, place the mouse over the lower axis tick, and then click to lock the zoomed section in place. Then, on the second window, draw a line vertically (either top to bottom or bottom to top works) over the axis tick. Return to the first window, and click to release the zoomed section and let it follow the mouse again.

Repeat the procedure but for the upper axis tick. Now the system is calibrated, and can measure the position of any edge on the graph relative to those axes. To measure a point, simply move the mouse over the edge to be measured, click to lock the window in place, and then draw a vertical line over the edge. The edge's pixel position and numerical value are then outputted to the standard output. This can be repeated for any number of edges.

## Example
This example uses `boxplot.png` in the `examples` folder. This image is derived from the standard `mtcars` dataset, and drawn using `R`. The command to run is 
```sh
./webplot ./examples/boxplot.png 10 30
```
You will be greeted with two windows as mentioned above.
![](https://github.com/Asa-Hopkins/WebPlot-Replacement/blob/main/Screenshots/Screenshot1.png)

Move the mouse over the lower axis tick, click to lock the window in place, and then draw a vertical line across it.

![](https://github.com/Asa-Hopkins/WebPlot-Replacement/blob/main/Screenshots/Screenshot2.png)

Repeat for the upper axis tick

![](https://github.com/Asa-Hopkins/WebPlot-Replacement/blob/main/Screenshots/Screenshot3.png)

And then repeat for every measurement you wish to take.

![](https://github.com/Asa-Hopkins/WebPlot-Replacement/blob/main/Screenshots/Screenshot4.png)

In this case, the output on the command line is
```sh
edges are 398.8 399.5333333
edges are 124.9333333 125.6666667
edge position is 180.0666667 so value is 26.00048685
edge position is 266.3333333 so value is 19.70058423
edge position is 327.9666667 so value is 15.19961052
```
So the three central values of the box plot are measured to be 26.00048685, 19.70058423 and 15.19961052. The true values are 26, 19.7 and 15.2, meaning we're getting (absolute) errors of only 0.0006. To put this in perspective, there are 275 pixels between the `10` tick and the `30` tick, meaning a single pixel is a difference of 0.07. This confirms we really can get sub-pixel accuracy when our assumptions hold.
## Limitations
Currently, the underlying line colour is assumed to be pure black. This is hard coded so changing it means recompiling currently. If the edges are thinner than two pixels, it is possible that there isn't a pixel that's entirely filled with the line, so it's impossible to know the true line colour.
As such it doesn't currently try to measure the true line colour either.

Another limitation is that the zoom amount and window sizes are currently fixed. It is also possible for jpeg artefacts to throw off the calculation, as currently any deviation from the background colour is interpreted as the start of the edge. This can be resolved by drawing the measuring line close enough to the edge, but it isn't ideal.

Only measurements along the Y-axis are supported currently, but again this can be solved manually by rotating the graph.

Despite these limitations, I have found this tool time saving when extracting data from graphs, so hopefully it can be of some use.

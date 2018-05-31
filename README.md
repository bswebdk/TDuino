# TDuino

TDuino is a convenience library for Arduino (and clones). With TDuino you will be able to
quickly implement basic timing features and different pin actions with only a few lines
of code.

__CODE @ https://github.com/bswebdk/tduino/__ <br/>
__DOCS @ http://tduino.bruchhaus.dk/__

## Installation

Go to <a href="https://github.com/bswebdk/tduino/" target="_blank">TDuino on GitHub</a>,
click the green "Clone or download" button in the upper area and select Download ZIP. When
the download has completed, you must <a href="https://www.arduino.cc/en/Guide/Libraries#toc4"
target="_blank">follow these instructions</a>. When you are done, you can use TDuino by
adding "#include <TDuino.h>" to the include section (first lines) of your sketch.

## The good :-)
* A lot of code already written
* Simple to use
* Multitasking done simple
* Event driven pin input
* Automatized pin outputs
* Nice debugging features
* NO F'IN DELAYS! <small>(almost.. see TPinInput::read(byte))</small>

## The bad :-(
* Slightly slower execution due to overhead
* Increased code size
* Increased memory usage

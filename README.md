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

## Change Log
__V1.4 -> V1.5__
* Fixed missing "TL_" in TTimeline::restart() (debug mode only).
* Added method TTimeline::hasOverlap().
* Fixed "beyond end of duration" bug in TTimelineT::loop().
* Added possibility to use of micros() instead of millis() for timing.
* Updated documentation.

__V1.3 -> V1.4__
* Fixed bad URL in library.properties (thankt to "per1234").
* Added include info to library.properties.
* Added new template class TTimelineT which is an optimized time line with no floating point math.
* Added stopAll(), restartAll() to TTimer, TTimeline and TTimelineT.
* Added resumeAll() to TTimer.
* Fixed an insignificant "bug" in TTimeline which would cause most slots to be invoked twice with "1.0".
* Fixed bug with initial timing for restarted slots in TTimer.

__V1.2 -> V1.3__
* Function TDefs::freeRam() may have caused problems on some none-arduino branded boards due to missing external values used to calculate
  available memory. Even though some problems are solved for now, freeRam() still lacks compatibility with some boards / processors.

__V1.1 -> V1.2__
* Removed constructor(pin, mode) from all pin classes (TPin, TPinInput, TPinOutput,
  TButton) in order to preven hardware to be setup before it is ready. Use "attach"
  during setup phase instead.

__V1.0 -> V1.1__
* Added a default contructor to TPin, TPinInput, TPinOutput, TButton in order to allow array instantiation of the classes (eg. TPin pins[3]).
* Added method attach(pin, mode) to TPin and all child classes.
* Removed all usage of "setup()" which is no longer needed. If you have sketches which uses the setup() method, just delete any calls to setup().
* TList has been removed, just declare array's instead.


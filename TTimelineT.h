/*
  
  Copyright © 2018 - Torben Bruchhaus
  TDuino.bruchhaus.dk - github.com/bswebdk/TDuino
  File: TTimelineT.h   
  
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU Lesser General Public License as published
  by the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.
  
  This program is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.
  
  You should have received a copy of the GNU Lesser General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>
  
*/

#ifndef TTIMELINET_H
#define TTIMELINET_H

#include "TTimeline.h"

/**
 * \brief Used to track actions on a virtual time line and map them to a value.
 * 
 * TTimelineT is usefull when you need to have sort of a virtual time line
 * where different things are happening gradually in a period of time which
 * can be postponed.
 * 
 * The difference between TTimeline and TTimelineT is that TTimelineT is a
 * template class which allows you to map any progress to a certain data type.
 * Since no floating point operations are used in TTimelineT it may in most
 * cases be more efficient that the TTimeline class.
 * 
 * \code
 * void TimelineCallback(byte slot, <DATATYPE> value)
 * {
 *   // "slot" is the index of the slot in the time line.
 *   // value is mapped progress in the wanted data type.
 * }
 * 
 * TTimelineT<byte> tline(TimelineCallback, 2); //Two slots
 * 
 * //Set the mapped value to be between 0 and 255
 * tline.setMinMax(0, 255);
 * 
 * //Set first slot to start after 5 seconds with a duration of 1 second
 * tline.set(0, 1000, 5000);
 * 
 * //Set the second slot to start after 10 seconds with a duration of 5 seconds
 * tline.set(1, 5000, 10000);
 * \endcode
 * 
 * \see TTimeline
 */
template <class DATATYPE> class TTimelineT : public TTimeline
{
private:
  void (*callback)(byte, DATATYPE);
  DATATYPE mapMin, mapMax;
  
public:
  
  /**
   * \brief The constructor for a TTimelineT.
   * 
   * See \ref TTimeline::TTimeline for more information.
   */
  TTimelineT(void(*callback)(byte, DATATYPE), byte numSlots = 1);
  
  using TTimeline::firstActive;
  using TTimeline::firstInactive;
  using TTimeline::getSize;
  using TTimeline::hasOverlap;
  using TTimeline::isActive;
  using TTimeline::isStarted;
  using TTimeline::restart;
  using TTimeline::restartAll;
  
  /**
   * \brief Set the minimum and maximum values for progress.
   * \param minValue The minimum progress value.
   * \param maxValue The maximum progress.
   * 
   * Must be called to set the range in which the progress should be mapped. The
   * arguments must be within the template data types's boundaries, eg. a "byte"
   * must use values between 0 and 255, a "int" must be between −32767 and 32767
   * and so on. 
   * 
   * The default values for the mapping is 0..255 for single byte data types and
   * 0..1023 for everything else.
   */
  void setMinMax(DATATYPE minValue, DATATYPE maxValue);
  
  using TTimeline::set;
  using TTimeline::stop;
  using TTimeline::stopAll;
  
  /**
   * \brief The TTimelineT's loop phase.
   * 
   * Must be called for each loop in the sketch.
   * 
   * \see TTimeline::loop()
   */
  virtual void loop();
  
};

#endif //TTIMELINET_H

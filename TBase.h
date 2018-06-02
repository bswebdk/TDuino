/*
  
  Copyright © 2018 - Torben Bruchhaus
  TDuino.bruchhaus.dk - github.com/bswebdk/TDuino
  File: TBase.h   
  
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

#ifndef TBASE_H
#define TBASE_H

#include "TDefs.h"

/**
 * \brief Base class for all classes in TDuino.
 * 
 * Used as a base class for all classes in TDuino. If you want to extend TDuino
 * with extra classes you should use TBase as a base class for your new classes.
 *
 * TBase is only intended as a "common root" and cannot be instantiated.
 * 
 * A minimal, valid subclass of TBase class would look something like this:
 * 
 * \code
 * // Header
 * TMyClass : public TBase
 * {
 *   ...
 * public:
 *   virtual void loop();
 * };
 * 
 * // Source
 * void TMyClass::loop()
 * {
 *   TBase::loop(); // Call loop of base class
 * 
 *   // Your own loop code goes here and you should use the protected
 *   // field "loopMillis" for timing.
 * }
 * 
 * \endcode
*/

class TBase {

#ifdef TDUINO_DEBUG
private:
  TBase* attachedTo;
  virtual void detachFrom(TBase *obj) { if (obj) obj->loop(); };
  friend class TList;
#endif

protected:

  unsigned long loopMillis; //!< The value of millis() to be used within loop()
  
public:

  /**
	* \brief Constructor for class TBase.
   * 
   * Initializes private and protected fields.
	*/
  TBase();
  
  /**
	* \brief Destructor for class TBase.
	* 
	* Does nothing else than implement a virtual destructor.
	*/
  virtual ~TBase();

  /**
	* \brief The loop method for TBase and all subclasses.
	*
	* Updates #loopMillis to the current value of millis()
	*/
  virtual void loop();
  
};

#endif //TBASE_H
